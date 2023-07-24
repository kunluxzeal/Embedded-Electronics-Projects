// TCS230 sensor and colour calibration
//
// Input and output using the Serial console.
//
#include <MD_TCS230.h>   // colour sensor library
#include <FreqCount.h>   // used by above library
#include <EEPROM.h>      // Ensure data is still valid after power offs
 
// Pin definitions
#define  S2_OUT  6
#define  S3_OUT  7
#define  OE_OUT   8               // LOW = ENABLED wh
#define MAX_COLOURS 10            // Max colours allowed to store, 10 takes up 140 bytes
#define MAX_COLOUR_NAME_CHARS 10  // Max chars allowed in name of colour
#define TOLERANCE 35              // How far out the red,green or blue can be to match
 
typedef struct
{
  char Name[MAX_COLOUR_NAME_CHARS+1];             // Name for Colour, plus 1 extra for terminator '0'
  uint8_t Red,Green,Blue;  // The colour values
}SingleColour;
 
SingleColour Colours[MAX_COLOURS];        // Our array of colours to match against      
uint8_t NumColours=0;                     // Current Number of colours  
 
MD_TCS230 CS(S2_OUT, S3_OUT, OE_OUT);
 
void setup()
{
  Serial.begin(115200);
  CS.begin();
  // Check if we have some calibration data
  ReadCalibrations();
  ReadColours();
}
 
void loop()
{
  static char Choice;         
 
  Choice=MainMenu();
  switch(Choice)
  {
    case '1':
      CalibrateSensor();
     break;
    case '2':  
      CalibrateColour();
      break;
    case '3':  
      DisplayColours(true);
      break;
    case '4':  
      DeleteColour();
      break;
    case '5':
      ClearEEPROMColours();
      break;
    case '6':
      ClearCalibration();
    case '7':
      ScanColour();
      break;
  }
}
 
void ClearCalibration()
{
  uint8_t DataIdx;
  EEPROM.put(DataIdx,"   ");
  DataIdx=3+sizeof(sensorData);
  EEPROM.put(DataIdx,"   ");
  Serial.println("\nSensor calibration cleared");
}
 
void ClearEEPROMColours()
{
  // Just reset number of colours to 0  
  uint16_t DataIdx;
  DataIdx=9+(2*sizeof(sensorData));   // Move past black and white calibration settings
  EEPROM.put(DataIdx,0);
  Serial.println("\nEEPROM colours cleared");
}
 
void WriteColoursToEEPROM()
{
  uint16_t DataIdx;
 
  DataIdx=6+(2*sizeof(sensorData));   // Move past black and white calibration settings
  EEPROM.put(DataIdx,"COL");          // Identifier
  DataIdx+=3;
  EEPROM.put(DataIdx,NumColours);
  DataIdx++;
  for(uint8_t i=0;i<NumColours;i++)
  {
    EEPROM.put(DataIdx,Colours[i]);
    DataIdx+=sizeof(SingleColour);
  }
  Serial.println("\nColours written to EEPROM\n");
}
 
void WriteColourToEEPROM(uint8_t ColIdx)
{
  uint16_t DataIdx;                   // index into data
 
  DataIdx=6+(2*sizeof(sensorData));   // Move past black and white calibration settings  
    // Check if any colours written at all, if not then set it up
  if(((EEPROM.read(DataIdx)=='C')&(EEPROM.read(DataIdx+1)=='O')&(EEPROM.read(DataIdx+2)=='L'))==false)
    EEPROM.put(DataIdx,"COL");          // Identifier
  // Store the colour
  DataIdx=ColourEEPROMStartAddress(ColIdx);
  EEPROM.put(DataIdx,Colours[ColIdx]);  
}
 
uint16_t ColourEEPROMStartAddress(uint8_t ColIdx)
{  
  // return address for this particular colour in the EEPROM
  // Broken down into steps so you can see how things are stored
  uint16_t DataIdx=0;
  DataIdx+=3;     // Black calibration Identifier 'BLK'
  DataIdx+=sizeof(sensorData);        //Black sensor data
  DataIdx+=3;     // White calibration Identifier  'WHT'
  DataIdx+=sizeof(sensorData);        //White sensor data
  DataIdx+=3;     // 'COL' identifier to show valid colour data
  DataIdx+=1;     // NUmber of colours
  // We are now at the start of the colours and this would be the first colour, simply multiply
  // the size of the colour data by the colidx
  DataIdx+=(ColIdx * sizeof(SingleColour));
  return DataIdx;
}
 
void DeleteColour()
{
  char ColourName[MAX_COLOUR_NAME_CHARS];             // Store of name
  int8_t ColIdx;
 
  Serial.print(F("\nEnter name for the colour to delete [Max "));
  Serial.print(MAX_COLOUR_NAME_CHARS);
  Serial.print(" chars] followed RETURN\n\n");
  GetColourName(ColourName);
  ColIdx=ColourIndex(ColourName);
  if(ColIdx==-1)
  {
    Serial.print(ColourName);
    Serial.print(F(" could not be found, returning to main menu\n"));
    return;
  }
  // To remove colour we just shuffle the colours around and reduce the count by 1
  ShuffleColoursDown(ColIdx);    
  Serial.print(F("\nThe colour "));
  Serial.print(ColourName);
  Serial.print(F(" has been removed\n"));  
  WriteColoursToEEPROM();
}
 
void ShuffleColoursDown(uint8_t ColIdx)
{
  // All colours above ColIdx are move down one position, effectivly removing the colour at ColIdx
  for(uint8_t i=ColIdx+1;i<NumColours;i++)
  {
    strcpy(Colours[i-1].Name,Colours[i].Name);
    Colours[i-1].Red=Colours[i].Red;
    Colours[i-1].Green=Colours[i].Green;
    Colours[i-1].Blue=Colours[i].Blue;
  }
  NumColours--;  // one less colour
}
 
char MainMenu()
{  
  char Choice;
  Serial.print(F("\nCalibration Menu (Choose option and press RETURN)\n"));
  Serial.print(F("1 Calibrate sensor\n"));
  Serial.print(F("2 Add/Update colour\n"));
  Serial.print(F("3 Display colours\n"));
  Serial.print(F("4 Delete colour\n"));
  Serial.print(F("5 Delete all colours\n"));
  Serial.print(F("6 Clear sensor calibration settings\n"));
  Serial.print(F("7 Scan Colour\n"));
  return getChar();
}
 
void ScanColour()
{
  // Scan and find a colour on the sensor
  colorData  rgb;  
  CS.read();
  while(CS.available()==0);  // wait for read to complete
  CS.getRGB(&rgb);  
  int8_t ColIdx=MatchColour(&rgb);  
  Serial.print(F("\nScanning for RGB["));
  Serial.print(rgb.value[TCS230_RGB_R]);
  Serial.print(F(","));
  Serial.print(rgb.value[TCS230_RGB_G]);
  Serial.print(F(","));
  Serial.print(rgb.value[TCS230_RGB_B]);
  Serial.print(F("]\n"));
  if(ColIdx==-1)
    Serial.println("\nNo match found");
  else
  {
    Serial.print("\nThat colour is ");
    Serial.print(Colours[ColIdx].Name);
    Serial.print(" [");
    Serial.print(Colours[ColIdx].Red);
    Serial.print(F(","));
    Serial.print(Colours[ColIdx].Green);
    Serial.print(F(","));
    Serial.print(Colours[ColIdx].Blue);
    Serial.println("]");
  }
}
 
int8_t MatchColour(colorData *rgb)
{
  // Look through colours looking for a match
  uint8_t Idx=0;
  bool Found=false;
  while((Idx<NumColours)&(Found==false))
  {
    if((CheckColour(rgb->value[TCS230_RGB_R],Colours[Idx].Red)) &(CheckColour(rgb->value[TCS230_RGB_G],Colours[Idx].Green))&(CheckColour(rgb->value[TCS230_RGB_B],Colours[Idx].Blue)))
          Found=true;
    Idx++;
  }
  if(Found) return Idx-1; else return -1;
}
 
bool CheckColour(uint8_t ScanCol, uint8_t StoredCol)
{
  // returns true if matched on this single colour else false
  int16_t StoreColLow,StoreColHigh;
  StoreColLow=StoredCol-TOLERANCE;
  StoreColHigh=StoredCol+TOLERANCE;
  if(StoreColLow<0) StoreColLow=0;
  if(StoreColHigh>255) StoreColHigh=255;
  return ((ScanCol>=StoreColLow)&(ScanCol<=StoreColHigh));
 
}
 
void DisplayColours(bool ShowTitle)
{
  // List all the current colours (not nessasarily saved back to EEPROM yet
 
  char ColStr[4];                   //Temp store for the integer colour value to a string for formatted printing
 
  if(NumColours==0)
  {
    Serial.print("No current colours stored in RAM memory\n");
    return;
  }
  if(ShowTitle)
    Serial.print("\nCurrent colours stored in RAM memory\n");
  Serial.println("Name       Red  Green  Blue");
  for(uint8_t i=0;i<NumColours;i++)
  {
    OutputPadded(Colours[i].Name,MAX_COLOUR_NAME_CHARS);
    Serial.print(" ");
    itoa(Colours[i].Red,ColStr,10);
    OutputPadded(ColStr,3);
    Serial.print("  ");
    itoa(Colours[i].Green,ColStr,10);
    OutputPadded(ColStr,3);
    Serial.print("    ");
    itoa(Colours[i].Blue,ColStr,10);
    OutputPadded(ColStr,3);
    Serial.println();
  }
}
 
void ReadColours()
{
  // Read in any colour data
  uint16_t DataIdx;                   // index into data
 
  DataIdx=6+(2*sizeof(sensorData));   // Move past black and white calibration settings
  
  if((EEPROM.read(DataIdx)=='C')&(EEPROM.read(DataIdx+1)=='O')&(EEPROM.read(DataIdx+2)=='L'))
  {
    // Valid colour data
    DataIdx+=3;
    // How many colours stored
    EEPROM.get(DataIdx,NumColours);
    if(NumColours==0)
    {
    Serial.println("No colours stored in EEPROM\n");
      return;
    }
    if(NumColours>MAX_COLOURS)
    {
      Serial.print(F("Number of colours ("));
      Serial.print(NumColours);
      Serial.print(F(") exceeds maximum of "));
      Serial.print(MAX_COLOURS);
      Serial.print(F(",only first "));
      Serial.print(MAX_COLOURS);
      Serial.print(F(",will be read in."));
      NumColours=MAX_COLOURS;
    }    
    DataIdx++;
    for(uint8_t i=0;i<NumColours;i++)
    {
      EEPROM.get(DataIdx,Colours[i]);
      DataIdx+=sizeof(SingleColour);
    }
    Serial.println("Colours read in from EEPROM\n");
    DisplayColours(false);
  }
  else
    Serial.println(F("No Colour data stored"));
}
 
void ReadCalibrations()
{
  uint16_t DataIdx;
  sensorData  sd;
  // If we find some calibration data then calibrate sensor
  if((EEPROM.read(0)=='B')&(EEPROM.read(1)=='L')&(EEPROM.read(2)=='K'))
  {
    // Black Calibration data present
    EEPROM.get(3, sd);
    CS.setDarkCal(&sd);
    Serial.print("Black Calibration read in, ");
  }
  else
    Serial.print("No black calibration data, ");
    
  DataIdx=3+sizeof(sensorData);
  if((EEPROM.read(DataIdx)=='W')&(EEPROM.read(DataIdx+1)=='H')&(EEPROM.read(DataIdx+2)=='T'))
  {
    // white Calibration data present
    EEPROM.get(DataIdx+3, sd);
    CS.setWhiteCal(&sd);
    Serial.println("White Calibration read in.");
  }
  else
    Serial.println("No white calibration data");
  
}
 
void CalibrateSensor()
{
  uint16_t DataIdx;
  sensorData  sd;
  Serial.print(F("\n****** Calibrate sensor ******\n"));
  Serial.print(F("Put black matt object over sensor, then press any key followed by RETURN\n"));
  getChar();
  CS.read();
  while(CS.available()==0);  // wait for read to complete
  CS.getRaw(&sd);
  CS.setDarkCal(&sd);  
  // Store this sensor black data
  EEPROM.write(0,'B');EEPROM.write(1,'L');EEPROM.write(2,'K');      // This acts as as simple indicator
  EEPROM.put(3,sd);
  Serial.print(F("Put white object over sensor, then press any key followed by RETURN\n"));
  getChar();
  CS.read();
  while(CS.available()==0);  // wait for read to complete
  CS.getRaw(&sd);
  CS.setWhiteCal(&sd);  
  DataIdx=3+sizeof(sensorData);  // Store white data after black data
  EEPROM.write(DataIdx,'W');EEPROM.write(DataIdx+1,'H');EEPROM.write(DataIdx+2,'T');      // This acts as as simple indicator
  EEPROM.put(DataIdx+3,sd);
  Serial.print(F("Calibration of sensor complete.\n\n"));
}
 
void CalibrateColour()
{
  colorData rgb;
  uint16_t DataIdx;                
  char ColourName[MAX_COLOUR_NAME_CHARS];             // Store of name
  int8_t ColIdx;
  char Choice;                     //
    
  Serial.print(F("Enter name for colour [Max "));
  Serial.print(MAX_COLOUR_NAME_CHARS);
  Serial.print(" chars] followed RETURN\n");
  GetColourName(ColourName);
  ColIdx=ColourIndex(ColourName);
  if(ColIdx>=0)
  {    
    Serial.print(F("\nWARNING: The colour "));
    Serial.print(ColourName);
    Serial.print(F(" already exists, overwrite (Y/N)\n"));
    if(toupper(getChar())!='Y')
    {
      Serial.print(F("\nColour calibration abandoned\n"));
      return;
    }    
  }
  if(ColIdx==-1)
  {
    // Check if can add a new colour
    if(NumColours==MAX_COLOURS)
    {      
      Serial.print(F("\nWARNING: There is no room left for new colours, you have reached the maximum of "));
      Serial.print(NumColours);
      Serial.print(F(" colours\n\n"));
      Serial.print(F("You will need to delete a colour first to make room.\n\n"));
      return;
    }
  }
  Serial.print(F("Reading Color '"));
  Serial.print(ColourName);
  CS.read();
  while(CS.available()==0);  // wait for read to complete
  CS.getRGB(&rgb);
  Serial.print(F("\nRGB is ["));
  Serial.print(rgb.value[TCS230_RGB_R]);
  Serial.print(F(","));
  Serial.print(rgb.value[TCS230_RGB_G]);
  Serial.print(F(","));
  Serial.print(rgb.value[TCS230_RGB_B]);
  Serial.print(F("]\n"));
  if(ColIdx==-1)
    ColIdx=NumColours;  // If not already stored set to next available colour slot
  strcpy(Colours[ColIdx].Name,ColourName);
  Colours[ColIdx].Red=rgb.value[TCS230_RGB_R];
  Colours[ColIdx].Green=rgb.value[TCS230_RGB_G];
  Colours[ColIdx].Blue=rgb.value[TCS230_RGB_B];
  if(ColIdx==NumColours)          // If same then added a new colour, increase the numcount
  {
    NumColours++;
    DataIdx=9+(2*sizeof(sensorData));   // Move past black and white calibration settings and 'COL' marker
    EEPROM.put(DataIdx,NumColours);     // Write new total to EEPROM
  }
  WriteColourToEEPROM(ColIdx);
}
 
int8_t ColourIndex(char *ColourName)
{
  // Looks through the colours to see if this colour already exists, if so returns it's index pos in the array
  // else returns -1
  int8_t Idx=0;
  bool Found=false;
  if(NumColours==0)
    return -1;
  while((Idx<NumColours)&(Found==false))
  {
    if(strcasecmp(Colours[Idx].Name,ColourName)==0)
      Found=true;
    else
      Idx++;
  }
  if(Found)
    return Idx;
  else
    return -1;
}
 
void GetColourName(char *ColourName)
{
  uint8_t NameLength=0;
  while (Serial.available() == 0);                                   // Wait for some chars
  NameLength=Serial.readBytes(ColourName,MAX_COLOUR_NAME_CHARS);     // Read them in
  ColourName[NameLength]=0;                                          // terminate string
  
}
 
char getChar()
// Wait for user to return a char over serial connection, returns uppercase version of any alpha char
{
  while (Serial.available() == 0)
    ;
  return(toupper(Serial.read()));
}
 
void OutputPadded(char *Str,uint8_t AmountToPad)
{
  // outputs the colour name post padded with spaces so that it will be a maximum of MAX_COLOUR_NAME_CHARS chars in total
  // This is used to create a more pleasant tidy display when listing colours
  uint8_t Amount=AmountToPad-strlen(Str);  
  Serial.print(Str);
  while(Amount>0)
  {
    Serial.print(" ");
    Amount--;
  }
}
