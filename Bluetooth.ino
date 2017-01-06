#include <SoftwareSerial.h>
char c;
int  LED = 13;

SoftwareSerial mySerial(10, 11);

void setup()
{
  pinMode(LED, OUTPUT);
  mySerial.begin(9600);
}

void loop()
{
  while (!mySerial.available());
  c = mySerial.read();
  if (c == '1') 
  {
    digitalWrite(LED, HIGH);
  }
  if (c == '0') 
  {
    digitalWrite(LED, LOW);
  } 
}

