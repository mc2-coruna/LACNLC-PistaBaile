                                  
//DETECCION RAYOS COSMICOS

//Arduino MEGA2560

//Lectura de 8 tubos Geiger (4 + 4)
//Bluetooth conectado en pines 19-18-17-16

#include <SoftwareSerial.h> //Para usar on MIDI

const int tubo10 = 46;
const int tubo11 = 47;
const int tubo12 = 48;
const int tubo13 = 49;
const int tubo20 = 50;
const int tubo21 = 51;
const int tubo22 = 52;
const int tubo23 = 53;

                 
const int C1 = 22;
const int C2 = 23;
const int C3 = 24;
const int C4 = 25;
const int C5 = 26;
const int C6 = 27;
const int C7 = 28;
const int C8 = 29;
const int C9 = 30;
const int C10 = 31;
const int C11 = 32;
const int C12 = 33;
const int C13 = 34;
const int C14 = 35;
const int C15 = 36;
const int C16 = 37;


int longitud;
byte dato[20];
unsigned int salida = 0; 
unsigned int salidaAnterior;

unsigned int tiempo1;
unsigned int tiempo2;
// unsigned int tiempo[16];
   
//Variables relacionadas con MIDI
// Instrumento que sonará
  int instrument = 32;
  
// Duración máxima de las notas
  unsigned long timeNoteOff = 7000;
  
// Escala de notas
  byte notas[10] = {67-12,76-12,81-12,84-12,86-12,67+12,76+12,81+12,84+12,86+12};
  
  //byte notas[5] = {67,76,81,84,86};
  //byte notas[5] = {67,69,71,73,75};



// Variables auxiliares para cada baldosa (canal)
// Nota que se está tocando. 0 si el canal está apagado
  byte tileNote [16];
// momento de apagado del canal en milisegundos desde la conexión
  unsigned long tileOffTime [16];
// Indicador de que la baldosa fue pisada
  bool tileSteped [16];

//Definición de variables e instancias
 // #include <SoftwareSerial.h>  //esta repetido
  SoftwareSerial VS1053_Serial (2, 3); //Soft TX on 3, we don't use RX in this code

  #define VS_RESET 8  // Tied to VS1053 Reset line
  #define VS_GPIO1 4  // Tied to VS1053 GPIO1



void setup() {
  
  
  // initialize the serial communication:
  Serial.begin(115200); // Monitorizar en pantalla  
  Serial.println ("Configurando entradas y salidas");
  
  Serial2.begin(9600);  //En mega Serial2 en pines 16 y 17
  
  //Entradas
  pinMode(tubo10, INPUT_PULLUP);
  pinMode(tubo11, INPUT_PULLUP);
  pinMode(tubo12, INPUT_PULLUP);
  pinMode(tubo13, INPUT_PULLUP);
  pinMode(tubo20, INPUT_PULLUP);
  pinMode(tubo21, INPUT_PULLUP);
  pinMode(tubo22, INPUT_PULLUP);
  pinMode(tubo23, INPUT_PULLUP);

  delay(2500); //Espero al test del suelo
  
  tiempo1 = millis();
  Serial.print("Tiempo en milis:");
  Serial.println(tiempo1);
 
  
  //Salidas
  
 // pinMode(18, OUTPUT);  //Alimentacion Bluetooth (0V - LOW)
 // pinMode(19, OUTPUT);  //Alimentacion Bluetooth (5V - HIGH) 
  pinMode(10, OUTPUT);
   
  pinMode(C1, OUTPUT); 
  pinMode(C2, OUTPUT);
  pinMode(C3, OUTPUT);
  pinMode(C4, OUTPUT);
  pinMode(C5, OUTPUT);
  pinMode(C6, OUTPUT);
  pinMode(C7, OUTPUT);
  pinMode(C8, OUTPUT);
  pinMode(C9, OUTPUT);
  pinMode(C10, OUTPUT);
  pinMode(C11, OUTPUT);
  pinMode(C12, OUTPUT);
  pinMode(C13, OUTPUT);
  pinMode(C14, OUTPUT);
  pinMode(C15, OUTPUT);
  pinMode(C16, OUTPUT);

  //Alimento Bluetooth con pin 10(HIGH) y GND
  digitalWrite(10, HIGH);


 
  //Test de leds placa techo
  Serial.println ("Test de los LED en placa de techo");
  for(int i=0; i<16; i++){
     digitalWrite(i+22, HIGH);
     envia_datos (1<<i);

     
     delay(300);
     digitalWrite(i+22, LOW); 
 
  }
  
//TEST inicial: Enciendo cada fila de la pista de baile
  Serial.println ("Test de la comunicación y los LED de suelo");
  /*
  Serial2.write(0x00);
  delay(1);
  Serial2.write(0x0F);
  delay(1500);
  
  Serial2.write(0x00);
  delay(1);
  Serial2.write(0xF0);
  delay(1500);
  
  Serial2.write(0x0F);
  delay(1);
  Serial2.write(0x00);
  delay(1500);
  
  Serial2.write(0xF0);
  delay(1);
  Serial2.write(0x00);
  delay(1500);

 */
  /*
  // Alternativo
  envia_datos (B0000000000001111);
  delay(1500);

  envia_datos (B0000000011110000);
  delay(1500);

  envia_datos (B0000111100000000);
  delay(1500);

  envia_datos (B1111000000000000);
  delay(1500);
  */

/*
  //mando por columnas
  Serial2.write(0x11);
  delay(1);
  Serial2.write(0x11);
  delay(1500);
  
  Serial2.write(0x22);
  delay(1);
  Serial2.write(0x22);
  delay(1500);
  
  Serial2.write(0x44);
  delay(1);
  Serial2.write(0x44);
  delay(1500);
  
  Serial2.write(0x88);
  delay(1);
  Serial2.write(0x88);
  delay(1500);
*/

  Serial.println ("Inicia el sistema MIDI");
  MIDIsetup(); //Inicialización MIDI
  
  
  Serial.println ("Comienza el ciclo de trabajo");

}

  boolean nuevaPisada = false;
  boolean nuevoRayo = false;
  
  // byte aux0, aux1;
       
  
  int sttubo10;
  int sttubo11;
  int sttubo12;
  int sttubo13;
  int sttubo20;
  int sttubo21;
  int sttubo22;
  int sttubo23;
  
  unsigned int entradas1;
  unsigned int entradas2;

  byte bytealto;  //Puse byte en vez de char
  byte bytebajo;  //Puse byte en lugar de char
  unsigned int tiempoON[16];
  int cuenta;
  unsigned long num_muestreos = 2000;  // Numero de muetreos de los tubos antes de revisar el puerto serie
  int tiempo_LED_on = 1500;
  
  unsigned int patron_pisadas, patron_pisadas_ant;


void loop() { 
  unsigned long muestreos = 0;
  unsigned long millis_anterior = millis();
  unsigned long micros_muestreo;

  // entradas1=0;  //Almaceno lectura de 4 tubos fila1
  // entradas2=0;  //Almaceno lectura de 4 tubos fila2
  
  salidaAnterior = salida;
  salida = 0; 
  
  nuevoRayo = false;
  nuevaPisada = false;

  // Muestrea los tubos GEIGER
  while (muestreos < num_muestreos)
  { 
    muestreos ++; 
    salida = muestreo_tubos ();    
    if(salida != salidaAnterior) 
    {        
      salidaAnterior = salida;
      break;
    }
  }

  micros_muestreo = (millis() - millis_anterior) *1000 / muestreos; 
  if (salida)
  {
    nuevoRayo = true;

        // bytealto = highByte(salida);
        // bytebajo = lowByte(salida);      
        //Envio palabra, dos caracteres,  al otro arduino a través de BT 
    envia_datos (salida);
    TileOn(salida);  //Activa la nota musical
  }

  
// lee el puerto serie 
  patron_pisadas = 0;
  unsigned int auxiliar = lee_dato (); 
  for (int i=0; i<16; i++)
  {
    bitWrite(patron_pisadas, i, bitRead(auxiliar, 15-i));
  }
  
  
  
  //nuevaPisada = true;
  

  if (patron_pisadas != patron_pisadas_ant) 
  {
    //Serial.println(); Serial.print ( "este es elpatrón de pisadas recibido: "); Serial.print (patron_pisadas,BIN);Serial.println();
    patron_pisadas_ant = patron_pisadas;
    nuevaPisada = true;
    //TileStep(patron_pisadas);
  }

  if (patron_pisadas) 
  {
    TileStep(patron_pisadas);
  }


  // Gestiona el encendido de los LED de la placa
  LEDs_placa (salida, 1500);
  
  if (nuevoRayo || nuevaPisada)  
  {
    //Serial.print (salida,BIN); Serial.print ("  -  ");
    //Serial.print (highByte(salida),BIN); Serial.print ("  -  ");
    //Serial.print (lowByte(salida),BIN); Serial.println ("  -  ");

    
    Serial.print (micros_muestreo); Serial.print (" micro_seg\t   "); ; Serial.print (patron_pisadas); Serial.print ("        ");
    imprime_datos (highByte(salida), lowByte(salida), highByte(patron_pisadas), lowByte(patron_pisadas));
  }
 
}

