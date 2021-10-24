#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
const char* ssid = "palak";
const char* password = "12345678";


const char* serverName = "http://192.168.85.179:8000/send-data";



const unsigned int time_period = 1000;
const float voltage_pin = 34;
const float current_pin = 33;
void setup() {
  pinMode (current_pin, INPUT);
  pinMode(voltage_pin, INPUT);


  // put your setup code here, to run once:
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

}

void loop()
{
  static float voltage_data[time_period];
  float energy = 0;
  float voltage_peak = 0;
  float current_peak = 0;
  while (Serial.available() > 0)
  {
    // put your main code here, to run repeatedly:


    for (int j = 0; j++; j < 15) {

      //static unsigned int info_position =0;

      for (int i = 0; i++; i < 1000)
      {
        float voltage = analogRead(voltage_pin);
        if (voltage > voltage_peak) {
          voltage_peak = voltage;
        }
        float current  = analogRead(current_pin);
        if (current > current_peak) {
          current_peak = current;
        }
        energy = (energy + (voltage * current)) / 3600000;
        delay(1);
      }


    }


  }
  float Vrms = voltage_peak / pow(2, 0.5);
  float Irms = current_peak / pow(2, 0.5);
  float Power = Vrms * Irms;
  {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/json");

      // DATA READING FROM VOLTAGE SENSOR AND CURRENT SENSOR SENDING TO SERVER

      //creating a staticJSONdocument  buffer "VALUES"
      StaticJsonDocument values;
      values["client_id"] = "palak";
      values["voltage_rms"] = Vrms;
      values[ "current_rms"] = Irms;
      values["average_power"] = Power;
      values["energy_consumption"] = energy;

      // Data to send with HTTP POST
      int httpResponseCode = http.POST();


    }
