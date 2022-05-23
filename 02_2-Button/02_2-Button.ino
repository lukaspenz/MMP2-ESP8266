#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const uint16_t port = 8585;
IPAddress server(192, 168, 2, 100);
WiFiClient client;

char buttons[2] = {D6, D5};
int buttonStates[2] = {0, 0};
int lastButtonStates[2] = {0, 0};

const int interactionId = 2;


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


  //get sensor data

  for (int i = 0; i < sizeof(buttons); i++) {
    checkButtonState(i);
  }

  delay(100);
}

void checkButtonState(int i) {
  String data = "";
  int buttonState = digitalRead(buttons[i]);
  if (buttonState != lastButtonStates[i]) {
    if (buttonState = HIGH) {
      switch (i) {
        case 0:
          data = "1";
          break;
        case 1:
          data = "2";
          break;
          //sendSensorData(data += "1");
      }
      sendSensorData(data);
    } else {
      sendSensorData(data += "0");
    }
  }
  lastButtonStates[i] = buttonState;
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
