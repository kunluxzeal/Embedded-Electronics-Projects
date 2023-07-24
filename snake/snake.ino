#include <Wire.h>
#include <LiquidCrystal.h>

const int RS = 8;
const int EN = 9;
const int d4 = 4;
const int d5 = 5;
const int d6 = 6;
const int d7 = 7;
const int pin_BL = 10; // arduino pin wired to LCD backlight circuit
LiquidCrystal lcd( RS,  EN,  d4,  d5,  d6,  d7);


//Define Yes and No

const int YES = 1;
const int NO = 0;

// Define Maximum Length of Snake

int maxLength = 12;

// Set the time it takes the snake to move

long timeToMove = 200;                                 


const int lcdRows = 2;                 //2 Rows
const int lcdCols = 16;                 //16 Columns

byte snake[8] = {
 B00000,
 B00000,
 B00000,
 B00000,
 B11111,
 B00000,
 B00000,
 B00000,
}; 

byte HEAD[8] = {
 B00000,
 B00000,
 B00000,
 B00010,
 B11111,
 B00010,
 B00000,
 B00000,
};

byte APPLE[] = {
 B00000,
 B00000,
 B00000,
 B00000,
 B00100,
 B00000,
 B00000,
 B00000,
};
int n = 0;
void setup() {

 //Set Pins as Output

  pinMode(RS, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(d5, OUTPUT);
  pinMode(d6, OUTPUT);
  pinMode(d7, OUTPUT);
  pinMode(pin_BL, OUTPUT);
  
//Turn on the Backlight

  digitalWrite(pin_BL, HIGH);
  
}
void loop() {
 int extend = NO;                 //Define and Set the Extend Snake flag to No
 const int CLEAR = 3; 
  const int TAIL = 4;               //The Tail is Custom Character 0
 //Custom Character 2 is used for Clear Space
   lcd.begin(lcdRows, lcdCols);
 lcd.createChar(0, snake);
 lcd.createChar(1, HEAD);
 lcd.createChar(2, APPLE);

int lcdOutput [lcdRows][lcdCols];

  int oldLcdOutput [lcdCols][lcdRows];     //Holds the previous LCD Output
 
 for (int i = 0; i <lcdRows; i++){ 
  for (int  j = 0; j<lcdCols ; j++){
    lcdOutput[lcdRows][lcdCols] =2;
   
    }
 }

lcdOutput[0][0] =snake;
lcdOutput[1][0] = HEAD;
int currentLength = 3;                  // Set current snake length
  unsigned long previousMillis = 0;       // Will store last time the snake moved
 
  while(1==1){                                           //Repeat this part forever!
                                                         //Render the output on the LCD
       for (int row=0; row < lcdRows; row++){            //For each Row
          for (int col =0; col < lcdCols; col++){        //For each column in the row
            lcd.setCursor(lcdCols, lcdCols);                     //Set the cursor position
            lcd.write(lcdOutput[lcdCols][lcdRows]);              //Write the required Character
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
                   if(oldLcdOutput[col][row] == APPLE){                   //If an apple spawns next to the snakes tail, clear the tail on the next refresh
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
}
   
  

 
  
 
