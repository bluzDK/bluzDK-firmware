#include "application.h"

void setup() {

}

#define TIME_TO_WAIT 5000 /* milliseconds */

typedef enum {
    WAIT_ONLINE,
    ALIVE,
    PUBLISH,
    SET_TIMER,
    WAIT
} FSM_state_t;

FSM_state_t myState = WAIT_ONLINE;
uint32_t saveTime;

void loop() {

    switch (myState) {

        case WAIT_ONLINE: // stay in this state  until we're connected to the big old cloud in the sky
            if (Particle.connected()) myState = ALIVE;
            break;

        case ALIVE: // we're alive! shout about it
            Particle.publish("Came online at", String(millis()) );
            myState = SET_TIMER; // next time through loop(), we'll set up our custom timer
            break;

        case PUBLISH: // it's been TIME_TO_WAIT milliseconds -- make some noise!
            if ( !Particle.connected() ) { // first though ... did we get disconnected somehow?
                myState = WAIT_ONLINE;
            } else {
                Particle.publish("PING!! at ", String(millis()) );
                myState = SET_TIMER;
            }
            break;

        case SET_TIMER: // record the current time for the WAIT state to reference back to
            saveTime = millis();
            myState = WAIT;
            break;

        case WAIT: // stay in this state until TIME_TO_WAIT milliseconds have gone by since the SET_TIMER state
            if ( millis() > (saveTime + TIME_TO_WAIT) ) {
                
                // Time's up! But we'll drop out of loop() for and get it done next time through
                // This allows all the background network stuff the best chance of keeping up with business
                myState = PUBLISH; 
            }
            break;
        
        default:
            myState = WAIT_ONLINE;
    }          

    // We can save MUCH more battery drain this way too! ...
    System.sleep(SLEEP_MODE_CPU);

}


