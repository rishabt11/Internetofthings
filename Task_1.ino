#include <SoftwareSerial.h>

// Ultrasonic sensor pins
const int incomingSensorTriggerPin = 2;
const int incomingSensorEchoPin = 3;
const int outgoingSensorTriggerPin = 4;
const int outgoingSensorEchoPin = 5;

// LED pins
const int availableSpaceLedPin = 6;
const int occupiedSpaceLedPin = 7;

// Distance thresholds
const int emptySpaceThreshold = 50;
const int occupiedSpaceThreshold = 10;

// Software serial communication
SoftwareSerial esp8266(8, 9); // RX, TX

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  esp8266.begin(9600);
  
  // Initialize sensor pins
  pinMode(incomingSensorTriggerPin, OUTPUT);
  pinMode(incomingSensorEchoPin, INPUT);
  pinMode(outgoingSensorTriggerPin, OUTPUT);
  pinMode(outgoingSensorEchoPin, INPUT);
  
  // Initialize LED pins
  pinMode(availableSpaceLedPin, OUTPUT);
  pinMode(occupiedSpaceLedPin, OUTPUT);
  
  // Connect to Wi-Fi
  connectToWiFi();
}

void loop() {
  // Check incoming car
  int incomingDistance = measureDistance(incomingSensorTriggerPin, incomingSensorEchoPin);
  if (incomingDistance < occupiedSpaceThreshold) {
    Serial.println("Incoming car detected");
    notifyServer("Incoming car detected");
    updateParkingStatus(false);
  }
  
  // Check outgoing car
  int outgoingDistance = measureDistance(outgoingSensorTriggerPin, outgoingSensorEchoPin);
  if (outgoingDistance < emptySpaceThreshold) {
    Serial.println("Outgoing car detected");
    notifyServer("Outgoing car detected");
    updateParkingStatus(true);
  }
  
  delay(1000); // Wait for a second before rechecking
}

int measureDistance(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}

void updateParkingStatus(bool isAvailable) {
  if (isAvailable) {
    digitalWrite(availableSpaceLedPin, HIGH);
    digitalWrite(occupiedSpaceLedPin, LOW);
  } else {
    digitalWrite(availableSpaceLedPin, LOW);
    digitalWrite(occupiedSpaceLedPin, HIGH);
  }
}

void connectToWiFi() {
  esp8266.println("AT");
  delay(1000);
  
  esp8266.println("AT+CWMODE=1");
  delay(1000);
  
  esp8266.println("AT+CWJAP=\"YourWiFiSSID\",\"YourWiFiPassword\"");
  delay(5000);
}

void notifyServer(String message) {
  esp8266.println("AT+CIPSTART=\"TCP\",\"yourserver.com\",80");
  delay(5000);
  
  String httpMessage = "GET /update?message=" + message + " HTTP/1.1\r\nHost: yourserver.com\r\n\r\n";
  esp8266.println("AT+CIPSEND=" + String(httpMessage.length()));
  delay(2000);
  
  esp8266.println(httpMessage);
  delay(2000);
  
  esp8266.println("AT+CIPCLOSE");
}
