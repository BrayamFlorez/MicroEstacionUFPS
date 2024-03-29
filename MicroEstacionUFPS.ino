#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include "secrets.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include "time.h"

//DEFINIMOS VARIABLES GLOBALES

#define RXD2 16
#define TXD2 17
#define NMEA 0
#define AWS_THING_NAME "esp32"
#define AWS_IOT_PUBLISH_TOPIC   "ufpsSensorDataCloud/public"
#define AWS_IOT_SUBSCRIBE_TOPIC "ufpsSensorDataCloud/subscribe"


float temperatureAHT;
float humidityAHT;
float temperatureBMP;
float pressure;
float altitudeBMP;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
String timestamp;
String codigoPostal = "541010";

//INICIALIZAMOS SENSORES

Adafruit_AHTX0 aht20;
Adafruit_BMP280 bmp280;
HardwareSerial neogps(1);
char datoCmd = 0;
TinyGPSPlus gps;

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(512);

void setup() {
  
  Serial.begin(115200);
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
  connectWiFi();
  connectAWS();  

  //OBTENEMOS FECHA Y HORA
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  // NOS SUBSCRIBIMOS A NUESTRO TOPICO
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  // CREAMOS CONTROLADOR DE MENSAJES
  client.onMessage(messageHandler);

  //CONFIGURACION DE SENSOR AHT20 DE TEMPERTURA Y HUMEDAD
  if (!aht20.begin()) {
    Serial.println("No se pudo encontrar el sensor AHT20. Revisa la conexión.");
    while (1);
  }

  if (!bmp280.begin(0x76)) { // Cambia a 0x77 si tu sensor está configurado en esa dirección
    Serial.println("No se pudo encontrar el sensor BMP280. Revisa la conexión.");
    while (1);
  }      

}

void messageHandler(String &topic, String &payload) {
  Serial.println("Receiving MQTT message:");
  Serial.println(topic + " " + payload);
}

void loop() {
  
  // ENVIAMOS Y RECIBIMOS PAQUETES
  client.loop();

  // Obtener el tiempo del GPS si está disponible
  if (gps.time.isValid() && gps.date.isValid()) {
  // Obtener la hora, minuto y segundo del GPS en UTC
  int horaUTC = gps.time.hour();
  int minuto = gps.time.minute();
  int segundo = gps.time.second();

  // Obtener el año, mes y día del GPS
  int anio = gps.date.year();
  int mes = gps.date.month();
  int dia = gps.date.day();

  // Ajustar la hora a la zona horaria de Colombia (UTC-5)
  horaUTC -= 5;

  // Verificar si la hora ajustada es menor que 0 y ajustar adecuadamente
  if (horaUTC < 0) {
    horaUTC += 24; // Sumar 24 horas para ajustar al día anterior
    // Reducir el día en 1
    if (dia > 1) {
      dia--;
    } else {
      // Si el día es 1, establecer el mes anterior y ajustar el día al último del mes anterior
      if (mes > 1) {
        mes--;
        dia = diasEnMes(mes, anio);
      } else {
        // Si el mes es enero, ajustar al 31 de diciembre del año anterior
        anio--;
        mes = 12;
        dia = 31;
      }
    }
  }

  // Formatear la fecha y hora ajustadas en un string (timestamp)
  timestamp = String(anio) + "-" + formatoDosDigitos(mes) + "-" + formatoDosDigitos(dia) + " ";
  timestamp += formatoDosDigitos(horaUTC) + ":" + formatoDosDigitos(minuto) + ":" + formatoDosDigitos(segundo);
}

  //INICIAMOS LA MEDICION DE TEMPERATURA,HUMEDAD, ALTITUD Y PRESION ATMOSFERICA 
  
  sensors_event_t humidity, temp;
  aht20.getEvent(&humidity, &temp); // Obtener lecturas del AHT20
  
  temperatureAHT=temp.temperature;
  humidityAHT = humidity.relative_humidity;
  pressure = bmp280.readPressure() / 100.0F; // La presión se lee en Pa, se convierte a hPa por estandar
  temperatureBMP = bmp280.readTemperature();
  altitudeBMP = bmp280.readAltitude(1013.25); // Presión estándar al nivel del mar

  //INICIAMOS LA MEDICION DE LA POSICION GEOGRAFICA POR MEDIO DE GPS

   if (NMEA) {
    while (neogps.available()) {
      datoCmd = (char)neogps.read();
      Serial.print(datoCmd);
    }
  } else {
    boolean newData = false;
    for (unsigned long start = millis(); millis() - start < 1000;) {
      while (neogps.available()) {
        if (gps.encode(neogps.read())) {
          newData = true;
        }
      }
    }

    if (newData == true) {
      newData = false;

      // Crear un objeto JSON que se enviara en el mensaje MQTT
      StaticJsonDocument<200> jsonDoc;

      //CREAMOS Y ENVIAMOS UN JSON SOLO SI EL GPS ESTA FUNCIONANDO, YA QUE LAS MEDIDAS SIN UBICACION SON INUTILES EN ESTE PROYECTO

      if (gps.location.isValid() == 1) {
        // Rellenar los datos en el objeto JSON
        jsonDoc["Timestamp"] = timestamp;
        jsonDoc["cpCity"] = codigoPostal;
        jsonDoc["Temperature1"] = temperatureAHT;
        jsonDoc["Humidity"] = humidityAHT;
        jsonDoc["Pressure"] = pressure;
        jsonDoc["Temperature2"] = temperatureBMP;
        jsonDoc["Altitude1"] = altitudeBMP;
        jsonDoc["Latitude"] = gps.location.lat();
        jsonDoc["Longitude"] = gps.location.lng();
        jsonDoc["Speed"] = gps.speed.kmph();
        jsonDoc["Satellites"] = gps.satellites.value();
        jsonDoc["Altitude2"] = gps.altitude.meters();
        jsonDoc["Date"] = String(gps.date.day()) + "/" + String(gps.date.month()) + "/" + String(gps.date.year());
        jsonDoc["Time"] = String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second());

      } else {
        jsonDoc["Temperature1"] = temperatureAHT;
        jsonDoc["Humidity"] = humidityAHT;
        jsonDoc["Pressure"] = pressure;
        jsonDoc["Temperature2"] = temperatureBMP;
        jsonDoc["Altitude1"] = altitudeBMP;
        jsonDoc["Latitude"] = gps.location.lat();
        jsonDoc["Longitude"] = gps.location.lng();
        jsonDoc["Speed"] = gps.speed.kmph();
        jsonDoc["Satellites"] = gps.satellites.value();
        jsonDoc["Altitude2"] = gps.altitude.meters();
        jsonDoc["Date"] = String(gps.date.day()) + "/" + String(gps.date.month()) + "/" + String(gps.date.year());
        jsonDoc["Time"] = String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second());
        jsonDoc["Error"] = "Sin señal GPS";
        printLocalTime();
      }

      // Convertir el objeto JSON a una cadena
      String jsonString;
      serializeJson(jsonDoc, jsonString);

      // Imprimir el JSON por el monitor serie
      Serial.println(jsonString);

      if (gps.location.isValid() == 1) {
        // PUBLICAMOS NUESTRO JSON EN EL TOPIC DE PUBLICACION
         client.publish(AWS_IOT_PUBLISH_TOPIC, jsonString);
        } else {
      }      
    }
  }  
  delay(10000);// no usar mas de 10000 milisegundos de pausa
}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

int diasEnMes(int mes, int anio) {
  if (mes == 2) {
    if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0)) {
      return 29;
    } else {
      return 28;
    }
  } else if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
    return 30;
  } else {
    return 31;
  }
}

String formatoDosDigitos(int numero) {
  if (numero < 10) {
    return "0" + String(numero);
  } else {
    return String(numero);
  }
}


