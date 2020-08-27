// Parámetros del programa

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

//Definiciónde variables e instancias
  #include <SoftwareSerial.h>
  SoftwareSerial VS1053_Serial (2, 3); //Soft TX on 3, we don't use RX in this code

  #define VS_RESET 8  // Tied to VS1053 Reset line
  #define VS_GPIO1 4  // Tied to VS1053 GPIO1



void setup() {
  VS1053_Serial.begin(31250);  // Abre el canal de comunicación con el VS1053
  
  Serial.begin(57600);         // Abre la comunicación serie de la tarjeta

  // Inicializa el VS-1053 en modo Real Time MIDI
  pinMode(VS_RESET, OUTPUT);
  pinMode(VS_GPIO1, OUTPUT);
  digitalWrite (VS_GPIO1,HIGH);

  digitalWrite(VS_RESET, LOW);
  delayMicroseconds(1);  
  digitalWrite(VS_RESET, HIGH);

  delay(100);

  // Inicializa los canales del generador MIDI
  talkMIDI(0xB0, 0x00, 0x00);          // Set default bank GM1

  // Establece los valores por defecto de los canales
  // Canal 1 para rayos detectados; Canal 2 para baldosas pisadas correctamente
  talkMIDI(0xB0, 0x78,0);            // Set channel 1 all sound off 0xB0
  talkMIDI(0xB1, 0x78,0);            // Set channel 2 all sound off 0xB0
  
  talkMIDI(0xB0, 0x07, 20);          // Set channel 1 volume to low value (20)
  talkMIDI(0xB1, 0x07, 120);         // Set channel 2 volume to near max (127)

  talkMIDI(0xC0, instrument, 0);     // Set channel 1 instrument
  talkMIDI(0xC1, instrument, 0);     // Set channel 2 instrument
  
  for(byte i=0; i<16 ; i++) 
  {
    tileNote[i] = 0;                 // Set the tile note to 0
  }  
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned int matrix;
  
  matrix = random (65536);
  
  matrix = 1<<random(16) | 1<<random(16) | 1<<random(16) ;
  TileOn(matrix);
  delay (400 + random(1200));
  
  matrix = random (65536);

  matrix = 1<<random(16) | 1<<random(16) | 1<<random(16) ;

  TileStep(matrix);
  delay (300+ random(600));
}


void TileOn(unsigned int matrix){
  
  unsigned long miliSeconds = millis();
  
  for (int i=0; i<16; i++)
  {
    // Comprueba si ya pasó el tiempo necesario para apagar la nota de la baldosa
    if ((tileNote[i] > 0) && (miliSeconds > tileOffTime [i] + timeNoteOff))
    {
      //                       Devuelve la baldosa a sus valores iniciales
      tileNote[i] = 0;
      tileSteped [i] = false;
    }

    // Si el evento incluye esta baldosa lanza una nueva nota
    if (1 & (matrix>>i))
    {
      tileOffTime [i]= miliSeconds;
      tileNote [i] = notas [random (sizeof (notas))];
      tileSteped [i] = false;
      noteOn(0, tileNote [i], 120);          // Activa la nota en el canal 1
    }
  }
  
  for (int i=0; i<16; i++)
  {
    boolean prueba;
    prueba = 1 & (matrix>>i);
    Serial.print (prueba,BIN);    
  }
  
  Serial.print ("\t");
  for (int i=0; i<16; i++)
  {
    if (tileNote[i] == 0) Serial.print (" --");
    else if (tileSteped[i] == false) {Serial.print (" "); Serial.print (tileNote[i]);}
    else if (tileSteped[i] == true) {Serial.print ("*"); Serial.print (tileNote[i]);}
    Serial.print ("   ");
  }
  Serial.println();
}



void TileStep(byte matrix1, byte matrix2){
  //unsigned int matrix = matrix1*256  + matrix2;
  
  unsigned int matrix = matrix1;
  matrix = matrix<<8 | matrix2;
  
  TileStep (matrix);
}

void TileStep(unsigned int matrix){

  unsigned long miliSeconds = millis();
  
  for (int i=0; i<16; i++)
  {
    // Comprueba si ya pasó el tiempo necesario para apagar la nota de la baldosa
    if (tileNote[i] > 0)
    {
      if(miliSeconds > tileOffTime [i] + timeNoteOff)
      {
        //                       Devuelve la baldosa a sus valores iniciales
        tileNote[i] = 0;
        tileSteped [i] = false;
      }

      else if (!tileSteped [i])
      {
        // Si el evento incluye esta baldosa lanza la segunda nota
        if (matrix & (1<<i))
        {
          tileOffTime [i] = miliSeconds;
          tileNote [i] = tileNote [i] + 12;      //Elige una nota una octava más alta
          tileSteped [i] = true;
          noteOn(1, tileNote [i], 127);          // Activa la nota en el canal 2
        }
      }
    }
  }
  
  Serial.print ("\t\t\t");
  for (int i=0; i<16; i++)
  {
    if (tileNote[i] == 0) Serial.print (" --");
    else if (tileSteped[i] == false) {Serial.print (" "); Serial.print (tileNote[i]);}
    else if (tileSteped[i] == true) {Serial.print ("*"); Serial.print (tileNote[i]);}
    Serial.print ("   ");
  }
  Serial.print ("\t");
  
  for (int i=0; i<16; i++)
  {
    boolean prueba;
    prueba = 1 & (matrix>>i);
    Serial.print (prueba,BIN);    
  }
  Serial.println ();    
}



//Send a MIDI note-on message.  Like pressing a piano key
//channel ranges from 0-15
void noteOn(byte channel, byte note, byte attack_velocity) {
  talkMIDI( (0x90 | channel), note, attack_velocity);
}

//Send a MIDI note-off message.  Like releasing a piano key
void noteOff(byte channel, byte note, byte release_velocity) {
  talkMIDI( (0x80 | channel), note, release_velocity);
}

//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
void talkMIDI(byte cmd, byte data1, byte data2) {
  VS1053_Serial.write(cmd);
  VS1053_Serial.write(data1);

  //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes 
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  if( (cmd & 0xF0) <= 0xB0)
    VS1053_Serial.write(data2);
}
