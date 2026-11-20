import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';
import 'dart:convert';
import 'package:flutter/services.dart';

class MQTTManager {
  final String topic;
  late MqttServerClient client;
  late String broker;
  late String channel;
  late String token;

  MQTTManager({required this.topic});

  Future<void> initializeAndConnect() async {
    try {
      // 1. Load the private.json file
      final String response = await rootBundle.loadString('config/private.json');
      final data = json.decode(response);
      broker = data['broker'];
      channel = data['channel'];
      token = data['iam-token'];

      String clientID = 'flutter_client_${DateTime.now().millisecondsSinceEpoch}';

      // 2. Initialize client
      client = MqttServerClient(broker, clientID);
      client.port = 1883;
      client.logging(on: true);
      client.keepAlivePeriod = 20;
      client.onDisconnected = onDisconnected;
      client.onConnected = onConnected;
      client.onSubscribed = onSubscribed;
      client.autoReconnect = true;
      
      final connMess = MqttConnectMessage()
          .withClientIdentifier(clientID)
          .authenticateAs(token, '')
          .startClean()
          .withProtocolVersion(MqttClientConstants.mqttV311ProtocolVersion)
          .withWillQos(MqttQos.atMostOnce);

      client.connectionMessage = connMess;

      print('MQTTManager::connecting to broker $broker...');
      await client.connect();
      
    } catch (e) {
      print('MQTTManager::connect - Exception: $e');
      disconnect();
    }
  }

  void disconnect() {
    print('MQTTManager::disconnect - Disconnecting');
    client.disconnect();
  }

  void onConnected() {
    print('MQTTManager::onConnected - Connected to the broker');
    client.subscribe(topic, MqttQos.atLeastOnce);
  }

  void onDisconnected() {
    print('MQTTManager::onDisconnected - Disconnected from the broker');
  }

  void onSubscribed(String topic) {
    print('MQTTManager::onSubscribed - Subscribed to topic: $topic');
  }

  void publishMessage(String topic, String message) {
    final builder = MqttClientPayloadBuilder();
    builder.addString(message);
    client.publishMessage(topic, MqttQos.atLeastOnce, builder.payload!);
    print('MQTTManager::publishMessage - Published message: $message to topic: $topic');
  }
}