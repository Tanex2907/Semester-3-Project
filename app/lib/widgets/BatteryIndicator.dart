import 'package:flutter/material.dart';

enum BatteryStatus {
  full,
  almost,
  half,
  low,
}

Map<BatteryStatus, IconData> batteryIcons = {
  BatteryStatus.full: Icons.battery_full,
  BatteryStatus.almost: Icons.battery_5_bar,
  BatteryStatus.half: Icons.battery_4_bar,
  BatteryStatus.low: Icons.battery_1_bar,
};

class BatteryIndicator extends StatelessWidget {
  
  final int batteryLevel;
  
  const BatteryIndicator({
    super.key,
    required this.batteryLevel,
  });

  BatteryStatus getBatteryStatus() {
    if (batteryLevel >= 80) {
      return BatteryStatus.full;
    } else if (batteryLevel >= 60) {
      return BatteryStatus.almost;
    } else if (batteryLevel >= 40) {
      return BatteryStatus.half;
    } else {
      return BatteryStatus.low;
    }
  }


  @override
  Widget build(BuildContext context) {
    //Change Icon and color based on battery level
    return Column(
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        Icon(
          batteryIcons[getBatteryStatus()]!,
          color: batteryLevel >= 40 ? Colors.green : Colors.red,
          size: 50,
        ),
        SizedBox(height: 8),
        Text(
          '${batteryLevel.toStringAsFixed(0)} %',
          style: TextStyle(fontSize: 16),
        ),
      ],
    );
  }
}