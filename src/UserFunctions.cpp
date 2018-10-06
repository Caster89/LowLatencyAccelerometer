// User data functions.  Modify these functions for your data items.
#include "UserTypes.h"
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <LiquidCrystal.h>
//------------------------------------------------------------------------------
MPU6050 mpu;
LiquidCrystal lcd(8,9,4,5,6,7);
static uint32_t startMicros;

// Acquire a data record.
void acquireData(data_t* data) {
  data->time = micros();
  mpu.getMotion6(&data->ax, &data->ay, &data->az,
                 &data->gx, &data->gy, &data->gz);
}

// setup AVR I2C
void userSetup() {
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    Wire.setClock(400000);
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif
    mpu.initialize();
    lcd.begin(16,2);
    updateLCD();
    /*mpu.setXGyroOffset(105);
    mpu.setYGyroOffset(5);
    mpu.setZGyroOffset(0);
    mpu.setXAccelOffset(-1651);
    mpu.setYAccelOffset(136);
    mpu.setZAccelOffset(1519);*/
}

void updateLCD(char* fileName){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saving to");
  lcd.setCursor(0,1);
  lcd.print(fileName);
}

void updateLCD()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press START");
}

// Print a data record.
void printData(Print* pr, data_t* data) {
  if (startMicros == 0) {
    startMicros = data->time;
  }
  pr->print(data->time- startMicros);
  pr->write(',');
  pr->print(data->ax);
  pr->write(',');
  pr->print(data->ay);
  pr->write(',');
  pr->print(data->az);
  pr->write(',');
  pr->print(data->gx);
  pr->write(',');
  pr->print(data->gy);
  pr->write(',');
  pr->println(data->gz);
}

// Print data header.
void printHeader(Print* pr) {
  startMicros = 0;
  pr->println(F("us,ax,ay,az,gx,gy,gz"));
}
