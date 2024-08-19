#include <SoftwareSerial.h>
//dht d2, ldr A0, ph A1, soil A2
//DHT 
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//LDR
#define LDRPIN A0

//PH E-201 C

#define PHPIN A1
long phTot, temTot;
float phAvg, temAvg;
int x;
float C = 7; //Constant of straight line (Y = mx + C)
float m = -6.80; // Slope of straight line (Y = mx + C)

//

#define SoilPIN A2

SoftwareSerial arduinoserial(10,11); // 10 คือ RX, 11 คือ TX

void setup() {
  Serial.begin(9600);
  arduinoserial.begin(9600);  

  dht.begin(); // data pin 2  
}

void loop() {
  
  //--------------------------------------------------------------------DHT
  //dht
  float Humidity = dht.readHumidity();
  float Temperature = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(Humidity) || isnan(Temperature) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }


  //--------------------------------------------------------------------LDR
  int Light;
  Light = analogRead(LDRPIN);

  int Soil;
  Soil=analogRead(SoilPIN);

  //--------------------------------------------------------------------PH
  phTot = 0;
  temTot = 0;
  phAvg = 0;
  temAvg = 0;
  for(x=0; x<10 ; x++)
    {
        phTot += analogRead(PHPIN);
        //temTot += analogRead(NULL);
        
    }
    //float temAvg = temTot/10;
    float phAvg = temTot/10;
    float phVoltage =  phAvg * (5.0 / 1023.0); //convert sensor reading into milli volt
    float pH = phVoltage*m+C;

  
  Serial.print("value:");
  Serial.print(Temperature);
  Serial.print(" value2:");
  Serial.print(Humidity);
  Serial.print(" value3:");
  Serial.print(Light);
  Serial.print(" value4:");
  Serial.print(Soil);
  Serial.print(" value5:");
  Serial.println(pH);
  
  
  arduinoserial.print("<");
  arduinoserial.print(Temperature);    arduinoserial.print("A");
  arduinoserial.print(Humidity);    arduinoserial.print("B");
  arduinoserial.print(Light);    arduinoserial.print("C");
  arduinoserial.print(Soil);    arduinoserial.print("D");
  arduinoserial.print(pH);    arduinoserial.print("E");
  arduinoserial.print("\n");
  arduinoserial.print("<");
  
  delay(100);

  
}
