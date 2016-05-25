#include "application.h"

TCPClient client;
IPAddress ip = { 10, 0, 1, 112 };
    
void setup()
{
}

void loop()
{
  static bool oneshot = true;
  if (oneshot && Particle.connected())
  {
    Particle.publish("TCPClient Test", "ONLINE");
    oneshot=false;
    if (client.connect(ip, 22))
    {
      Particle.publish("TCP connect", "success!");
    }
    else
    {
      Particle.publish("TCP connect", "failed :-(");
    }
  }

}


