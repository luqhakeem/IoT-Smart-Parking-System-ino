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
#include <WiFi.h>
#include <Wire.h>
/**
#include <SoftwareSerial.h>                         //including the SoftwareSerial library will allow you to use the pin no. 2,3 as Rx, Tx.      
SoftwareSerial esp8266(2,3);                        //set the Rx ==> Pin 2; TX ==> Pin3.
**/      

// ---------------------------------- //
// WiFi Configuration
// ---------------------------------- //
const char* ssid     = "PKK_KM_HOTSPOT";
const char* password = "luqman123";


const char* serverName = "http://192.168.48.24/post_sensor_data.php"; //Change the IP based on CMD, ipconfig
String apiKeyValue = "tPmAT5Ab3j7F9";

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

/** 
int US1_Trig = 18;
int US1_Echo = 19;
int US2_Trig = 20;
int US2_Echo = 21;
int US3_Trig = 22;
int US3_Echo = 23;
int US4_Trig = 24;
int US4_Echo = 25;
**/

int IR1 = 21;
int IR2 = 23;

Servo Servo_Entrance;
Servo Servo_Exit;

// ---------------------------------- //
// Define variables
// ---------------------------------- //
/**
long duration_1;
long duration_2;
long duration_3;
long duration_4;

int distance_1;
int distance_2;
int distance_3;
int distance_4;
**/

int ScanLot1;
int ScanLot2;

// ---------------------------------- //
// Constant value
// ---------------------------------- //
const int limit_distance = 4; // Default = 4cm
const int buzzer_freq = 500; // Default buzzer frequency = 4.5kHz
const int buzzer_time;
const boolean debug_mode = true;

// ---------------------------------- //
// Set pin mode (INPUT/OUTPUT)
// ---------------------------------- //
void setup() {
  /**
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  **/
  
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
  
  /**
  pinMode(US1_Trig, OUTPUT);
  pinMode(US1_Echo, INPUT);
  pinMode(US2_Trig, OUTPUT);
  pinMode(US2_Echo, INPUT);
  pinMode(US3_Trig, OUTPUT);
  pinMode(US3_Echo, INPUT);
  pinMode(US4_Trig, OUTPUT);
  pinMode(US4_Echo, INPUT);
  **/

  pinMode(LED_BUILTIN, OUTPUT);

  /**
  SendCommand("AT+RST", "Ready");
  delay(5000);
  SendCommand("AT+CWMODE=1","OK");
  SendCommand("AT+CIFSR", "OK");
  SendCommand("AT+CIPMUX=1","OK");
  SendCommand("AT+CIPSERVER=1,80","OK");
  **/

  Servo_Entrance.attach(ServoEntrancePin); 
  Servo_Entrance.write(180);
   
  Servo_Exit.attach(ServoExitPin);
  Servo_Exit.write(0);  
}

void EntranceGate() {
  int val = digitalRead(IRSensor_Entrance);

  if (val == HIGH){
    Servo_Entrance.write(90);
    //digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Entrance Gate Closed");
    delay(1000);        // ...for 1 sec
  }
  else {
    Servo_Entrance.write(180);
    //digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Entrance Gate Opened"); 
    delay(3000);  
  }
}

void ExitGate() {
  int val2 = digitalRead(IRSensor_Exit);

  if (val2 == HIGH){
    Servo_Exit.write(90);
    //digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Exit Gate Closed");
    delay(1000);        // ...for 1 sec
  }
  else {
    Servo_Exit.write(180);
    //digitalWrite(LED_BUILTIN, LOW); 
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
    tone(Buzzer_2, buzzer_freq);
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
    tone(Buzzer_3, buzzer_freq);
    delay(100);  
  }
}

/**
void PostData() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&lot=" + int(1) + "&level=" + int(1) + "&isavailable=" + String(ScanLot1);
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    String httpRequestData2 = "api_key=" + apiKeyValue + "&lot=" + int(2) + "&level=" + int(1) + "&isavailable=" + String(ScanLot2);
    Serial.print("httpRequestData2: ");
    Serial.println(httpRequestData2);

    //Debugging
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&lot=1&level=1&isAvailable=0";
    //String httpRequestData2 = "api_key=tPmAT5Ab3j7F9&lot=2q2&level=1&isAvailable=0";
    
    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
    int httpResponseCode2 = http.POST(httpRequestData2);
        
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    if (httpResponseCode2>0) {
      Serial.print("HTTP Response code 2: ");
      Serial.println(httpResponseCode2);
    }
    else {
      Serial.print("Error code 2: ");
      Serial.println(httpResponseCode2);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 1 second
  delay(1000);
}
**/

/**
void Lot1_ScanVehiclePresence() {
  digitalWrite(US1_Trig, LOW);
  delay(500);
  digitalWrite(US1_Trig, HIGH);
  delay(500);
  digitalWrite(US1_Trig, LOW);
  duration_1 = pulseIn(US1_Echo, HIGH);
  distance_1 = duration_1 * 0.034 / 2;

  if (distance_1 < limit_distance) {
    Serial.println("Lot 1 - Vehicle presence");
    digitalWrite(RedLED_1, HIGH);
    digitalWrite(GreenLED_1, LOW);
    delay(100);
    tone(Buzzer_1, buzzer_freq);
    delay(100);
  }
  else {
    Serial.println("Lot 1 - No vehicle");
    digitalWrite(RedLED_1, LOW);
    digitalWrite(GreenLED_1, HIGH);
    delay(100);
    noTone(Buzzer_1);
    delay(100);
  }
}

void Lot2_ScanVehiclePresence() {
  digitalWrite(US2_Trig, LOW);
  delay(500);
  digitalWrite(US2_Trig, HIGH);
  delay(500);
  digitalWrite(US2_Trig, LOW);
  duration_2 = pulseIn(US2_Echo, HIGH);
  distance_2 = duration_2 * 0.034 / 2;

  if (distance_2 < limit_distance) {
    Serial.println("Lot 2 - Vehicle presence");
    digitalWrite(RedLED_2, HIGH);
    digitalWrite(GreenLED_2, LOW);
    delay(100);
    tone(Buzzer_2, buzzer_freq);
    delay(100);
  }
  else {
    Serial.println("Lot 2 - No vehicle");
    digitalWrite(RedLED_2, LOW);
    digitalWrite(GreenLED_2, HIGH);
    delay(100);
    noTone(Buzzer_2);
    delay(100);
  }
}

void Lot3_ScanVehiclePresence() {
  digitalWrite(US3_Trig, LOW);
  delay(500);
  digitalWrite(US3_Trig, HIGH);
  delay(500);
  digitalWrite(US3_Trig, LOW);
  duration_3 = pulseIn(US3_Echo, HIGH);
  distance_3 = duration_3 * 0.034 / 2;

  if (distance_3 < limit_distance) {
    Serial.println("Lot 3 - Vehicle presence");
    digitalWrite(RedLED_3, HIGH);
    digitalWrite(GreenLED_3, LOW);
    delay(100);
    tone(Buzzer_3, buzzer_freq);
    delay(100);
  }
  else {
    Serial.println("Lot 3 - No vehicle");
    digitalWrite(RedLED_3, LOW);
    digitalWrite(GreenLED_3, HIGH);
    delay(100);
    noTone(Buzzer_3);
    delay(100);
  }
}

void Lot4_ScanVehiclePresence() {
  digitalWrite(US4_Trig, LOW);
  delay(500);
  digitalWrite(US4_Trig, HIGH);
  delay(500);
  digitalWrite(US4_Trig, LOW);
  duration_4 = pulseIn(US4_Echo, HIGH);
  distance_4 = duration_4 * 0.034 / 2;

  if (distance_4 < limit_distance) {
    Serial.println("Lot 4 - Vehicle presence");
    digitalWrite(RedLED_4, HIGH);
    digitalWrite(GreenLED_4, LOW);
    delay(100);
    tone(Buzzer_4, buzzer_freq);
    delay(100);
  }
  else {
    Serial.println("Lot 4 - No vehicle");
    digitalWrite(RedLED_4, LOW);
    digitalWrite(GreenLED_4, HIGH);
    delay(100);
    noTone(Buzzer_4);
    delay(100);
  }
}

void TestUltrasonicSensor() {
  digitalWrite(US1_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(US1_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(US1_Trig, LOW);
  duration_1 = pulseIn(US1_Echo, HIGH);
  distance_1 = duration_1 * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.print(distance_1);
  Serial.println(" cm");
}
**/

void loop() {
  /**
  InitWifiModule();
  delay(100);
  **/
  EntranceGate();
  delay(100);
  IR1_ScanVehicle();
  delay(100);
  IR2_ScanVehicle();
  delay(100);
  ExitGate();
  delay(100);
  /**
  PostData();
  delay(100);
  **/
  /**
  Lot1_ScanVehiclePresence();
  delay(100);
  Lot2_ScanVehiclePresence();
  delay(100);
  Lot3_ScanVehiclePresence();
  delay(100);
  Lot4_ScanVehiclePresence();
  delay(100);
  TestUltrasonicSensor();
  delay(500);
  **/ 
}
