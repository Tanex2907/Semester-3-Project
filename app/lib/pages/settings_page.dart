import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';

class SettingsPage extends StatefulWidget {
  const SettingsPage({super.key});

  @override
  State<SettingsPage> createState() => _SettingsPageState();

}

class _SettingsPageState extends State<SettingsPage> {

  Future<void> resetPrefs() async
  {
    final SharedPreferences prefs = await SharedPreferences.getInstance();
    prefs.setInt('runtime', 0);
    prefs.setInt('temperature', 0);
    prefs.setInt('fullness', 0);
    prefs.setInt('battery', 0);
    prefs.setInt('timesStarted', 0);
  }

  Future showResetAlert(BuildContext context) {
    return showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: const Text('Reset Lifetime Data'),
          content: SizedBox(
            height: 150,
            child: Column(
              mainAxisSize: MainAxisSize.min,
              children: const [
                Text('Are you sure you want to reset all lifetime data?'),
              ],
            ),
          ),
          actions: [
            InkWell(
              onTap: () {
                Navigator.pop(context); // Closes the dialog
              },
              child: const Text('Cancel'),
            ),
            ElevatedButton(
              onPressed: () {
                resetPrefs();
                Navigator.pop(context);
              },
              child: const Text('Confirm'),
            ),
          ],
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: ListView(
        padding: const EdgeInsets.all(16),
            children: [
              const SizedBox(height: 40),
              Text(
                'Settings',
                textAlign: TextAlign.center,
                style: Theme.of(context).textTheme.titleLarge?.copyWith(
                  fontWeight: FontWeight.bold,
                ),
              ),
              const SizedBox(height: 8),
              Card(
                  color: Colors.red,
                  margin: const EdgeInsets.symmetric(vertical: 4),
                  shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12)),
                clipBehavior: Clip.antiAlias, // Clips the InkWell ripple to the card's rounded corners
                child: InkWell(
                  onTap: () {
                    showResetAlert(context);
                  },
                  child: Padding(
                    padding: const EdgeInsets.symmetric(horizontal: 16, vertical: 12),
                    child: Row(
                        children: [
                          Text(
                            'Reset Lifetime Data',
                            style: Theme.of(context).textTheme.titleMedium?.copyWith(
                              fontWeight: FontWeight.bold,
                            ),
                          ),
                        ]
                    ),
                  ),
                )
                )
            ],
          )
    );
  }
}

