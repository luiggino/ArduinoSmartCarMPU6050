#include <L298NX2.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Pin definition
const uint8_t IN1_A = D5;
const uint8_t IN2_A = D6;

const uint8_t IN1_B = D7;
const uint8_t IN2_B = D8;

// Create one motor instance
L298NX2 motors(IN1_A, IN2_A, IN1_B, IN2_B);
int speedCar = 255;

const char* ssid = "";
const char* password = "";
ESP8266WebServer server(80);
String command;             //String to store app command state.

void wifiConnect() {
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.print("Connecting to "); Serial.print(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
}

void initServer() {
  // Starting WEB-server
  server.on ( "/", HTTP_handleRoot );
  server.onNotFound ( HTTP_handleRoot );
  server.begin();
}

void setup() {
  Serial.begin(115200);
  Serial.println(" ");
  Serial.println("Init...");

  wifiConnect();
  initServer();

  // Set initial speed
  motors.setSpeed(speedCar);
}

void loop()
{
  server.handleClient();
  motors.setSpeed(speedCar);

  command = server.arg("State");
  if (command == "F") forward();
  else if (command == "B") backward();
  else if (command == "L") leftF();
  else if (command == "R") rightF();
  else if (command == "I") rightB();
  else if (command == "G") leftB();
  /*else if (command == "J") goBackRight();
    else if (command == "H") goBackLeft();*/
  else if (command == "S") Stop();
}

void HTTP_handleRoot(void) {
  if ( server.hasArg("State") ) {
    Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}

void forward()
{
  Serial.println("forward.");
  motors.forward();
}

void backward()
{
  Serial.println("backward.");
  motors.backward();
}

void leftF()
{
  Serial.println("left forward.");
  motors.forwardA();
}

void rightF()
{
  Serial.println("right forward.");
  motors.forwardB();
}

void leftB()
{
  Serial.println("left backward.");
  motors.backwardA();
}

void rightB()
{
  Serial.println("right backward.");
  motors.backwardB();
}


void Stop()
{
  Serial.println("Stop.");
  motors.stop();
}
