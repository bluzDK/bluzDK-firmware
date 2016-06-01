#include "application.h"

Serial1DebugOutput debugOutput(38400); // default is 9600 and log everything

TCPClient client;
IPAddress ip = { 10, 0, 1, 112 };
    
void setup()
{
}

void loop()
{
  static bool oneshot = true;
  static uint32_t t = millis(); 
  if (oneshot && Particle.connected())
  {
    Particle.publish("TCPClient Test", "ONLINE");
    oneshot=false;
    t = millis();
    if (client.connect(ip, 22))
    {
      Particle.publish("TCP connect", "success!");
    }
    else
    {
      Particle.publish("TCP connect", "failed :-(");
    }
  }
  if (!oneshot && millis() > (t + 5000)) { 
    t = millis(); Particle.publish("PING"); 
/*
    String s = "";
    int bytes;
    while ( (bytes = client.available()) ) s += client.read();
    if (s.length()) { 
      char sbuf[1024]; 
      s.toCharArray(sbuf, sizeof(sbuf));
      DEBUG("TCP_DATA: %s", sbuf); 
      s = "";
    }
*/
  }
}


