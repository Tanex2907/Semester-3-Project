import 'dart:convert';

import 'package:app/dto/LiveData.dart';
import 'package:http/http.dart' as http;

//TODO add permissions to IOS manifest for building so that we can access internet
class LiveDataController  {
  static Future<Livedata> fetchLiveData() async
  {
    final response = await http.get(
      Uri.parse('http://robot.local/livedata'),
      headers: <String, String>{
        'Content-Type': 'application/json; charset=UTF-8',
      },
    );
    if (response.statusCode == 200) {
      return Livedata.fromJson(jsonDecode(response.body) as Map<String, dynamic>);
    } else {
      throw Exception('Failed to load Livedata');
    }
  }

}

