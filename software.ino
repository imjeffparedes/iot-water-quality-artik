/********************
* Here's my Code for Water Quality Device Monitoring
* This Arduino code will collect ph and temp data from MKR1000 
* and send them over Samsung ARTIK Cloud API
*
* Author: Jeff Paredes
* Version 1.0
*
********************/


#include <WiFi101.h>
#include <WiFiClient.h>
#include <ArduinoJson.h> 
#include <ArduinoHttpClient.h> 
#include <SPI.h> 
#include <OneWire.h>
#include <DallasTemperature.h>

/**Temperature Sensor Initialization**/
#define ONE_WIRE_BUS 1                // Data wire is plugged into digital port 1 of Arduino
OneWire oneWire(ONE_WIRE_BUS);        // Setup a oneWire instance to communicate with any OneWire 
                                      //devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature. 

/** ARTIK Cloud REST Initialization **/
char server[] = "api.artik.cloud";    // Samsung ARTIK Cloud API Host
int port = 443;                       // 443 for HTTPS 

char buf[200];                        // body data to store the JSON to be sent to the ARTIK cloud 

String deviceID = "artik cloud device id"; // put your device id here created from tutorial 
String deviceToken = "artik cloud device token"; // put your device token here created from tutorial

/**pH meter initialization**/
#define SensorPin A1                  // pH meter Analog output to Arduino Analog Input 1
#define Offset 0.00                   // deviation compensate
#define samplingInterval 20
#define ArrayLenth  40                // times of collection
int pHArray[ArrayLenth];              // Store the average value of the sensor feedback
int pHArrayIndex=0;   

int status = -1;
int millis_start;
int sendInterval = 5;                 // send time interval in seconds

/**Wifi Setting**/
#define WIFI_AP "your wifi ssid"
#define WIFI_PWD "wifi password"

WiFiSSLClient wifi; 
HttpClient client = HttpClient(wifi, server, port);



void setup(void) {
  pinMode(13, OUTPUT);           // set pin 13 (LED) to output
  millis_start = millis();
  Serial.begin(9600);
  startWifi();                             //start connecting to wifi
}

void loop(void) {
    /*Aquiring current temperature*/
   float celsius = 0;
   sensors.requestTemperatures();          // Send the command to get temperatures
   celsius = sensors.getTempCByIndex(0);
   sensors.requestTemperatures();          // Send the command to get temperatures
   celsius = sensors.getTempCByIndex(0);


  /*Aquiring current pH value*/
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+Offset;
      samplingTime=millis();
  }
  
  Serial.println("==========================================="); 
  Serial.println("We will send these json data"); 
  //print to json format
  Serial.println("data: { ");
  Serial.print("ph: ");
  Serial.print(pHValue);
  Serial.print(" , temp: ");
  Serial.print(celsius);
  Serial.println("} ");

  Serial.println("");
  
  Serial.println("Start sending data"); 
  String contentType = "application/json"; 
  String AuthorizationData = "Bearer " + deviceToken; //Device Token 
  int len = loadBuffer(celsius,pHValue);   
  Serial.println("Sending temp: "+String(celsius) +" and ph: "+String(pHValue) );  
  Serial.println("Send POST to ARTIK Cloud API"); 
  client.beginRequest(); 
  client.post("/v1.1/messages"); //, contentType, buf 
  client.sendHeader("Authorization", AuthorizationData); 
  client.sendHeader("Content-Type", "application/json"); 
  client.sendHeader("Content-Length", len); 
  client.endRequest(); 
  client.print(buf); 
  
  // print response from api
  int statusCode = client.responseStatusCode(); 
  String response = client.responseBody(); 
  Serial.println("");
  if(statusCode==200){
    digitalWrite(13, HIGH);       // turn on LED
    delay(500);                  // Make delay fro blink
    digitalWrite(13, LOW);       // turn on LED
  }
  Serial.print("Status code: "); 
  Serial.println(statusCode); 
  Serial.print("Response: "); 
  Serial.println(response);   
  delay(sendInterval); // delay of update 
  
  
}
/*Init Connection to Wifi*/
void startWifi(){
  Serial.println("Connecting MKR1000 to network...");
  //  WiFi.begin();
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED ) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_AP);
    WiFi.begin(WIFI_AP, WIFI_PWD);
    // wait 10 seconds for connection:
    delay(10000);
    status = WiFi.status();
  }
}


/*DfRobot pH Meter Source*/
double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}

/*Buffer to send on REST*/
int loadBuffer(float temp, float ph ) {   
  StaticJsonBuffer<200> jsonBuffer; // reserve spot in memory 
  JsonObject& root = jsonBuffer.createObject(); // create root objects 
  root["sdid"] =  deviceID;   
  root["type"] = "message"; 
  JsonObject& dataPair = root.createNestedObject("data"); // create nested objects 
  dataPair["temp"] = temp;   
  dataPair["ph"] = ph; 
  root.printTo(buf, sizeof(buf)); // JSON-print to buffer 
  return (root.measureLength()); // also return length 
} 
