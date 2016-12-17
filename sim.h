

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