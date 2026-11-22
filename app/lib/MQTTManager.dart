import 'dart:async';

import 'package:app/dto/LiveData.dart';
import 'package:app/widgets/connectionToast.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';
import 'dart:convert';
import 'package:flutter/services.dart';

class MQTTManager {
  final String topic;
  late MqttServerClient client;
  late String broker;
  late String user;
  late String pwd;

  Livedata? latestLivedata;

  final _livedataController = StreamController<Livedata>.broadcast();

  Stream<Livedata> get livedataStream => _livedataController.stream;

  MQTTManager({required this.topic});

  Future<void> initializeAndConnect() async {
    try {
      // 1. Load the private.json file
      final String response = await rootBundle.loadString('config/private.json');
      final data = json.decode(response);
      broker = data['broker'];
      user = data['user'];
      pwd = data['pwd'];

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
      client.setProtocolV311();
      
      final connMess = MqttConnectMessage()
          .withClientIdentifier(clientID)
          .authenticateAs(user, pwd)
          .withWillQos(MqttQos.atMostOnce);

      client.connectionMessage = connMess;
      
      print('MQTTManager::connecting to broker $broker...');
      await client.connect();

      client.updates!.listen(_onMessageReceived);
      
    } catch (e) {
      print('MQTTManager::connect - Exception: $e');
      disconnect();
    }
  }

  void _onMessageReceived(List<MqttReceivedMessage<MqttMessage?>>? events)
  {
    if (events == null || events.isEmpty) return;

    final String topic = events[0].topic;
    final recMessage = events[0].payload as MqttPublishMessage;
    final String payload = MqttPublishPayload.bytesToStringAsString(recMessage.payload.message);

    try {
      final decoded = jsonDecode(payload);
      final parsed = Livedata.fromJson(decoded);
      latestLivedata = parsed;
      if (decoded is! Map<String, dynamic>) return;

      switch (topic) {
        case "telemetry/livedata":
          _livedataController.add(Livedata.fromJson(decoded));
          break;
        default:
      }
    } catch (e) {
      print("Couldnt decode payload");
    }
  }

  void disconnect() {
    print('MQTTManager::disconnect - Disconnecting');
    client.disconnect();
  }

  void onConnected() {
    print('MQTTManager::onConnected - Connected to the broker');
    client.subscribe(topic, MqttQos.atLeastOnce);
    showConnectionSnackBar(isConnected: true);
  }

  void onDisconnected() {
    print('MQTTManager::onDisconnected - Disconnected from the broker');
    showConnectionSnackBar(isConnected: false);
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

void dispose()
{
  _livedataController.close();
  disconnect();
}
}

