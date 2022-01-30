// ---------------------------------------------------------------- //
// IoT Smart Parking System
// Politeknik Kota Kinabalu
//
// DEM 1   
// 1. LUQMAN HAKEEM BIN MOHAMAD NOR
// 2. MUHAMMAD NURAZMIE BIN SOMIE
//  
// ---------------------------------------------------------------- //

// ---------------------------------- //
// Include header file
// ---------------------------------- //
#include <Servo.h>

// ---------------------------------- //
// Declare variables, data type and pin number
// ---------------------------------- //
int IRSensor_Entrance = 2;
int IRSensor_Exit = 3;

int ServoEntrancePin = 4;
int ServoExitPin = 5; 

int GreenLED_1 = 6;
int GreenLED_2 = 7;
int GreenLED_3 = 8;
int GreenLED_4 = 9;

int RedLED_1 = 10;
int RedLED_2 = 11;
int RedLED_3 = 12;
int RedLED_4 = 13;

int Buzzer_1 = 14;
int Buzzer_2 = 15;
int Buzzer_3 = 16;
int Buzzer_4 = 17;

int IR1 = 21;
int IR2 = 23;

Servo Servo_Entrance;
Servo Servo_Exit;

int ScanLot1;
int ScanLot2;

// ---------------------------------- //
// Constant value
// ---------------------------------- //
const int buzzer_freq = 500; // Default buzzer frequency = 4.5kHz
const int buzzer_time;
const boolean debug_mode = true;

// ---------------------------------- //
// Set pin mode (INPUT/OUTPUT)
// ---------------------------------- //
void setup() {
  
  pinMode(IRSensor_Entrance, INPUT);
  pinMode(IRSensor_Exit, INPUT);
  pinMode(GreenLED_1, OUTPUT);
  pinMode(GreenLED_2, OUTPUT);  
  pinMode(GreenLED_3, OUTPUT);  
  pinMode(GreenLED_4, OUTPUT);
  pinMode(Buzzer_1, OUTPUT);
  pinMode(Buzzer_2, OUTPUT);
  pinMode(Buzzer_3, OUTPUT);
  pinMode(Buzzer_4, OUTPUT);   
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  
  pinMode(LED_BUILTIN, OUTPUT);

  Servo_Entrance.attach(ServoEntrancePin); 
  Servo_Entrance.write(180);
   
  Servo_Exit.attach(ServoExitPin);
  Servo_Exit.write(0);  
}

void EntranceGate() {
  int val = digitalRead(IRSensor_Entrance);

  if (val == HIGH){
    Servo_Entrance.write(90);
    Serial.println("Entrance Gate Closed");
    delay(1000);
  }
  else {
    Servo_Entrance.write(180);
    Serial.println("Entrance Gate Opened"); 
    delay(3000);  
  }
}

void ExitGate() {
  int val2 = digitalRead(IRSensor_Exit);

  if (val2 == HIGH){
    Servo_Exit.write(90);
    Serial.println("Exit Gate Closed");
    delay(1000);
  }
  else {
    Servo_Exit.write(180);
    Serial.println("Exit Gate Opened");
    delay(3000);  
  }
}

void IR1_ScanVehicle() {
  int ScanLot1 = digitalRead(IR1);

  if (ScanLot1 == HIGH){
    Servo_Exit.write(90);
    Serial.println("Lot 1 - No Vehicle");
    digitalWrite(RedLED_2, LOW);
    digitalWrite(GreenLED_2, HIGH);
    noTone(Buzzer_2);
    delay(100);
  }
  else {
    Serial.println("Lot 1 - There is a vehicle");
    digitalWrite(RedLED_2, HIGH);
    digitalWrite(GreenLED_2, LOW);
    // tone(Buzzer_2, buzzer_freq);
    delay(100);  
  }
}

void IR2_ScanVehicle() {
  int ScanLot2 = digitalRead(IR2);

  if (ScanLot2 == HIGH){
    Servo_Exit.write(90);
    Serial.println("Lot 2 - No Vehicle");
    digitalWrite(RedLED_3, LOW);
    digitalWrite(GreenLED_3, HIGH);
    noTone(Buzzer_3);
    delay(100);
  }
  else {
    Serial.println("Lot 2 - There is a vehicle");
    digitalWrite(RedLED_3, HIGH);
    digitalWrite(GreenLED_3, LOW);
    // tone(Buzzer_3, buzzer_freq);
    delay(100);  
  }
}

void loop() {
  EntranceGate();
  delay(100);
  IR1_ScanVehicle();
  delay(100);
  IR2_ScanVehicle();
  delay(100);
  ExitGate();
  delay(100);
}
