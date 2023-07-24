#include <DS3231.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Button.h>

//Pin assignment for button and  
int DN_PIN = 2; //Down Button
int UP_PIN = 3; //Up Button
int SET_PIN = 4;  //Setup Button
int ALR_PIN = 5 ; //Setup Button
#define PULLUP true //internal Pull Up
#define INVERT true 
#define DEBOUNCE_MS 20     
#define REPEAT_FIRST 500 
#define REPEAT_INCR 100 

//Declare push buttons
Button btnUP(UP_PIN, PULLUP, INVERT, DEBOUNCE_MS);    
Button btnDN(DN_PIN, PULLUP, INVERT, DEBOUNCE_MS);
Button btnSET(SET_PIN, PULLUP, INVERT, DEBOUNCE_MS);
Button btnALR(ALR_PIN, PULLUP, INVERT, DEBOUNCE_MS);
enum {WAIT, INCR, DECR};             
uint8_t STATE;                       
int count;                           
int lastCount = -1;                   
unsigned long rpt = REPEAT_FIRST;  
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

DS3231  rtc(SDA, SCL); //RTC3231 connected pin
Time  t;
uint32_t targetTime = 0;
uint8_t conv2d(const char* p) {
uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
   v = *p - '0';
  return 10 * v + *++p - '0';}

uint8_t hh = 0, mm = 0, ss = 0, dd = 0, bb = 0;//initialize date and time to zero
int yy = 0;
String Day = "  ";
uint8_t alarmHH = 9, alarmMM = 30;
int alarmLONG = 5;
uint8_t setMode = 0, setAlarm = 0, alarmMode=0;

void setup()
{
  pinMode (13, OUTPUT); 
  lcd.begin(16,2);
  rtc.begin();
  lcd.setCursor (0,0);
  lcd.print(F("  Digital Clock  "));
  lcd.setCursor (0,1);
  lcd.print(F("With Alarm"));
  delay (2000);
  lcd.clear();

//rtc.setTime (15, 30, 00); // set time in format of HH:MM:SS in 24 hour format
//rtc.setDate(4, 3, 2019);  // Set the date in format of DD:MM:YY
}

void loop()
{
t = rtc.getTime();
Day = rtc.getDOWStr(1);

if (setMode == 0)
{
hh = t.hour,DEC;
mm = t.min,DEC;
ss = t.sec,DEC;
dd = t.date,DEC;
bb = t.mon,DEC;
yy = t.year,DEC;
}  
  if (setAlarm <2)
  {
  lcd.setCursor(1,0);
  if(setMode==0)
  lcd.print(Day);
  else
  {
    lcd.print(F("SET"));
    }
  lcd.print(F(" "));
  lcd.print(dd);
  lcd.print(F("-")); 
  lcd.print(bb);
  lcd.print(F("-")); 
  lcd.print(yy);
  lcd.print(F("  ")); 

  lcd.setCursor(0,1); 
  if (hh<10)
  {
    lcd.print(F("0"));
    }
  lcd.print(hh);
  lcd.print(F(":"));
  if (mm<10)
  {
    lcd.print(F("0"));
    } 
  lcd.print(mm);
  lcd.print(F(":"));
  if (ss<10){ lcd.print(F("0"));} 
  lcd.print(ss);
  lcd.print(F(" ")); 
  lcd.print(rtc.getTemp());
  lcd.print((char)0); 
  lcd.print(F("C"));
  lcd.print(F(" ")); 
  }
  
  setupClock();
  Alarm (alarmHH, alarmMM, alarmLONG);

if (setAlarm <2 && setMode != 0)
{
  delay (100);
  }
if (setAlarm <2 && setMode == 1)
{
  lcd.setCursor(5,0);
  lcd.print(F("  "));
  delay(100);
  }
if (setAlarm <2 && setMode == 2 && bb>9)
{
  lcd.setCursor(8,0);
  lcd.print(F("  "));
  delay(100);
  }
if (setAlarm <2 && setMode == 2 && bb<=9)
{
  lcd.setCursor(8,0);
  lcd.print(F(" "));
  delay(100);
  }
if (setAlarm <2 && setMode == 3 && bb>9)
{
  lcd.setCursor(11,0);
  lcd.print(F("    "));
  delay(100);
  }
if (setAlarm <2 && setMode == 3 && bb<=9)
{
  lcd.setCursor(10,0);
  lcd.print(F("    "));
  delay(100);
  }
if (setAlarm <2 && setMode == 4)
{
  lcd.setCursor(0,1);
  lcd.print(F("  "));
  delay(100);
  }
if (setAlarm <2 && setMode == 5)
{
  lcd.setCursor(3,1);
  lcd.print(F("  "));
  delay(100);
  }
if (setAlarm <2 && setMode == 6)
{
  lcd.setCursor(6,1);
  lcd.print(F("  "));
  delay(100);
  }

if (setMode == 0 && setAlarm != 0  && setAlarm != 1)
{
  delay (100);
  }
if (setMode == 0 && setAlarm == 2)
{
  lcd.setCursor(0,1);
  lcd.print(F("  "));
  delay(100);
  }
if (setMode == 0 && setAlarm == 3)
{
  lcd.setCursor(3,1);
  lcd.print(F("  "));
  delay(100);
  }
if (setMode == 0 && setAlarm == 4 && alarmLONG >9)
{
  lcd.setCursor(12,1);
  lcd.print(F("  "));
  delay(100);
  }
if (setMode == 0 && setAlarm == 4 && alarmLONG <10)
{
  lcd.setCursor(12,1);
  lcd.print(F(" "));
  delay(100);
  }

Serial.print (setMode);
Serial.print ("  ");
Serial.println (setAlarm);
}

void setupClock (void)
{
  //read the buttons
   btnUP.read();                             
   btnDN.read();
   btnSET.read();
   btnALR.read();
   
    if (setMode == 7)
    {
    lcd.setCursor (0,0);
    lcd.print (F("Date is set"));
    lcd.setCursor (0,1);
    lcd.print (F("Time is set"));
    delay (1000);
    rtc.setTime (hh, mm, ss);
    rtc.setDate (dd, bb, yy);  
    lcd.clear();
    setMode = 0;
    }

    if (setAlarm == 5)
    {
    lcd.setCursor (0,0);
    lcd.print (F("Alarm is set"));
    lcd.setCursor (0,1);
    lcd.print (F("Alarm at : "));
    lcd.print (alarmHH);
    lcd.print (F(":"));
    lcd.print (alarmMM); 
    delay (1000);
    lcd.clear();
    setAlarm=0;
    alarmMode=1;
    }
    
    if (setAlarm >0)
    {
      alarmMode=0;
      }
    
    switch (STATE)
    {
        
        case WAIT:                            
            if (btnSET.wasPressed())
               {
                setMode = setMode+1;
                }
            if (btnALR.wasPressed())
               {
                setAlarm = setAlarm+1;
                } 
            if (btnUP.wasPressed())
                STATE = INCR;
            else if (btnDN.wasPressed())
                STATE = DECR;
            else if (btnUP.wasReleased())    
                rpt = REPEAT_FIRST;
            else if (btnDN.wasReleased())
                rpt = REPEAT_FIRST;
            else if (btnUP.pressedFor(rpt))
            {  
                rpt += REPEAT_INCR;            
                STATE = INCR;
            }
            else if (btnDN.pressedFor(rpt))
            {
                rpt += REPEAT_INCR;
                STATE = DECR;
            }
            break;

        case INCR:                               
            if (setAlarm<2 && setMode==1 && dd<31)dd=dd+1; 
            if (setAlarm<2 && setMode==2 && bb<12)bb=bb+1;
            if (setAlarm<2 && setMode==3 && yy<2050)yy=yy+1;
            if (setAlarm<2 && setMode==4 && hh<23)hh=hh+1;
            if (setAlarm<2 && setMode==5 && mm<59)mm=mm+1;
            if (setAlarm<2 && setMode==6 && ss<59)ss=ss+1;
            if (setMode==0 && setAlarm==2 && alarmHH<23)alarmHH=alarmHH+1;
            if (setMode==0 && setAlarm==3 && alarmMM<59)alarmMM=alarmMM+1;
            if (setMode==0 && setAlarm==4 && alarmLONG<59)alarmLONG=alarmLONG+1;
            STATE = WAIT;
            break;

        case DECR:                               
            if (setAlarm<2 && setMode==1 && dd>0)dd=dd-1;
            if (setAlarm<2 && setMode==2 && bb>0)bb=bb-1;
            if (setAlarm<2 && setMode==3 && yy>2000)yy=yy-1;
            if (setAlarm<2 && setMode==4 && hh>0)hh=hh-1;
            if (setAlarm<2 && setMode==5 && mm>0)mm=mm-1;
            if (setAlarm<2 && setMode==6 && ss>0)ss=ss-1;  
            if (setMode==0 && setAlarm==2 && alarmHH>0)alarmHH=alarmHH-1;
            if (setMode==0 && setAlarm==3 && alarmMM>0)alarmMM=alarmMM-1;
            if (setMode==0 && setAlarm==4 && alarmLONG>0)alarmLONG=alarmLONG-1;
            STATE = WAIT;
            break;
    }
}


void Alarm (uint8_t alarmHH, uint8_t alarmMM,int alarmLONG)
{

 if (alarmMode==1 && hh == alarmHH && (mm - alarmMM >= 0 ) && (mm - alarmMM <= alarmLONG ))
 {
  tone (13, 1200);
  btnALR.read();
  if (btnALR.wasPressed())
 {
  alarmMode = 0;
  }
  }
 else
 {
  noTone (13);
  }

 if (setMode == 0 && setAlarm !=0 && setAlarm !=1)
 {
  lcd.setCursor (1,0);
  lcd.print(F("  Set Alarm    "));
  lcd.setCursor (0,1);
  if(alarmHH<10)
  {
    lcd.print(F("0"));
    }
  lcd.print (alarmHH);
  lcd.print(F(":"));
  if(alarmMM<10)
  {
    lcd.print(F("0"));
    }
  lcd.print (alarmMM);
  lcd.print(F("  Long:"));
  lcd.print (alarmLONG);
  lcd.print(F("mnt "));
 }
}
