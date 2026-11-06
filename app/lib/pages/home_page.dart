import 'dart:async';

import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';
import '../widgets/BatteryIndicator.dart';
import '../dto/LiveData.dart';
import '../controller/LiveDataController.dart';

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
  Livedata? livedata;


  @override
  void initState() {
    super.initState();
    _elapsedTime = Duration.zero;
    _elapsedTimeString = _formatElapsedTime(_elapsedTime);

    WidgetsBinding.instance.addPostFrameCallback((_) {
      _fetchLiveData();
    });

    timer = Timer.periodic(Duration(seconds: 1), (timer) {
      setState(() {
        if(stopwatch.isRunning){
          _updateElapsedTime();
        }
      });
    });
  }

    Future<void> _fetchLiveData() async {
    try {
      final data = await LiveDataController.fetchLiveData();
      if (!mounted) return;
      setState(() {
        livedata = data;
      });
    } catch (e) {
      debugPrint('Error fetching live data: $e');
    }
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
      await _saveRuntime(elapsedToSave);
      await _increaseTimesStarted();
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

  Future showTimerStopAlert(BuildContext context) {
    return showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: const Text('Stop Timer'),
          content: SizedBox(
            height: 150,
            child: Column(
              mainAxisSize: MainAxisSize.min,
              children: const [
                Text('Are you sure you want to stop the timer?'),
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
                _startStopwatch();
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
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            crossAxisAlignment: CrossAxisAlignment.center,
            children: [
              Container(
                width: double.infinity,
                  height: MediaQuery.of(context).size.width / 4,
                margin: EdgeInsets.all(20),

                child:BatteryIndicator(
                  batteryLevel: livedata?.battery ?? 0,
                )
              ),
              Container(
                  width: double.infinity,
                  height: MediaQuery.of(context).size.width,

                  child:IconButton(
                    onPressed: () {

                      if(isPlaying){
                        showTimerStopAlert(context);
                      } else {
                        _startStopwatch();
                      }
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
                  value: livedata?.fullness != null ? livedata!.fullness / 100 : 0,
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
