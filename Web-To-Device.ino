#include <Arduino.h>
#include <WiFiNINA.h>
#include <Firebase_Arduino_WiFiNINA.h>

// Firebase configuration
#define FIREBASE_HOST "https://web-to-device-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "wRi63ckmjSp1wqiHouujbrqFyupZS479WNZ9s3dW"

// WiFi credentials
#define WIFI_SSID "Kartik"
#define WIFI_PASSWORD "87654321"

// Firebase data path
const String firebasePath = "/status/status";

// LED pins
const int RED_LED_PIN = 13;
const int YELLOW_LED_PIN = 3;
const int GREEN_LED_PIN = 4;

FirebaseData firebaseData;

void setup()
{
  Serial.begin(9600);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  // Connect to Wi-Fi
  connectToWiFi();

  // Initialize Firebase
  initializeFirebase();

  // Set initial status to "OFF"
  setFirebaseStatus("OFF");
}

void loop()
{
  // Read status from Firebase
  String status = getFirebaseStatus();

  if (status == "RED")
  {
    controlLEDs(HIGH, LOW, LOW);
    Serial.println("Turning the RED LED ON");
  }
  else if (status == "YELLOW")
  {
    controlLEDs(LOW, HIGH, LOW);
    Serial.println("Turning the YELLOW LED ON");
  }
  else if (status == "GREEN")
  {
    controlLEDs(LOW, LOW, HIGH);
    Serial.println("Turning the GREEN LED ON");
  }
  else if (status == "OFF")
  {
    controlLEDs(LOW, LOW, LOW);
    Serial.println("Turning all LEDs OFF");
  }
  else
  {
    Serial.println("Unknown status received from Firebase: " + status);
  }

  delay(1000); // Adjust the delay as needed
}

void connectToWiFi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("Local IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void initializeFirebase()
{
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);
}

void setFirebaseStatus(const String &status)
{
  if (Firebase.setString(firebaseData, firebasePath, status))
  {
    Serial.println("Firebase status updated to: " + status);
  }
  else
  {
    Serial.println("Failed to update Firebase status. Error: " + firebaseData.errorReason());
  }
}

String getFirebaseStatus()
{
  if (Firebase.getString(firebaseData, firebasePath))
  {
    return firebaseData.stringData();
  }
  else
  {
    Serial.println("Failed to retrieve Firebase status. Error: " + firebaseData.errorReason());
    return "ERROR";
  }
}

void controlLEDs(int red, int yellow, int green)
{
  digitalWrite(RED_LED_PIN, red);
  digitalWrite(YELLOW_LED_PIN, yellow);
  digitalWrite(GREEN_LED_PIN, green);
}
