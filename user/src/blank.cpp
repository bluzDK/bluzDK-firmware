#include "application.h"
#include "rtc_timer.h"

Serial1DebugOutput debugOutput(38400);          // default is 9600 and log everything

char message[25];

//char *hello = "Hello World";


uint32_t m = 0;
int blink(String command)
{
    digitalWrite(D7, HIGH);
    HAL_Delay_Milliseconds(250);
    digitalWrite(D7, LOW);
    HAL_Delay_Milliseconds(250);
    return 1;
}

void radioCallbackHandler(bool radio_active) {
    //Do something about it
}

void timerCallback(void *context)
{
  if (BLE.getState() == BLE_CONNECTED)
    Particle.publish("OMG IT WORKED!!");
}

RTCTimer testTimer(20, timerCallback);

/* executes once at startup */
void setup() {
//  testTimer.start();
//    Particle.variable("hello", hello, STRING);
//    Particle.variable("mess", message, STRING);
//    BLE.registerNotifications(radioCallbackHandler);
//    
//    pinMode(D7, OUTPUT);
//    Particle.function("blink", blink);
//    DEBUG("Starting Wiring Example!");
//    Wire.setSpeed(100000);
//    DEBUG("Set wire speed");
//    Wire.begin();
//    DEBUG("Began wiring");
//    
//    Wire.beginTransmission(0x67);
//    DEBUG("Began wiring transmission");
//    Wire.write(0x00);
//    DEBUG("Wrote to wiring");
//    Wire.endTransmission();
//    DEBUG("Finished TWI");
    
//    pinMode(D7, OUTPUT);
//    
//    SPI.begin();
//    SPI.transfer(0x03);
//    
//    digitalWrite(D7, HIGH);
}

int adder = 0;
bool ledOn = false;
bool once = false;
/* executes continuously after setup() runs */
void loop() {
  static bool one = true;
  if (one && BLE.getState() == BLE_CONNECTED) {
    one = false;
    Spark.publish("ERROR CODE: ", String(testTimer.getError()) );
  }

    if (Particle.connected() && once == false && millis() > 30000) {
//        DEBUG("Connection Interval: %d", BLE.getConnectionInterval());
        once = true;
    }
    System.sleep(SLEEP_MODE_DEEP);
//    m = micros();
//    if (Particle.connected() && millis() % 1000 == 0) {
//        if (ledOn) {
//            digitalWrite(D7, LOW);
//        } else {
//            digitalWrite(D7, HIGH);
//        }
//        ledOn = !ledOn;
//    }
//    if (Particle.connected() && millis() > 1000 && millis() % 5000 == 0) {
//        DEBUG("Abut to kill the system!");
//        Particle.publish("This is not a test", "Did it work?");
//    }
//    if (millis() % 2000 == 0) {
//        BLEState state = BLE.getState();
//        DEBUG("State = %d", state);
//        sprintf(message, "I have run %d times", (int)(adder++));
//    }
}
