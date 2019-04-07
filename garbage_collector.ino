#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; //LCD pins
const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SoftwareSerial mySerial(9, 10); //rx, tx - > tx is connected to rx



void setup() {
  lcd.begin(16, 2);
  lcd.print("SmartDustbin");

  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
}

bool msgSent = false;
void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0,1);
  
  //input from ultrasonic on loop
  long duration, inches, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  
  bool full = false; //sets full to false at every loop then checks
  if(inches >= 15){
    Serial.println("Empty");
    full = false;
    msgSent = false;
  }
  else if(inches < 15 && inches > 3){
     Serial.println("Medium");
     full = false;
     msgSent = false;
  }
  else if(inches <= 3){
    Serial.println("Full!");
    full = true;
  }

  //if dustbin is full and message is not already sent, send the message and set message flag to true
  if(full && !msgSent){
    //send message code here
    SendMessage();
      Serial.print("Sent!");
      break;
    delay(1000);
    msgSent = true;
  }

  delay(1000);
}



long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}


void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+919826971199\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Bin Number: 323 is Full kindly empty the bin. Location: ");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
