#include <OneWire.h>
#include <DallasTemperature.h>
#define DQ_pin 12
OneWire oneWire(DQ_pin);
DallasTemperature sensors(&oneWire);
int temp;//current temp
int ideal,ideal1,ideal2,error;

//water pump
#define hot 2
#define cold 3

//water level
#define limmit 400
int level;

//bluetooth
#include  <SoftwareSerial.h>  
SoftwareSerial BTSerial(7,8); // RX | TX  
int input1,input2;


void setup(){  
  //temperature
  Serial.begin(9600);
  sensors.begin();

  //water pump
  pinMode(hot,OUTPUT); 
  pinMode(cold,OUTPUT);  
  
  BTSerial.begin(38400);
}

void loop(){
  while(BTSerial.available()==0){}

  Serial.println("START");  
  input1 = BTSerial.read()-48;
  input2 = BTSerial.read()-48;
  ideal= (input1*10+input2);
  Serial.print("Ideal temperature:");
  Serial.println(ideal);

  while(level<limmit){
    //get current temperature
    sensors.requestTemperatures();
    temp=(sensors.getTempCByIndex(0));
    Serial.print("Current temperature:");
    Serial.println(temp);
    
    //get current water level
    level=analogRead(A5);
    Serial.print("Level:");
    Serial.println(level);
    
    if (temp<ideal){ //too cold
      digitalWrite(hot,HIGH);
      digitalWrite(cold,LOW);     
      Serial.println("TOO COLD");
    }
     
    else if (temp>ideal){ //too hot
      digitalWrite(cold,HIGH);
      digitalWrite(hot,LOW);
      Serial.println("TOO HOT");
    }

    else{
      digitalWrite(cold,HIGH);
      digitalWrite(hot,HIGH);
      Serial.println("PERFECT TEMPERATURE");
    }

    delay(100);
  } 
  digitalWrite(cold,LOW);
  digitalWrite(hot,LOW);
 }
