#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Ticker.h>

const uint16_t port = 8585;
IPAddress server(192, 168, 2, 100);
WiFiClient client;


const int analogPin = A0;
const int digitalPin = D1;

int interval, wheel, counter;
unsigned long previousMicros, usInterval, calc;

Ticker ticker;


const int interactionId = 6;


void setup()
{
  Serial.begin(115200);

  pinMode(D8, OUTPUT);
  pinMode(digitalPin, INPUT);

  counter = 0; // counter auf 0 setzen
  interval = 1; // 5 Sekunden Intervall
  wheel = 20; // Loecher in der Encoder-Scheibe

  wheel = wheel * 2; // Anzahl der Loecher in der Encoder-Scheibe mit 2
  // multiplizieren, da der Interrupt bei jeder
  // Aenderung des Signals ausgefuehrt wird


  // set wifi settings
  WiFi.mode(WIFI_STA);
  WiFi.begin("NEXUS_2.4G", "#sABP@@B?VAuWqcm?c!98Co4"); // change it to your ussid and password

  connectToWIFI();
  connectToServer();

  // switching on the LEDs
  bool lightOn = false;
  while (!lightOn)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      connectToWIFI();
    }

    if (!client.connected())
    {
      connectToServer();
    }
    // wait for light on message from server
    String line = client.readStringUntil('\n');
    if (line.length() > 1)
    {
      //Implement turning light on here
      Serial.println("light on");
      lightOn = true;
    }
    else
    {
      Serial.println("Wrong input was sent");
    }
  }
  digitalWrite(D8, HIGH);
  
  ticker.attach(5, outputFunc); // Timer initialisieren auf dem Intervall
}

void count() {
  if (micros() - previousMicros >= 700) {
    counter++;
    previousMicros = micros();
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED)
  {
    connectToWIFI();
  }

  if (!client.connected())
  {
    connectToServer();
  }
}

void sendSensorData(String dataToSend) {
  Serial.println(dataToSend);
  client.println(dataToSend);
}

void outputFunc() {
    int speedData = ((counter) * calc) / wheel;
    // Berechnung der Umdrehungen pro Minute

    sendSensorData(String(speedData));
    counter = 0; // zuruecksetzen des Zaehlers
  }

void connectToWIFI()
{
  Serial.println("Connecting to WIFI...\n");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
}

void connectToServer()
{
  while (!client.connect(server, port))
  {
    Serial.println("Connection to host failed");
  }

  client.println(String(interactionId));
}
