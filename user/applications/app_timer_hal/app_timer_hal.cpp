

#include "application.h"
#include "rtc_timer.h"

void tmcallback()
{
  if (Particle.connected())
    Particle.publish("HOLY CRAP BATMAN!!! IT WORKS!!!");
}

void tmcallback2()
{
  if (Particle.connected())
    Particle.publish("Yiiiup! Me too! :-)");
}

RTCTimer *testTimer = new RTCTimer(6300, tmcallback, false);
RTCTimer *testTimer2 = new RTCTimer(20000, tmcallback2, false);

void setup()
{
    testTimer->start();
    testTimer2->start();
}

void loop()
{
  static bool once = true;
  if (once && Particle.connected() == BLE_CONNECTED) {
    once = false;
    Particle.publish("I'm back!");
  }
}

