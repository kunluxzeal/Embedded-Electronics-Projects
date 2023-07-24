#include <DS3231.h> //Library for Real Time Clock.
#include <Wire.h>


char val;
int x = 0;


int Relay = 8; //Relay declaration pin
DS3231  rtc(SDA, SCL); //RTC declaration
Time t;
int k = 0;
void setup()
{

  
  Serial.begin(9600);
 
  rtc.begin();
  pinMode(Relay, OUTPUT);

  rtc.setTime(4 , 8 , 20); // Set the time 
  rtc.setDate(12, 11, 2022); // Set the date
  rtc.setDOW("Saturday"); // Set the Day Of Week (No ring on Saturday and Sunday)
  digitalWrite(Relay, HIGH);
}

void loop() {

  
  Serial.print("now:  ");
  Serial.print(rtc.getTimeStr());


  Serial.print("Date: ");
  Serial.print(rtc.getDateStr());



  //lcd.begin(16, 2);

  Serial.print("Time:  ");
  Serial.print(rtc.getTimeStr());

  
  Serial.print("Date: ");
  Serial.print(rtc.getDateStr());

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
  const int no_of_hours = 13;// Number of hours
  int now[no_of_hours] {8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19}; // Hours for ring
  int minute[2] {0, 50}; // Minutes when ring
  int secunde[4] {0, 1, 2, 3}; // How many seconds ring
  bool relayflag = true;
  digitalWrite(Relay, HIGH);
  // Serial.print(t.date);

  if (Serial.available() > 0 ) {
    val = Serial.read();
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
    digitalWrite(Relay, HIGH);
    
  }
  else if (val == '3')
    digitalWrite(Relay, LOW);
  if (x != 1)
  {
    if (t.dow != 6 && t.dow != 7)
    {

      for (int i = 0; i <  no_of_hours; i++) {

        if (now[i] == 20) {
          if (t.min == minute[0]
              && (t.sec == secunde[0] || t.sec == secunde[1] || t.sec == secunde[2] || t.sec == secunde[3])) {
            digitalWrite(Relay, LOW);
            Serial.println("PORNIT");//ON
            relayflag = false;
            break;
          }
        }


        if (t.hour == now[i] && (t.min == minute[0] || t.min == minute[1]) && (t.sec == secunde[0] || t.sec == secunde[1] || t.sec == secunde[2] || t.sec == secunde[3]))
        {


          digitalWrite(Relay, LOW);
          Serial.println("PORNIT");
          relayflag = false;
          break;
        }
      }
      if (((t.sec == 0 || t.sec == 1 || t.sec == 2 || t.sec == 3) && (t.min == 50 || t.min == 0) && (t.hour == 21 || t.hour == 22 || t.hour == 23 || t.hour == 24 || t.hour == 0 || t.hour == 1 || t.hour == 2 || t.hour == 3 || t.hour == 4 || t.hour == 5 || t.hour == 6 || t.hour == 7)))
      {
        digitalWrite(Relay, HIGH);


        if (relayflag) {
          digitalWrite(Relay, HIGH) ;
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