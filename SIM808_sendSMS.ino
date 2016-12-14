#include <SoftwareSerial.h>
SoftwareSerial SIM808(9, 10);

//Xively info:
#define APIKEY "F5k6VfJlhMhLE7MQwmsqJI4tg7W3ClAPa5fRjclsrvNLnIH0"
#define FEEDID "673950585"
#define USERAGENT "Divale"
String ctlZ= "\x1A" ;
bool simEnabled = false;


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
