/* Includes ------------------------------------------------------------------*/
#include "application.h"
int timeLastPublish = 0;
int timeBetweenPublishes = 5000;

void setup()
{
    //set Eastern Daylight Time
    Time.zone(-4);
}

void loop()
{
    System.sleep(SLEEP_MODE_CPU);
    if (Particle.connected() && millis() - timeLastPublish > timeBetweenPublishes)
    {
        timeLastPublish = millis();
        Particle.publish("Time", String(Time.month()) + "-" + String(Time.day()) + "-" + String(Time.year()) + " " + String(Time.hour()) + ":" +  String(Time.minute()) + ":" + String(Time.second()));
    }
}

