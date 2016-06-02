#include "application.h"

TCPClient client;
IPAddress ip = { 10, 1, 10, 175 };
Serial1DebugOutput debugOutput(38400);

int blink(String pin) {
    digitalWrite(D7, HIGH);
    delay(200);
    digitalWrite(D7, LOW);
}
    
void setup()
{
    pinMode(D7, OUTPUT);
    Serial1.begin(38400);
    Serial1.println("Starting!");
    Particle.function("blink", blink);
}

void loop()
{
  static bool oneshot = true;
  static bool twoshot = true;
  if (oneshot && Particle.connected())
  {
    Serial1.println("TCPClient Test - ONLINE");
    oneshot=false;
    if (client.connect(ip, 22))
    {
      Serial1.println("TCP connect - success!");
    }
    else
    {
      Serial1.println("TCP connect - failed :-(");
    }
  }
    if (client.available() > 0 && twoshot) {
        twoshot = false;
        Serial1.print("TCP has data of: ");
        Serial1.print(client.available());
        Serial1.println("");
        while (client.available() > 0) {
            char inByte = client.read();
            Serial1.print(inByte);
        }
        Serial1.println(" bytes!");
    }
}


