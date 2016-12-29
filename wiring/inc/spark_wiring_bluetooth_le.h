/**
 Copyright (c) 2015 MidAir Technology, LLC.  All rights reserved.
 
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
 */

#ifndef __SPARK_WIRING_BLUETOOTH_LE_H
#define __SPARK_WIRING_BLUETOOTH_LE_H

#include "spark_wiring.h"
#include "spark_wiring_platform.h"
#include "bluetooth_le_hal.h"
    
enum BLEState {
    BLE_OFF,
    BLE_ADVERTISING,
    BLE_SLEEPING,
    BLE_GAP_CONNECTED,
    BLE_CONNECTED,
};

class BLEClass {
private:

public:
    BLEClass(void) {};
    BLEClass(char* name) {
        HAL_BLE_Set_Adv_Name(name);
    };
    
    static BLEState getState();
    
    //information
    static uint32_t getConnectionInterval();
    
    //advertising functions
    static void startAdvertising();
    static void stopAdvertising();
    
    //actions
    static void disconnect();
    
    //notification handler
    static int registerNotifications(void (*radio_callback)(bool radio_active));

    //data callback handler
    static void registerDataCallback(void (*data_callback)(uint8_t *data, uint16_t length));

    //function to send data
    static void sendData(uint8_t *data, uint16_t length);

    //function to set TX power
    static void setTxPower(int power);

    //function to set the connection paramaters
    static void setConnectionParameters(int minimum, int maximum);

    //function to set the target name for the gateway to connect to
    static void setGatewayTargetName(char* name);

    //start iBeacon advertising
    static void beacon(uint32_t major, uint32_t minor, uint8_t *UUID);
    
};

extern BLEClass BLE;

#define BLE_ADV_NAME(name)  BLEClass AdvName(name);

#endif  // __SPARK_WIRING_BLUETOOTH_LE_H
