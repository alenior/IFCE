import 'dart:convert';
import 'package:http/http.dart' as http;

class GeocodingService {
  static const _apiKey = 'AIzaSyBh2LrTK_h_k3chcKTccjSe2m7rLoYj9YE';

  static Future<String?> getPlaceName(double lat, double lon) async {
    final url = Uri.parse(
      'https://maps.googleapis.com/maps/api/geocode/json?latlng=$lat,$lon&key=$_apiKey&language=pt-BR',
    );

    final response = await http.get(url);
    if (response.statusCode == 200) {
      final data = jsonDecode(response.body);
      if (data['status'] == 'OK' &&
          data['results'] != null &&
          data['results'].isNotEmpty) {
        final place = data['results'][0]['formatted_address'];
        return place;
      }
    }
    return null;
  }
}
