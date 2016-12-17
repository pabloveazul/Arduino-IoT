#include <SoftwareSerial.h>
#include <input_output.h>
#include <sim.h>
SoftwareSerial SIM808(9, 10);

//Xively info:
#define APIKEY "F5k6VfJlhMhLE7MQwmsqJI4tg7W3ClAPa5fRjclsrvNLnIH0"
#define FEEDID "673950585"
#define USERAGENT "Divale"
String ctlZ= "\x1A" ;
bool simEnabled = false;


void setup(){
  pinMode(9, INPUT);
  pinMode(10, OUTPUT);
  SIM808.begin(9600);   
  Serial.begin(9600);   

  delay(2000);
  if(expectedAnswer("AT","OK\n")){
    Serial.println("GPRS AT ");
  }
  /*if(expectedAnswer("AT+CPIN=?","OK\n")){
    Serial.println("Unlocking PIN...");
  }*/
  UnlockSIM();
  
  if(simEnabled){
    sendSMS("+33622811856","Wena pelao"); 
    delay(300);
    
  }
  Serial.println("Loop");
}
void loop(){
 if (Serial.available()>0)
  SIM808.write(Serial.read());
 if (SIM808.available()>0)
  Serial.write(SIM808.read());
 
}
