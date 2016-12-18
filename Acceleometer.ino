# include "lis.h";
float X,Y,Z;
void setup()
{
 Serial.begin(9600);
 lis331SetUp();
 
}

void loop()
{
X = getAccelerationX();
X = getAccelerationY();
Z = getAccelerationZ();
Serial.print("X = ");
Serial.print(X);
Serial.print(" ,");
Serial.print("Y = ");
Serial.print(Y);
Serial.print(" ,");
Serial.print("Z = ");
Serial.println(Z);
  
}

