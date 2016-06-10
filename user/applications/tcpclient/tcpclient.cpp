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
    Serial1.println("Starting!");
    Particle.function("blink", blink);
}

void loop()
{
  static bool oneshot = true;
  static bool finished = false;
  if (oneshot && Particle.connected())
  {
    Serial1.println("TCPClient Test - ONLINE");
    oneshot=false;
    if (client.connect(ip, 22))
    {
      Serial1.println("TCP connect - success! Remote IP:" + String(client.remoteIP()) );
    }
    else
    {
      Serial1.println("TCP connect - failed :-(");
    }

  } 
  else if (!oneshot && !finished && !client.connected())
  {
    finished = true;
    Serial1.println("TCP Socket Closed!");
  }
  else if (!finished && client.available())
  {  
    int bytes = client.available();
    Serial1.print("TCP has data of: ");
    Serial1.print(bytes);
    Serial1.println(" bytes!");
    Serial1.println();

    char s[bytes+1];
    client.read((uint8_t *)s, bytes);
    s[bytes+1] = '\0';

    Serial1.println(s);

    client.write("BOO!\n"); // probably not a valid SSH handshake signal ;)
  }

  System.sleep(SLEEP_MODE_CPU);
}


