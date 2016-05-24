/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social groups:              http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************/
#define BLYNK_DEBUG // Uncomment this to see debug prints
#define BLYNK_PRINT Serial
#include "blynk/blynk.h"

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "4229e55cba0944ccaab6e8786167bde0";

// Attach a Button widget (mode: Switch) to the Digital pin 7 - and control the built-in blue led.
// Attach a Graph widget to Analog pin 1
// Attach a Gauge widget to Analog pin 2
// No coding is required for direct pin operations!

WidgetTerminal terminal(V3);
WidgetLCD lcd(V4);
WidgetLED ledLightStatus(6);

void setup()
{
    Serial.begin(9600);
    Blynk.begin(auth);
//    pinMode(D6, OUTPUT);
//    digitalWrite(D6, 0);
    while (!Blynk.connect());  // Wait until connected
    lcd.clear();
}

// Attach a Button widget (mode: Push) to the Virtual pin 1
BLYNK_WRITE(V1) {
    static int count = 0;
    char buf[255];
    if (param.asInt() == 1) { // On button down...
    
        sprintf(buf, "Coming to live from a big red button on a #Blynk powered #particle core [U=%d]", count++);
        Blynk.tweet(buf);

        terminal.println("You pressed the button and I know it! ;)");
        terminal.flush();
        
        lcd.print(0, 0, "Hey! Don't push!");
        lcd.print(10, 1, count);
    } else {
        lcd.print(0, 0, "                ");
    }
    
    
}

// Attach a Slider widget to the Virtual pin 2 - and control the built-in RGB led!
BLYNK_WRITE(V2) {
    if (param.asInt() > 0) {
        RGB.control(true);
        
        byte rgb[3];
        HsvToRgb(param.asDouble()/255, 1, 1, rgb);
        RGB.color(rgb[0], rgb[1], rgb[2]);
    } else {
        RGB.control(false);
    }
}

void loop()
{
    static int lightStatus = 0;
    
    Blynk.run();
    
    if (ModeBtnPressed()) {
        Blynk.notify("Mode button was pressed");
    }
    
    // Update Blynk LED on V6
    int d6Now = digitalRead(D6);
    if (d6Now != lightStatus) {
        lightStatus = d6Now;
        ledLightStatus.setValue(d6Now ? 255 : 0);
        terminal.printf("Light switched %s\n", (d6Now) ? "ON" : "OFF");
        terminal.flush();
    }    
    
}

// *** Utility functions

bool ModeBtnPressed() {
    if(millis() > 5000) {
        if(BUTTON_GetDebouncedTime(BUTTON1) >= 50) {
            BUTTON_ResetDebouncedState(BUTTON1);
            return 1;
        }
    }
    return 0;
}

void HsvToRgb(double h, double s, double v, byte rgb[]) {
    double r, g, b;

    int i = int(h * 6);
    double f = h * 6 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    switch(i % 6){
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    rgb[0] = r * 255;
    rgb[1] = g * 255;
    rgb[2] = b * 255;
}



