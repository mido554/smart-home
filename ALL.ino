
  
#include <SoftwareSerial.h>
#include <Servo.h> 
#include "VoiceRecognitionV3.h"
#include <DHT11.h>
#include <LiquidCrystal.h>


LiquidCrystal lcdVoice(52, 50, 48, 46, 44, 42);
LiquidCrystal LcdInHome(62, 63, 64, 65, 66, 67);
LiquidCrystal LcdOutHome(14, 15, 16, 17, 18, 19);


/* temp and humi sensor */
int pin=23;
DHT11 dht11(pin);
int err;
float temp, humi; 

#define AcPin 45
#define PumpPin 35
#define SolarPin 36
#define BuzzerPin 25
#define LdrPin 37
#define LightsPin 47
#define FingerledPin 29
#define InLightsPin 21
#define AlarmLedPin1 40
#define AlarmLedPin2 41
#define LedDoorBlue 26


Servo ServoDoor; // servo intiate
Servo ServoGarage;
Servo ServoWindow;
Servo ServoSECDoor;
Servo ServoSECWindow;

/**        
  Connection
  Arduino    VoiceRecognitionModule
   10   ------->     TX
   24   ------->     RX
*/
VR myVR(10,24);    // 2:RX 3:TX, you can choose your favourite pins.



uint8_t record[7]; // save record
uint8_t buf[64];

int led = 13;

int group = 0;

#define switchRecord        (0)

#define group0Record1       (1) 
#define group0Record2       (2) 
#define group0Record3       (3) 
#define group0Record4       (4) 
#define group0Record5       (5) 
#define group0Record6       (6) 

#define group1Record1       (7) 
#define group1Record2       (8) 
#define group1Record3       (9) 
#define group1Record4       (10) 
#define group1Record5       (11) 
#define group1Record6       (12) 

#define group2Record1       (13) 
#define group2Record2       (14) 
#define group2Record3       (15) 
#define group2Record4       (16) 
#define group2Record5       (17) 
#define group2Record6       (18) 


void setup()
{

  
  /** initialize */
  myVR.begin(9600);
  
  Serial.begin(57600);
  Serial.println("Elechouse Voice Recognition V3 Module\r\nMulti Commands sample");
  
  pinMode(led, OUTPUT);
  pinMode(AcPin,OUTPUT); // ac led
  pinMode(PumpPin,OUTPUT); // pump relay
  pinMode(BuzzerPin,OUTPUT); // buzzer
  pinMode(LightsPin,OUTPUT); // Led House
  pinMode(FingerledPin,INPUT);
  pinMode(InLightsPin,OUTPUT);
  pinMode(AlarmLedPin1,OUTPUT);
  pinMode(AlarmLedPin2,OUTPUT);
  pinMode(SolarPin,OUTPUT);
  pinMode(LedDoorBlue,INPUT);

  

  lcdVoice.begin(16, 2);
  LcdInHome.begin(16, 2);
  LcdOutHome.begin(16, 2);

    
  if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  }else{
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
  record[0] = switchRecord;
  record[1] = group0Record1;
  record[2] = group0Record2;
  record[3] = group0Record3;
  record[4] = group0Record4;
  record[5] = group0Record5;
  record[6] = group0Record6;
  group = 0;
  if(myVR.load(record, 7) >= 0){
    printRecord(record, 7);
    Serial.println(F("loaded."));
  }




  ServoDoor.attach(7);
  ServoGarage.attach(8);
  ServoWindow.attach(9);
  ServoSECDoor.attach(10);
  ServoSECWindow.attach(11);

  /* ------- */
  
}

void loop()
{


  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){
      case switchRecord:
        /** turn on LED */
        if(digitalRead(led) == HIGH){
          digitalWrite(led, LOW);
        }else{
          digitalWrite(led, HIGH);
        }

        


if(group == 0){
          group = 1;
          myVR.clear();
          record[0] = switchRecord;
          record[1] = group2Record1;
          record[2] = group2Record2;
          record[3] = group2Record3;
          record[4] = group2Record4;
          record[5] = group2Record5;
          record[6] = group2Record6;
          
          if(myVR.load(record, 7) >= 0){
            printRecord(record, 7);
            Serial.println(F("loaded."));
            
            }
          

          




        
 }else if(group == 1){
          group = 2;
          myVR.clear();
          record[0] = switchRecord;
          record[1] = group1Record1;
          record[2] = group1Record2;
          record[3] = group1Record3;
          record[4] = group1Record4;
          record[5] = group1Record5;
          record[6] = group1Record6;
          if(myVR.load(record, 7) >= 0){
            printRecord(record, 7);
            Serial.println(F("loaded."));



            

            
              
            }
          }
        else {
          group = 0;
          myVR.clear();
          record[0] = switchRecord;
          record[1] = group0Record1;
          record[2] = group0Record2;
          record[3] = group0Record3;
          record[4] = group0Record4;
          record[5] = group0Record5;
          record[6] = group0Record6;
          if(myVR.load(record, 7) >= 0){
            printRecord(record, 7);
            Serial.println(F("loaded."));
          }
        }
        break;
        
     
      default:
        break;
       
      
      
    }
    switch(buf[1]){
      case group0Record1: // Ac Open
      {
        delay(50);
        lcdVoice.clear();
        lcdVoice.print(" Group 1 :");
        lcdVoice.setCursor(0,1);
        lcdVoice.print(" 1, 2, 3, 4, 5, 6 ");
        
        delay(50);
        LcdInHome.clear();
        LcdInHome.print("Temp:  Humidity:");
        LcdInHome.setCursor(0, 1);
        LcdInHome.print(temp);
        LcdInHome.setCursor(7,1);
        LcdInHome.print(humi);

        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Hello!, please");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Enter Ur Finger");

        

        
        digitalWrite(InLightsPin,HIGH);
        digitalWrite(SolarPin,HIGH);
        
        ServoSECDoor.write(0);
        ServoSECWindow.write(0);
        

        if((err=dht11.read(humi, temp))==0) // 29 normal , 36 high
  {
    if(temp>=36)
    {
      digitalWrite(AcPin,HIGH); // led ac HIGH
    }
  }
    int PumpSensor = analogRead(A0);
      if(PumpSensor>=550)
      {
        digitalWrite(PumpPin,LOW); // pump working
      }
      if(PumpSensor<550)
      {
        digitalWrite(PumpPin,HIGH); // pump stop
      }
     int FlameSensor = analogRead(A2); // Flame Sensor
      if(FlameSensor<=600)
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Fire Alarm");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Call 999 Please");
        
         digitalWrite(BuzzerPin,HIGH); // buzzer on
         digitalWrite(PumpPin,LOW); // pump working
         digitalWrite(AlarmLedPin1,HIGH); // fire alarm
         digitalWrite(AlarmLedPin2,HIGH); // fire alarm 2
      }
     int IrSensor = analogRead(A4); // Ir Sensor
      if(IrSensor<=50)
      {
        ServoGarage.write(115); // open garage
      if(IrSensor>=900)
          {
        ServoGarage.write(0); // close garage
          }
      }
      int SmokeSensor = analogRead(A3); // Smoke Sensor
      if(SmokeSensor>=250)
      {
        digitalWrite(BuzzerPin,HIGH); // Buzzer on
      }
      int RainSensor = analogRead(A1); // Rain sensor
      if(RainSensor>=100)
      {
        ServoWindow.write(0); // close Window
      }
      unsigned int LdrSensor;
      LdrSensor = digitalRead(LdrPin);  // 1: no light 0: light Ldr Sensor
      if(digitalRead(LdrPin)==1)
      {
        digitalWrite(LightsPin,HIGH); // Light house 
      }
      if(digitalRead(FingerledPin==HIGH) || digitalRead(LedDoorBlue==HIGH)) // led finger print anohter ARDUINO
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Welcome  :) ");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Door opened");
        
        ServoDoor.write(115); // open door
      }
      if(digitalRead(FingerledPin==LOW)) 
      {
        ServoDoor.write(0);
      
      }
      }
      
      break;
      

      /* ________________________________________________________________________________________ */ 




      
      case group0Record2: // ac off
      {
        delay(50);
        lcdVoice.clear();
        lcdVoice.print(" Group 1 :");
        lcdVoice.setCursor(0,1);
        lcdVoice.print(" 1, 2, 3, 4, 5, 6 ");

        delay(50);
        LcdInHome.clear();
        LcdInHome.print("Temp:  Humidity:");
        LcdInHome.setCursor(0, 1);
        LcdInHome.print(temp);
        LcdInHome.setCursor(7,1);
        LcdInHome.print(humi);

        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Hello!, please");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Enter Ur Finger");
        
        digitalWrite(InLightsPin,HIGH);
        digitalWrite(SolarPin,HIGH);

        
        ServoSECDoor.write(0);
        ServoSECWindow.write(0);
        
        digitalWrite(AcPin,LOW); // MAIN !!!

        
    int PumpSensor = analogRead(A0);
      if(PumpSensor>=550)
      {
        digitalWrite(PumpPin,LOW); // pump working
      }
      if(PumpSensor<550)
      {
        digitalWrite(PumpPin,HIGH); // pump stop
      }
     int FlameSensor = analogRead(A2); // Flame Sensor
      if(FlameSensor<=600)
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Fire Alarm");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Call 999 Please");
        
         digitalWrite(BuzzerPin,HIGH); // buzzer on
         digitalWrite(PumpPin,LOW); // pump working
         digitalWrite(AlarmLedPin1,HIGH); // fire alarm
         digitalWrite(AlarmLedPin2,HIGH); // fire alarm 2
      }
     int IrSensor = analogRead(A4); // Ir Sensor
      if(IrSensor<=50)
      {
        ServoGarage.write(115); // open garage
      if(IrSensor>=900)
          {
        ServoGarage.write(0); // close garage
          }
      }
      int SmokeSensor = analogRead(A3); // Smoke Sensor
      if(SmokeSensor>=250)
      {
        digitalWrite(BuzzerPin,HIGH); // Buzzer on
      }
      int RainSensor = analogRead(A1); // Rain sensor
      if(RainSensor>=100)
      {
        ServoWindow.write(0); // close Window
      }
      unsigned int LdrSensor;
      LdrSensor = digitalRead(LdrPin);  // 1: no light 0: light Ldr Sensor
      if(digitalRead(LdrPin)==1)
      {
        digitalWrite(LightsPin,HIGH); // Light house 
      }
      if(digitalRead(FingerledPin==HIGH) || digitalRead(LedDoorBlue==HIGH)) // led finger print anohter ARDUINO
      {

        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Welcome  :) ");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Door opened");
        
        ServoDoor.write(115); // open door
      }
      if(digitalRead(FingerledPin==LOW)) 
      {
        ServoDoor.write(0);
      
      }
      }
      break;

      /* ________________________________________________________________________ */ 
      
      case group0Record3: // open door
      {
        delay(50);
        lcdVoice.clear();
        lcdVoice.print(" Group 1 :");
        lcdVoice.setCursor(0,1);
        lcdVoice.print(" 1, 2, 3, 4, 5, 6 ");
        
        delay(50);
        LcdInHome.clear();
        LcdInHome.print("Temp:  Humidity:");
        LcdInHome.setCursor(0, 1);
        LcdInHome.print(temp);
        LcdInHome.setCursor(7,1);
        LcdInHome.print(humi);

        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Hello!, Sir :) ");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Door oepened");
        
        digitalWrite(InLightsPin,HIGH);
        digitalWrite(SolarPin,HIGH);

        
        ServoSECDoor.write(0);
        ServoSECWindow.write(0);
        
        ServoDoor.write(115); // MAIN !!

        if((err=dht11.read(humi, temp))==0) // 29 normal , 36 high
  {
    if(temp>=36)
    {
      digitalWrite(AcPin,HIGH); // led ac HIGH
    }
  }
    int PumpSensor = analogRead(A0);
      if(PumpSensor>=550)
      {
        digitalWrite(PumpPin,LOW); // pump working
      }
      if(PumpSensor<550)
      {
        digitalWrite(PumpPin,HIGH); // pump stop
      }
     int FlameSensor = analogRead(A2); // Flame Sensor
      if(FlameSensor<=600)
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Fire Alarm");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Call 999 Please");
        
         digitalWrite(BuzzerPin,HIGH); // buzzer on
         digitalWrite(PumpPin,LOW); // pump working
         digitalWrite(AlarmLedPin1,HIGH); // fire alarm
         digitalWrite(AlarmLedPin2,HIGH); // fire alarm 2
      }
     int IrSensor = analogRead(A4); // Ir Sensor
      if(IrSensor<=50)
      {
        ServoGarage.write(115); // open garage
      if(IrSensor>=900)
          {
        ServoGarage.write(0); // close garage
          }
      }
      int SmokeSensor = analogRead(A3); // Smoke Sensor
      if(SmokeSensor>=250)
      {
        digitalWrite(BuzzerPin,HIGH); // Buzzer on
      }
      int RainSensor = analogRead(A1); // Rain sensor
      if(RainSensor>=100)
      {
        ServoWindow.write(0); // close Window
      }
      unsigned int LdrSensor;
      LdrSensor = digitalRead(LdrPin);  // 1: no light 0: light Ldr Sensor
      if(digitalRead(LdrPin)==1)
      {
        digitalWrite(LightsPin,HIGH); // Light house 
      }
      
      }
      break;

      /* ___________________________________________________________________ */
      
      case group0Record4: // close door
      { 
        delay(50);
        lcdVoice.clear();
        lcdVoice.print(" Group 1 :");
        lcdVoice.setCursor(0,1);
        lcdVoice.print(" 1, 2, 3, 4, 5, 6 ");

        delay(50);
        LcdInHome.clear();
        LcdInHome.print("Temp:  Humidity:");
        LcdInHome.setCursor(0, 1);
        LcdInHome.print(temp);
        LcdInHome.setCursor(7,1);
        LcdInHome.print(humi);

        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Hello!, Stranger ");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Door closed");
        
        digitalWrite(InLightsPin,HIGH);
        digitalWrite(SolarPin,HIGH);
        
        ServoSECDoor.write(0);
        ServoSECWindow.write(0);
        
       ServoDoor.write(0); // MAIN !!


        if((err=dht11.read(humi, temp))==0) // 29 normal , 36 high
  {
    if(temp>=36)
    {
      digitalWrite(AcPin,HIGH); // led ac HIGH
    }
  }
    int PumpSensor = analogRead(A0);
      if(PumpSensor>=550)
      {
        digitalWrite(PumpPin,LOW); // pump working
      }
      if(PumpSensor<550)
      {
        digitalWrite(PumpPin,HIGH); // pump stop
      }
     int FlameSensor = analogRead(A2); // Flame Sensor
      if(FlameSensor<=600)
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Fire Alarm");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Call 999 Please");
        
         digitalWrite(BuzzerPin,HIGH); // buzzer on
         digitalWrite(PumpPin,LOW); // pump working
         digitalWrite(AlarmLedPin1,HIGH); // fire alarm
         digitalWrite(AlarmLedPin2,HIGH); // fire alarm 2
      }
     int IrSensor = analogRead(A4); // Ir Sensor
      if(IrSensor<=50)
      {
        ServoGarage.write(115); // open garage
      if(IrSensor>=900)
          {
        ServoGarage.write(0); // close garage
          }
      }
      int SmokeSensor = analogRead(A3); // Smoke Sensor
      if(SmokeSensor>=250)
      {
        digitalWrite(BuzzerPin,HIGH); // Buzzer on
      }
      int RainSensor = analogRead(A1); // Rain sensor
      if(RainSensor>=100)
      {
        ServoWindow.write(0); // close Window
      }
      unsigned int LdrSensor;
      LdrSensor = digitalRead(LdrPin);  // 1: no light 0: light Ldr Sensor
      if(digitalRead(LdrPin)==1)
      {
        digitalWrite(LightsPin,HIGH); // Light house 
      }
      
      }
      
      break;


      /* _________________________________________________________________ */


      
      case group0Record5: // open window
      { 
        delay(50);
        lcdVoice.clear();
        lcdVoice.print(" Group 1 :");
        lcdVoice.setCursor(0,1);
        lcdVoice.print(" 1, 2, 3, 4, 5, 6 ");
        
        delay(50);
        LcdInHome.clear();
        LcdInHome.print("Temp:  Humidity:");
        LcdInHome.setCursor(0, 1);
        LcdInHome.print(temp);
        LcdInHome.setCursor(7,1);
        LcdInHome.print(humi);

        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Hello!, please");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Enter Ur Finger");
        
        digitalWrite(InLightsPin,HIGH);
        digitalWrite(SolarPin,HIGH);
        
        ServoSECDoor.write(0);
        ServoSECWindow.write(0);
        
        ServoWindow.write(115); // MAIN !!

        if((err=dht11.read(humi, temp))==0) // 29 normal , 36 high
  {
    if(temp>=36)
    {
      digitalWrite(AcPin,HIGH); // led ac HIGH
    }
  }
    int PumpSensor = analogRead(A0);
      if(PumpSensor>=550)
      {
        digitalWrite(PumpPin,LOW); // pump working
      }
      if(PumpSensor<550)
      {
        digitalWrite(PumpPin,HIGH); // pump stop
      }
     int FlameSensor = analogRead(A2); // Flame Sensor
      if(FlameSensor<=600)
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Fire Alarm");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Call 999 Please");
        
         digitalWrite(BuzzerPin,HIGH); // buzzer on
         digitalWrite(PumpPin,LOW); // pump working
         digitalWrite(AlarmLedPin1,HIGH); // fire alarm
         digitalWrite(AlarmLedPin2,HIGH); // fire alarm 2
      }
     int IrSensor = analogRead(A4); // Ir Sensor
      if(IrSensor<=50)
      {
        ServoGarage.write(115); // open garage
      if(IrSensor>=900)
          {
        ServoGarage.write(0); // close garage
          }
      }
      int SmokeSensor = analogRead(A3); // Smoke Sensor
      if(SmokeSensor>=250)
      {
        digitalWrite(BuzzerPin,HIGH); // Buzzer on
      }
      int RainSensor = analogRead(A1); // Rain sensor
      if(RainSensor>=100)
      {
         // edited
      }
      unsigned int LdrSensor;
      LdrSensor = digitalRead(LdrPin);  // 1: no light 0: light Ldr Sensor
      if(digitalRead(LdrPin)==1)
      {
        digitalWrite(LightsPin,HIGH); // Light house 
      }
      if(digitalRead(FingerledPin==HIGH) || digitalRead(LedDoorBlue==HIGH)) // led finger print anohter ARDUINO
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Welcome  :) ");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Door opened");
        
        ServoDoor.write(115); // open door
      }
      if(digitalRead(FingerledPin==LOW)) 
      {
        ServoDoor.write(0);
      
      }
      }
      
      break;

      /* _______________________________________________________________ */ 

      
      case group0Record6: // close window
      { 
        delay(50);
        lcdVoice.clear();
        lcdVoice.print(" Group 1 :");
        lcdVoice.setCursor(0,1);
        lcdVoice.print(" 1, 2, 3, 4, 5, 6 ");

        delay(50);
        LcdInHome.clear();
        LcdInHome.print("Temp:  Humidity:");
        LcdInHome.setCursor(0, 1);
        LcdInHome.print(temp);
        LcdInHome.setCursor(7,1);
        LcdInHome.print(humi);

        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Hello!, please");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Enter Ur Finger");
        
        digitalWrite(InLightsPin,HIGH);
        digitalWrite(SolarPin,HIGH);
        
        ServoSECDoor.write(0);
        ServoSECWindow.write(0);
        
        ServoWindow.write(0); // MAIN !!

        if((err=dht11.read(humi, temp))==0) // 29 normal , 36 high
  {
    if(temp>=36)
    {
      digitalWrite(AcPin,HIGH); // led ac HIGH
    }
  }
    int PumpSensor = analogRead(A0);
      if(PumpSensor>=550)
      {
        digitalWrite(PumpPin,LOW); // pump working
      }
      if(PumpSensor<550)
      {
        digitalWrite(PumpPin,HIGH); // pump stop
      }
     int FlameSensor = analogRead(A2); // Flame Sensor
      if(FlameSensor<=600)
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Fire Alarm");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Call 999 Please");
        
         digitalWrite(BuzzerPin,HIGH); // buzzer on
         digitalWrite(PumpPin,LOW); // pump working
         digitalWrite(AlarmLedPin1,HIGH); // fire alarm
         digitalWrite(AlarmLedPin2,HIGH); // fire alarm 2
      }
     int IrSensor = analogRead(A4); // Ir Sensor
      if(IrSensor<=50)
      {
        ServoGarage.write(115); // open garage
      if(IrSensor>=900)
          {
        ServoGarage.write(0); // close garage
          }
      }
      int SmokeSensor = analogRead(A3); // Smoke Sensor
      if(SmokeSensor>=250)
      {
        digitalWrite(BuzzerPin,HIGH); // Buzzer on
      }
      int RainSensor = analogRead(A1); // Rain sensor
      if(RainSensor>=100)
      {
        // edited
      }
      unsigned int LdrSensor;
      LdrSensor = digitalRead(LdrPin);  // 1: no light 0: light Ldr Sensor
      if(digitalRead(LdrPin)==1)
      {
        digitalWrite(LightsPin,HIGH); // Light house 
      }
      if(digitalRead(FingerledPin==HIGH) || digitalRead(LedDoorBlue==HIGH)) // led finger print anohter ARDUINO
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Welcome  :) ");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Door opened");
        
        ServoDoor.write(115); // open door
      }
      if(digitalRead(FingerledPin==LOW)) 
      {
        ServoDoor.write(0);
      
      }
      }
      
      break;

      /* ______________________________________________________________ */
      
      case group1Record1: // open garage
      {
        delay(50);
        lcdVoice.clear();
        lcdVoice.print(" Group 2 :");
        lcdVoice.setCursor(0,1);
        lcdVoice.print(" 7,8,9,10,11,12 ");
        
        delay(50);
        LcdInHome.clear();
        LcdInHome.print("Temp:  Humidity:");
        LcdInHome.setCursor(0, 1);
        LcdInHome.print(temp);
        LcdInHome.setCursor(7,1);
        LcdInHome.print(humi);

        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Hello!, please");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Enter Ur Finger");

        digitalWrite(InLightsPin,HIGH);
        digitalWrite(SolarPin,HIGH);
        
        ServoSECDoor.write(0);
        ServoSECWindow.write(0);
        
        ServoGarage.write(115); // MAIN !!

        if((err=dht11.read(humi, temp))==0) // 29 normal , 36 high
  {
    if(temp>=36)
    {
      digitalWrite(AcPin,HIGH); // led ac HIGH
    }
  }
    int PumpSensor = analogRead(A0);
      if(PumpSensor>=550)
      {
        digitalWrite(PumpPin,LOW); // pump working
      }
      if(PumpSensor<550)
      {
        digitalWrite(PumpPin,HIGH); // pump stop
      }
     int FlameSensor = analogRead(A2); // Flame Sensor
      if(FlameSensor<=600)
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Fire Alarm");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Call 999 Please");
        
         digitalWrite(BuzzerPin,HIGH); // buzzer on
         digitalWrite(PumpPin,LOW); // pump working
         digitalWrite(AlarmLedPin1,HIGH); // fire alarm
         digitalWrite(AlarmLedPin2,HIGH); // fire alarm 2
      }
     int IrSensor = analogRead(A4); // Ir Sensor
      if(IrSensor<=50)
      {
        
      if(IrSensor>=900)
          {
         // edited
          }
      }
      int SmokeSensor = analogRead(A3); // Smoke Sensor
      if(SmokeSensor>=250)
      {
        digitalWrite(BuzzerPin,HIGH); // Buzzer on
      }
      int RainSensor = analogRead(A1); // Rain sensor
      if(RainSensor>=100)
      {
        ServoWindow.write(0); // close Window
      }
      unsigned int LdrSensor;
      LdrSensor = digitalRead(LdrPin);  // 1: no light 0: light Ldr Sensor
      if(digitalRead(LdrPin)==1)
      {
        digitalWrite(LightsPin,HIGH); // Light house 
      }
      if(digitalRead(FingerledPin==HIGH) || digitalRead(LedDoorBlue==HIGH)) // led finger print anohter ARDUINO
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Welcome  :) ");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Door opened");
        
        ServoDoor.write(115); // open door
      }
      if(digitalRead(FingerledPin==LOW)) 
      {
        ServoDoor.write(0);
      
      }
      }
      break;


      /* ______________________________________________________________ */


      
      case group1Record2: // close garage
      {
        delay(50);
        lcdVoice.clear();
        lcdVoice.print(" Group 2 :");
        lcdVoice.setCursor(0,1);
        lcdVoice.print(" 7,8,9,10,11,12 ");

        delay(50);
        LcdInHome.clear();
        LcdInHome.print("Temp:  Humidity:");
        LcdInHome.setCursor(0, 1);
        LcdInHome.print(temp);
        LcdInHome.setCursor(7,1);
        LcdInHome.print(humi);

        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Hello!, please");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Enter Ur Finger");

        digitalWrite(InLightsPin,HIGH);
        digitalWrite(SolarPin,HIGH);
        
        ServoSECDoor.write(0);
        ServoSECWindow.write(0);
        
        ServoGarage.write(0); // MAIN !!
        if((err=dht11.read(humi, temp))==0) // 29 normal , 36 high
  {
    if(temp>=36)
    {
      digitalWrite(AcPin,HIGH); // led ac HIGH
    }
  }
    int PumpSensor = analogRead(A0);
      if(PumpSensor>=550)
      {
        digitalWrite(PumpPin,LOW); // pump working
      }
      if(PumpSensor<550)
      {
        digitalWrite(PumpPin,HIGH); // pump stop
      }
     int FlameSensor = analogRead(A2); // Flame Sensor
      if(FlameSensor<=600)
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Fire Alarm");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Call 999 Please");
        
         digitalWrite(BuzzerPin,HIGH); // buzzer on
         digitalWrite(PumpPin,LOW); // pump working
         digitalWrite(AlarmLedPin1,HIGH); // fire alarm
         digitalWrite(AlarmLedPin2,HIGH); // fire alarm 2
      }
     int IrSensor = analogRead(A4); // Ir Sensor
      if(IrSensor<=50)
      {
      if(IrSensor>=900)
          {
        
          }
      }
      int SmokeSensor = analogRead(A3); // Smoke Sensor
      if(SmokeSensor>=250)
      {
        digitalWrite(BuzzerPin,HIGH); // Buzzer on
      }
      int RainSensor = analogRead(A1); // Rain sensor
      if(RainSensor>=100)
      {
        ServoWindow.write(0); // close Window
      }
      unsigned int LdrSensor;
      LdrSensor = digitalRead(LdrPin);  // 1: no light 0: light Ldr Sensor
      if(digitalRead(LdrPin)==1)
      {
        digitalWrite(LightsPin,HIGH); // Light house 
      }
      if(digitalRead(FingerledPin==HIGH) || digitalRead(LedDoorBlue==HIGH)) // led finger print anohter ARDUINO
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Welcome  :) ");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Door opened");
        
        ServoDoor.write(115); // open door
      }
      if(digitalRead(FingerledPin==LOW)) 
      {
        ServoDoor.write(0);
      
      }
      }
      
      break;

      /*__________________________________________________________________ */

      
      case group1Record3: // lights on 


      {
        delay(50);
        lcdVoice.clear();
        lcdVoice.print(" Group 2 :");
        lcdVoice.setCursor(0,1);
        lcdVoice.print(" 7,8,9,10,11,12 ");

        delay(50);
        LcdInHome.clear();
        LcdInHome.print("Temp:  Humidity:");
        LcdInHome.setCursor(0, 1);
        LcdInHome.print(temp);
        LcdInHome.setCursor(7,1);
        LcdInHome.print(humi);

        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Hello!, please");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Enter Ur Finger");

        digitalWrite(InLightsPin,HIGH);
        digitalWrite(SolarPin,HIGH);
        
        ServoSECDoor.write(0);
        ServoSECWindow.write(0);
        
        digitalWrite(LightsPin,HIGH); // MAIN !!

        if((err=dht11.read(humi, temp))==0) // 29 normal , 36 high
  {
    if(temp>=36)
    {
      digitalWrite(AcPin,HIGH); // led ac HIGH
    }
  }
    int PumpSensor = analogRead(A0);
      if(PumpSensor>=550)
      {
        digitalWrite(PumpPin,LOW); // pump working
      }
      if(PumpSensor<550)
      {
        digitalWrite(PumpPin,HIGH); // pump stop
      }
     int FlameSensor = analogRead(A2); // Flame Sensor
      if(FlameSensor<=600)
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Fire Alarm");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Call 999 Please");
        
         digitalWrite(BuzzerPin,HIGH); // buzzer on
         digitalWrite(PumpPin,LOW); // pump working
         digitalWrite(AlarmLedPin1,HIGH); // fire alarm
         digitalWrite(AlarmLedPin2,HIGH); // fire alarm 2
      }
     int IrSensor = analogRead(A4); // Ir Sensor
      if(IrSensor<=50)
      {
        ServoGarage.write(115); // open garage
      if(IrSensor>=900)
          {
        ServoGarage.write(0); // close garage
          }
      }
      int SmokeSensor = analogRead(A3); // Smoke Sensor
      if(SmokeSensor>=250)
      {
        digitalWrite(BuzzerPin,HIGH); // Buzzer on
      }
      int RainSensor = analogRead(A1); // Rain sensor
      if(RainSensor>=100)
      {
        ServoWindow.write(0); // close Window
      }
      unsigned int LdrSensor;
      LdrSensor = digitalRead(LdrPin);  // 1: no light 0: light Ldr Sensor
      if(digitalRead(LdrPin)==1)
      {
         // edited
      }
      if(digitalRead(FingerledPin==HIGH) || digitalRead(LedDoorBlue==HIGH)) // led finger print anohter ARDUINO
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Welcome  :) ");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Door opened");
        
        ServoDoor.write(115); // open door
      }
      if(digitalRead(FingerledPin==LOW)) 
      {
        ServoDoor.write(0);
      
      }
      }
      break;

      /* _______________________________________________________ */


      case group1Record4: // lights off

      {
        delay(50);
        lcdVoice.clear();
        lcdVoice.print(" Group 2 :");
        lcdVoice.setCursor(0,1);
        lcdVoice.print(" 7,8,9,10,11,12 ");

        delay(50);
        LcdInHome.clear();
        LcdInHome.print("Temp:  Humidity:");
        LcdInHome.setCursor(0, 1);
        LcdInHome.print(temp);
        LcdInHome.setCursor(7,1);
        LcdInHome.print(humi);

        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Hello!, please");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Enter Ur Finger");
        

        digitalWrite(InLightsPin,HIGH);
        digitalWrite(SolarPin,HIGH);
        
        ServoSECDoor.write(0);
        ServoSECWindow.write(0);
        
        digitalWrite(LightsPin,LOW); // MAIN !!

        if((err=dht11.read(humi, temp))==0) // 29 normal , 36 high
  {
    if(temp>=36)
    {
      digitalWrite(AcPin,HIGH); // led ac HIGH
    }
  }
    int PumpSensor = analogRead(A0);
      if(PumpSensor>=550)
      {
        digitalWrite(PumpPin,LOW); // pump working
      }
      if(PumpSensor<550)
      {
        digitalWrite(PumpPin,HIGH); // pump stop
      }
     int FlameSensor = analogRead(A2); // Flame Sensor
      if(FlameSensor<=600)
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Fire Alarm");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Call 999 Please");
        
         digitalWrite(BuzzerPin,HIGH); // buzzer on
         digitalWrite(PumpPin,LOW); // pump working
         digitalWrite(AlarmLedPin1,HIGH); // fire alarm
         digitalWrite(AlarmLedPin2,HIGH); // fire alarm 2
      }
     int IrSensor = analogRead(A4); // Ir Sensor
      if(IrSensor<=50)
      {
        ServoGarage.write(115); // open garage
      if(IrSensor>=900)
          {
        ServoGarage.write(0); // close garage
          }
      }
      int SmokeSensor = analogRead(A3); // Smoke Sensor
      if(SmokeSensor>=250)
      {
        digitalWrite(BuzzerPin,HIGH); // Buzzer on
      }
      int RainSensor = analogRead(A1); // Rain sensor
      if(RainSensor>=100)
      {
        ServoWindow.write(0); // close Window
      }
      unsigned int LdrSensor;
      LdrSensor = digitalRead(LdrPin);  // 1: no light 0: light Ldr Sensor
      if(digitalRead(LdrPin)==1)
      {
        digitalWrite(LightsPin,HIGH); // edited
      }
      if(digitalRead(FingerledPin==HIGH) || digitalRead(LedDoorBlue==HIGH)) // led finger print anohter ARDUINO
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Welcome  :) ");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Door opened");
        
        ServoDoor.write(115); // open door
      }
      if(digitalRead(FingerledPin==LOW)) 
      {
        ServoDoor.write(0);
      
      }
      }

      break;


      /* ___________________________________________________________________ */
      

      case group1Record5: // sleep mode
      {
        delay(50);
        lcdVoice.clear();
        lcdVoice.print(" Group 2 :");
        lcdVoice.setCursor(0,1);
        lcdVoice.print(" 7,8,9,10,11,12 ");

        delay(50);
        LcdInHome.clear();
        LcdInHome.print("Temp:  Humidity:");
        LcdInHome.setCursor(0, 1);
        LcdInHome.print(temp);
        LcdInHome.setCursor(7,1);
        LcdInHome.print(humi);

        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("sleep mode");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Enter Ur Finger");
        
        digitalWrite(InLightsPin,LOW);
        digitalWrite(SolarPin,HIGH);
        
        ServoSECDoor.write(0);
        ServoSECWindow.write(0);

        if((err=dht11.read(humi, temp))==0) // 29 normal , 36 high
  {
    if(temp>=36)
    {
      digitalWrite(AcPin,HIGH); // led ac HIGH
    }
  }
    int PumpSensor = analogRead(A0);
      if(PumpSensor>=550)
      {
        digitalWrite(PumpPin,LOW); // pump working
      }
      if(PumpSensor<550)
      {
        digitalWrite(PumpPin,HIGH); // pump stop
      }
     int FlameSensor = analogRead(A2); // Flame Sensor
      if(FlameSensor<=600)
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Fire Alarm");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Call 999 Please");
        
         digitalWrite(BuzzerPin,HIGH); // buzzer on
         digitalWrite(PumpPin,LOW); // pump working
         digitalWrite(AlarmLedPin1,HIGH); // fire alarm
         digitalWrite(AlarmLedPin2,HIGH); // fire alarm 2
      }
     int IrSensor = analogRead(A4); // Ir Sensor
      if(IrSensor<=50)
      {
        ServoGarage.write(115); // open garage
      if(IrSensor>=900)
          {
        ServoGarage.write(0); // close garage
          }
      }
      int SmokeSensor = analogRead(A3); // Smoke Sensor
      if(SmokeSensor>=250)
      {
        digitalWrite(BuzzerPin,HIGH); // Buzzer on
      }
      int RainSensor = analogRead(A1); // Rain sensor
      if(RainSensor>=100)
      {
        ServoWindow.write(0); // close Window
      }
      unsigned int LdrSensor;
      LdrSensor = digitalRead(LdrPin);  // 1: no light 0: light Ldr Sensor
      if(digitalRead(LdrPin)==1)
      {
        digitalWrite(LightsPin,HIGH); // Light house 
      }
      if(digitalRead(FingerledPin==HIGH) || digitalRead(LedDoorBlue==HIGH)) // led finger print anohter ARDUINO
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Welcome  :) ");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Door opened");
        
        ServoDoor.write(115); // open door
      }
      if(digitalRead(FingerledPin==LOW)) 
      {
        ServoDoor.write(0);
      
      }
      }
      
      break;


      /* _______________________________________________________________ */ 

      case group1Record6: // awake mode
      {
        delay(50);
        lcdVoice.clear();
        lcdVoice.print(" Group 2 :");
        lcdVoice.setCursor(0,1);
        lcdVoice.print(" 7,8,9,10,11,12 ");

        delay(50);
        LcdInHome.clear();
        LcdInHome.print("Temp:  Humidity:");
        LcdInHome.setCursor(0, 1);
        LcdInHome.print(temp);
        LcdInHome.setCursor(7,1);
        LcdInHome.print(humi);

        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Hello!, please");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Enter Ur Finger");
        
        digitalWrite(InLightsPin,HIGH);
        digitalWrite(SolarPin,HIGH);
        
        ServoSECDoor.write(0);
        ServoSECWindow.write(0);

        if((err=dht11.read(humi, temp))==0) // 29 normal , 36 high
  {
    if(temp>=36)
    {
      digitalWrite(AcPin,HIGH); // led ac HIGH
    }
  }
    int PumpSensor = analogRead(A0);
      if(PumpSensor>=550)
      {
        digitalWrite(PumpPin,LOW); // pump working
      }
      if(PumpSensor<550)
      {
        digitalWrite(PumpPin,HIGH); // pump stop
      }
     int FlameSensor = analogRead(A2); // Flame Sensor
      if(FlameSensor<=600)
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Fire Alarm");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Call 999 Please");
        
         digitalWrite(BuzzerPin,HIGH); // buzzer on
         digitalWrite(PumpPin,LOW); // pump working
         digitalWrite(AlarmLedPin1,HIGH); // fire alarm
         digitalWrite(AlarmLedPin2,HIGH); // fire alarm 2
      }
     int IrSensor = analogRead(A4); // Ir Sensor
      if(IrSensor<=50)
      {
        ServoGarage.write(115); // open garage
      if(IrSensor>=900)
          {
        ServoGarage.write(0); // close garage
          }
      }
      int SmokeSensor = analogRead(A3); // Smoke Sensor
      if(SmokeSensor>=250)
      {
        digitalWrite(BuzzerPin,HIGH); // Buzzer on
      }
      int RainSensor = analogRead(A1); // Rain sensor
      if(RainSensor>=100)
      {
        ServoWindow.write(0); // close Window
      }
      unsigned int LdrSensor;
      LdrSensor = digitalRead(LdrPin);  // 1: no light 0: light Ldr Sensor
      if(digitalRead(LdrPin)==1)
      {
        digitalWrite(LightsPin,HIGH); // Light house 
      }
      if(digitalRead(FingerledPin==HIGH) || digitalRead(LedDoorBlue==HIGH)) // led finger print anohter ARDUINO
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Welcome  :) ");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Door opened");
        
        ServoDoor.write(115); // open door
      }
      if(digitalRead(FingerledPin==LOW)) 
      {
        ServoDoor.write(0);
      
      }
      }
      
      break;


      /*_______________________________________________________________*/

      

      case group2Record1: // Out mode 
      {
        delay(50);
        lcdVoice.clear();
        lcdVoice.print(" Group 3 :");
        lcdVoice.setCursor(0,1);
        lcdVoice.print("13,14,15,16");

        delay(50);
        LcdInHome.clear();
        LcdInHome.print("Temp:  Humidity:");
        LcdInHome.setCursor(0, 1);
        LcdInHome.print(temp);
        LcdInHome.setCursor(7,1);
        LcdInHome.print(humi);

        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("No One InHome ");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Enter Ur Finger");
        
        digitalWrite(InLightsPin,LOW);
        digitalWrite(SolarPin,HIGH);
        
        ServoSECDoor.write(115);
        ServoSECWindow.write(115);


        if((err=dht11.read(humi, temp))==0) // 29 normal , 36 high
  {
    if(temp>=36)
    {
      digitalWrite(AcPin,HIGH); // led ac HIGH
    }
  }
    int PumpSensor = analogRead(A0);
      if(PumpSensor>=550)
      {
        digitalWrite(PumpPin,LOW); // pump working
      }
      if(PumpSensor<550)
      {
        digitalWrite(PumpPin,HIGH); // pump stop
      }
     int FlameSensor = analogRead(A2); // Flame Sensor
      if(FlameSensor<=600)
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Fire Alarm");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Call 999 Please");
        
         digitalWrite(BuzzerPin,HIGH); // buzzer on
         digitalWrite(PumpPin,LOW); // pump working
         digitalWrite(AlarmLedPin1,HIGH); // fire alarm
         digitalWrite(AlarmLedPin2,HIGH); // fire alarm 2
         
      }
     int IrSensor = analogRead(A4); // Ir Sensor
      if(IrSensor<=50)
      {
        ServoGarage.write(115); // open garage
      if(IrSensor>=900)
          {
        ServoGarage.write(0); // close garage
          }
      }
      int SmokeSensor = analogRead(A3); // Smoke Sensor
      if(SmokeSensor>=250)
      {
        digitalWrite(BuzzerPin,HIGH); // Buzzer on
      }
      int RainSensor = analogRead(A1); // Rain sensor
      if(RainSensor>=100)
      {
        ServoWindow.write(0); // close Window
      }
      unsigned int LdrSensor;
      LdrSensor = digitalRead(LdrPin);  // 1: no light 0: light Ldr Sensor
      if(digitalRead(LdrPin)==1)
      {
        digitalWrite(LightsPin,HIGH); // Light house 
      }
      if(digitalRead(FingerledPin==HIGH) || digitalRead(LedDoorBlue==HIGH)) // led finger print anohter ARDUINO
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Welcome  :) ");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Door opened");
        
        ServoDoor.write(115); // open door
      }
      if(digitalRead(FingerledPin==LOW)) 
      {
        ServoDoor.write(0);
      
      }
      }
      
      break;


      /*____________________________________________________________*/
      

      case group2Record2: // home mode
      {
        delay(50);
        lcdVoice.clear();
        lcdVoice.print(" Group 3 :");
        lcdVoice.setCursor(0,1);
        lcdVoice.print("13,14,15,16");

        delay(50);
        LcdInHome.clear();
        LcdInHome.print("Temp:  Humidity:");
        LcdInHome.setCursor(0, 1);
        LcdInHome.print(temp);
        LcdInHome.setCursor(7,1);
        LcdInHome.print(humi);

        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Hello!, please");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Enter Ur Finger");
        
        digitalWrite(InLightsPin,HIGH);
        digitalWrite(SolarPin,HIGH);
        
        ServoSECDoor.write(0);
        ServoSECWindow.write(0);

        if((err=dht11.read(humi, temp))==0) // 29 normal , 36 high
  {
    if(temp>=36)
    {
      digitalWrite(AcPin,HIGH); // led ac HIGH
    }
  }
    int PumpSensor = analogRead(A0);
      if(PumpSensor>=550)
      {
        digitalWrite(PumpPin,LOW); // pump working
      }
      if(PumpSensor<550)
      {
        digitalWrite(PumpPin,HIGH); // pump stop
      }
     int FlameSensor = analogRead(A2); // Flame Sensor
      if(FlameSensor<=600)
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Fire Alarm");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Call 999 Please");
        
         digitalWrite(BuzzerPin,HIGH); // buzzer on
         digitalWrite(PumpPin,LOW); // pump working
         digitalWrite(AlarmLedPin1,HIGH); // fire alarm
         digitalWrite(AlarmLedPin2,HIGH); // fire alarm 2
      }
     int IrSensor = analogRead(A4); // Ir Sensor
      if(IrSensor<=50)
      {
        ServoGarage.write(115); // open garage
      if(IrSensor>=900)
          {
        ServoGarage.write(0); // close garage
          }
      }
      int SmokeSensor = analogRead(A3); // Smoke Sensor
      if(SmokeSensor>=250)
      {
        digitalWrite(BuzzerPin,HIGH); // Buzzer on
      }
      int RainSensor = analogRead(A1); // Rain sensor
      if(RainSensor>=100)
      {
        ServoWindow.write(0); // close Window
      }
      unsigned int LdrSensor;
      LdrSensor = digitalRead(LdrPin);  // 1: no light 0: light Ldr Sensor
      if(digitalRead(LdrPin)==1)
      {
        digitalWrite(LightsPin,HIGH); // Light house 
      }
      if(digitalRead(FingerledPin==HIGH) || digitalRead(LedDoorBlue==HIGH)) // led finger print anohter ARDUINO
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Welcome  :) ");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Door opened");
        
        ServoDoor.write(115); // open door
      }
      if(digitalRead(FingerledPin==LOW)) 
      {
        ServoDoor.write(0);
      
      }
      }
      
      break;


      /* ___________________________________________________________________ */

      case group2Record3: // pump on
      {
        delay(50);
        lcdVoice.clear();
        lcdVoice.print(" Group 3 :");
        lcdVoice.setCursor(0,1);
        lcdVoice.print("13,14,15,16");

        delay(50);
        LcdInHome.clear();
        LcdInHome.print("Temp:  Humidity:");
        LcdInHome.setCursor(0, 1);
        LcdInHome.print(temp);
        LcdInHome.setCursor(7,1);
        LcdInHome.print(humi);

        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Hello!, please");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Enter Ur Finger");
        
        digitalWrite(InLightsPin,HIGH);
        digitalWrite(SolarPin,HIGH);
        
        ServoSECDoor.write(0);
        ServoSECWindow.write(0);
        
         digitalWrite(PumpPin,LOW); // MAIN !!

        if((err=dht11.read(humi, temp))==0) // 29 normal , 36 high
  {
    if(temp>=36)
    {
      digitalWrite(AcPin,HIGH); // led ac HIGH
    }
  }
    int PumpSensor = analogRead(A0);
      if(PumpSensor>=550)
      {
        digitalWrite(PumpPin,LOW); // pump working
      }
      if(PumpSensor<550)
      {
        // edited
      }
     int FlameSensor = analogRead(A2); // Flame Sensor
      if(FlameSensor<=600)
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Fire Alarm");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Call 999 Please");
        
         digitalWrite(BuzzerPin,HIGH); // buzzer on
         digitalWrite(PumpPin,LOW); // pump working
         digitalWrite(AlarmLedPin1,HIGH); // fire alarm
         digitalWrite(AlarmLedPin2,HIGH); // fire alarm 2
      }
     int IrSensor = analogRead(A4); // Ir Sensor
      if(IrSensor<=50)
      {
        ServoGarage.write(115); // open garage
      if(IrSensor>=900)
          {
        ServoGarage.write(0); // close garage
          }
      }
      int SmokeSensor = analogRead(A3); // Smoke Sensor
      if(SmokeSensor>=250)
      {
        digitalWrite(BuzzerPin,HIGH); // Buzzer on
      }
      int RainSensor = analogRead(A1); // Rain sensor
      if(RainSensor>=100)
      {
        ServoWindow.write(0); // close Window
      }
      unsigned int LdrSensor;
      LdrSensor = digitalRead(LdrPin);  // 1: no light 0: light Ldr Sensor
      if(digitalRead(LdrPin)==1)
      {
        digitalWrite(LightsPin,HIGH); // Light house 
      }
      if(digitalRead(FingerledPin==HIGH) || digitalRead(LedDoorBlue==HIGH)) // led finger print anohter ARDUINO
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Welcome  :) ");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Door opened");
        
        ServoDoor.write(115); // open door
      }
      if(digitalRead(FingerledPin==LOW)) 
      {
        ServoDoor.write(0);
      
      }
      }
      
      break;


      /*______________________________________________________________ */

      case group2Record4: // pump off
      {
        delay(50);
        lcdVoice.clear();
        lcdVoice.print(" Group 3 :");
        lcdVoice.setCursor(0,1);
        lcdVoice.print("13,14,15,16");

        delay(50);
        LcdInHome.clear();
        LcdInHome.print("Temp:  Humidity:");
        LcdInHome.setCursor(0, 1);
        LcdInHome.print(temp);
        LcdInHome.setCursor(7,1);
        LcdInHome.print(humi);

        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Hello!, please");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Enter Ur Finger");
        
        digitalWrite(InLightsPin,HIGH);
        digitalWrite(SolarPin,HIGH);
        
        ServoSECDoor.write(0);
        ServoSECWindow.write(0);
        
         digitalWrite(PumpPin,HIGH); // MAIN !!

        if((err=dht11.read(humi, temp))==0) // 29 normal , 36 high
  {
    if(temp>=36)
    {
      digitalWrite(AcPin,HIGH); // led ac HIGH
    }
  }
    int PumpSensor = analogRead(A0);
      if(PumpSensor>=550)
      {
      // edited
      }
      if(PumpSensor<550)
      {
        // edited
      }
     int FlameSensor = analogRead(A2); // Flame Sensor
      if(FlameSensor<=600)
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Fire Alarm");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Call 999 Please");
        
         digitalWrite(BuzzerPin,HIGH); // buzzer on
         digitalWrite(AlarmLedPin1,HIGH); // fire alarm
         digitalWrite(AlarmLedPin2,HIGH); // fire alarm 2
         
        // edited
      }
     int IrSensor = analogRead(A4); // Ir Sensor
      if(IrSensor<=50)
      {
        ServoGarage.write(115); // open garage
      if(IrSensor>=900)
          {
        ServoGarage.write(0); // close garage
          }
      }
      int SmokeSensor = analogRead(A3); // Smoke Sensor
      if(SmokeSensor>=250)
      {
        digitalWrite(BuzzerPin,HIGH); // Buzzer on
      }
      int RainSensor = analogRead(A1); // Rain sensor
      if(RainSensor>=100)
      {
        ServoWindow.write(0); // close Window
      }
      unsigned int LdrSensor;
      LdrSensor = digitalRead(LdrPin);  // 1: no light 0: light Ldr Sensor
      if(digitalRead(LdrPin)==1)
      {
        digitalWrite(LightsPin,HIGH); // Light house 
      }
      if(digitalRead(FingerledPin==HIGH) || digitalRead(LedDoorBlue==HIGH)) // led finger print anohter ARDUINO
      {
        delay(50);
        LcdOutHome.clear();
        LcdOutHome.print("Welcome  :) ");
        LcdOutHome.setCursor(0, 1);
        LcdOutHome.print("Door opened");
        
        ServoDoor.write(115); // open door
      }
      if(digitalRead(FingerledPin==LOW)) 
      {
        ServoDoor.write(0);
      
      }
      }
      
      break;

      
      default:
      break;
    }
    /** voice recognized */
    printVR(buf);
  }
   
   }


/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf     --> command length
           len     --> number of parameters
*/
void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf  -->  VR module return value when voice is recognized.
             buf[0]  -->  Group mode(FF: None Group, 0x8n: User, 0x0n:System
             buf[1]  -->  number of record which is recognized. 
             buf[2]  -->  Recognizer index(position) value of the recognized record.
             buf[3]  -->  Signature length
             buf[4]~buf[n] --> Signature
*/
void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
//  Serial.println("\r\n");
  Serial.println();
}

void printRecord(uint8_t *buf, uint8_t len)
{
  Serial.print(F("Record: "));
  for(int i=0; i<len; i++){
    Serial.print(buf[i], DEC);
    Serial.print(", ");
  }
}



/*-------------------------------------------------------------------------------------------------------------- */



