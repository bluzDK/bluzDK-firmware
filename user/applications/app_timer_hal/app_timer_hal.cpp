

#include "application.h"
#include "rtc_timer.h"


class TimerTest : public RTCTimer
{
  uint16_t counter;

  public:
  TimerTest(uint32_t interval) 
    : RTCTimer(interval, nullptr) // no callback, as we'll be simply overriding RTCTimer::timeout 
  {
  };
  
  virtual void timeout()
  {
    if (Particle.connected())
    {
      if (millis() < 60000) 
        Particle.publish("Yiiiup! Me too! :-)", String(millis()));
      else
      {
        stop(); 
        Particle.publish("Yiiiup! I'm otta here!", String(millis()));
      }
    }
  };
};
TimerTest classMethodOverrider(20000);

///////////////////////////////////////////////////////////
// standard function as the timeout handler callback ...
void timer1_Callback();
RTCTimer timer1(6300, timer1_Callback, /*one_shot=*/true ); 

void timer1_Callback()
{
  if (Particle.connected())
  {
    Particle.publish("IT WORKS!!! :-D C'ya!", String(millis()));
  } 
}


void setup()
{
    classMethodOverrider.start();
}

void loop()
{
  static bool once = true;
  if (once && Particle.connected()) {
    once = false;
    Particle.publish("I'm back!", String(millis()));
    timer1.start();
  }

}

