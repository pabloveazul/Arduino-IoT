# include "input_output.h"
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
String getLatitude(String data){
return getValue(data, ',',4);
}
String getLongitude(String data){
return getValue(data, ',',3);
}
String getTime(String data){
return getValue(data, ',',2);
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
