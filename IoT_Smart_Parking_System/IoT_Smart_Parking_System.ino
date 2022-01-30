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
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <ArduinoHttpClient.h>
//#include <MySQL_Connection.h>
//#include <MySQL_Cursor.h>

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif


// ---------------------------------- //
// WiFi Settings
// ---------------------------------- //
char ssid[] = "PKK_Hotspot";
char pass[] = "ilovepkk";
int status = WL_IDLE_STATUS;

// use a ring buffer to increase speed and reduce memory allocation
RingBuffer buf(8);


// ---------------------------------- //
// MySQL Settings
// ---------------------------------- //
char user[] = "root";
char password[] = "luq123";
  
// ---------------------------------- //
// Declare variables, data type and pin number
// ---------------------------------- //
int US_Entrance_Trig = 2;
int US_Entrance_Echo = 3;
int US_Exit_Trig = 4;
int US_Exit_Echo = 5;

int ServoEntrancePin = 8;
int ServoExitPin = 11;

int GreenLED_1 = 22;
int GreenLED_2 = 23;

int RedLED_1 = 26;
int RedLED_2 = 27;

int IR1 = 14;
int IR2 = 15;

Servo Servo_Entrance;
Servo Servo_Exit;


// ---------------------------------- //
// Define variables
// ---------------------------------- //

long duration_1;
long duration_2;
int distance_1;
int distance_2;

int ScanLot1;
int ScanLot2;
int Availability1;
int Availability2;

int pingResult;

// ---------------------------------- //
// Constant value
// ---------------------------------- //
const boolean debug_mode = true;
const int limit_distance = 7; // Default value = 3cm
const IPAddress remote_ip(192, 168, 1, 17);
char* serverAddress = "192.168.1.17"; //Change the IP based on CMD, ipconfig
int port = 8080;
String apiKeyValue = "tPmAT5Ab3j7F9";
char urlPayload[] = "192.168.1.17";


// ---------------------------------- //
// Set pin mode (INPUT/OUTPUT)
// ---------------------------------- //
void setup() {

  pinMode(US_Entrance_Trig, OUTPUT);
  pinMode(US_Entrance_Echo, INPUT);
  pinMode(US_Exit_Trig, OUTPUT);
  pinMode(US_Exit_Echo, INPUT);
  pinMode(GreenLED_1, OUTPUT);
  pinMode(GreenLED_2, OUTPUT);
  pinMode(RedLED_1, OUTPUT);
  pinMode(RedLED_2, OUTPUT);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  Servo_Entrance.attach(ServoEntrancePin);
  Servo_Entrance.write(90);

  Servo_Exit.attach(ServoExitPin);
  Servo_Exit.write(90);

  Serial.begin(115200);   // initialize Serial Monitor for debugging
  Serial1.begin(115200);    // initialize Serial1 for ESP module
  WiFi.init(&Serial1);    // initialize ESP module

  // attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    //Serial.print("Connecting to WiFi network: ");
    //Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  //Serial.println("Connected to the network");
  IPAddress ip = WiFi.localIP();
  //Serial.print("IP Address: ");
  //Serial.println(ip);
  //Serial.println();

  //PingServer();
  //connectMySQL();
}

void PingServer() {

  Serial.print("Pinging to host server: ");
  Serial.println(remote_ip);
  pingResult = WiFi.ping(remote_ip);
  if (pingResult >= 0) {
    Serial.print("Connected! RTT = ");
    Serial.print(pingResult);
    Serial.println(" ms");
    Serial.println();
  }
  else {
    Serial.print("Connection failed! Error code: ");
    Serial.println(pingResult);
    Serial.println();
  }
}

/**
void connectMySQL() {
  Serial.println("Connecting to MySQL Server...");
  if(conn.connect(remote_ip,3306,user,password)) 
  {
    delay(1000);
    Serial1.println("Connected to MySQL.");
  }
  else
    Serial.println("Connection to MySQL has failed.");

  // conn.close(); 
}
**/

void EntranceGate() {
  digitalWrite(US_Entrance_Trig, LOW);
  delay(500);
  digitalWrite(US_Entrance_Trig, HIGH);
  delay(500);
  digitalWrite(US_Entrance_Trig, LOW);
  duration_1 = pulseIn(US_Entrance_Echo, HIGH);
  distance_1 = duration_1 * 0.034 / 2;

  if (distance_1 < limit_distance) {
    Servo_Entrance.write(0);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Entrance Gate Opened");
    Serial.print("Distance: ");
    Serial.print(distance_1);
    Serial.println(" cm");
    Serial.println();
    delay(1000);
  }
  else {
    Servo_Entrance.write(90);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Entrance Gate Closed");
    Serial.print("Distance: ");
    Serial.print(distance_1);
    Serial.println(" cm");
    Serial.println();
    delay(1000);
  }
}

void ExitGate() {
  digitalWrite(US_Exit_Trig, LOW);
  delay(500);
  digitalWrite(US_Exit_Trig, HIGH);
  delay(500);
  digitalWrite(US_Exit_Trig, LOW);
  duration_2 = pulseIn(US_Exit_Echo, HIGH);
  distance_2 = duration_2 * 0.034 / 2;

  if (distance_2 < limit_distance) {
    Servo_Exit.write(180);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Exit Gate Opened");
    Serial.print("Distance: ");
    Serial.print(distance_2);
    Serial.println(" cm");
    Serial.println();
    delay(1000);
  }
  else {
    Servo_Exit.write(90);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Exit Gate Closed");
    Serial.print("Distance: ");
    Serial.print(distance_2);
    Serial.println(" cm");
    Serial.println();
    delay(1000);
  }
}

void IR1_ScanVehicle() {
  int ScanLot1 = digitalRead(IR1);

  if (ScanLot1 == HIGH) {
    Serial.println("Lot 1 - No Vehicle");
    digitalWrite(RedLED_1, LOW);
    digitalWrite(GreenLED_1, HIGH);
    Availability1 = 1;
    delay(100);
  }
  else {
    Serial.println("Lot 1 - There is a vehicle");
    digitalWrite(RedLED_1, HIGH);
    digitalWrite(GreenLED_1, LOW);
    Availability1 = 0;
    delay(100);
  }
}

void IR2_ScanVehicle() {
  int ScanLot2 = digitalRead(IR2);

  if (ScanLot2 == HIGH) {
    Serial.println("Lot 2 - No Vehicle");
    digitalWrite(RedLED_2, LOW);
    digitalWrite(GreenLED_2, HIGH);
    Availability2 = 1;
    delay(100);
  }
  else {
    Serial.println("Lot 2 - There is a vehicle");
    digitalWrite(RedLED_2, HIGH);
    digitalWrite(GreenLED_2, LOW);
    Availability2 = 0;
    delay(100);
  }
}

/**
void execQuery() {
  char SQL_QUERY[] = "UPDATE iot_parking.parking SET isAvailable = '0' WHERE (level = '1' AND lot = '1')";
  
  Serial.println("Updating column data...");
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(SQL_QUERY);
  delete cur_mem;
}
**/


void GetData1() {
  WiFiEspClient client;
  
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial
  if (client.connect(urlPayload, 80)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    String QUERY1 = " /post_sensor_data.php?api_key=" + apiKeyValue + "&lot=" + int(1) + "&level=" + int(1) + "&isavailable=" + Availability1;
    String GETQUERY1 = "GET " + QUERY1 + " HTTP/1.1";
    Serial.println(GETQUERY1);
    client.println(GETQUERY1);
    client.println("Host: 192.168.1.17");
    client.println("Connection: close");
    client.println();
  }

  // if there are incoming bytes available
  // from the server, read them and print them
  Serial.println("Printing incoming bytes on serial monitor");
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client
  if (!client.connected()) {
    Serial.println();
    Serial.println();
    Serial.println("Disconnecting from server...");
    client.stop();
  }
  
  /**
  WiFiEspClient wifi;
  //MySQL_Connection conn((Client *)&wifi);
  HttpClient client = HttpClient(wifi, serverAddress, port);
  
  Serial.println("POST Request Init");
  // http://192.168.1.17/post_sensor_data.php?api_key=tPmAT5Ab3j7F9&lot=1&level=1&isavailable=0
  //String postData = "api_key=" + apiKeyValue + "&lot=" + int(1) + "&level=" + int(1) + "&isavailable=" + ScanLot1;
  String postData = "api_key=tPmAT5Ab3j7F9&lot=1&level=1&isavailable=0";
  Serial.print("POST Query: ");
  Serial.println(postData);

  client.beginRequest();
  client.post("/");
  client.sendHeader("Content-Type", "application/x-www-form-urlencoded");
  client.sendHeader("Content-Length", postData.length());
  client.sendHeader("X-Custom-Header", "Data from Arduino");
  client.beginBody();
  client.print(postData);
  client.endRequest();

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  Serial.println();
  delay(1000); // Send an HTTP POST request every 1 second.
**/
}

void GetData2() {
  WiFiEspClient client;
  
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial
  if (client.connect(urlPayload, 80)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    String QUERY2 = " /post_sensor_data.php?api_key=" + apiKeyValue + "&lot=" + int(2) + "&level=" + int(1) + "&isavailable=" + Availability2;
    String GETQUERY2 = "GET " + QUERY2 + " HTTP/1.1";
    Serial.println(GETQUERY2);
    client.println(GETQUERY2);
    client.println("Host: 192.168.1.17");
    client.println("Connection: close");
    client.println();
  }

  // if there are incoming bytes available
  // from the server, read them and print them
  Serial.println("Printing incoming bytes on serial monitor");
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client
  if (!client.connected()) {
    Serial.println();
    Serial.println();
    Serial.println("Disconnecting from server...");
    client.stop();
  }
}

/**
  void TestUltrasonicSensor() {
  digitalWrite(US_Exit_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(US_Exit_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_Exit_Trig, LOW);
  duration_2 = pulseIn(US_Exit_Echo, HIGH);
  distance_2 = duration_2 * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.print(distance_2);
  Serial.println(" cm");
  }
**/

void loop() {
  //EntranceGate();
  //delay(100);
  //ExitGate();
  //delay(100);
  IR1_ScanVehicle();
  delay(1000);
  IR2_ScanVehicle();
  delay(1000);
  GetData1();
  delay(1000);
  GetData2();
  delay(1000);
  //TestUltrasonicSensor();
  //delay(1000);
}
