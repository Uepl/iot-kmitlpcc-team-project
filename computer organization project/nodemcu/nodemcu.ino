#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <SoftwareSerial.h>

//==================serial func var=======================
SoftwareSerial nodemcu(D1,D2);
char c;
String dataIn;
int8_t indexofA, indexofB, indexofC, indexofD, indexofE;
String data1, data2, data3, data4, data5; 
const byte numChars = 32;
char receivedData[numChars];     //for received string with <> terminators
char receivedChars[numChars];    //for received string less <> terminators
char tempChars[numChars];
boolean newData = false;
//=======================================================

//==================Wifi func to sheet===================
const char* ssid = "---"; //--> Nama Wifi / SSID.
const char* password = "---"; //-->  Password wifi .

const char* host = "script.google.com";
const int httpsPort = 443;
WiFiClientSecure client; //--> Create a WiFiClientSecure object.
long now = millis();
long lastMeasure = 0;

String GAS_ID = "---"; //--> spreadsheet script ID
//=======================================================

void setup() {
  Serial.begin(57600);
  nodemcu.begin(9600);
  delay(5000);
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
  Serial.print("Connecting");
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  client.setInsecure();
}

void loop() {

  now = millis();
  if (now - lastMeasure > 10000) {
    lastMeasure = now;
    while(nodemcu.available()>0){
    c = nodemcu.read();

    data1 = data1.toFloat();
    data2 = data2.toFloat();
    data3 = data3.toFloat();
    data4 = data4.toFloat();
    data5 = data5.toFloat();
    
    Serial.println("Data1 = " + data1); 
    Serial.println("Data2 = " + data2); 
    Serial.println("Data3 = " + data3); 
    Serial.println("Data4 = " + data4); 
    Serial.println("Data5 = " + data5);  
    Serial.println("-----------------------------------------");
    
    if(c=='\n'){break;}
    else {dataIn+=c;}
    }
    if(c=='\n'){
      Parse_the_Data();
      c=0;
      dataIn="";
    }
  sendData(data1.toFloat(), data2.toFloat(), data3.toFloat(), data4.toFloat(), data5.toFloat());
  }
}

//=======================================================
void Parse_the_Data(){
  recv();
  indexofA = dataIn.indexOf("A");
  indexofB = dataIn.indexOf("B");
  indexofC = dataIn.indexOf("C");
  indexofD = dataIn.indexOf("D");
  indexofE = dataIn.indexOf("E");
  
  data1 = dataIn.substring(1,indexofA);
  data2 = dataIn.substring(indexofA+1,indexofB);
  data3 = dataIn.substring(indexofB+1,indexofC);
  data4 = dataIn.substring(indexofC+1,indexofD);
  data5 = dataIn.substring(indexofD+1,indexofE);
  delay(1000);
}

void recAstringChar(char rc) { // updates both receivedData and ReceivedChars
  static boolean recvInProgress = false;
  static byte ndx = 0;
  const char startMarker = '<';
  const char endMarker = '>';
  
  if (recvInProgress == true) {
    if (rc != endMarker) {
      receivedChars[ndx-1] = rc;
      receivedData[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
          ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx-1] = '\0';    // terminate the string
      receivedData[ndx] = rc;
      receivedData[ndx+1] = '\0';     // terminate the string
      recvInProgress = false;
      ndx = 0;
      newData = true;
    }
  }
  else if (rc == startMarker) {
    receivedData[ndx] = rc;
    ndx++;
    recvInProgress = true;
  }
} 

void recv() {
  while (nodemcu.available() > 0 && newData == false) {
        recAstringChar(nodemcu.read());
    }
}
//=======================================================

//==================Wifi func to sheet===================
void sendData(float x,float y,float z,float a,float b) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  float string_temp = x; 
  float string_humi = y;
  float string_light = z;
  float string_soilmos = a;
  float string_ph = b;
  String url = "/macros/s/" + GAS_ID + "/exec?temp=" + string_temp + "&humi="+string_humi + "&light="+string_light + "&soilmos="+string_soilmos + "&ph="+string_ph; //  2 variables 
  Serial.print("requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");
  Serial.println("request sent");
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
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  //----------------------------------------
} 
//=======================================================
