/*

This Sketch Demonstrates the use of a 16x2 LCD display and Customer Characters (Glyphs).

The LiquidCrystal library works with all LCD displays that are compatible with the Hitachi HD44780 driver.

The Sketch moves a custom defined snake Glyph clockwise around the edge of the LCD screen. It also randomly spawns an apple which causes the snake to grow when it eats it.

The code is designed to work for any size LCD - you just need to set the LCD size appropriately in the Global Variables.

The circuit:

    * LCD RS pin to digital pin 12
    * LCD Enable pin to digital pin 11
    * LCD D4 pin to digital pin 5
    * LCD D5 pin to digital pin 4
    * LCD D6 pin to digital pin 3
    * LCD D7 pin to digital pin 2
    * LCD R/W pin to ground
    * LCD Backlight Power (A) to digital pin 9 (Through a Resistor)
    * LCD Backlight GND (K) to GND
    * 10K resistor:
       * ends to +5V and ground
       * wiper to LCD VO pin (pin 3)
*/


// include the library code

#include <LiquidCrystal.h>

//Allocate LCD Control Pins

const int RS = 8;             //Set RS pin as Pin 12
const int Enable = 9;         //Set Enable as Pin 11
const int D4 = 4;              //Set D4 pin as Pin 5
const int D5 = 5;              //Set D5 pin as Pin 4
const int D6 = 6;              //Set D6 pin as Pin 3
const int D7 = 7;             //Set D7 pin as Pin 2
const int Backlight = 10;      //Set the backlight power to pin 9

//Set LCD Size

const int lcdRows = 2;                 //2 Rows
const int lcdCols = 16;                 //16 Columns

//Declare the LCD variable, with defined Pins

LiquidCrystal lcd(RS, Enable, D4, D5, D6, D7);

//Define Yes and No

const int YES = 1;
const int NO = 0;

// Define Maximum Length of Snake

int maxLength = 12;

// Set the time it takes the snake to move

long timeToMove = 200;                                 



    int n;
      
    
    

void setup(){

//Set Pins as Output

  pinMode(RS, OUTPUT);
  pinMode(Enable, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(Backlight, OUTPUT);
  pinMode(A0 , INPUT);
//Turn on the Backlight

  digitalWrite(Backlight, HIGH);
  
}


void loop(){
  n = analogRead(A0);


//Initialise LCD

  int extend = NO;                 //Define and Set the Extend Snake flag to No

  lcd.begin(lcdRows, lcdCols);     //Initialise the LCD to the correct size


// Define Snake Custom Character Numbers

  const int TAIL = 4;               //The Tail is Custom Character 0
  const int HEAD = 1;               //The Head is Customer Character 1
  const int CLEAR = 2;              //Custom Character 2 is used for Clear Space
  const int APPLE = 3;

//Define Snake Character BitMaps

  byte snakeHead[8] = {     //The BitMap for the head
     B00000,
     B10001,
     B01110,
     B11011,
     B10001,
     B10101,
     B01010,
     B00100
  };

  byte snakeTail[8] = {    //The BitMap for the tail
     B00000,
     B01110,
     B01110,
     B11011,
     B11011,
     B01110,
     B01110,
     B00000
  };

byte noSnake[8] = {        //The BitMap for Clear Space
     B00000,
     B00000,
     B00000,
     B00000,
     B00000,
     B00000,
     B00000,
     B00000
  };

byte apple[8] = {          //The BitMap for Clear Space
     B00000,
     B00000,
     B00010,
     B00100,
     B01110,
     B11011,
     B11110,
     B01100
  };

//Create Custom Characters (Glyphs)

  lcd.createChar(HEAD, snakeHead);
  lcd.createChar(TAIL, snakeTail);
  lcd.createChar(CLEAR, noSnake);
  lcd.createChar(APPLE, apple);


//Define Arrays for LCD data - Columns by Rows

  int lcdOutput [lcdCols][lcdRows];        //Holds the LCD output
  int oldLcdOutput [lcdCols][lcdRows];     //Holds the previous LCD Output


// Display Title Screen

   lcd.setCursor(6,0);
   lcd.print("Snake!");
   delay(2000);

   lcd.setCursor(7,1); 
   lcd.print("GAME");
   delay(2000);

   lcd.setCursor(2,0);
   lcd.write(HEAD);
   delay(500);
   lcd.setCursor(1,0);
   lcd.write(TAIL);
   delay(500);
   lcd.setCursor(0,0);
   lcd.write(TAIL);
   delay(5000);


//Set snake start position

  for(int row=0; row < lcdRows; row++){    //Clear the Output LCD Array
    for(int col=0; col < lcdCols; col++){
      lcdOutput[col][row]=CLEAR;
    }  
  }

// Put the snake in the 1st 3 Positions on Row 0 : oooO

  lcdOutput[0][0] = TAIL;
  lcdOutput[1][0] = TAIL;
  lcdOutput[2][0] = HEAD;
  
  int currentLength = 3;                  // Set current snake length
  unsigned long previousMillis = n;       // Will store last time the snake moved



  while(1==1){                                           //Repeat this part forever!
                                                         //Render the output on the LCD
       for (int row=0; row < lcdRows; row++){            //For each Row
          for (int col =0; col < lcdCols; col++){        //For each column in the row
            lcd.setCursor(col, row);                     //Set the cursor position
            lcd.write(lcdOutput[col][row]);              //Write the required Character
          }
       }

       unsigned long currentMillis = millis();               //Establish current time

     if(currentMillis - previousMillis > timeToMove) {       //Is it time to move?

      
        previousMillis = currentMillis;                      //If so, save the last time the snake moved and.......

                                                                         //Set current LCD Output to Old LCD Output
        for (int oldrow = 0; oldrow < lcdRows; oldrow++){                   //For each Row
           for (int oldcol = 0; oldcol < lcdCols; oldcol++){                //For each column in the row
              oldLcdOutput[oldcol][oldrow] = lcdOutput[oldcol][oldrow];      //Copy the lcd output to old lcd output
             
           }
        } 

       //Work out the snakes next position
      
      //lcd.noAutoscroll();
     

  }
  
        
         for(int row=0; row < lcdRows; row++){                       //Cycle through each row
       
           if(row == 0){                                             //Process Row 0
                     
             for(int col = 1; col < lcdCols ; col++){
               lcdOutput[col][0]=oldLcdOutput[col-1][0];               //Move Old Cols 1-15 1 place Right
             }
             lcdOutput[0][row]=oldLcdOutput[0][row+1];                 //Move Column 0 up from Row below
           }
           
           if(row >0 && row < lcdRows-1){                            //Process Intermediate Rows
                lcdOutput[0][row]=oldLcdOutput[0][row+1];              //Move Column 0 up from Row below
                lcdOutput[15][row]=oldLcdOutput[15][row-1];            //Move Column 15 down from a row above
           }
           
           if(row == lcdRows-1){                                     //Process last Row//
  
                for (int col = 0; col < 15 ; col++){                    //for Columns 1-15
                   if(oldLcdOutput[col][row] == APPLE && oldLcdOutput[col+1][row] == HEAD && currentLength <= maxLength){   //Check to see if the Snake is about to eat an apple - if so, and the snake is not at maximum length
                     extend = YES;                                                                                          //Mark for Extension

     
                     
                     lcd.setCursor(0,0);
                        lcd.print("Yum, Yum!!!");
                        delay(1000);
                   }

                   if(oldLcdOutput[col+1][row] != APPLE){               //If the column to the right is not an Apple (Stops apples being moved to left)
                     if(oldLcdOutput[col][row] != APPLE){               //and the column in the current position is not an apple (stops apples being deleted by Clear Space)
                        lcdOutput[col][row]=oldLcdOutput[col+1][row];   //Move old Cols 1-15 1 place left
                        
                        if(extend == YES && lcdOutput[col][row] == CLEAR && lcdOutput[col-1][row] == TAIL && col != 0){  //If the snake needs extending and the current column is clear and the one to the left is a tail
                           lcdOutput[col][row] = TAIL;                                                                   //extend the tail in the current column - Will wait until the snake end is in the bottom row, if it extends into the top row
                           currentLength++;                                                                              //Add 1 to the current length
                           extend = NO;                                                                                  //Reset the extend flag
                        }
                     }
                     if(oldLcdOutput[col+1][row] == HEAD){               //If the old column to the right is a head



                      
                        lcdOutput[col][row]=oldLcdOutput[col+1][row];    //Move the head Left to eat the apple!
                        
                     }
                   }
                   if(oldLcdOutput[col][row] == APPLE){                  //If an apple spawns next to the snakes tail, clear the tail on the next refresh
                      lcdOutput[col-1][row] = CLEAR;
                   }
                }
                lcdOutput[15][row]=oldLcdOutput[15][row-1];                //Move previous Row, Column 15 down a row
            }
         }
         
         if(lcdOutput[0][0] == HEAD){                          // When the Head is in Row 0, Column 0 add an apple to a random column in the last row.
            int randomCol = random(3, 13);
            if(lcdOutput[randomCol][lcdRows-1] == CLEAR){
               lcdOutput[randomCol][lcdRows-1] = APPLE;
            }
         }  
     }
  }      
