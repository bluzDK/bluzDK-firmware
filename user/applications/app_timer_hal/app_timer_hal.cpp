

#include "application.h"
#include "apptimer_hal.h"

void tmcallback(void *p_context)
{
  if (Particle.connected())
    Particle.publish("HOLY CRAP BATMAN!!! IT WORKS!!!");
}

uint32_t id;
uint32_t err_code;
/* executes once at startup */
void setup() {
  err_code = HAL_app_timer_create(&id, APP_TIMER_MODE_REPEATED, tmcallback);
  HAL_app_timer_start(id, 200000L, NULL);
}

/* executes continuously after setup() runs */
void loop() {
  static bool once = true;
  if (once && Particle.connected() && BLE.getState() == BLE_CONNECTED) {
    once = false;
    Particle.publish("err_code=", String(err_code));
  }
}
