#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <NewPing.h>

#define TRIGGER_PIN D1
#define ECHO_PIN D2
#define MAX_DISTANCE 280

const uint16_t port = 8585;
IPAddress server(192, 168, 2, 100);
WiFiClient client;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

const int interactionId = 4;


void setup()
{
  Serial.begin(115200);

  pinMode(D8, OUTPUT);

  

  // set wifi settings
  WiFi.mode(WIFI_STA);
  WiFi.begin("NEXUS_2.4G", "#sABP@@B?VAuWqcm?c!98Co4"); // change it to your ussid and password

  connectToWIFI();
  connectToServer();

  // switching on the LEDs
  bool lightOn = false;
  while(!lightOn)
  {
    if(WiFi.status() != WL_CONNECTED)
    {
      connectToWIFI();
    }

    if(!client.connected())
    {
      connectToServer();
    }
    // wait for light on message from server
    String line = client.readStringUntil('\n');
    if(line.length()> 1)
    {
        //Implement turning light on here
        Serial.println("light on");
        lightOn = true;
        digitalWrite(D8, HIGH);
    }
    else
    {
      Serial.println("Wrong input was sent");
    }
  }
  

}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    connectToWIFI();
  }

  if (!client.connected())
  {
    connectToServer();
  }


  unsigned int distance = sonar.ping_cm();
  sendSensorData(String(distance));

  delay(100);
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
