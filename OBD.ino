/*************************************************************************
* Arduino Implementation for OBD-II UART/I2C Adapter
* Distributed under GPL v2.0
* (C)2014-2015 Kristian Smith <kasmith11@alaska.edu>
*************************************************************************/
#include <SPI.h>
#include <Wire.h>
#include <OBD.h>
#include <SoftwareSerial.h>

const int chipSelect = 10;
SoftwareSerial GPS(2,3);
SoftwareSerial SIM900(4,5);

COBD obd;

void setup()
{//Cell
  Serial.begin(9600);
  SIM900.begin(19200);
  GPS.begin(9600);  
  SIM900power();  
  delay(20000);  // give time to log on to network. 
  
  //OBD
  // we'll use the debug LED as output
  pinMode(13, OUTPUT);
  // start communication with OBD-II adapter
  obd.begin();
  // initiate OBD-II connection until success
  while (!obd.init());
}

void SIM900power()  // software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(5000);
}
/*void sendSMS()
{
  SIM900.print("AT+CMGF=1\r");                                                        // AT command to send SMS message
  delay(100);
  SIM900.println("AT + CMGS = \"+19076027732\"");                                     // recipient's mobile number, in international format
  delay(100);
  SIM900.println("Hello, world. This is a text message from an Arduino Uno.");        // message to send
  delay(100);
  SIM900.println((char)26);                       // End AT command with a ^Z, ASCII code 26
  delay(100); 
  SIM900.println();
  delay(5000);                                     // give module time to send SMS
  SIM900power();                                   // turn off module
}*/



void loop()
{
  int value;
  
  // GPS ---make a string for assembling the data to log:
  char index = 0;
  char temp = 0;
  String dataString = "";
  //end GPS
  //OBD
  // save engine RPM in variable 'value', return true on success
  if (obd.read(PID_RPM, value)) {
    // light on LED on Arduino board when the RPM exceeds 1500
    //digitalWrite(13, value > 1500 ? HIGH : LOW);
    if (1){
    //END OBD
      //GPS
      while(GPS.available())
   {
     temp = GPS.read();
     dataString += String(temp);
     index++;
     if(String(temp) == "$")
       Serial.println("");
     if(index>200){
       break;  
     }
   }//END GPS
    
    //SIM900
    SIM900.print("AT+CMGF=1\r");                                                        // AT command to send SMS message
    delay(100);
    SIM900.println("AT + CMGS = \"+12533502780\"");                                     // recipient's mobile number, in international format
    delay(100);
    SIM900.println("Relative Throttle Pos = " + String(value) + " \nGPS Coordinates = " + dataString);        // message to send
    delay(100);
    SIM900.println((char)26);                       // End AT command with a ^Z, ASCII code 26
    delay(100); 
    SIM900.println();
    delay(5000);                                     // give module time to send SMS
    SIM900power();                                   // turn off module 
    //END SIM900
  }
  
  }
  
  
}
