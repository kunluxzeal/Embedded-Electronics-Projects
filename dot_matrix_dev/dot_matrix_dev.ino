#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include <Arial_black_16.h>
#include <Wire.h>
#define ROWS 1
#define COLUMNS 1

DMD dmd(ROWS , COLUMNS);

void ScanDMD(){
  dmd.scanDisplayBySPI();
  }

void setup() {
  // put your setup code here, to run once:
Timer1.initialize(1000);
Timer1.attachInterrupt(ScanDMD);
dmd.clearScreen(true);
}

void loop() {
  // put your main code here, to run repeatedly:

dmd.selectFont(Arial_Black_16);
dmd.drawString(2,1,"HI",2,GRAPHICS_NORMAL);
}
