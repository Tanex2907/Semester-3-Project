import 'dart:async';

import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';
import '../dto/LiveData.dart';
import '../widgets/StatCard.dart';
import 'package:app/MQTTManager.dart';

class StatsPage extends StatefulWidget {
  const StatsPage({
    super.key,
    required this.manager,
    });

  final MQTTManager manager;

  @override
  State<StatsPage> createState() => _StatsPageState();
}

class _StatsPageState extends State<StatsPage> {

  int? runtime;
  int? timesStarted;

  Livedata? livedata;
  StreamSubscription<Livedata>? _livedataSub;

  //TODO only use this for lifetime stats since we fetch the live ones from the ESP constantly
  Future<void> _getPersistenceData() async
  {
    final SharedPreferences prefs = await SharedPreferences.getInstance();
    if (!mounted) return;
    setState(() {
      if(prefs.getInt('runtime') != 0 && prefs.getInt('runtime')! > 60)
      {
        runtime = prefs.getInt('runtime')! ~/ 60;
      }
      timesStarted = prefs.getInt('timesStarted')  ?? 0;
    });
  }

  @override
  void initState() {
    super.initState();
    WidgetsBinding.instance.addPostFrameCallback((_) {
      _getPersistenceData();
      livedata = widget.manager.latestLivedata;
      
      _livedataSub = widget.manager.livedataStream.listen(
        (data) {
          if(!mounted) return;
          setState(() {
            livedata = data;
          });
        }
      );
    });
  }

  @override
  void dispose() {
    _livedataSub?.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: ListView(
        padding: const EdgeInsets.all(16),
        children: [
          const SizedBox(height: 40),
          Text(
            'Live Stats',
            textAlign: TextAlign.center,
            style: Theme.of(context).textTheme.titleLarge?.copyWith(
              fontWeight: FontWeight.bold,
            ),
          ),
          const SizedBox(height: 8),
          StatCard(icon: Icons.thermostat, title: 'Temperature', value: '${livedata?.temperature} °C'),
          StatCard(icon: Icons.disc_full, title: 'Fullness', value: '${livedata?.fullness} %'),
          StatCard(icon: Icons.battery_4_bar, title: 'Battery', value: '${livedata?.battery} %'),
          const SizedBox(height: 24),

          Text(
            'Lifetime Stats',
            textAlign: TextAlign.center,
            style: Theme.of(context).textTheme.titleLarge?.copyWith(
              fontWeight: FontWeight.bold,
            ),
          ),
          const SizedBox(height: 8),
          StatCard(icon: Icons.run_circle, title: 'Runtime', value: '$runtime min'),
          StatCard(icon: Icons.start, title: 'Times started', value: '$timesStarted'),
        ],
      ),
    );
  }
}