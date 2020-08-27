
//Programa para mandar los comandos AT a un modulo B  T HC05
//a traves de un arduino UNO
//Convertir señales de 5V(Arduino) a 3,3V(Modulo BT) con un partidor de tensión
//Con el HC05 van a 5V sin problema
//Apretar pulsador de placa BT para activar modo configuración o poner EN=5V en módulo HC05 (Permiso para comandos AT)
//¡¡¡ojo!!!Monitor serial que mande CR+LF

#include <SoftwareSerial.h>

//Conectamos los módulos BT Rx y Tx a estos pines del Arduino
//Cruzando siempre Rx y Tx

SoftwareSerial BT(10,11); //Rx=10, Tx=11
  
void setup() {
  // initialize both serial ports:
  Serial.begin(9600);
  BT.begin(38400);
}

void loop() {
 
  if (BT.available())      Serial.write(BT.read());  
  if (Serial.available())  BT.write(Serial.read());
  
}
