//NOT HARDWARE DESIGN PROJECT 
//  ACTING SUBLT.SILAR SIRAMASAKUL
//
 
//servo motor (MG996R)  SIGNAL  -> D5           NODEMCU
//IR sensor 1           SIGNAL  -> D7           NODEMCU
//IR sensor 2           SIGNAL  -> D6           NODEMCU
//DISPLAY               SCL     -> D1           NODEMCU
//                      SDA     -> D2           NODEMCU
//
//LM2596S               OUT-    -> GND          NODEMCU
//                      OUT+    -> VIN          NODEMCU
//                      IN+     -> V+           POWER SUPPLY
//
//                      IN-     -> V-           POWER SUPPLY
//
 
 
 
 
//////////esp32s3
 
//servo motor (MG996R)  SIGNAL  -> 5           esp32s3
//IR sensor 1           SIGNAL  -> 7           esp32s3
//IR sensor 2           SIGNAL  -> 6           esp32s3
//DISPLAY               SCL     -> 9           esp32s3
//                      SDA     -> 8           esp32s3
//
//LM2596S               OUT-    -> GND          esp32s3
//                      OUT+    -> 5VIN          esp32s3
//                      IN+     -> V+           POWER SUPPLY
//
//                      IN-     -> V-           POWER SUPPLY
//
//esp32cam              IO2     -> 16            esp32s3
//                      IO4     -> 15            esp32s3
#define DEBUG 0
 
#define irpin 7
 
#define irpin2 6
#define servopin 5
 
#define waittime 1500
 
#define lcdtimer 1000
 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);
 
// #include <Servo.h>
#include <ESP32Servo.h>
Servo myservo;
 
#include <Wire.h> // 2. this too
 
void setup() {
  LCD.init();
  LCD.backlight();
  LCD.setCursor(0,0);
 
  Wire.begin(8,9); // 1. why u add this.
 
  pinMode(irpin, INPUT); 
  pinMode(irpin2, INPUT);
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);     
  myservo.attach(servopin,500,2400);
  Serial.begin(9600);
  Serial1.begin(9600,SERIAL_8N1,16,15);//rxtx
}
int count=0;
boolean state =true;
boolean ir1 = false;
boolean ir2 = false;
int last;
int lcdlast=0;
void upio();
 
void loop() {  
  upio();
 
  //--------------------------OPEN--------------------------------------------------//
  if(digitalRead(irpin)==0 && state){                             //d3 ir1 saw first
    delay(waittime);
 
    upio();
    myservo.write(90);
 
    Serial1.print("ir1");
    Serial1.print("\n");
 
    state = false;
    ir1= true;  
    last = millis();
 
    if(DEBUG)
      Serial.println(state);
  }
  if(digitalRead(irpin2)==0 && state){                            //d4 ir2  saw first 
    delay(waittime);
 
    upio();
    myservo.write(90);
 
    Serial1.print("ir2");
    Serial1.print("\n");
 
    state = false;
    ir2 = true;
    last = millis();
    if(DEBUG)
      Serial.println(state);
  }
  //--------------------------TIMEOUT--------------------------------------------------//
  if(!state && (millis()-last)>waittime && digitalRead(irpin)==1 && digitalRead(irpin2)==1){
    state =true;
    ir1=false;
    ir2=false;
    myservo.write(0);
    // LCD.print("livobopolibplvn"); 
    upio();
    Serial.println("closed: timeout");
  }
 
  //--------------------------CLOSE--------------------------------------------------//
  if(digitalRead(irpin2)==0 && ir1 && (state == false)){ 
    delay(waittime);        
    state =true;
    ir1=false;
    count++;
    myservo.write(0);
    upio();
    Serial.print("Count= ");
    Serial.println(count);
  }
  if(digitalRead(irpin)==0 && ir2 && (state == false)){
    delay(waittime);
    state =true;
    ir2=false;
    count--;
    myservo.write(0);
    upio();
    Serial.print("Count= ");
    Serial.println(count); 
  }
 
  if(millis()-lcdlast > lcdtimer){
    lcdlast = millis();
    LCD.clear();
  } 
 
}
 
 
void upio(){
 
  LCD.setCursor(0,0);
  LCD.print("Parking lot");
  LCD.setCursor(0,1);
  LCD.print(count);
  LCD.setCursor(3,1);
  LCD.print("/ 5");
}