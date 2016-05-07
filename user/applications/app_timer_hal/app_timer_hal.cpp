

#include "application.h"
#include "rtc_timer.h"

void tmcallback()
{
  if (Particle.connected())
    Particle.publish("HOLY CRAP BATMAN!!! IT WORKS!!!");
}

RTCTimer *testTimer = new RTCTimer(200000L, tmcallback, false);

void setup()
{
}

void loop()
{
  static bool once = true;
  if (once && Particle.connected() && BLE.getState() == BLE_CONNECTED) {
    once = false;
    Particle.publish("I'm back!");
    testTimer->start();
  }
}

