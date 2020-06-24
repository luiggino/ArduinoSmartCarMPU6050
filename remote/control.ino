#include <MPU6050.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

MPU6050 mpu;

const char* ssid = "";
const char* password = "";
String url = "";

long tiempo_prev = 0;
float pitch_prev = 0;
float roll_prev = 0;

const float A_S = 0.122;
const float G_S = 70.0;

void wifiConnect() {
  WiFi.begin(ssid, password);
  //WiFi.mode(WIFI_STA);
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

void setup(void) {

  Serial.begin(115200);

  Wire.begin();

  // initialize device
  Serial.println("Initializing I2C devices...");
  mpu.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  wifiConnect();
}

void loop() {
  int16_t Ax, Ay, Az, T, Gx, Gy, Gz;
  float pitch, _pitch;
  float roll, _roll;
  float dt;

  mpu.getAcceleration(&Ax, &Ay, &Az);

  pitch = atan2(Ax, sqrt(Ay * Ay + Az * Az));
  roll =  atan2(Ay, sqrt(Ax * Ax + Az * Az));

  pitch *= (180.0 / PI);
  roll  *= (180.0 / PI);


  Serial.print("Rotacion en X:  ");
  Serial.print(pitch);
  Serial.print("\tRotacion en Y: ");
  Serial.println(roll);


  String command;

  if ((roll >= 30) && (pitch > -20) && (pitch < 20)) {
    //forward
    command = "F";
  }
  else if ((roll <= -30) && (pitch > -20) && (pitch < 20)) {
    //backward
    command = "B";
  }

  else if ((pitch >= 20) && (roll > -30) && (roll < 30)) {
    //left
    command = "L";
  }

  else if ((pitch <= -20) && (roll > -30) && (roll < 30)) {
    //right
    command = "R";
  }
  else
  {
    //stop
    command = "S";
  }
  Serial.println("comand :" + command);

  if (command != "") {
    sendMessage(command);
  }


  delay(1000);
}

void sendMessage(String command)
{
  HTTPClient http;
  WiFiClient client;

  String turl = url + "/?State=" + command;
  Serial.println("url " + turl);
  if (http.begin(client, turl)) //Iniciar conexión
  {
    Serial.print("[HTTP] GET...\n");
    int httpCode = http.GET();
    Serial.println(httpCode);

    http.end();
  }
}
