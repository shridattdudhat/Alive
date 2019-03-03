//#include "SoftwareSerial.h"

#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

#define PIN D7
#define NUMPIXELS 2
#define BLYNK_PRINT Serial
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned int PM1 = 0; //initializing values for the particulate matter
unsigned int PM25 = 0;
unsigned int PM10 = 0;
unsigned int count = 0;

byte data[24];

BLYNK_WRITE(V4)
{
  int R = param[0].asInt();
  int G = param[1].asInt();
  int B = param[2].asInt();
  
  for(int i=0;i<NUMPIXELS;i++)
  {
    pixels.setPixelColor(i, pixels.Color(R,G,B));
    pixels.show();
  }
}

void setup()
{

  pixels.begin();

  blinking(2000,250,8);
  
  Serial.begin(9600);
  Blynk.begin("e1febb568b4e4fd1b520934393f48a5c", "Imagineering", "Shr1Meja1");

  for(int j=0;j<NUMPIXELS;j++)
  {
  pixels.setPixelColor(j, pixels.Color(255,255,255));
  pixels.show();
  }
  
}

void loop()
{ 
  
  Blynk.run();
                       
  byte i = 0;
  int checksum = 0;
  Serial.flush();
  while (Serial.available()) {
    data[i] = Serial.read();                //reads serial data from sensor
    if (i <= 21)                            //condition for the sensor reading data
    {
      checksum += data[i];
    }
    if (i == 23) {
      if (checksum == ((256 * data[22]) + data[23]))
      {
        if (data[0] == 66)
        {
          if (data[1] == 77)
          {
            if (i == 23)
            {
              PM1 += ((data[4] * 256) + data[5]);  
              //retrives and stores data for PM1
              PM25 += ((data[6] * 256) + data[7]); 
              //retrives and stores data for PM2.5
              PM10 += ((data[8] * 256) + data[9]);
              //retrives and stores data for PM10
              count++; 
              Serial.flush(); //waits for serial data to transmit
              break;
            }
          }
        }
      }
      else
      {
        break;
      }
    }
     i++;
    delay(10);
  }

  if(count != 0 )
  {
//  Serial.println(PM1/count);
//  Serial.println(PM25/count);
//  Serial.println(PM10/count);
//  Serial.println("Code Running");

  Blynk.virtualWrite(V1, PM1/count);
  Blynk.virtualWrite(V2, PM25/count);
  Blynk.virtualWrite(V3, PM10/count);
  
  }

}


void blinking(int ontime, int offtime, int loopCount)
{
  for(int k=0;k<loopCount;k++)
  {
  
  for(int j=0;j<NUMPIXELS;j++)
  {
  pixels.setPixelColor(j, pixels.Color(203,4,25));
  pixels.show();
  }

  delay(ontime);
  
  for(int j=0;j<NUMPIXELS;j++)
  {
  pixels.setPixelColor(j, pixels.Color(0,0,0));
  pixels.show();
  }
  
  delay(offtime);
  
  }  
}
