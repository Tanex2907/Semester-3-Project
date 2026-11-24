import 'package:flutter/material.dart';
import 'package:app/PreferenceHandler.dart';

class ClientSetCard extends StatefulWidget {
  const ClientSetCard({super.key});


  @override
  State<ClientSetCard> createState() => _ClientSetCardState();
}

class _ClientSetCardState extends State<ClientSetCard>
{

  final myController = TextEditingController();

  @override
  void dispose() {
    // Clean up the controller when the widget is removed from the
    // widget tree.
    myController.dispose();
    super.dispose();
  }

  Future showSetClientAlert(BuildContext context) {
    return showDialog(
        context: context,
        builder: (BuildContext context) {
          return AlertDialog(
            title: Text("Insert Client ID of your robot"),
            content: TextField(
              decoration: InputDecoration(hintText: "Enter ID here"),
              controller: myController,
            ),
            actions: [
              TextButton(
                  onPressed: () async {
                    final clientID = myController.text.trim();
                    if (clientID.isEmpty) return;

                    await PreferenceHandler.setClientID(clientID);
                    if (!context.mounted) return;
                    Navigator.of(context).pop();
                  },
                  child: Text("Submit")
              )
            ],
          );
        }
    );
  }

  @override
  Widget build(BuildContext context) {
    return  Card(
        margin: const EdgeInsets.symmetric(vertical: 4),
        shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12)),
        clipBehavior: Clip.antiAlias, // Clips the InkWell ripple to the card's rounded corners
        child: InkWell(
          onTap: () {
            showSetClientAlert(context);
          },
          child: Padding(
            padding: const EdgeInsets.symmetric(horizontal: 16, vertical: 12),
            child: Row(
                children: [
                  Text(
                    'Set Robot ID',
                    style: Theme.of(context).textTheme.titleMedium?.copyWith(
                      fontWeight: FontWeight.bold,
                    ),
                  ),
                ]
            ),
          ),
        )
    );
  }
}
