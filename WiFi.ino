const char* ssid = "ACA EL NOMBRE DE TU RED WIFI";
const char* password = "ACA LA CLAVE DE TU RED WIFI";

void connectWiFi() {
  Serial.println("Connecting");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
