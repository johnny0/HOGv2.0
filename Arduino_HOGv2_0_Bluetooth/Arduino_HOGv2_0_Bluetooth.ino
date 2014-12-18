/*
Bluetooth HOGv2.0 software

Made by: Martijn van Ingen & Johnny Demarsin
PXL HOGeschool Electronics-ICT

partially inspired by the code of: Jim Lindblom
https://learn.sparkfun.com/tutorials/using-the-bluesmirf#firmware-overview

This code reads a string in the form of "[servo1][servo2][motor];".
It will convert the data and send it to the servo's on the HOGv2.0.

For information about our project and the .stl files for the HOGv2.0, pleas go the the following sites
http://projecthog.drupalgardens.com/
http://www.thingiverse.com/thing:350787
*/


#include <SoftwareSerial.h>      
#include <String.h>
#include <Servo.h>



Servo HOGServo1;      // define the two servo's
Servo HOGServo2;

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3

int motorPin = 9;    // define the motor

String stringIn;    // define some strings for reading the strings from LabVIEW
String servo1;
String servo2;
String motor;

int servo1Val;      // define some variables for the servo's
int servo2Val;
int motorVal;

char c;            // place for the incomming data

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup()
{
	Serial.begin(9600);  // Begin the serial monitor at 9600bps

	bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
	bluetooth.print("$");  // Print three times individually
	bluetooth.print("$");
	bluetooth.print("$");  // Enter command mode
	delay(100);  // Short delay, wait for the Mate to send back CMD
	bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
	// 115200 can be too fast at times for NewSoftSerial to relay the data reliably
	bluetooth.begin(9600);  // Start bluetooth serial at 9600
        
        HOGServo1.attach(11);      // attach the servo's
        HOGServo2.attach(10);
        pinMode(motorPin, OUTPUT);    //data mode for the motor
}



void loop()
{
	if (bluetooth.available())  // If the bluetooth sent any characters
	{
		
		
             c = (char)bluetooth.read();    // place the incomming data in a character
             
             stringIn = String(stringIn + c);    //place the data in a string
        
        
        if (c == ';')      // if the end of string is seen, work with the data
        {
      
          servo1 =  stringIn.substring(0, 1);    //split the data into smaller pieces for each servo or motor
          servo2 =  stringIn.substring(1, 2);
          motor  =  stringIn.substring(2, 3);
      
          Serial.print("Servo1 = ");      // This part is only for debugging
          Serial.println(servo1);         // It shows the data that it received from LabVIEW
          Serial.print("Servo1 = ");      // after it has bin assigned to it's correct servo or motor
          Serial.println(servo2);
          Serial.print("Motor = ");
          Serial.println(motor);
          Serial.println("");
          
          
          servo1Val = servo1.toInt();      //changes the String to int
          servo2Val = servo2.toInt();
          motorVal = motor.toInt();    
    
          servo1Val = map(servo1Val, 0, 9, 70, 120);    //mapping of the values to angles for the servo
          servo2Val = map(servo2Val, 0, 9, 45, 135);  
          
          HOGServo1.write(servo1Val);                   //writing the angles to the servos
          HOGServo2.write(servo2Val);
          
          if (motorVal == 1)                            //if the motor must be on
          {
            digitalWrite(motorPin, HIGH);               //activate the motor
          }
          if (motorVal == 0)                            // if the motor must be off
          {
            digitalWrite(motorPin, LOW);                //deactivate the motor
          }
          
      
          servo1 = "";                                  //clear all strings for new data
          servo2 = "";
          motor = "";
          stringIn = "";
      
        }
               
}
}
