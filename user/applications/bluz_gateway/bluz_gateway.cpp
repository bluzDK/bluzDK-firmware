/*
 ******************************************************************************
  Copyright (c) 2015 Particle Industries, Inc.  All rights reserved.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "application.h"
#include "stdarg.h"

PRODUCT_ID(PLATFORM_ID);

#define xstr(s) str(s)
#define str(s) #s

#define TARGET_NAME_ADDR 10
struct TargetName {
    uint8_t length;
    char name[24];
};


/* Function prototypes -------------------------------------------------------*/
int setConnectionParameters(String command);
int setGatewayTarget(String name);

String version = xstr(SYSTEM_VERSION_STRING);

/* This function is called once at start up ----------------------------------*/
void setup()
{
    //Setup the Tinker application here

    //Register all the Tinker functions
    Particle.function("setConParam", setConnectionParameters);
    Particle.function("setGWTarget", setGatewayTarget);

    Particle.variable("version", version);

    TargetName setTarget;
    EEPROM.get(TARGET_NAME_ADDR, setTarget);
    if (setTarget.length > 0 && setTarget.length != 0xFF) {
        BLE.setGatewayTargetName(setTarget.name);
    }

    //gateway shield
#if PLATFORM_ID==269
    pinMode(D5, INPUT_PULLDOWN);
//    if (digitalRead(D5)) {
//        SYSTEM_MODE(AUTOMATIC);
//    } else {
//        SYSTEM_MODE(MANUAL);
//    }
#endif

}

/* This function loops forever --------------------------------------------*/
void loop()
{
    System.sleep(SLEEP_MODE_CPU);
    //This will run in a loop
}

int setConnectionParameters(String cmd)
{
    int minimm = atoi(cmd.substring(0, cmd.indexOf(',')));
    int maximum = atoi(cmd.substring(cmd.indexOf(',')+1, cmd.length()));

    Particle.publish("Setting conn params to min: " + String (minimm) + " max: " + String(maximum));
    BLE.setConnectionParameters(minimm, maximum);
    return 0;
}

int setGatewayTarget(String name)
{
    char c_name[name.length()+1];
    name.toCharArray(c_name, name.length()+1);
    Particle.publish("Setting gateway target to: ", String(c_name));
    BLE.setGatewayTargetName(c_name);
    TargetName setTarget;
    setTarget.length = (uint8_t)name.length()+1;
    memcpy(setTarget.name, c_name, setTarget.length);
    EEPROM.put(TARGET_NAME_ADDR, setTarget);

    return 0;
}