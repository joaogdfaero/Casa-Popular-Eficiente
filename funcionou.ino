
#include <dht.h>
#include <SoftwareSerial.h>
String apiKey = "3FSOOWSNE2XEJZY7";

dht DHT;
#define DHT22_PIN 8
int humi,temp;
SoftwareSerial ser(10,11); // TX, RX
void setup(){
 ser.begin(9600);
    Serial.begin(9600);  
    unsigned char check_connection=0;
    unsigned char times_check=0;
    Serial.println("Connecting to Wifi");
  while(check_connection==0)
  {
   Serial.print("..");
   ser.print("AT+CWJAP=\"homewifi_4E6\",\"10830020\"\r\n");
   ser.setTimeout(5000);
 if(ser.find("WIFI CONNECTED\r\n")==1 )
 {
 Serial.println("WIFI CONNECTED");
 break;
 }
 times_check++;
 if(times_check>3) 
 {
  times_check=0;
  Serial.println("Trying to Reconnect..");
  }
  }
  delay(5000);
}
void loop()
{
  DHT.read22(DHT22_PIN);
  humi=DHT.humidity;
  temp=DHT.temperature;
  Serial.print("H:");
  Serial.print(humi);
  Serial.println("%");
  Serial.print("T:");
  Serial.print(temp);
  Serial.println("C");
  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  // prepare GET string
  String getStr = "GET /update?api_key=3FSOOWSNE2XEJZY7&field1&field2=0";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(temp);
  getStr +="&field2=";
  getStr += String(humi);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
    Serial.println(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    Serial.println("CIPCLOSE");
  }
    
  // thingspeak needs 15 sec delay between updates
 delay(16000);  
}
