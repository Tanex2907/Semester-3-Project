import 'package:shared_preferences/shared_preferences.dart';

class PreferenceHandler {

  static Future<String?> getClientID() async {
    final SharedPreferences prefs = await SharedPreferences.getInstance();
    return prefs.getString("ClientID");
  }

  static Future<bool> setClientID(String id) async {
    final SharedPreferences prefs = await SharedPreferences.getInstance();
    return prefs.setString("ClientID", id);
  }

}