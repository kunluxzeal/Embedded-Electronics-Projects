
//------------Program developed by R.GIRISH------------//
#include<EEPROM.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);
int i = 0;
int H = 0;
int M = 0;
int S = 0;
int setting_value;
const int bell = 10;
const int P = A3;
const int N = A2;
const int setting_address = 0;
const int over_ride_off = 11;
boolean bell_status = true;
boolean Over_ride = true;
//------------------- Set Bell Timings from hours 1 to 23 hrs -------------------//
//---- 1st bell ------//
const int h1 = 0; //hours
const int m1 = 0; //Minutes
//---- 2nd bell ------//
const int h2 = 0;
const int m2 = 0;
//---- 3rd bell ------//
const int h3 = 0;
const int m3 = 0;
//---- 4th bell ------//
const int h4 = 0;
const int m4 = 0;
//---- 5th bell ------//
const int h5 = 0;
const int m5 = 0;
//---- 6th bell ------//
const int h6 = 0;
const int m6 = 0;
//---- 7th bell ------//
const int h7 = 0;
const int m7 = 0;
//---- 8th bell ------//
const int h8 = 0;
const int m8 = 0;
//---- 9th bell ------//
const int h9 = 0;
const int m9 = 0;
//---- 10th bell ------//
const int h10 = 0;
const int m10 = 0;
//---- 11th bell ------//
const int h11 = 0;
const int m11 = 0;
//---- 12th bell ------//
const int h12 = 0;
const int m12 = 0;
//---- 13th bell ------//
const int h13 = 0;
const int m13 = 0;
//---- 14th bell ------//
const int h14 = 0;
const int m14 = 0;
//---- 15th bell ------//
const int h15 = 0;
const int m15 = 0;
//---- 16th bell ------//
const int h16 = 0;
const int m16 = 0;
//--------------- bell ring lenght in seconds -------//
const int Lenght = 3; //in seconds
//-------------------------- -------------------------//
void setup()
{
lcd.begin(16, 2);
pinMode(P, OUTPUT);
pinMode(N, OUTPUT);
pinMode(bell, OUTPUT);
pinMode(over_ride_off, INPUT);
digitalWrite(P, HIGH);
digitalWrite(N, LOW);
digitalWrite(over_ride_off, HIGH);
attachInterrupt(0, over_ride, RISING);
attachInterrupt(1, bell_setting, RISING);
if (EEPROM.read(setting_address) != 1)
{
bell_setting();
}
}
void loop()
{
tmElements_t tm;
lcd.clear();
if (RTC.read(tm))
{
H = tm.Hour;
M = tm.Minute;
S = tm.Second;
lcd.setCursor(0, 0);
lcd.print("TIME:");
lcd.print(tm.Hour);
lcd.print(":");
lcd.print(tm.Minute);
lcd.print(":");
lcd.print(tm.Second);
lcd.setCursor(0, 1);
lcd.print("DATE:");
lcd.print(tm.Day);
lcd.print("/");
lcd.print(tm.Month);
lcd.print("/");
lcd.print(tmYearToCalendar(tm.Year));
} else {
if (RTC.chipPresent())
{
lcd.setCursor(0, 0);
lcd.print("RTC stopped!!!");
lcd.setCursor(0, 1);
lcd.print("Run SetTime code");
} else {
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Read error!");
lcd.setCursor(0, 1);
lcd.print("Check circuitry!");
}
}
if (EEPROM.read(setting_address) == 1)
{
if (H == 0 && M == 0 && S == 0)
{
digitalWrite(bell, LOW);
}
if (H == h1 && M == m1 && S == 0)
{
for (i = 0; i < Lenght; i++)
{
digitalWrite(bell, HIGH);
delay(1000);
}
digitalWrite(bell, LOW);
i = 0;
}
if (H == h2 && M == m2 && S == 0)
{
for (i = 0; i < Lenght; i++)
{
digitalWrite(bell, HIGH);
delay(1000);
}
digitalWrite(bell, LOW);
i = 0;
}
if (H == h3 && M == m3 && S == 0)
{
for (i = 0; i < Lenght; i++)
{
digitalWrite(bell, HIGH);
delay(1000);
}
digitalWrite(bell, LOW);
i = 0;
}
if (H == h4 && M == m4 && S == 0)
{
for (i = 0; i < Lenght; i++)
{
digitalWrite(bell, HIGH);
delay(1000);
}
digitalWrite(bell, LOW);
i = 0;
}
if (H == h5 && M == m5 && S == 0)
{
for (i = 0; i < Lenght; i++)
{
digitalWrite(bell, HIGH);
delay(1000);
}
digitalWrite(bell, LOW);
i = 0;
}
if (H == h6 && M == m6 && S == 0)
{
for (i = 0; i < Lenght; i++)
{
digitalWrite(bell, HIGH);
delay(1000);
}
digitalWrite(bell, LOW);
i = 0;
}
if (H == h7 && M == m7 && S == 0)
{
for (i = 0; i < Lenght; i++)
{
digitalWrite(bell, HIGH);
delay(1000);
}
digitalWrite(bell, LOW);
i = 0;
}
if (H == h8 && M == m8 && S == 0)
{
for (i = 0; i < Lenght; i++)
{
digitalWrite(bell, HIGH);
delay(1000);
}
digitalWrite(bell, LOW);
i = 0;
}
if (H == h9 && M == m9 && S == 0)
{
for (i = 0; i < Lenght; i++)
{
digitalWrite(bell, HIGH);
delay(1000);
}
digitalWrite(bell, LOW);
i = 0;
}
if (H == h10 && M == m10 && S == 0)
{
for (i = 0; i < Lenght; i++)
{
digitalWrite(bell, HIGH);
delay(1000);
}
digitalWrite(bell, LOW);
i = 0;
}
if (H == h11 && M == m11 && S == 0)
{
for (i = 0; i < Lenght; i++)
{
digitalWrite(bell, HIGH);
delay(1000);
}
digitalWrite(bell, LOW);
i = 0;
}
if (H == h12 && M == m12 && S == 0)
{
for (i = 0; i < Lenght; i++)
{
digitalWrite(bell, HIGH);
delay(1000);
}
digitalWrite(bell, LOW);
i = 0;
}
if (H == h13 && M == m13 && S == 0)
{
for (i = 0; i < Lenght; i++)
{
digitalWrite(bell, HIGH);
delay(1000);
}
digitalWrite(bell, LOW);
i = 0;
}
if (H == h14 && M == m14 && S == 0)
{
for (i = 0; i < Lenght; i++)
{
digitalWrite(bell, HIGH);
delay(1000);
}
digitalWrite(bell, LOW);
i = 0;
}
if (H == h15 && M == m15 && S == 0)
{
for (i = 0; i < Lenght; i++)
{
digitalWrite(bell, HIGH);
delay(1000);
}
digitalWrite(bell, LOW);
i = 0;
}
if (H == h16 && M == m16 && S == 0)
{
for (i = 0; i < Lenght; i++)
{
digitalWrite(bell, HIGH);
delay(1000);
}
digitalWrite(bell, LOW);
i = 0;
}
}
delay(1000);
}
void over_ride()
{
lcd.clear();
while (Over_ride)
{
digitalWrite(bell, HIGH);
lcd.setCursor(0, 0);
lcd.print("Press Exit to");
lcd.setCursor(0, 1);
lcd.print("Stop the bell!!!");
if (digitalRead(over_ride_off) == LOW)
{
Over_ride = false;
digitalWrite(bell, LOW);
}
}
Over_ride = true;
}
void bell_setting()
{
setting_value = 0;
EEPROM.write(setting_address, setting_value);
lcd.clear();
while (bell_status)
{
lcd.setCursor(0, 0);
lcd.print("Bell is Disabled");
lcd.setCursor(0, 1);
lcd.print("Press Exit.");
if (digitalRead(over_ride_off) == LOW)
{
bell_status = false;
}
}
bell_status = true;
setting_value = 1;
EEPROM.write(setting_address, setting_value);
}
