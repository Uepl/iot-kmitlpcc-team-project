//Transmitter Arduino
#include <SPI.h>
#include <nRF24L01p.h>
nRF24L01p transmitter(8, 7); //CSN,CE (D8,D4)

void setup() {
  Serial.begin(115200);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  transmitter.channel(69); // ตั้งช่องความถี่ให้ตรงกัน
  transmitter.TXaddress("MOLY"); // ตั้งชื่อตำแหน่งให้ตรงกัน ชื่อตั้งได้สูงสุด 5 ตัวอักษร
  transmitter.init();
}

float led1=0;
float led2=0;
float led3=0;
float led4=0;
void loop() {
    if(digitalRead(2)==HIGH){
      led1=!led1;
    }
    if(digitalRead(3)==HIGH){
      led2=!led2;
    }
    if(digitalRead(4)==HIGH){
      led3=!led3;
    }
    if(digitalRead(5)==HIGH){
      led4=!led4;
    }
    transmitter.txPL(led1); // ค่าที่ต้องการส่ง
    transmitter.txPL(led2);
    transmitter.txPL(led3);
    transmitter.txPL(led4);

    transmitter.send(FAST); // สั่งให้ส่งออกไป
    Serial.println(led1);
    Serial.println(led2);
    Serial.println(led3);
    Serial.println(led4);

    Serial.println("ส่ง");
    delay(250);
 
}
