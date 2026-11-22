import 'package:app/main.dart';
import 'package:flutter/material.dart';

void showConnectionSnackBar({required bool isConnected}) {
  final messenger = rootScaffoldMessengerKey.currentState;
  if (messenger == null) return;

  messenger.clearSnackBars();

  messenger.showSnackBar(
    SnackBar(
      content: Text(isConnected ? 'Connected!' : 'Connection Lost!'),
      backgroundColor: isConnected ? Colors.green : Colors.red,
      duration: const Duration(seconds: 3),
    ),
  );
}