
#include <Wire.h>

int LIS331 = 0x19;
#define X_Axis_Register_DATAX0 0x28 // Hexadecima address for the DATAX0 internal register.
#define X_Axis_Register_DATAX1 0x29 // Hexadecima address for the DATAX1internal register.
#define Y_Axis_Register_DATAY0 0x2A // Hexadecima address for the DATAY0 internal register.
#define Y_Axis_Register_DATAY1 0x2B // Hexadecima address for the DATAY1internal register.
#define Z_Axis_Register_DATAZ0 0x2C // Hexadecima address for the DATAZ0 internal register.
#define Z_Axis_Register_DATAZ1 0x2D // Hexadecima address for the DATAZ1internal register.
#define Power_Register 00100111 // Power Control Register

float getAccelerationX(){
<<<<<<< HEAD
  float A
=======
  float A;
  float A1;
  Wire.beginTransmission(LIS331);
>>>>>>> origin/master
  Wire.write(X_Axis_Register_DATAX1);
  Wire.endTransmission(); // Ends the transmission and transmits the data from the two registers
  Wire.requestFrom(LIS331,1); // Request the transmitted two bytes from the two registers
  if(Wire.available()<=1) {
      A1 = Wire.read();}
  if (A1<=127){
     A1=-A1/19.0;}
  else{
     A1=-(A1-255.0)/19.0;}
  A=A1;
<<<<<<< HEAD
  return X;
}

float getAccelerationY(){
  float A
=======
  return A;
}

float getAccelerationY(){
  float A;
  float A1;
  Wire.beginTransmission(LIS331);
>>>>>>> origin/master
  Wire.write(Y_Axis_Register_DATAY1);
  Wire.endTransmission(); // Ends the transmission and transmits the data from the two registers
  Wire.requestFrom(LIS331,1); // Request the transmitted two bytes from the two registers
  if(Wire.available()<=1) {
      A1 = Wire.read();}
  if (A1<=127){
     A1=-A1/19.0;}
  else{
     A1=-(A1-255.0)/19.0;}
  A=A1;
<<<<<<< HEAD
  return X;
}

float getAccelerationZ(){
  float A
=======
  return A;
}

float getAccelerationZ(){
  float A;
  float A1;
  Wire.beginTransmission(LIS331);
>>>>>>> origin/master
  Wire.write(Z_Axis_Register_DATAZ1);
  Wire.endTransmission(); // Ends the transmission and transmits the data from the two registers
  Wire.requestFrom(LIS331,1); // Request the transmitted two bytes from the two registers
  if(Wire.available()<=1) {
      A1 = Wire.read();}
  if (A1<=127){
     A1=-A1/19.0;}
  else{
     A1=-(A1-255.0)/19.0;}
  A=A1;
<<<<<<< HEAD
  return X;
=======
  return A;
>>>>>>> origin/master
}

void lis331SetUp()
{
  Wire.begin(); // Initiate the Wire library
  delay(100);
  // Enable measurement
  Wire.beginTransmission(LIS331);
  Wire.write(0x20);
  Wire.write(39);
  Wire.endTransmission();
  Wire.beginTransmission(LIS331);
  Wire.write(0x23);
  Wire.write(0);
  Wire.endTransmission();
}

