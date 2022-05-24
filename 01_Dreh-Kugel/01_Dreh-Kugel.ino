#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Ticker.h>

const uint16_t port = 8585;
IPAddress server(192, 168, 2, 100);
WiFiClient client;

const int digitalPin = D1;

unsigned long start_time = 0;
unsigned long end_time = 0;
int steps = 0;
float steps_old = 0;
float temp = 0;
float rps = 0;


const int interactionId = 1;


void setup()
{
  Serial.begin(115200);

  pinMode(D8, OUTPUT);
  pinMode(digitalPin, INPUT_PULLUP);


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
      digitalWrite(D8, HIGH);
      Serial.println("light on");
      lightOn = true;
    }
    else
    {
      Serial.println("Wrong input was sent");
    }
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

  start_time = millis();
  end_time = start_time + 100;
  while (millis() < end_time)
  {
    if (digitalRead(digitalPin))
    {
      steps = steps + 1;
      while (digitalRead(digitalPin));
    }
  }
  temp = steps - steps_old;
  steps_old = steps;
  rps = (temp / 20);

  sendSensorData(String(rps));
}

void sendSensorData(String dataToSend) {
  Serial.println(dataToSend);
  client.println(dataToSend);
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
