/*
 * 
 * All the resources for this project: https://www.hackster.io/Aritro
 * Modified by Aritro Mukherjee
 * 
 * https://create.arduino.cc/projecthub/Aritro/security-access-using-rfid-reader-f7c746
 */
 //This is for the 2 side program
 
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

#include <Servo.h>
int servoPin = 14;
Servo servo1;
#define pointAtBedroom 70
#define pointAtBathroom 135
#define pointAtKitchen 180

#define SS_PIN 10
#define RST_PIN 9
#define OUT 4
#define OUT2 5
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

 void receiveEvent(int howMany) {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
    if(c == 'K'){
      servo1.write(pointAtKitchen);
    }
    else if(c == 'B'){
      servo1.write(pointAtBedroom);

    }
        else if(c == 'R'){
      servo1.write(pointAtBathroom);

    }
 
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}

void setup() 
{
  //  Wire.begin(); // join i2c bus (address optional for master)
      Wire.begin(8);                // join i2c bus with address #8
          Wire.onReceive(receiveEvent); // register event

  Serial.begin(9600);   // Initiate a serial communication 
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
//  pinMode(servoSignal, INPUT);
  servo1.attach(servoPin);

  


}
void loop() 
{

 
  //delay(1000);
    digitalWrite(OUT, LOW);
        digitalWrite(OUT2, LOW);
//  Serial.println(analogRead(servoSignal));
//  if(analogRead(servoSignal) == pointAtBedroom){
//     servo1.write(pointAtBedroom);
//  }
//  else if(analogRead(servoSignal) == pointAtBathroom){
//         servo1.write(pointAtBathroom);
//  }
//  
//    else if(analogRead(servoSignal) == pointAtKitchen){
//         servo1.write(pointAtKitchen);
//  }

  //Serial.println(digitalRead(OUT));
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
      digitalWrite(OUT, LOW);
        digitalWrite(OUT2, LOW);
  if (content.substring(1) == "FA 13 FB 29") //change here the UID of the card/cards that you want to give access
  {
    digitalWrite(OUT, HIGH);
        digitalWrite(OUT2, HIGH);



    Serial.println("Authorized access");
    Serial.println();
    delay(1000);
   // digitalWrite(OUT,LOW);
  }
 
 else   {
    digitalWrite(OUT, LOW);
    digitalWrite(OUT2, LOW);
    Serial.println(" Access denied");
   // delay(3000);
  }
} 
