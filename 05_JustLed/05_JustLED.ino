#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const uint16_t port = 8585;
IPAddress server(192, 168, 2, 100);
WiFiClient client;

const int interactionId = 5;


void setup()
{
  Serial.begin(115200);

  pinMode(D8, OUTPUT);
  for (int i = 0; i < sizeof(buttons); i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }

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
