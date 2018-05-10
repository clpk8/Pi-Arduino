
//----------------------------Display-------------------------------
//#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;
const int colorR = 0;
const int colorG = 0;
const int colorB = 40;

//----------------------------RFID1-------------------------------
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

////----------------------------RFID2 and 3-------------------------------
#define BARFID1 8
#define BARFID2 7
#define KRFID1 2
#define KRFID2 3

////----------------------------V and B-------------------------------
#define vibratorPin 6
#define buzzer1 5
#define buzzer2 4

////----------------------------Pressure sensor-------------------------------
#define fsrAnalogPin 0 // FSR is connected to analog 0
int fsrReading;      // the analog reading from the FSR resistor divider
int LEDbrightness;

////----------------------------Light sensor sensor-------------------------------
//#define LIGHT_SENSOR A5//Grove - Light Sensor is connected to A0 of Arduino

// -----------------------------LEDS --------------------------------------------
#define kitchenGreen A2
#define bathGreen 1
//#define bathRed 0
#define bedGreen A1

// ----------------------------servo------------------------------------------

#include <Servo.h>
int servoPin = 17;
Servo servo1;
#define pointAtBedroom 70
#define pointAtBathroom 135
#define pointAtKitchen 180

//------------------------------Define States--------------------------------
enum state_s {
    idleState,
    goToBath,
    goToBed,
    goToKitchen,
    drinkWater,
    takeMeds,
    bedToBath
};
//----------------------------RFID1 function---------------------------
int readRFID1() {
    delay(500);
    //Serial.println(digitalRead(OUT));
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent())
    {
        return -1 ;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
    {
        return -1 ;
    }
    //Show UID on serial monitor
    // Serial.print("UID tag :");
    String content = "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        //  Serial.print(mfrc522.uid.uidByte[i], HEX);
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    // Serial.println();
    // Serial.print("Message : ");
    content.toUpperCase();
    if (content.substring(1) == "FA 13 FB 29") //change here the UID of the card/cards that you want to give access
    {
        // digitalWrite(OUT, HIGH);
        //  Serial.println("Authorized access");
        //  Serial.println();
        // delay(3000);
        // digitalWrite(OUT,LOW);
        return 1;
    }
    
    else   {
        return 0;
        //   digitalWrite(OUT,LOW);
        // Serial.println(" Access denied");
        // delay(3000);
    }
}


void turnOffAllLeds() {
    //  digitalWrite(kitchenRed, LOW);
    //  digitalWrite(bathRed, LOW);
    digitalWrite(bathGreen, LOW);
    digitalWrite(kitchenGreen, LOW);
    //  digitalWrite(bedRed, LOW);
    digitalWrite(bedGreen, LOW);
}

void setup() {
    
    // Serial.println("lalala");
    Serial.begin(9600);   // Initiate a serial communication
    ////----------------------------Display setup-------------------------------
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);
    // Print a message to the LCD.
    lcd.print("hello, world!");
    
    ////----------------------------RFID setup-------------------------------
    
    SPI.begin();      // Initiate  SPI bus
    mfrc522.PCD_Init();   // Initiate MFRC522
    //Serial.write("Start");
    // Serial.println();
    pinMode(BARFID1, INPUT);
    pinMode(BARFID2, INPUT);
    pinMode(KRFID1, INPUT);
    pinMode(KRFID2, INPUT);
    
    ////----------------------------V and B setup-------------------------------
    pinMode(vibratorPin, OUTPUT);
    pinMode(buzzer1, OUTPUT);
    pinMode(buzzer2, OUTPUT);
    
    
    //--------------------------------LEDS as output ---------------------------
    //  pinMode(kitchenRed, OUTPUT);
    pinMode(kitchenGreen, OUTPUT);
    //  pinMode(bathRed, OUTPUT);
    pinMode(bathGreen, OUTPUT);
    //  pinMode(bedRed, OUTPUT);
    pinMode(bedGreen, OUTPUT);
    
    turnOffAllLeds();
    
    //-----------------------------Servo-----------------------------
    servo1.attach(servoPin);
    //  int i = 0;
    //  for(i = 0; i < 180; i+10){
    //         servo1.write(i); //br
    //         delay(1000);
    //  }
    servo1.write(100);
    // pinMode(servoSignal,OUTPUT);
    
    delay(1000);
}

void loop() {
    
    state_s state = idleState;
    state_s nextState;
    
    
    while (1) {
        turnOffAllLeds();
        //A = gotobath, B = gotobed, C = goto kitchen, D = water, E = meds, F = bedtobath
        if (state == idleState) {
            //handmake blocking function
            while (Serial.available() == 0) {
                //do nothing, wait
            }
            //   Serial.println(state);
            //when it break the look, ingo switch
            char incomingByte = Serial.read();
            lcd.print(incomingByte);
            // Serial.write("Received");
            switch (incomingByte) {
                case 'A':
                    // Serial.println("A received");
                    nextState = goToBath;
                    break;
                case 'B':
                    //  Serial.println("B received");
                    nextState = goToBed;
                    break;
                case 'C':
                    nextState = goToKitchen;
                    //  Serial.println("C received");
                    
                    break;
                case 'D':
                    nextState = drinkWater;
                    //  Serial.println("D received");
                    break;
                case 'E':
                    nextState = takeMeds;
                    //    Serial.println("E received");
                    break;
                case 'F':
                    nextState = bedToBath;
                    //      Serial.println("F received");
                    break;
                default:
                    //      Serial.write("received wrong result");
                    break;
            }
            
        }
        else if (state == goToBath) {
            //   Serial.println("GOTOBATH state");
            lcd.clear();
            lcd.print("Go to Bath Room!");
            servo1.write(pointAtBathroom);
            
            turnOffAllLeds();
            digitalWrite(bathGreen,HIGH);
            delay(5000);
            int waitTime = 0;
            while (1) {
                //   Serial.println(digitalRead(KRFID1));
                //          Serial.println(digitalRead(KRFID2));
                
                if (digitalRead(BARFID1) == 1 && digitalRead(BARFID2) == 1) {
                    nextState = idleState;
                    lcd.clear();
                    turnOffAllLeds();
                    break;
                }
                else if (readRFID1() == 1) {
                    digitalWrite(vibratorPin, HIGH);
                    delay(500);
                    digitalWrite(vibratorPin, LOW);
                    delay(500);
                }
                else if (digitalRead(KRFID1) == 1 && digitalRead(KRFID2) == 1) {
                    digitalWrite(buzzer2, HIGH);
                    delay(500);
                    digitalWrite(buzzer2, LOW);
                    delay(500);
                }
                
                waitTime++;
                delay(500);
                if (waitTime % 10 == 0) {
                    Serial.write("A");
                }
            }
            
        }
        else if (state == goToBed) {
            // Serial.println("GOTOBED state");
            lcd.clear();
            lcd.print("Goto bed room!");
            servo1.write(pointAtBedroom);
            
            turnOffAllLeds();
            digitalWrite(bedGreen,HIGH);
            delay(5000);
            int waitTime = 0;
            while (1) {
                if (readRFID1() == 1) {
                    fsrReading = analogRead(fsrAnalogPin);
                    if(fsrReading > 100){
                        nextState = idleState;
                        lcd.clear();
                        turnOffAllLeds();
                        break;
                    }
                    else{
                        lcd.clear();
                        lcd.print("Time to sleep");
                    }
                    
                }
                else if (digitalRead(BARFID2) == 1 && digitalRead(BARFID1) == 1) {
                    digitalWrite(buzzer1, HIGH);
                    delay(500);
                    digitalWrite(buzzer1, LOW);
                    delay(500);
                }
                
                else if (digitalRead(KRFID2) == 1 && digitalRead(KRFID1) == 1) {
                    digitalWrite(buzzer2, HIGH);
                    delay(500);
                    digitalWrite(buzzer2, LOW);
                    delay(500);
                }
                
                waitTime++;
                delay(500);
                if (waitTime % 10 == 0) {
                    Serial.write("B");
                }
            }
        }
        
        
        else if (state == goToKitchen) {
            // Serial.println("GOTO kitchen state");
            lcd.clear();
            lcd.println("Go to kitchen");
            servo1.write(pointAtKitchen);
            
            turnOffAllLeds();
            digitalWrite(kitchenGreen,HIGH);
            //turn on kitchen green
            delay(5000);
            int waitTime = 0;
            while (1) {
                if (digitalRead(KRFID2) == 1 && digitalRead(KRFID1) == 1) {
                    nextState = idleState;
                    lcd.clear();
                    turnOffAllLeds();
                    break;
                }
                else if (readRFID1() == 1) {
                    digitalWrite(vibratorPin, HIGH);
                    delay(500);
                    digitalWrite(vibratorPin, LOW);
                    delay(500);
                }
                
                else if (digitalRead(BARFID2) == 1 && digitalRead(BARFID1) == 1) {
                    digitalWrite(buzzer1, HIGH);
                    delay(500);
                    digitalWrite(buzzer1, LOW);
                    delay(500);
                }
                waitTime++;
                delay(500);
                if (waitTime % 10 == 0) {
                    Serial.write("C");
                }
                
            }
        }
        
        else if (state == drinkWater) {
            // Serial.println("drink state");
            lcd.clear();
            lcd.print("Drink some water");
            servo1.write(pointAtKitchen);
            
            turnOffAllLeds();
            digitalWrite(kitchenGreen,HIGH);
            //turn on kitchen LEDs
            delay(5000);
            int waitTime = 0;
            while (1) {
                if (digitalRead(KRFID2) == 1 && digitalRead(KRFID1) == 1) {
                    nextState = idleState;
                    lcd.clear();
                    turnOffAllLeds();
                    break;
                }
                else if (readRFID1() == 1) {
                    digitalWrite(vibratorPin, HIGH);
                    delay(500);
                    digitalWrite(vibratorPin, LOW);
                    delay(500);
                }
                
                else if (digitalRead(BARFID2) == 1 && digitalRead(BARFID1) == 1) {
                    digitalWrite(buzzer1, HIGH);
                    delay(500);
                    digitalWrite(buzzer1, LOW);
                    delay(500);
                }
                waitTime++;
                delay(500);
                if (waitTime % 10 == 0) {
                    Serial.write("D");
                }
            }
        }
        else if (state == takeMeds) {
            //   Serial.println("take med state");
            lcd.clear();
            lcd.print("Take medicine!");
            servo1.write(pointAtKitchen);
            
            turnOffAllLeds();
            digitalWrite(kitchenGreen,HIGH);
            //turn kitchen green
            delay(5000);
            
            int waitTime = 0;
            while (1) {
                if (digitalRead(KRFID2) == 1 && digitalRead(KRFID1) == 1) {
                    nextState = idleState;
                    lcd.clear();
                    turnOffAllLeds();
                    break;
                }
                else if (readRFID1() == 1) {
                    digitalWrite(vibratorPin, HIGH);
                    delay(500);
                    digitalWrite(vibratorPin, LOW);
                    delay(500);
                }
                
                else if (digitalRead(BARFID2) == 1 && digitalRead(BARFID1) == 1) {
                    digitalWrite(buzzer1, HIGH);
                    delay(500);
                    digitalWrite(buzzer1, LOW);
                    delay(500);
                }
                waitTime++;
                delay(500);
                if (waitTime % 10 == 0) {
                    Serial.write("E");
                }
                
                
            }
        }
        else if (state == bedToBath) {
            //raise the shade
            //  Serial.println("Wake up state");
            lcd.clear();
            lcd.print("Go wash up");
            servo1.write(pointAtBathroom);
            turnOffAllLeds();
            digitalWrite(bathGreen,HIGH);
            //turn restroom green
            delay(5000);
            int waitTime = 0;
            while (1) {
                fsrReading = analogRead(fsrAnalogPin);
                //   Serial.println(fsrAnalogPin);
                if (digitalRead(BARFID1) == 1 && digitalRead(BARFID2) == 1) {
                    nextState = idleState;
                    lcd.clear();
                    turnOffAllLeds();
                    break;
                }
                else if (fsrReading > 100) {
                    digitalWrite(vibratorPin, HIGH);
                    delay(500);
                    digitalWrite(vibratorPin, LOW);
                    delay(500);
                }
                else if (digitalRead(KRFID2) == 1 && digitalRead(KRFID1) == 1) {
                    digitalWrite(buzzer2, HIGH);
                    delay(500);
                    digitalWrite(buzzer2, LOW);
                    delay(500);
                }
                waitTime++;
                if (waitTime == 10) {
                    Serial.write("F");
                }
            }
        }
        state = nextState;
    }//end of while
}//end of loop






