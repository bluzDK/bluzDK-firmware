#include "application.h"

int blink(String command)
{
    digitalWrite(D7, HIGH);
    HAL_Delay_Milliseconds(250);
    digitalWrite(D7, LOW);
    HAL_Delay_Milliseconds(250);
    return 1;
}


/* executes once at startup */
void setup() {
//    Serial1.begin(38400);
    pinMode(D7, OUTPUT);
    Particle.function("blink", blink);
}

//uint32_t nextRun = 30000;
/* executes continuously after setup() runs */
void loop() {
//    Serial1.print("Millis = ");
//    Serial1.print(millis());
//    Serial1.println("");
//    if (millis() > nextRun) {
//        if (Particle.connected()) {
//            Particle.publish("Millis on the 30", String(millis()));
//        }
//        nextRun = millis() + 30000;
//    }
    System.sleep(SLEEP_MODE_CPU);
}
