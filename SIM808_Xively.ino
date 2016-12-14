#include <SoftwareSerial.h>
#include <OneWire.h> 
SoftwareSerial SIM808(9, 10);

//Xively info:
#define APIKEY "F5k6VfJlhMhLE7MQwmsqJI4tg7W3ClAPa5fRjclsrvNLnIH0"
#define FEEDID "673950585"
#define USERAGENT "Divale"
String ctlZ= "\x1A" ;
bool simEnabled = false;

int DS18S20_Pin = 2; //DS18S20 Signal pin on digital 2
OneWire ds(DS18S20_Pin); // on digital pin 2

float getTemp(){
 //returns the temperature from one DS18S20 in DEG Celsius
 byte data[12];
 byte addr[8];

 if ( !ds.search(addr)) {
   //no more sensors on chain, reset search
   ds.reset_search();
   return -1000;
 }
 if ( OneWire::crc8( addr, 7) != addr[7]) {
   Serial.println("CRC");
   return -1000;
 }
 if ( addr[0] != 0x10 && addr[0] != 0x28) {
   Serial.print("not reco");
   return -1000;
 }
 ds.reset();
 ds.select(addr);
 ds.write(0x44,1); // start conversion, with parasite power on at the end

 byte present = ds.reset();
 ds.select(addr);  
 ds.write(0xBE); // Read Scratchpad

 for (int i = 0; i < 9; i++) { // we need 9 bytes
  data[i] = ds.read();
 }
 
 ds.reset_search();
 byte MSB = data[1];
 byte LSB = data[0];

 float tempRead = ((MSB << 8) | LSB); //using two's compliment
 float TemperatureSum = tempRead / 16;
 return TemperatureSum;
}


bool fine(char c){
  return (c != '\r');
}

String simpleRead(){
  String answer = "";
  while(SIM808.available()>0){
    char c = SIM808.read();
    if(fine(c)) answer += c;
    delay(20);
  }
  return answer;
}

bool expectedAnswer(String message, String expectedResult, int timeDelay = 300){
  SIM808.println(message);
  delay(timeDelay);
  String reponse = simpleRead();
  bool value = reponse.equals(message+"\n"+expectedResult);
  /*if(not value){
    Serial.println("Fail: received answer equals to : ");
    Serial.println("----------------------------------");
    Serial.print(reponse);
    Serial.println("----------------------------------");
    Serial.println("expected result equals to : ");
    Serial.println("----------------------------------");
    Serial.print(expectedResult);
    Serial.println("----------------------------------");
    
  }*/
  return value;
}



void sendSimpleMessage(String message, int timeDelay = 200){
  SIM808.println(message);
  delay(timeDelay);
}


String getReponseFrom(String message,int timeDelay=300){
  SIM808.println(message);
  delay(timeDelay);
  String reponse = simpleRead();
  return reponse;
}

void sendSMS(String number, String message){
  if(expectedAnswer("AT+CMGF=1","OK\n")){
    Serial.println("Inside SMS mode...");
    delay(50);
  }
  String introNumber = "AT+CMGS=\"" +  number + "\"";
  sendSimpleMessage(introNumber);
  if(expectedAnswer(message+ctlZ,"> "+message+"\n"+"> \n"+"+CMGS: 28\n\nOK\n")) 
  { Serial.println("Message correctly sent");}
    
  else {
    Serial.println("Error : Message not sent");
  }
}

bool connectToXively(){
  String cipStatus = getReponseFrom("AT+CIPSTATUS",1000);
  String Closed = "AT+CIPSTATUS\nOK\n\nSTATE: TCP CLOSED\n";
  String Connected = "AT+CIPSTATUS\nOK\n\nSTATE: CONNECT OK\n";
  String Stat = "AT+CIPSTATUS\nOK\n\nSTATE: IP STATUS\n";
  String Rara = "AT+CIPSTATUS\nOK\n\nSTATE: CONNECT OK\n\nCLOSED\n";
  if (cipStatus.equals(Connected)){
    Serial.println("Connected x.api");
    return true;
  }
  else if (cipStatus.equals(Stat) or cipStatus.equals(Rara)){
    Serial.println("Connecting x.api");
    delay(100);
    if(expectedAnswer("AT+CIPSTART=\"TCP\",\"api.xively.com\",\"80\"","OK\n\nCONNECT OK\n",5000)){
      Serial.println("Done");
      return true;
    }
  }     
  else if (cipStatus.equals(Closed)){
    Serial.println("Reconnecting x.api");
    delay(100);
    if(expectedAnswer("AT+CIPSTART=\"TCP\",\"api.xively.com\",\"80\"","OK\n\nCONNECT OK\n",5000)){
      Serial.println("Done");
      return true;
    }   
  }
  else {
    Serial.println("Error: Invalid C.S :");
    Serial.println(cipStatus);
    return false;
  }
}

void UnlockSIM(){
  String pinState = getReponseFrom("AT+CPIN?",1000);
  String pinLocked = "AT+CPIN?\n+CPIN: SIM PIN\n\nOK\n";
  String pinUnlocked = "AT+CPIN?\n+CPIN: READY\n\nOK\n";
  if(pinState.equals(pinUnlocked)){
    simEnabled = true;
    /*Serial.println("Sim already unlocked");*/
  } else if(pinState.equals(pinLocked)){
    /*Serial.println("Pin Locked, Unlocking...");*/
    if(expectedAnswer("AT+CPIN=1234","OK\n\n+CPIN: READY\n\nCall Ready\n\nSMS Ready\n",7000)){
      Serial.println("SIM Ready");
      simEnabled = true;
      delay(5000);
      Serial.println(simpleRead());
    }
  } else {
    Serial.println("Error SIM:");
    Serial.println(pinState);
  }
  
}

int getLength(int n){
  int cnt = 1;
  while(n >= 10){
    n/= 10;
    cnt++;
  }
  return cnt;
}

bool internetUp(){
    if(expectedAnswer("AT+CIPSHUT","SHUT OK\n",1000)){
      Serial.println("SHUT");   
    }  
    else{return false;}
    if(expectedAnswer("AT+CIPSTATUS","OK\n\nSTATE: IP INITIAL\n",1000)){
      Serial.println("IS");   
    }
    else{return false;}
    if(expectedAnswer("AT+CIPMUX=0","OK\n",1000)){
      Serial.println("MUX=0");
    }
    else{return false;}
    if(expectedAnswer("AT+CSTT=free","OK\n",5000)){
      Serial.println("free APN");
    }
    else{return false;}
    if(expectedAnswer("AT+CIICR","OK\n",5000)){
      Serial.println("ciicr.ok");
      String IP= getReponseFrom("AT+CIFSR");
      Serial.print("IP : ");
      Serial.println(IP);
    }
    else{return false;}
}

void dataToChannel(float data, String channel){
    
    if(connectToXively()){
    int lengthData = getLength(data);
    int messageLength = channel.length() + lengthData;
    delay(300);
    sendSimpleMessage("AT+CIPSEND");
    delay(300);
    SIM808.print("PUT /v2/feeds/");
    SIM808.print(FEEDID);
    SIM808.println(".csv HTTP/1.1");
    SIM808.print("Host: api.xively.com\n");
    SIM808.print("X-ApiKey: ");
    SIM808.println(APIKEY);
    SIM808.print("User-Agent: ");
    SIM808.println(USERAGENT);
    SIM808.print("Content-Length: ");
    SIM808.println(messageLength);
    SIM808.print("Content-Type: text/csv\n");
    SIM808.println("Connection: close");
    SIM808.println();
    SIM808.print(channel);
    SIM808.println(data);
    SIM808.println(ctlZ);
    Serial.print("Data sent: ");
    Serial.println(data); 
    delay(200);
    Serial.println(simpleRead());       
    }
    else {
      Serial.println("Error sending");
    }
}

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
    /*sendSMS("+33622811856","Wena pelao"); 
    delay(300);*/
    
    internetUp(); //lift internet connection
    delay(200);

    if(expectedAnswer("AT+CIPSTART=\"TCP\",\"api.xively.com\",\"80\"","OK\n\nCONNECT OK\n",5000)){
      Serial.println("Connection x.api");
    }

      int thisData = 60;
      for(int i = 0; i < 1; ++i){
        Serial.print(" sending data n: ");
        Serial.println(i);
        dataToChannel((thisData+i), "example,");
        delay(1000);
      }
  }
  Serial.println("Loop");
}
void loop(){
 if (Serial.available()>0)
  SIM808.write(Serial.read());
 if (SIM808.available()>0)
  Serial.write(SIM808.read());
 float Temp= getTemp();
 dataToChannel(Temp, "example,");  
 
}
