# include "input_output.h"
String getLatitude(String data){
return getValue(data, ',',4);
}
String getTime(String data){
return getValue(data, ',',3);
}
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
String GPS_Data(){
  String rep ="";
  String Data = getReponseFrom("AT+CGNSINF");
  for (int i=0; i < Data.length()-1; ++i){
    if (Data[i] == 58){
      for (int j=i+2; j < Data.length()-1; ++j){
        if(Data[i] == 10){
          break;
        }
        rep = rep + Data[j];
      }
     break;
    }
  }
  return rep;
}
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
 
}
