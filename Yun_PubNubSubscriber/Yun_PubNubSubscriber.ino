/*
  PubNub sample subscribe client

  This sample client will subscribe to and handle raw PubNub messages
  (not doing any JSON decoding).  It does so with a randomly generated
  UUID.

  Circuit:
  * Ethernet shield attached to pins 10, 11, 12, 13
  * (Optional.) LED on pin 8 for reception indication.
  * Pin A4 unconnected (noise source for random number generator)

  created 23 October 2012
  by Petr Baudis

  https://github.com/pubnub/pubnub-api/tree/master/arduino
  This code is in the public domain.
  */

#include <SPI.h>
#include <Bridge.h>
#include <YunClient.h>
#include <PubNub.h>
#include <ArduinoJson.h>

char pubkey[] = "demo";
char subkey[] = "demo";
char channel[] = "nice295";

void setup()
{
  Serial.begin(9600);  
  Bridge.begin();
  PubNub.begin(pubkey, subkey);
}

void checkWifiStatus()
{
  Process wifiCheck;  // initialize a new process

  wifiCheck.runShellCommand("/usr/bin/pretty-wifi-info.lua");  // command you want to run

  // while there's any characters coming back from the
  // process, print them to the serial monitor:
  while (wifiCheck.available() > 0) {
    char c = wifiCheck.read();
    Serial.print(c);
  }

  Serial.println();
}

void loop()
{
	PubSubClient *client;
  String thejson;
  char sub_buff[200];

	Serial.println("waiting for a message (subscribe)");
	client = PubNub.subscribe(channel);
	if (!client) {
		Serial.println("subscription error");
    checkWifiStatus();
		delay(1000);
		return;
	}
	Serial.print("Received: ");
	while (client->wait_for_data()) {
		char c = client->read();
		Serial.print(c);
    if (c != '[' && c != ']')
      thejson += c;
	}
	client->stop();
	Serial.println();
  Serial.print("thejson: "); 
  Serial.println(thejson);

  thejson.toCharArray(sub_buff, 200);

  StaticJsonBuffer<200> jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(sub_buff);

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  const char* name = root["name"];
  if (name)
  {
    Serial.println(name);
  }
  const char* value = root["value"];
  if (value)
  {
    Serial.println(value);
  }
    
	delay(2000);
}
