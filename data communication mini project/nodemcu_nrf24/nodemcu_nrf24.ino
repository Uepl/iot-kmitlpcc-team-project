#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <SPI.h>
#include <nRF24L01p.h>
#define pinLed1 1
#define pinLed2 4
#define pinLed3 5
#define pinLed4 16
nRF24L01p receiver(D4,D3); //csn,ce
//----------------------------------------


const char* ssid = "";
const char* password = "";
//----------------------------------------

//----------------------------------------Host & httpsPort
const char* host = "script.google.com";
const int httpsPort = 443;
//----------------------------------------

WiFiClientSecure client; //--> Create a WiFiClientSecure object.

// Timers auxiliar variables


String GAS_ID = "---"; //--> spreadsheet *SCRIPT* ID //ใส่ Script ID ของ Spreadsheet



//============================================ void setup
void setup() {
  // put your setup code here, to run once:
  delay(150);
  Serial.begin(115200); //ตั้ง baudrate เป็น 115200
  delay(500);
  
  pinMode(pinLed1,OUTPUT);
  pinMode(pinLed2,OUTPUT);
  pinMode(pinLed3,OUTPUT);
  pinMode(pinLed4,OUTPUT);
  
  WiFi.begin(ssid, password); //--> Connect to your WiFi router //เริ่มเชื่อมต่อกับ WIFI ที่เรากำหนดให้
  Serial.println("");
  //---------------------------------------- Wait for connection //รอการเชื่อมต่อกับ WIFI
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
   

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  receiver.channel(69);
  receiver.RXaddress("MOLY");
  receiver.init();

  client.setInsecure();
  
}

float led1=0;
float led2=0;
float led3=0;
float led4=0;
long timer=millis();

void loop() {
  receiver.read();
  receiver.rxPL(led1);
  receiver.rxPL(led2);
  receiver.rxPL(led3);
  receiver.rxPL(led4);
  //
  Serial.print("led1 <- ");
  Serial.print(led1);
  if(led1==1){
    digitalWrite(pinLed1,LOW);  
    Serial.println("\tON");
  }
  else if(led1==0){
    digitalWrite(pinLed1,HIGH);
    Serial.println("\tOFF");
  }
  //
  Serial.print("led2 <- ");
  Serial.print(led2);
  if(led2==1){
    digitalWrite(pinLed2,LOW);
    Serial.println("\tON");
  }
  else if(led2==0){
    digitalWrite(pinLed2,HIGH);
    Serial.println("\tOFF");
  }
  //
  Serial.print("led3 <- ");
  Serial.print(led3);
  if(led3==1){
    digitalWrite(pinLed3,LOW);
    Serial.println("\tON");
  }
  else if(led3==0){
    digitalWrite(pinLed3,HIGH);
    Serial.println("\tOFF");
  }
  //
  Serial.print("led4 <- ");
  Serial.print(led4);
  if(led4==1){
    digitalWrite(pinLed4,LOW);
    Serial.println("\tON");
  }
  else if(led4==0){
    digitalWrite(pinLed4,HIGH);
    Serial.println("\tOFF");
  }
  
  if(millis()-timer>=2000){
    sendData(led1,led2,led3,led4); //sendData(ข้อมูลที่จะส่ง1,ข้อมูลที่จะส่ง2,ข้อมูลที่จะส่ง3,ข้อมูลที่จะส่ง4,ข้อมูลที่จะส่ง5)
    timer=millis();
  }
  delay(200);
}

void sendData(float value,float value2, float value3, float value4) { //ใช้เพิ่มตัวแปรและกำหนดตัวแปรที่จะส่งข้อมูล
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  
  //----------------------------------------Connect to Google host //เชื่อมต่อกับ Host ของ Google
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  //----------------------------------------Process Data //ในตอนนี้บรรทัดที่ 103-107 เป็นการกำหนดชื่อตัวแปรข้อมูลที่จะส่ง และ บรรทัดที่ 108 เป็นการทำ url ที่จะส่งข้อมูล

  float string_led1 = value; 
  float string_led2 = value2;
  float string_led3 = value3;
  float string_led4 = value4;
  String url = "/macros/s/" + GAS_ID + "/exec?led1=" + string_led1 + "&led2=" + string_led2 + "&led3=" + string_led3 + "&led4=" + string_led4; //  4 variables 
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  //----------------------------------------

  //---------------------------------------
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
} 
