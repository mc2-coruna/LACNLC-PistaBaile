                                  
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
unsigned int salidaanterior;

unsigned int tiempo1;
unsigned int tiempo2;
unsigned int tiempo[16];
   
//Variables relacionadas con MIDI
// Instrumento que sonará
  int instrument = 32;
  
// Duración máxima de las notas
  unsigned long timeNoteOff = 3000;
  
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
  for(int i=0; i<16; i++){
     digitalWrite(i+22, HIGH);
     delay(300);
     digitalWrite(i+22, LOW); 
 
  }
  
//TEST inicial: Enciendo cada fila de la pista de baile
 
 /* Serial2.write(0x00);
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
  
  
  MIDIsetup(); //Inicialización MIDI
}

void loop() { 

  unsigned int tiempoON[16];
  
  boolean nuevaPisada = false;
  boolean nuevoRayo = false;
  
  byte aux0, aux1;
       
  
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
 
  
  
  entradas1=0;  //Almaceno lectura de 4 tubos fila1
  entradas2=0;  //Almaceno lectura de 4 tubos fila2
  
  salidaanterior = salida;
 
  salida = 0; 
  
  //Lectura del estado de los tubos
  sttubo10 = digitalRead(tubo10);
  sttubo11 = digitalRead(tubo11);
  sttubo12 = digitalRead(tubo12);
  sttubo13 = digitalRead(tubo13);
  sttubo20 = digitalRead(tubo20);
  sttubo21 = digitalRead(tubo21);
  sttubo22 = digitalRead(tubo22);
  sttubo23 = digitalRead(tubo23);
  bitWrite(entradas1, 0, sttubo10);
  bitWrite(entradas1, 1, sttubo11);
  bitWrite(entradas1, 2, sttubo12);
  bitWrite(entradas1, 3, sttubo13);
  bitWrite(entradas1, 4, 1);  //Lo añado para facilitar lectura en pantalla y ...
  bitWrite(entradas2, 0, sttubo20);
  bitWrite(entradas2, 1, sttubo21);
  bitWrite(entradas2, 2, sttubo22);
  bitWrite(entradas2, 3, sttubo23);
  bitWrite(entradas2, 4, 1);  //...así empieza por 1 y visualizo los 0

 
 
  //Miramos coincidencias de deteccion de rayo entre todos los tubos 
  //y generamos una palabra de salida - 16 bits(16 salidas)
  
  /*if (sttubo10 ) { bitWrite(salida, 15, 1); Serial.println(salida, BIN); delay(1000); bitWrite(salida, 14, 1); 
  Serial.print("Salida: "); Serial.println(salida, BIN);
  }*/
  
  
  if (!sttubo10 && !sttubo20)  bitWrite(salida, 0, 1);
  if (!sttubo10 && !sttubo21)  bitWrite(salida, 1, 1);
  if (!sttubo10 && !sttubo22)  bitWrite(salida, 2, 1);
  if (!sttubo10 && !sttubo23)  bitWrite(salida, 3, 1);
  if (!sttubo11 && !sttubo20)  bitWrite(salida, 4, 1);
  if (!sttubo11 && !sttubo21)  bitWrite(salida, 5, 1);
  if (!sttubo11 && !sttubo22)  bitWrite(salida, 6, 1);
  if (!sttubo11 && !sttubo23)  bitWrite(salida, 7, 1);
  if (!sttubo12 && !sttubo20)  bitWrite(salida, 8, 1);
  if (!sttubo12 && !sttubo21)  bitWrite(salida, 9, 1);
  if (!sttubo12 && !sttubo22)  bitWrite(salida, 10, 1);
  if (!sttubo12 && !sttubo23)  bitWrite(salida, 11, 1);
  if (!sttubo13 && !sttubo20)  bitWrite(salida, 12, 1);
  if (!sttubo13 && !sttubo21)  bitWrite(salida, 13, 1);
  if (!sttubo13 && !sttubo22)  bitWrite(salida, 14, 1);
  if (!sttubo13 && !sttubo23)  bitWrite(salida, 15, 1);
  
  /*Serial.println(salida, BIN);
  Serial.println(entradas1, BIN);
     
  Serial.println(entradas2, BIN);
  delay(1000);
  */


 
 
  
  if(salida!=salidaanterior) {
    
    bytealto = highByte(salida);
    bytebajo = lowByte(salida);
    
    //Envio palabra, dos caracteres,  al otro arduino a través de BT
   
    if (salida != 0)
    {
      tiempo1 = millis();
      nuevoRayo = true;

      Serial2.write(bytealto);
      delay(1);
     
      Serial2.write(bytebajo);
      delay(75);
      /*
      //repito el envio
      Serial2.write(bytealto); delay(1);
      Serial2.write(bytebajo); delay(75);
      */

      for(int j = 0; j < 16; j++) 
      {
        if(bitRead(salida, j)) 
        {
          //Serial.print("Tiempo en milis salida distinta a la anterior:");
          //Serial.println(tiempo1);
          digitalWrite(j+22, HIGH);  //Enciendo leds techo
          tiempo[j] = tiempo1;
        }
      }
     
     /*
      Serial.print("byte alto-byte bajo: ");
      Serial.print(bytealto,BIN);
      Serial.print("-");
      Serial.println(bytebajo,BIN);
      
      Serial.print("Salida-16 bits: ");
      Serial.println(salida, BIN);
      Serial.print("Tiempo en milis salida:");
      Serial.println(tiempo1);
      //Enciendo los leds  donde el bit de la palabra salidas esté a 1. Leo los 16 bits
      //Leds están desde salida 38 a 53. Los enciendo o apago
     */

    }

    if (salida) TileOn(salida);  //Nota musical
    
    // delay(100);  //Tenia 350 antes del midi
     
    /*
    Serial.print("Entradas1: ");
    Serial.println(entradas1, BIN);
    Serial.print("Entradas2: ");
    Serial.println(entradas2, BIN);
    Serial.println("");
    */
     
    delay(1);
  }  

   
   
   
  //if (Serial2.available())
  //{
  longitud = Serial2.readBytes (dato, 2);
  if (longitud == 2)
  {
    nuevaPisada = true;
    Serial.println ("El dato cono!");

    if(dato[0]>0 || dato[1]>0)
    {
      TileStep(dato[0], dato[1]);
    }
   
    while (Serial2.available())
    {
      Serial2.read();
      Serial.print (".");
    }
  }
    
  /*
  }
  while( Serial2.available())
  {
    dato[longitud]=Serial2.read();
    longitud++;
    if (longitud>4) longitud=0;
  }
  /* //leer los dos char y llamar a TileStep //Envio al MIDIel caracter recibido por puerto serie
  dato[0] = 0;
  dato[1] = 0;
  longitud = Serial2.readBytes(dato, 2); //Leo los 2 bytes dato[0]  y dato[1] y me da el nº(longitud leida) de bytes

  //Cuando tenga los 2 bytes construyo mi dato de 16 bits
  if( longitud >= 2) 
  {
    longitud=0;

        /*
        Serial.print ("datos recibidos   ");
        
        Serial.print(dato[1], BIN);
        Serial.print("  ");
        Serial.println(dato[0], BIN);
        //llamo a TileStep : pisada correcta. Si son 0 no llamo a la función
        
        
        if(dato[0]>0 || dato[1]>0){
          
          
             
           TileStep(dato[0], dato[1]);
        } 
      
      }
          
    */
   
    
  //Sólo tengo encendidos los leds un tiempo, luego lo apago
  //Cambiar el valor de tiempo de encendido aquí
  tiempo2=millis();
  for(int i = 0; i < 16; i++)
  {
    tiempoON[i] = tiempo2-tiempo[i];
    if(tiempoON[i] > 1500)  digitalWrite(i+22, LOW);
  }
  

  if (nuevaPisada) 
  {
      /*
      Serial.print ("dato 0 ="); Serial.println (dato[0]);
      Serial.print ("dato 1 ="); Serial.println (dato[1]);
      Serial.print ("dato 2 ="); Serial.println (dato[3]);
      Serial.print ("dato 3 ="); Serial.println (dato[4]);
      */
    for (int i=0; i<8; i++)
    {
      bitWrite(aux0, i, bitRead (dato[0], 7-i));
      bitWrite(aux1, i, bitRead (dato[1], 7-i));            
    }
  }
      
      
  if ((salida != 0) || !dato[0] || !dato[1])
  {
      // imprime_datos (bytealto, bytebajo, dato[0], dato[1]);
    imprime_datos (bytealto, bytebajo, aux0, aux1);
  }
}


void imprime_datos (byte dato_0 ,byte dato_1 , byte switchVar1, byte switchVar2)
{
  boolean prueba;

  // Impresión de datos alternativa
  Serial.print ("rayos   ");
    for (int i=0; i<4; i++) {prueba = 1 & (dato_0>>i); Serial.print (prueba,BIN);}
    Serial.print (" ");
    for (int i=4; i<8; i++) {prueba = 1 & (dato_0>>i); Serial.print (prueba,BIN);}
     
  Serial.print (" - ");
    for (int i=0; i<4; i++) {prueba = 1 & (dato_1>>i); Serial.print (prueba,BIN);}
    Serial.print (" ");
    for (int i=4; i<8; i++) {prueba = 1 & (dato_1>>i); Serial.print (prueba,BIN);}
     
  Serial.print (" --- ");
  Serial.print ("pisadas ");
    for (int i=0; i<4; i++) {prueba = 1 & (switchVar1>>i); Serial.print (prueba,BIN);}
    Serial.print (" ");
    for (int i=4; i<8; i++) {prueba = 1 & (switchVar1>>i); Serial.print (prueba,BIN);}

  Serial.print (" - ");
    for (int i=0; i<4; i++) {prueba = 1 & (switchVar2>>i); Serial.print (prueba,BIN);}
    Serial.print (" ");
    for (int i=4; i<8; i++) {prueba = 1 & (switchVar2>>i); Serial.print (prueba,BIN);}

   Serial.println ();     
}



