

// Funzione per ottenere l'autorizzazione basic
String getBasicAuth(const char* username, const char* password) {
  String auth = String(username) + ":" + String(password);
  return "Basic " + base64::encode(auth);
}

// Funzione per effettuare la richiesta GET con autenticazione
String get_power(const char* url, const char* username, const char* password) {
  String response = "";
  
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    
    if (http.begin(client, url)) {
      // Aggiungi l'header di autorizzazione
      String authHeader = getBasicAuth(username, password);
      http.addHeader("Authorization", authHeader);
      
      // Effettua la richiesta GET
      int httpCode = http.GET();
      
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {
          response = http.getString();
        }
      } else {
        response = "Error: " + String(http.errorToString(httpCode).c_str());
      }
      
      http.end();
    } else {
      response = "Error: Unable to connect to server";
    }
  } else {
    response = "Error: WiFi not connected";
  }
  
  return response;
}


float get_point(String inverter_response, String varTag){
  DynamicJsonDocument doc(4096); 
  DeserializationError error = deserializeJson(doc, inverter_response);
  if (!error) {
    // Naviga attraverso il JSON per trovare Igrid
    //const char* INVERTER_ID = "118810-3P46-0221"; 
    JsonObject inverter = doc["118810-3P46-0221"];
    if (!inverter.isNull()) {
      JsonArray points = inverter["points"];
      for (JsonObject point : points) {
        if (point["name"] == varTag) {
          return point["value"].as<float>();
          break;
        }
      }
    }

  }
  return -1;
}



float get_live_data(String tag){
  String response = get_power("http://192.168.1.39/v1/livedata", "DEROSA2021", "sucamilla");
  return get_point(response, tag);
}
