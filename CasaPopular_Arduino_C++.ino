//////////////////////// Configurando o DSB ///////////////////////////////////////
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);  

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

int deviceCount = 0;
float tempC;
float tempC1;
float tempC2;

////////////////// Consigurando o ultravioleta ///////////////////

float sensor = 0.0;
float raw    = 0.0;
float sensorUV;

////////////////// Configurando o DHT /////////////////////////////
#include <dht.h>
#include <SoftwareSerial.h>
String apiKey = "3FSOOWSNE2XEJZY7";

dht DHT;
#define DHT22_PIN1 8  //banheiro
#define DHT22_PIN2 9  //quarto 1
#define DHT22_PIN3 7 //quarto 2
#define DHT22_PIN4 11 //sala
#define DHT22_PIN5 12 //cozinha

int humi1,temp1,humi2,temp2,humi3,temp3;
humi4,temp4,humi5,temp5;
SoftwareSerial ser(10,11); // (TX, RX)

/////////////////////// GOOGLE SHEETS /////////////////////////////
const char WEBSITE[] = "api.pushingbox.com"; //pushingbox API server
const String devid = "v9A175EEEE26CF95"; //device ID from Pushingbox 
#define DST_HOST "api.pushingbox.com"
//////////////////////////////////////////////////////////////////// 

void setup(){ 
  Serial.begin(115200); //GOOGLE SHEETS
  WiFi.begin(MY_SSID, MY_PWD); //GOOGLE SHEETS 
  sensors.begin();  // Iniciando a biblioteca
  Serial.begin(9600);
  
  /// Buscando os sensores ///
  Serial.println("Locating devices...");
  Serial.print("Found ");
  deviceCount = sensors.getDeviceCount();
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");
  Serial.print("\n\n");
  
    ser.begin(9600);
    Serial.begin(9600); 
    sensors.begin();
 
  /// Checando a conexão dO ESP com o Wifi ///
    unsigned char check_connection=0;
    unsigned char times_check=0;
    Serial.println("Connecting to Wifi");
  
  while(check_connection==0){
   Serial.print("..");
   ser.print("AT+CWJAP=\"homewifi_4E6\",\"10830020\"\r\n");
   ser.setTimeout(5000);
   
 if(ser.find("WIFI CONNECTED\r\n")==1 )
 {
 Serial.println("WIFI CONNECTED");
 break;
 }
 times_check++;
 if(times_check>3) {
  times_check=0;
  Serial.println("Trying to Reconnect..");
                   }
                            }
  delay(5000);}

void loop(){

//// Obtendo as temperatudas dos DSBs //////
  sensors.requestTemperatures(); 
  
/// Mostra a temperatura de cada sensor ///
  for (int i = 0;  i < deviceCount;  i++){
      tempC = sensors.getTempCByIndex(i);
      tempC1 = sensors.getTempCByIndex(0);
      tempC2 = sensors.getTempCByIndex(1);
  
  Serial.print("DSB1: ");
  Serial.print(tempC1);
  Serial.println(" ºC");
  Serial.print("DSB2: ");
  Serial.print(tempC2); 
  Serial.println(" ºC");                 }
  
  delay(5000);
  
  //////// Obtendo as temperaturas dos DHTs ///////////
  
DHT.read22(DHT22_PIN1);
humi1=DHT.humidity;
temp1=DHT.temperature;
Serial.print("DHT1(umidade): ");
Serial.print(humi1);
Serial.println(" %");
Serial.print("DHT1(temperatura): ");
Serial.print(temp1);
Serial.println(" ºC (DHT)");
 
DHT.read22(DHT22_PIN2);
humi2=DHT.humidity;
temp2=DHT.temperature;
Serial.print("DHT2(umidade): ");
Serial.print(humi2);
Serial.println(" %");
Serial.print("DHT2(temperatura): ");
Serial.print(temp2);
Serial.println(" ºC (DHT)");

DHT.read22(DHT22_PIN3);
humi3=DHT.humidity;
temp3=DHT.temperature;
Serial.print("DHT3(umidade): ");
Serial.print(humi3);
Serial.println(" %");
Serial.print("DHT3(temperatura): ");
Serial.print(temp3);
Serial.println(" ºC (DHT)");

 DHT.read22(DHT22_PIN4);
 humi4=DHT.humidity;
 temp4=DHT.temperature;
 Serial.print("H4:");
 Serial.print(humi4);
 Serial.println("%");
 Serial.print("T4:");
 Serial.print(temp4);
 Serial.println("C (DHT)");

 DHT.read22(DHT22_PIN5);
 humi5=DHT.humidity;
 temp5=DHT.temperature;
 Serial.print("H5:");
 Serial.print(humi5);
 Serial.println("%");
 Serial.print("T5:");
 Serial.print(temp5);
 Serial.println("C (DHT)");

///////////////// Obtendo Índice UV do sensor ultravioleta //////////////////

    for (int i=0;i <5;i++){
       raw += analogRead(A7);  
    }
    Serial.print("Valor medio da leitura: ");
    Serial.println(raw/5);
    sensorUV = raw/1024.0*5.0;
    sensorUV = sensor/5;
    Serial.print("Nivel UV: ");
    Serial.println(sensorUV);
    raw = 0.0;
    delay(1500);
  

  /////////// FAZENDO A CONEXÃO COM O THINGSPEAK /////////////////////
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
  String getStr = "GET /update?api_key=3FSOOWSNE2XEJZY7&field1&field2&field3&field4&field5&field6=0";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(temp1);
  getStr +="&field2=";
  getStr += String(temp2);
  getStr +="&field3=";
  getStr += String(temp3);
  getStr +="&field4=";
  getStr += String(temp4);
  getStr +="&field5=";
  getStr += String(tempC1);
  getStr +="&field6=";
  getStr += String(tempC2);
  getStr +="&field7=";
  getStr += String(temp4);
  getStr +="&field8=";
  getStr += String(humi4);
  getStr +="&field9=";
  getStr += String(temp5);
  getStr +="&field10=";
  getStr += String(humi5);
 
  getStr +="&field7=";
  getStr += String(sensorUV);
  getStr +="&field6=";
  getStr += String(tempC2);
  getStr +="&field7=";
  getStr += String(sensorUV);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
    Serial.print("\n\n");
    Serial.println(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    Serial.println("CIPCLOSE");
  }
///////////////////// ENVIO PARA O GOOGLE SHEETS//////////////////////////////
WiFiClient client;  
   //Start or API service using our WiFi Client through PushingBox
   if (client.connect(WEBSITE, 80))
     { 
     client.print("GET /pushingbox?devid=" + devid
      + "&temp1=" + (String) temp1
      + "&humi1="      + (String) humi1
      + "&temp2="     + (String) temp2
      + "&humi2="      + (String) humi2
      + "&temp3="      + (String) temp3
      + "&humi3="      + (String) humi3
      + "&tempC1="      + (String) tempC1
      + "&tempC2="      + (String) tempC2
      + "&sensorUV="      + (String) sensorUV    );

     client.println(" HTTP/1.1"); 
     client.print("Host: ");
     client.println(WEBSITE);
     client.println("User-Agent: ESP8266/1.0");
     client.println("Connection: close");
     client.println();
     }
/////////////////////////////////////////////////////////
 // thingspeak and google sheets needs 15 sec delay between updates
 delay(100000);  
}
