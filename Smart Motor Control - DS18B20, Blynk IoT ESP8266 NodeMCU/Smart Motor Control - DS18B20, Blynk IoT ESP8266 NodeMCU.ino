#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include<DallasTemperature.h>

char auth[] = "";//fill your token
char ssid[] = "";//fill your wifi  ssid
char pass[] = "";//fill your wifi password

#define M1 D1
#define M2 D2
#define ONE_WIRE_BUS D5

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

bool _start = false;
bool _forward = true;
bool _alert = false;

BLYNK_WRITE(V0)  
{
  int pinValue = param.asInt();
  if(pinValue == 1){
    _start = true;
  }else{
    _start = false;
  }
}

BLYNK_WRITE(V1)   
{
  int pinValue = param.asInt();
  if(pinValue == 1){
    _forward = true;
  }else{
    _forward = false;
  }
}

void setup(){
  Serial.begin(9600);
  DS18B20.begin();
  pinMode(M1,OUTPUT);
  pinMode(M2,OUTPUT);
  Blynk.begin(auth, ssid, pass);
}

void get_temperature(){
  DS18B20.requestTemperatures();
  double temp_data_c = DS18B20.getTempCByIndex(0);
  if(temp_data_c > 0)
    Blynk.virtualWrite(V2,String(temp_data_c)+" C");
  if(temp_data_c > 35.0){
    Blynk.virtualWrite(V4,255);
    _alert = true;
  }else{
    Blynk.virtualWrite(V4,0);
    _alert = false;
  }
}

void stop_motor(){
  digitalWrite(M1,LOW);
  digitalWrite(M2,LOW);
}

void ccw(){
  digitalWrite(M1,HIGH);
  digitalWrite(M2,LOW);
}

void cw(){
  digitalWrite(M1,LOW);
  digitalWrite(M2,HIGH);
}
void set_motor(){
  if(_start == true && _forward == true && _alert == false){
    cw();
    Blynk.virtualWrite(V3,"Feedback: Motor running in forward direction");
  }
  else if(_start == true && _forward == false && _alert == false){
    ccw();
    Blynk.virtualWrite(V3,"Feedback: Motor running in reverse direction");
  }
  else{
    stop_motor();    
    Blynk.virtualWrite(V3,"Feedback: Motor is in stopped condition");
  }
  if(_alert == true){
    Blynk.virtualWrite(V3,"Feedback: Motor stopped due to high Temperature");
  }
}
void loop()
{
  Blynk.run();
  get_temperature();
  set_motor();
}
