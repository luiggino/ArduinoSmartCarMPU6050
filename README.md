# ArduinoSmartCarMPU6050
Arduino Smart Car con movimiento desde un MPU6050

Smart Car con una placa esp8266 conectada a un L298N para los motores, se controla remotamente con una placa esp8266 y un sensor MPU6050 que permite obtener la navegación de acuerdo a ciertos ángulos.

Se debe configurar para ambos códigos:

```
const char* ssid = "";
const char* password = "";
```
donde ssid y password son los datos de la red Wifi, además de la ip del smart car.

```
String url = "";
```
