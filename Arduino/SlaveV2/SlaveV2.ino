//Arduino micro
//pines  11 y 12 para comunicar con el HC05

#include <SoftwareSerial.h>
SoftwareSerial BTSlave(11,12); //Rx=11, Tx=12 

  
byte dato1 = 0;  
byte dato2 = 0;
byte dato3 = 0;
byte dato4 = 0;
byte Qluz1[4] = {0,0,0,0};
byte Qluz2[4] = {0,0,0,0};
byte Qluz3[4] = {0,0,0,0};
byte Qluz4[4] = {0,0,0,0};

byte color1[4] = {0,0,0,0};
byte color2[4] = {0,0,0,0};
byte color3[4] = {0,0,0,0};
byte color4[4] = {0,0,0,0};


boolean bitdimmer; //para dimmerizar las luces d baldosas
boolean bitlongitud = LOW;

byte dato[4] = {0,0,0,0}; 
int longitud = 0;
unsigned int tpoanterior;
unsigned int tpoactual;

long aleatorio = 0;  //para genenar COLOR, ahora secuencial


//Del convertidor parallel to serial
//pines para controlarlo
int latchPin = 8;
int dataPin = 9;
int clockPin = 7;

//Pongo a 1 valor inicial: es como no estar presionado el sensor
byte switchVar1 = 255;  
byte switchVar2 = 255; 

byte switchVar1_anterior = 255;
byte switchVar2_anterior = 255; 

byte Lswitch1 = 15;
byte Mswitch1 = 15;
byte Lswitch2 = 15;
byte Mswitch2 = 15;

byte Acierto1 = 0;
byte Acierto2 = 0;  //Bytes para enviar al techo y sonar MIDI

unsigned int patron_pisadas, patron_pisadas_ant;
unsigned int patron_rayos, patron_rayos_ant;


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Pin Arduino conectado a los  NeoPixels?

#define PIN_1             10
#define PIN_2             6
#define PIN_3             5
#define PIN_4             3


//Tendré 4 tiras de neopixel, una por fila con igual número de pixels cada una


#define NUMPIXELS  136

Adafruit_NeoPixel pixels_1 = Adafruit_NeoPixel(NUMPIXELS, PIN_1, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel pixels_2 = Adafruit_NeoPixel(NUMPIXELS, PIN_2, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel pixels_3 = Adafruit_NeoPixel(NUMPIXELS, PIN_3, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel pixels_4 = Adafruit_NeoPixel(NUMPIXELS, PIN_4, NEO_GRB + NEO_KHZ800);



void setup() {
    delay (5000);
    
    //define pin modes del parallel-serial
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT); 
    pinMode(dataPin, INPUT);

    BTSlave.begin(9600); //en UNO pines 11 y 12 para el bluetooth
    Serial.begin(115200);
    pixels_1.begin(); // This initializes the NeoPixel library.
    pixels_2.begin(); 
    pixels_3.begin();
    pixels_4.begin();
    ResetPista();
    PruebaPista(); //Prueba funcionamiento leds pista por filas
    ResetPista();

   tpoanterior=millis();
}

  unsigned long num_muestreos = 10;  // Numero de muetreos de los tubos antes de revisar el puerto serie
  unsigned long muestreos = 0;


void loop() {
  int k;
  boolean nuevaPisada = false;
  boolean nuevoRayo = false;

  //Lectura de los sensores de presion
  //si presiono obtengo un 0
  {
    //Pulse the latch pin:
    //set it to 1 to collect parallel data
    digitalWrite(latchPin,1);
    //set it to 1 to collect parallel data, wait
    delayMicroseconds(20);
    //set it to 0 to transmit data serially  
    digitalWrite(latchPin,0);

    //while the shift register is in serial mode
    //collect each shift register into a byte
    //the register attached to the chip comes in first 
    switchVar1 = shiftIn(dataPin, clockPin);
    switchVar2 = shiftIn(dataPin, clockPin);
  }
  
  // Reune las 16 pisadas con logica pisado = 1
  patron_pisadas = (switchVar1) + 256 * (switchVar2);
  patron_pisadas = ~ patron_pisadas;
  //patron_pisadas = patron_pisadas | ~switchVar2;
  /* Serial.print (" Esto es patron de pisadas: "); Serial.print (patron_pisadas,BIN); Serial.print (" -- "); 
    
    
   imprime_datos (switchVar1 ,switchVar2, lowByte(patron_pisadas), highByte(patron_pisadas));

    
    //Serial.print (" -- "); Serial.print (switchVar1,BIN);
    //Serial.print (" -- "); Serial.println (switchVar2,BIN);
    delay (10);
    */

  // Atiende al canal serie.
  // Espera una secuencia de 8 bytes, que contienen la repetición 
     // 4 veces de los dos bytes que representan el patrón de rayos
     // Busca coincidencias entre los envíos 1y2; y entre 3y4
     // Toma como validas cualesquiera de esas coincidencias.
  //int bytes_libres;
  
  patron_rayos = lee_dato ();
  
  //if ((patron_rayos) & (patron_rayos != patron_rayos_ant)) 
  if (patron_rayos)
  {
    nuevoRayo = true;
    patron_rayos_ant = patron_rayos;
    
      
    // Para mantener la compatibilidad con lo que hay
    dato[0] = highByte (patron_rayos);
    dato[1] = lowByte  (patron_rayos);
    // dato[0] = dato[0] & dato[2];
    // dato[1] = dato[1] & dato[3];
     
    dato1 = dato[1] & 0x0F; //4 bits menos significativos 
    dato2 = dato[1] & 0xF0; //4 bits más significativos 
        
    dato3 = dato[0] & 0x0F;
    dato4 = dato[0] & 0xF0;
        
        
    aleatorio = ++aleatorio;
    
    if(aleatorio > 6) aleatorio = 1;
        //Puse número secuencial, ya no es aleatorio
    
    longitud=0;
    
    bitlongitud = HIGH;
        //Si es uno ponemos luz al maximo(120 no 255) y el color secuencial que toque
    
    for(int k=0; k<4; k++) 
    {
      if(bitRead(dato1, k))   { Qluz1[k] = 120; color1[k] = aleatorio; }
      if(bitRead(dato2, k+4)) { Qluz2[k] = 120; color2[k] = aleatorio; }
      if(bitRead(dato3, k))   { Qluz3[k] = 120; color3[k] = aleatorio; }
      if(bitRead(dato4, k+4)) { Qluz4[k] = 120; color4[k] = aleatorio; }
    }
    
  }
     
  muestreos ++;
  //if (patron_pisadas != patron_pisadas_ant)
  if (muestreos >= num_muestreos)
  {
    patron_pisadas_ant = patron_pisadas;
    // envía el patron de pisadas
    
    envia_datos (patron_pisadas);
    
    Serial.print (" envie patron de pisadas: "); Serial.println (patron_pisadas,BIN);
  }


   
   //reading  y si son distintos los visualizo en el canal serie
   if((switchVar1 != switchVar1_anterior) || (switchVar2 != switchVar2_anterior)){
     /*
     Serial.print("Lectura Variable1switch ");
     Serial.println(switchVar1, BIN);
     Serial.print("Lectura Variable2switch ");
     Serial.println(switchVar2, BIN);
     Serial.println("-------------------");
     */
     
     nuevaPisada = true;


     // BTSlave.write(~switchVar1);
     // BTSlave.write(~switchVar2);

     switchVar1_anterior = switchVar1;
     switchVar2_anterior = switchVar2;    

     //Obtengo los datos de las 4 filas de mi pista    
     Mswitch2 = switchVar1 & 0x0F;
     Lswitch2 = switchVar1 & 0xF0;
     Lswitch2 = Lswitch2 >> 4;                                /////////// AQUIIIIIIIIII
     Mswitch1 = switchVar2 & 0x0F;
     Lswitch1 = switchVar2 & 0xF0; 
     Lswitch1 = Lswitch1 >> 4; 
     
     /*
     Serial.println(Lswitch1, BIN);
     Serial.println(Mswitch1, BIN);
     Serial.println("-------------------");
     Serial.println(Lswitch2, BIN);
     Serial.println(Mswitch2, BIN);
     Serial.println("-------------------");
     */
     
   }
   

   
   
   /*
   int bytes_libres = BTSlave.available();
//   if(BTSlave.available()) {
  Serial.print (bytes_libres);
  Serial.print ("  ***  ");
  
   //if(bytes_libres=2) 
   {
     memset(dato, 0, sizeof(dato)); //Borro los datos anteriores. Ojo. ahora borra 4 bytes
     longitud = BTSlave.readBytes(dato, 2); //Leo los 2 bytes dato[0]  y dato[1] y me da el nº(longitud leida) de bytes
     
     
       
     
     //Cuando tenga los 2 bytes construyo mi dato de 16 bits
     if(bytes_libres==2) 
     //if(longitud==2) 
     
     {
      longitud = BTSlave.readBytes(dato, 2);
        nuevoRayo = true;
        
        /*
        Serial.println(" ");
        //Serial.print("  ");
        //Serial.println(dato[1], BIN);
        Serial.print("Dato recibido: ");
        Serial.print(dato[0], BIN);
        Serial.print("\t ");
        Serial.print(dato[1], BIN);
        Serial.print("\t\t");
        Serial.print(bytes_libres);
        Serial.print("\t");
        Serial.println(pruebas_int, BIN);
        
    
        //BTSlave.write(dato[1]); //Envío byte de Confirmación de que llegaron los 2 bytes
    
        dato1 = dato[1] & 0x0F; //4 bits menos significativos 
        dato2 = dato[1] & 0xF0; //4 bits más significativos 
        
        dato3 = dato[0] & 0x0F;
        dato4 = dato[0] & 0xF0;
        
        
        aleatorio = ++aleatorio;
        if(aleatorio > 6) aleatorio = 1;
        //Puse número secuencial, ya no es aleatorio
        longitud=0;
        bitlongitud = HIGH;
        //Si es uno ponemos luz al maximo(120 no 255) y el color secuencial que toque
        for(int k=0; k<4; k++) {
           if(bitRead(dato1, k)) { Qluz1[k] = 120; color1[k] = aleatorio; }
           if(bitRead(dato2, k+4)) { Qluz2[k] = 120; color2[k] = aleatorio; }
           if(bitRead(dato3, k)) { Qluz3[k] = 120; color3[k] = aleatorio; }
           if(bitRead(dato4, k+4)) { Qluz4[k] = 120; color4[k] = aleatorio; }
         }
       
      }
       
 
   
   else {
     while (BTSlave.available())
     {
       BTSlave.read()  ;
       Serial.print (".");
     }
   }
   }
*/

   
   tpoactual=millis();

   //Cada x ms actualizamos estado de baldosas
   if((tpoactual-tpoanterior)>150){
      tpoanterior = tpoactual;
      bitdimmer = HIGH;
      
   }
   else bitdimmer = LOW;
   if(bitdimmer && bitlongitud) {

    //POner aqui para bajar Qluz
    //si mayor que 5 ir restando
    //si se pisa baldosa lo apago
    //genero palabra para mandar a placa midi
    
    Acierto1=0;
    Acierto2=0;
    
    for (int i=0, k=3; i<4; i++, k--) {
       
        if(bitRead(Lswitch1, k)) {  //Si no piso baldosa apago lentamente
           if(Qluz1[i] >= 5) Qluz1[i] = Qluz1[i]-5;
        }
        else {  //Si piso baldosa apago luz y mando acierto (un 1) al techo
          if (Qluz1[i]==0)  bitWrite(Acierto1, i, 0);  //Pisé una apagada 
          else              bitWrite(Acierto1, i, 1);  //Pisé una encendida: acierto
          Qluz1[i]=0;                                  //se ha pulsado entonces lo apago
        }


        if(bitRead(Mswitch1, k)) {
           if(Qluz2[i] >= 5){ Qluz2[i] = Qluz2[i]-5; }
        }
        else {
          if (Qluz2[i]==0) bitWrite(Acierto1, i+4, 0);  //Pisé una apagada  
          else             bitWrite(Acierto1, i+4, 1);  //Pisé una encendida: acierto
          Qluz2[i] = 0;
        }


        if(bitRead(Lswitch2, k)) {
           if(Qluz3[i] >= 5) Qluz3[i] = Qluz3[i]-5;
        }
        else {
          if (Qluz3[i]==0) bitWrite(Acierto2, i, 0);  //Pisé una apagada  
          else             bitWrite(Acierto2, i, 1);  //Pisé una encendida: acierto
          Qluz3[i]=0; //se ha pulsado entonces lo apago
        }


        if(bitRead(Mswitch2, k)) {
           if(Qluz4[i] >= 5){Qluz4[i] = Qluz4[i]-5; }
        }
        else{
          if (Qluz4[i]==0) bitWrite(Acierto2, i+4, 0);  //Pisé una apagada  
          else             bitWrite(Acierto2, i+4, 1);  //Pisé una encendida: acierto  
          Qluz4[i] = 0;
        }
       
    }
    
    
    if (Acierto1 != 0 || Acierto2 != 0){    
      // BTSlave.write(Acierto1);
      // BTSlave.write(Acierto2);
    
   
    
      /*
       Serial.println("...................................");
       Serial.print("Acierto1: ");
       Serial.print(Acierto1, BIN);
       Serial.print("  Acierto2: ");
       Serial.println(Acierto2, BIN);
       Serial.println("...................................");
       Serial.println("...................................");
       Serial.print("Acierto1: ");
       Serial.print(Acierto1, BIN);
       Serial.print("  Acierto2: ");
       Serial.println(Acierto2, BIN);
       Serial.println("...................................");
       
      */
    }   
    
    iluminapista(); //encendemos baldosas
      
   }
    Acierto1 = 0;
    Acierto2 = 0;
   
    

   
   if (nuevaPisada || nuevoRayo) 
   {
     Serial.print ("   free RAM = "); Serial.print (freeRam()); Serial.print (" - "); 
     
     for (int i=0; i<4; i++) {Serial.print (Qluz1[i]); Serial.print (" ");}
     for (int i=0; i<4; i++) {Serial.print (Qluz2[i]); Serial.print (" ");}
     for (int i=0; i<4; i++) {Serial.print (Qluz3[i]); Serial.print (" ");}
     for (int i=0; i<4; i++) {Serial.print (Qluz4[i]); Serial.print (" ");} Serial.print ("   -   ");
     imprime_datos (dato[0] ,dato[1] , switchVar1, switchVar2);
   }
   
}


