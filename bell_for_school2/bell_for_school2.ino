#include <DS3231.h> //Library for Real Time Clock.
#include <Wire.h>
#include <LiquidCrystal_I2C.h>// Library for I2c 16X2 display
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); //Pin10 RX , Pin 11 TX connected to--> Bluetooth TX,RX

char val;
int x = 0;

//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
int Releu = 8; //Relay declaration pin
DS3231  rtc(SDA, SCL); //RTC declaration
Time t;
int k = 0;
void setup()
{

  mySerial.begin(9600);
  Serial.begin(9600);
 
  rtc.begin();
  pinMode(Releu, OUTPUT);

  rtc.setTime(11 , 49 , 50); // Set the time 
  rtc.setDate(11, 9, 2018); // Set the date
  rtc.setDOW(TUESDAY); // Set the Day Of Week (No ring on Saturday and Sunday)
  digitalWrite(Releu, HIGH);
}

void loop() {

  /
  lcd.print("Ora:  ");
  lcd.print(rtc.getTimeStr());

  lcd.setCursor(0, 1);
  lcd.print("Data: ");
  lcd.print(rtc.getDateStr());



  //lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Time:  ");
  lcd.print(rtc.getTimeStr());

  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(rtc.getDateStr());

  t = rtc.getTime();

  Serial.print(" hour  ");
  Serial.print(t.hour);
  Serial.print(", ");
  Serial.print(t.min);
  Serial.print(" minutes, ");
  Serial.print(t.sec);
  Serial.println(" seconds.");
  Serial.print(t.dow);
  delay (1000);
  const int numarore = 13;// Number of hours
  int ore[numarore] {8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19}; // Hours for ring
  int minute[2] {0, 50}; // Minutes when ring
  int secunde[4] {0, 1, 2, 3}; // How many seconds ring
  bool releuflag = true;
  digitalWrite(Releu, HIGH);
  // Serial.print(t.date);

  if ( mySerial.available() > 0 ) {
    val = mySerial.read();
    Serial.println(val);
  }
  if (t.dow == 7 && t.mon == 10 && t.hour == 3 && t.min == 0 && t.sec == 0 ) // This is for country when hours change (Summer and winter hour ) EET Romania
  {
    k = k + 1;
    if (k != 3)
    {
      rtc.setTime(02 , 00 , 00);
      k = 0;
    }

  }

  if (t.dow == 7 && t.mon ==  3 && t.hour == 2 && t.min == 0 && t.sec == 0)

  {
    k = k + 1;
    if (k != 3)
    {
      rtc.setTime(03 , 00 , 00);
      k = 0;
    }
  }
  if ( val == '2' ) // Vacation mode On
  { //program vacanta on
    x = 1;
  }


  else if ( val == 'A' ) //Vacation mode off
  {
    x = 0;
    //program vacanta off
  }
  else if (val == 'B')
  {
    //suna Ring
    digitalWrite(Releu, HIGH);
    
  }
  else if (val == '3')
    digitalWrite(Releu, LOW);
  if (x != 1)
  {
    if (t.dow != 6 && t.dow != 7)
    {

      for (int i = 0; i <  numarore; i++) {

        if (ore[i] == 20) {
          if (t.min == minute[0]
              && (t.sec == secunde[0] || t.sec == secunde[1] || t.sec == secunde[2] || t.sec == secunde[3])) {
            digitalWrite(Releu, LOW);
            Serial.println("PORNIT");//ON
            releuflag = false;
            break;
          }
        }


        if (t.hour == ore[i] && (t.min == minute[0] || t.min == minute[1]) && (t.sec == secunde[0] || t.sec == secunde[1] || t.sec == secunde[2] || t.sec == secunde[3]))
        {


          digitalWrite(Releu, LOW);
          Serial.println("PORNIT");
          releuflag = false;
          break;
        }
      }
      if (((t.sec == 0 || t.sec == 1 || t.sec == 2 || t.sec == 3) && (t.min == 50 || t.min == 0) && (t.hour == 21 || t.hour == 22 || t.hour == 23 || t.hour == 24 || t.hour == 0 || t.hour == 1 || t.hour == 2 || t.hour == 3 || t.hour == 4 || t.hour == 5 || t.hour == 6 || t.hour == 7)))
      {
        digitalWrite(Releu, HIGH);


        if (releuflag) {
          digitalWrite(Releu, HIGH) ;
        }


      }
    }
  }
  ///Cod Pauza Mare
  //Code with big break
  /*


    if((t.hour==8||t.hour==9||t.hour==10)&&(t.min==0||t.min==50)&& (t.sec == secunde[0] || t.sec == secunde[1] || t.sec == secunde[2] || t.sec == secunde[3]))
       {
       digitalWrite(Releu, LOW);
           Serial.println("PORNIT");
           releuflag = false;


       }
       if(t.hour==11&&(t.min==10)&&(t.sec == secunde[0] || t.sec == secunde[1] || t.sec == secunde[2] || t.sec == secunde[3]))
         {
       digitalWrite(Releu, LOW);
           Serial.println("PORNIT");
           releuflag = false;

       }
       if((t.hour==12||t.hour==13||t.hour==14||t.hour==15||)&&(t.min==10||t.min=0)&&(t.sec == secunde[0] || t.sec == secunde[1] || t.sec == secunde[2] || t.sec == secunde[3]))
       {
       digitalWrite(Releu, LOW);
           Serial.println("PORNIT");
           releuflag = false;

       }

  */

}
