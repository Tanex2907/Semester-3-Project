class Livedata {
  final int temperature;
  final int battery;
  final int fullness;

  const Livedata({required this.temperature, required this.battery, required this.fullness});

  factory Livedata.fromJson(Map<String, dynamic> json) {
    return switch (json) {
      {'temperature': int temperature, 'battery': int battery, 'fullness': int fullness} => Livedata(
          temperature: temperature,
          battery: battery,
          fullness: fullness
      ),
      _ => throw Exception('Invalid JSON format'),
    };
  }
}

