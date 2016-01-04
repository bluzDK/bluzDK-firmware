#include "application.h"
#include "PietteTech_DHT.h"
#include "DS18B20.h"
#include "Particle-OneWire.h"

#define DHTTYPE  DHT11             // Sensor type DHT11/21/22/AM2301/AM2302
#define DHTPIN   3         	    // Digital pin for communications
#define DHT_SAMPLE_INTERVAL   4000  // Sample Air Temp every 4 seconds

Serial1DebugOutput debugOutput(38400);          // default is 9600 and log everything

DS18B20 ds18b20 = DS18B20(D2);

void dht_wrapper();
PietteTech_DHT DHT(DHTPIN, DHTTYPE, dht_wrapper);


char szInfo[75];
char status[64];
float pubTemp;
double celsius;
double fahrenheit;
float airtemp = 0;
float airhumidity = 0;
unsigned long metric_Publish_Rate = 30000;
unsigned long metricNextPublishTime = 35000;
unsigned long tempNextSampleTime = 30000;
unsigned long temp_Sample_Rate = 10000;
unsigned long dsNextSampleTime = 30000;
unsigned long ds_Sample_Rate = 10000;
uint8_t dsAttempts = 0;
bool bDHTstarted;
bool online;

void getairtemp();
void getwatertemp();
void publishData();
/* executes once at startup */
void setup() {
//    pinMode(D7, OUTPUT);
//    digitalWrite(D7, HIGH);
//  Serial1.begin(38400);
//  DEBUG("Hello World");
  //pinMode(D3, INPUT);
  //pinMode(D2, INPUT);
}

void dht_wrapper() {
    DHT.isrCallback();
    }

/* executes continuously after setup() runs */
void loop() {
  if (millis() > tempNextSampleTime){
    DEBUG("Air Sample Time!");
    getairtemp();
    }
  if (millis() > dsNextSampleTime){
    DEBUG("Water Sample Time!");
    getwatertemp();
    }
  if (millis() > metricNextPublishTime){
    DEBUG("Publish Time!");
    publishData();
    }

  /*if (Particle.connected()){
      System.sleep(SLEEP_MODE_DEEP);
    }*/
}

void publishData(){
//  if(!ds18b20.crcCheck()){
  //  return;
  //}
  sprintf(szInfo, "Air Temp is: %2.2f and Humidity is: %2.2f. Water Temp is: %2.2f" , airtemp, airhumidity, fahrenheit);
  Particle.publish("bluzTmp", szInfo, PRIVATE);
  metricNextPublishTime = millis() + metric_Publish_Rate;
  DEBUG("Next Publish in: %d" + metricNextPublishTime);
}

void getwatertemp(){
    if(!ds18b20.search()){
      ds18b20.resetsearch();
      celsius = ds18b20.getTemperature();
      DEBUG("Water Temp Celsius: %d", celsius);
      while (!ds18b20.crcCheck() && dsAttempts < 4){
        DEBUG("Caught bad value.");
        dsAttempts++;
        DEBUG("Attempts to Read: %d", dsAttempts);
        if (dsAttempts == 3){
          HAL_Delay_Milliseconds(1000);
        }
        ds18b20.resetsearch();
        celsius = ds18b20.getTemperature();
        continue;
      }
      dsAttempts = 0;
      fahrenheit = ds18b20.convertToFahrenheit(celsius);
      dsNextSampleTime = millis() + ds_Sample_Rate;
      DEBUG("Water Temp Fahrenheit: %d", fahrenheit);
    }
}

void getairtemp(){
  tempNextSampleTime = millis() + temp_Sample_Rate;  // set the time for next sample
  if (!bDHTstarted) {		// start the sample
      DEBUG("Acquiring from DHT.");
      DHT.acquire();
      bDHTstarted = true;
      DEBUG("bDHTstarted is: %d", bDHTstarted);
      } //End of DHTstarted IF
      DEBUG("Checking for Sample Completion.");
      if (!DHT.acquiring())
     {		// has sample completed?
      DEBUG("Not acquiring.");
      DEBUG("Getting Status");
    	int result = DHT.getStatus();
      switch (result) {
    case DHTLIB_OK:
        DEBUG("OK");
        break;
    case DHTLIB_ERROR_CHECKSUM:
      DEBUG("Error\n\r\tChecksum error");
        //status = "Checksum error.";
        break;
    case DHTLIB_ERROR_ISR_TIMEOUT:
      DEBUG("Error\n\r\tISR time out error");
        //status = "ISR Timeout.";
        break;
    case DHTLIB_ERROR_RESPONSE_TIMEOUT:
      DEBUG("Error\n\r\tResponse time out error");
        //status = "Response Timeout.";
        break;
    case DHTLIB_ERROR_DATA_TIMEOUT:
       DEBUG("Error\n\r\tData time out error");
        //status = "Data Timeout error.";
        break;
    case DHTLIB_ERROR_ACQUIRING:
        DEBUG("Error\n\r\tAcquiring");
        //status = "Error Acquiring";
        break;
    case DHTLIB_ERROR_DELTA:
       DEBUG("Error\n\r\tDelta time to small");
      //status = "Delta time too small.";
        break;
    case DHTLIB_ERROR_NOTSTARTED:
        DEBUG("Error\n\r\tNot started");
      //status = "Error Not Started.";
        break;
    default:
        DEBUG("Unknown error");
    //  status = "Unknown Error.";
        break;
        }
      DEBUG("Getting Fahrenheit.");
      airtemp = (DHT.getFahrenheit());
      DEBUG("Getting Humidity.");
      airhumidity = (DHT.getHumidity());
      //n++;  // increment DHT Sample counter
      bDHTstarted = false;  // reset the sample flag so we can take another
      //tempNextSampleTime = millis() + temp_Sample_Rate;  // set the time for next sample
      DEBUG("Next Temp Sample in: %d", tempNextSampleTime);
    }
}
