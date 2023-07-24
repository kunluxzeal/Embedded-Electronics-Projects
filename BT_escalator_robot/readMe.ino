/*
 * This is a Bluetooth controlled escalator robot that takes number of holes
 *  number of seeds and spacings as input from the MIT app inventor to control
 *  the escalator robot
 *  The MIT app to this code sends a string value "<string1,string2,string3>"
 *  The bluetooth section of the arduino code checks if the incoming strings
 *  starts from "<" and extracts the strings and later convert them to an 
 *  integer to control the direction of the motor
 *  When the programmer button in the mit app is pressed it shows a notification
 *  programmed and the bluetooth recieve and send it to arduino.
 */
