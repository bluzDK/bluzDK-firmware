#include "application.h"

#if DEBUG==y
Serial1DebugOutput debugOutput(38400);
#endif

TCPClient client;
// IPAddress ip = { 10, 1, 10, 175 };
IPAddress ip = { 10, 0, 1, 112 };


int blink(String pin) {
    digitalWrite(D7, HIGH);
    delay(200);
    digitalWrite(D7, LOW);
    return 0;
}
    
void setup()
{
    pinMode(D7, OUTPUT);
    Serial1.begin(38400);
    Serial1.print("\E[2J\E[H"); // xterm/ANSI terminal escape sequence to clear screen and home cursor
    Serial1.println("Starting TCPClient Class Testing");
    Particle.function("blink", blink);
}

void loop()
{
  static enum { START, CLOUD, TCP, RECEIVE, DONE, STOP } state;
  static uint32_t time_ms;
  const int TIMEOUT = 10000;
  int bytes;


  switch (state) 
  {
    case START:
      Serial1.print("Conecting to cloud ... ");
      state = CLOUD;
      break;

    case CLOUD:
      if (Particle.connected())
      {
        Serial1.println("ONLINE!");
        Serial1.println("Initiating TCP client connection ...");
        state = TCP;
      }
      break;

    case TCP:
      // if (client.connect(ip, 22)) // a public time server 
      if (client.connect("india.colorado.edu", 13)) // a public time server 
      {
        Serial1.println("TCP connected! Remote IP:" + String(client.remoteIP()) );
        Serial1.println("Expecting time string ...");
        time_ms = millis();
        state = RECEIVE;
      }
      else
      {
        Serial1.println("TCP connection failed! :-(");
        state = DONE;
      }
      break;
      
    case RECEIVE:
      if (!client.connected())
      {
        Serial.println("Remote server closed the connection!");
        state = STOP;
      }
      else if (millis() > (time_ms + TIMEOUT)) {
        Serial1.println("Nothing heard for " + String(TIMEOUT/1000) + " seconds.");
        state = DONE;
      }
      else if (client.available())
      { 
        time_ms = millis(); // reset our timeout clock
        
        bytes = client.available();
        Serial1.print("TCP client received ");
        Serial1.print(bytes);
        Serial1.println(" bytes ...");
        Serial1.println();

        char s[bytes+1];
        client.read((uint8_t *)s, bytes);
        s[bytes] = '\0';
        Serial1.println(s);
      }
      break;

    case DONE:
        Serial1.println("We're done. Going to sleep.");
        state = STOP;
      break;

    case STOP:
      break;

    default:
      state = START;
      break;
  }

  System.sleep(SLEEP_MODE_CPU);
}


