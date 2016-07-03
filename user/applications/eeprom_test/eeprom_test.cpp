#include "application.h"

/* executes once at startup */
void setup() {
    Serial1.begin(38400);
    Serial1.println("Hello EEPROM Tester!");

    EEPROM.clear();

    int length = EEPROM.length();
    Serial1.print("We have EEPROM available of size: ");
    Serial1.println(length);
    Serial1.println("");

    uint16_t addr = 100;
    uint8_t value;

    value = EEPROM.read(addr);
    Serial1.print("Value currently at address is: ");
    Serial1.println(value);
    Serial1.println("");

    Serial1.println("Writing to change it to 182");
    uint8_t newVal = 182;
    EEPROM.write(addr, newVal);
    Serial1.println("");

    value = EEPROM.read(addr);
    Serial1.print("Value after storage is: ");
    Serial1.println(value);
    Serial1.println("");

    addr = 50;
    struct MyObject {
        int version;
        int type;
        int state;
    };
    MyObject myObj, newObj;
    myObj.state = 3;
    myObj.type = 19;
    myObj.version = 53;


    EEPROM.get(addr, newObj);

    Serial1.print("Version currently is: ");
    Serial1.println(newObj.version);
    Serial1.println("");

    Serial1.print("Writing object to address: ");
    Serial1.println(addr);
    Serial1.println("");
    EEPROM.put(addr, myObj);

    EEPROM.get(addr, newObj);
    Serial1.print("Version is now: ");
    Serial1.println(newObj.version);
    Serial1.println("");
}

/* executes continuously after setup() runs */
void loop() {
}
