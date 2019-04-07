#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10); //rx, tx - > tx is connected to rx

void setup()
{
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
}


void loop()
{
  if (Serial.available()>0)
   switch(Serial.read())
  {
    case 's':
      SendMessage();
      Serial.print("Sent!");
      break;
    case 'r':
      RecieveMessage();
      break;
     case 'l':
      GPRSLocation();
      break;
  }

 if (mySerial.available()>0)
   Serial.write(mySerial.read());
}


 void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+918839312029\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("This is a SMS from GSM module via Arduino Mega 2560");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}


 void RecieveMessage()
{
  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);
 }


void GPRSLocation(){
  //setup location data
  mySerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");    //set mode to gprs
  mySerial.println("AT+SAPBR=3,1,\"APN\",\"bsnlnet\"");  //set APN for network
  mySerial.println("AT+SAPBR =1,1");  //Enable Gprs
  mySerial.println("AT+CIPGSMLOC=1,1"); //Detect location
  Serial.println(mySerial.read());
  Serial.println("Location Data Recieved");
}
