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


int getLength(int n){
  int cnt = 1;
  while(n >= 10){
    n/= 10;
    cnt++;
  }
  return cnt;
}