import 'dart:async';

import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';

class HomePage extends StatefulWidget {
  HomePage({super.key});

@override
State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {

  bool isPlaying = false;
  final stopwatch = Stopwatch();
  late Duration _elapsedTime;
  late String _elapsedTimeString;
  late Timer timer;

  @override
  void initState() {
    super.initState();
    _elapsedTime = Duration.zero;
    _elapsedTimeString = _formatElapsedTime(_elapsedTime);

    timer = Timer.periodic(Duration(seconds: 1), (timer) {
      setState(() {
        if(stopwatch.isRunning){
          _updateElapsedTime();
        }
      });
    });
  }


  void _startStopwatch() async
  {
    if(!stopwatch.isRunning){
      stopwatch.start();
      _updateElapsedTime();
      setState(() {
        isPlaying = true;
      });
    } else {
      stopwatch.stop();
      setState(() {
        isPlaying = false;
      });
      final elapsedToSave = stopwatch.elapsed;
      _resetStopwatch();
      if(elapsedToSave.inSeconds > 10) {
        await _saveRuntime(elapsedToSave);
        await _increaseTimesStarted();
      }
    }
  }

  void _resetStopwatch()
  {
    stopwatch.reset();
    _updateElapsedTime();
  }

  void _updateElapsedTime()
  {
    setState(() {
      _elapsedTime = stopwatch.elapsed;
      _elapsedTimeString = _formatElapsedTime(_elapsedTime);
    });
  }

  String _formatElapsedTime(Duration time){
    return '${time.inMinutes.remainder(60).toString().padLeft(2, '0')}:${(time.inSeconds.remainder(60)).toString().padLeft(2, '0')}';
  }

  Future<void> _increaseTimesStarted() async
  {
    final SharedPreferences prefs = await SharedPreferences.getInstance();
    prefs.setInt('timesStarted', (prefs.getInt('timesStarted') ?? 0) + 1);
  }

  Future<void> _saveRuntime(Duration elapsed) async {
    final SharedPreferences prefs = await SharedPreferences.getInstance();
    final currentRuntime = prefs.getInt('runtime') ?? 0;
    await prefs.setInt('runtime', currentRuntime + elapsed.inSeconds);
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
  }

  @override
  void dispose() {
    timer.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            crossAxisAlignment: CrossAxisAlignment.center,
            children: [
              Container(
                width: double.infinity,
                  height: MediaQuery.of(context).size.width / 4,
                margin: EdgeInsets.all(20),

                child:Column(
                  children: [
                    Icon(Icons.battery_4_bar, color: Colors.green),
                    Text('69 %'),
                  ],
                )
              ),
              Container(
                  width: double.infinity,
                  height: MediaQuery.of(context).size.width,

                  child:IconButton(
                    onPressed: () {
                      _startStopwatch();
                    },
                    icon: isPlaying ?
                    Icon(Icons.pause_rounded, color: Colors.red):
                    Icon(Icons.play_arrow_rounded, color: Colors.green),
                    iconSize: 100,
                  )
              ),
              Container(
                width: double.infinity,
                margin: EdgeInsets.all(20),

                child: LinearProgressIndicator(
                  value: 0.6,
                  color: Colors.green,
                ),
              ),
              Container(
                child: Text(_elapsedTimeString,
                  style: TextStyle(fontSize: 40),
                ),
              )
            ],
          )
        )
    );
  }

}
