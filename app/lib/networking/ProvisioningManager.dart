import 'package:flutter/material.dart';

class ProvisioningManager extends StatefulWidget {
  const ProvisioningManager({super.key});

  @override
  State<ProvisioningManager> createState() => _ProvisioningManagerState();
}

class _ProvisioningManagerState extends State<ProvisioningManager> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Provisioning Manager'),
      ),
      body: const Center(
        child: Text('Provisioning Manager Page'),
      ),
    );
  }
}