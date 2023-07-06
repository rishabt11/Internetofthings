#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int CO_SENSOR_PIN = A0;
const int NH3_SENSOR_PIN = A1;

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
 
  float coLevel = readCO();
  float nh3Level = readNH3();

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("CO Level: ");
  Serial.print(coLevel);
  Serial.println(" ppm");

  Serial.print("NH3 Level: ");
  Serial.print(nh3Level);
  Serial.println(" ppm");

  delay(2000);
}

float readCO() {
  int sensorValue = analogRead(CO_SENSOR_PIN);
  float voltage = sensorValue * (5.0 / 1023.0);
  float coLevel = map(voltage, 0.0, 5.0, 0.0, 1000.0);  // Convert voltage to ppm (calibration required)
  return coLevel;
}

float readNH3() {
  int sensorValue = analogRead(NH3_SENSOR_PIN);
  float voltage = sensorValue * (5.0 / 1023.0);
  float nh3Level = map(voltage, 0.0, 5.0, 0.0, 100.0);  // Convert voltage to ppm (calibration required)
  return nh3Level;
}
