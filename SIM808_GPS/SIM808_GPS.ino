#include "gps.h"
void setup() {
  pinMode(10, INPUT);
  pinMode(11, OUTPUT);
  SIM808.begin(9600);   
  Serial.begin(9600);   
  delay(2000);
  if(expectedAnswer("AT+CGNSPWR=1","OK\n")){
    Serial.println("GPS ON");
  }
  if(expectedAnswer("AT+CGNSSEQ=RMC","OK\n")){
    Serial.println("RMC Mode");
  }
  String n = GPS_Data();
  Serial.println(n);
  String Time = getTime(n);
  String latitude = getLatitude(n);
  Serial.print("Lat = ");
  Serial.println(latitude);
  Serial.print("Time = ");
  Serial.println(Time);
}

void loop() {

 if (Serial.available()>0)
 SIM808.write(Serial.read());
 if (SIM808.available()>0)
 Serial.write(SIM808.read());
 String n = GPS_Data();
 Serial.println(n);
 String Time = getTime(n);
 String latitude = getLatitude(n);
 String longitude = getLongitude(n);
 Serial.print("Lat = ");
 Serial.println(latitude);
 Serial.print("Long = ");
 Serial.println(longitude);
 Serial.print("Time = ");
 Serial.println(Time);

 
}
