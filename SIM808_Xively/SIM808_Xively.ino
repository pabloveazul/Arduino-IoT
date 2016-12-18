#include <SoftwareSerial.h>
#include "sensors.h"
#include "input_output.h"
#include "sim.h"


//Xively info:
#define APIKEY "F5k6VfJlhMhLE7MQwmsqJI4tg7W3ClAPa5fRjclsrvNLnIH0"
#define FEEDID "673950585"
#define USERAGENT "Divale"




bool connectToXively(){
  String cipStatus = getReponseFrom("AT+CIPSTATUS",300);
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
    if(expectedAnswer("AT+CIPSTART=\"TCP\",\"api.xively.com\",\"80\"","OK\n\nCONNECT OK\n",400)){
      Serial.println("Done");
      return true;
    }
  }     
  else if (cipStatus.equals(Closed)){
    Serial.println("Reconnecting x.api");
    delay(100);
    if(expectedAnswer("AT+CIPSTART=\"TCP\",\"api.xively.com\",\"80\"","OK\n\nCONNECT OK\n",400)){
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

bool internetUp(){
    if(expectedAnswer("AT+CIPSHUT","SHUT OK\n",400)){
      Serial.println("Shut down OK");   
    }  
    else{return false;}
    if(expectedAnswer("AT+CIPSTATUS","OK\n\nSTATE: IP INITIAL\n",400)){
      Serial.println("IP initial State");   
    }
    else{return false;}
    if(expectedAnswer("AT+CIPMUX=0","OK\n",400)){
      Serial.println("MUX=0");
    }
    else{return false;}
    if(expectedAnswer("AT+CSTT=free","OK\n",1000)){
      Serial.println("Connected to free APN");
    }
    else{return false;}
    if(expectedAnswer("AT+CIICR","OK\n",1000)){
      Serial.println("Connection up");
      String IP= getReponseFrom("AT+CIFSR");
      Serial.print("IP : ");
      Serial.println(IP);
    }
    else{return false;}
    if(expectedAnswer("AT+CIPQSEND=1","OK\n",400)){
      Serial.println("Quick Send OK");
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
  pinMode(10, INPUT);
  pinMode(11, OUTPUT);
  SIM808.begin(9600);   
  Serial.begin(9600);   

  delay(2000);
  if(expectedAnswer("AT","OK\n")){
    Serial.println("GPRS AT ");
  }
 
  UnlockSIM();
  
  if(simEnabled){
    
    
    internetUp(); //lift internet connection
    delay(200);

    if(expectedAnswer("AT+CIPSTART=\"TCP\",\"api.xively.com\",\"80\"","OK\n\nCONNECT OK\n",400)){
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
