#include <SoftwareSerial.h>
SoftwareSerial mySerial (2, 3); //Soft TX on 3, we don't use RX in this code

#define VS_RESET 8  //Tied to VS1053 Reset line
#define VS_GPIO1 4  // Tied to VS1053 GPIO1

int note = 0; //The MIDI note value to be played
int instrument = 0;
unsigned long proximaNota;
//byte notas[10] = {67-12,76-12,81-12,84-12,86-12,67,76,81,84,86};
byte notas[5] = {67,76,81,84,86};
int totalNotas;

boolean channelBusy[16];
int channelNote[16];
unsigned long channelToff[16];

int proximoInstrumento = 0;

unsigned long timeNoteOff = 750;     // DURACIÓN MÁXIMA DE LAS NOTAS

unsigned long tileOffTime [16];
byte tileNote [16];
bool tileSteped [16];
unsigned long miliSeconds;



void setup() {
  mySerial.begin(31250);
  Serial.begin(57600);

  instrument = 16;

  
  totalNotas = sizeof (notas);

  //Reset the VS1053
  pinMode(VS_RESET, OUTPUT);
  pinMode(VS_GPIO1, OUTPUT);
  digitalWrite (VS_GPIO1,HIGH);

  digitalWrite(VS_RESET, LOW);
  delayMicroseconds(1);  
  digitalWrite(VS_RESET, HIGH);

  delay(1000);

  //set channel volume to near max (127)
  talkMIDI(0xB0, 0x07, 120); //0xB0 is channel message
  delay (100);
  talkMIDI(0xB0, 0x00, 0x00); //Default bank GM1

  proximaNota = millis() + random (1000,2000);
  
  for(byte i=0; i<16 ; i++) {

    tileNote[i] = 0;
    talkMIDI(0xC0|i, instrument, 0); //Set instrument number. 0xC0 is a 1 data byte command
  }
  
}


void loop() {
  // put your main code here, to run repeatedly:
  
  unsigned int matrix;
  matrix = random (65536);
  //Serial.print ("esto es matrix: ");
  //Serial.println (matrix, BIN);
  
  TileOn(matrix);
  
  delay (500);
  
  matrix = random (65536);
  //Serial.print ("esto es matrix: ");
  //Serial.println (matrix, BIN);
  
  TileStep(matrix);
  
  delay (500);

}


void TileOn(unsigned int matrix){
  
  //tileNote[0] = notas[random (totalNotas)];
  //noteOn(0, tileNote [0], 127);  // usa el canal i
  
  delay (500);

  
  unsigned int test;
  miliSeconds = millis();
  
  //Serial.println (matrix, HEX);
  //Serial.println ();
  
  for (int i=0; i<16; i++)
  {
    // Comprueba si ya pasó el tiempo necesario para apagar la nota de la baldosa
    if (miliSeconds > tileOffTime [i] + timeNoteOff)
    {
      noteOff(i, tileNote [i], 60);  // usa el canal i
      tileNote[i] = 0;
      tileSteped [i] = false;
      //Serial.print ("desconectado canal: ");
      //Serial.println (i);
    }

    // Si el evento incluye esta baldosa lanza una nueva nota
    test = 1 << i;
    if (matrix & test)
    {
      tileOffTime [i]= miliSeconds;
      tileNote [i] = notas [random (totalNotas)];
      //TalkMIDI(0xB0 | i, 0x07, 40);     //0xB0 is channel message, set channel volume bajo (40)
      noteOn(i, tileNote [i], 30);  // usa el canal i
      
      /*
      Serial.print ("SUENA canal: ");
      Serial.print (i);
      Serial.print ("\t\t tON: ");
      Serial.print (tileOffTime [i]);
      Serial.print ("\t\t millis: ");
      Serial.print (miliSeconds);
      Serial.println ();
      */

    }
  }
  
   Serial.print (matrix);
  Serial.print ("\t");
  for (int i=0; i<16; i++)
  {
    if (tileNote[i] == 0) Serial.print (" --");
    else if (tileNote[i] != 0) {Serial.print (" "); Serial.print (tileNote[i]);}
    else if (tileSteped [i] == true) {Serial.print ("*"); Serial.print (tileNote[i]);}
    Serial.print ("   ");
  }
  Serial.println();
}



void TileStep(byte matrix1, byte matrix2){
  unsigned int matrix = matrix1*256  + matrix2;
  TileStep (matrix);
}

void TileStep(unsigned int matrix){
  unsigned int test;
  miliSeconds = millis();
  
  for (int i=0; i<16; i++)
  {
    // Comprueba si ya pasó el tiempo necesario para apagar la nota de la baldosa
    if (miliSeconds > tileOffTime [i] + timeNoteOff)
    {
      noteOff(i, tileNote [i], 60);  // usa el canal i
      // TalkMIDI(0xB0 | i, 0x78,0);     //0xB0 is channel message, APAGA EL CANAL i
      tileSteped [i] = false;
    }

    else if (!tileSteped [i])
    {
      // Si el evento incluye esta baldosa lanza la segunda nota
      test = 1 << i;
      if (matrix & test)
      {
        tileOffTime [i]= miliSeconds;
        noteOff(i, tileNote [i], 60);  // usa el canal i
        // TalkMIDI(0xB0 | i, 0x78,0);     //0xB0 is channel message, APAGA EL CANAL i

        tileNote [i] = tileNote [i] + 13; //Elige una nota una octava más alta

        //TalkMIDI(0xB0 | i, 0x07, 120);     //0xB0 is channel message, set channel volume ALTO (120)
        noteOn(i, tileNote [i], 127);  // usa el canal i
        tileSteped [i] = true;
        
      //Serial.print ("\tPOTENCIA canal: ");
      //Serial.print (i);
      //Serial.println ();

      }
    }
  }
  
  Serial.print ("\t");
  for (int i=0; i<16; i++)
  {
    if (tileNote[i] == 0) Serial.print (" --");
    else if (tileSteped[i] == false) {Serial.print (" "); Serial.print (tileNote[i]);}
    else if (tileSteped[i] == true) {Serial.print ("*"); Serial.print (tileNote[i]);}
    Serial.print ("   ");
  }
  Serial.print ("\t");
  Serial.print (matrix);
  Serial.println();
   
  
}


/*
void loop() {
  cleanChannels();
  
  if (millis() > proximaNota)
  {
    //note = random (30,40);

    note = notas[random(10)];
    talkMIDI(0xC0, instrument, 0); //Set instrument number. 0xC0 is a 1 data byte command

    //noteOn(0, note, 127);
    noteChannelOn (note, 127);

    Serial.print ("Reproduciendo nota ");
    Serial.print (note,DEC);
    Serial.print ("  con instrumento ");
    Serial.print (instrument, DEC);
    Serial.println ();

    proximaNota = millis() + random (200,1000);
    delay (75);
    //noteOff(0, note, 127);
    //proximoInstrumento ++;
    }

  /*if (proximoInstrumento >5)
  {
    instrument ++;
    if (instrument == 17) instrument = 97;
    if (instrument > 127) instrument = 0; 
    proximoInstrumento = 0;
  }
  


}
*/

void noteChannelOn (byte note, byte attack_velocity) 
{
  for (int channel = 0; channel<12; channel++)
  {
   if (!channelBusy[channel])
    {
      instrument = 14;
      // instrument = channel%6+10;
      talkMIDI(0xC0, instrument, 0); //Set instrument number. 0xC0 is a 1 data byte command
      noteOn(channel, note, 127);
      //noteOn(channel, note, attack_velocity);

      talkMIDI( (0xA0 | channel), note, 32);

      
      channelToff[channel] = millis() + random (1000,6000);
      channelBusy[channel] = true;
      channelNote[channel] = note;
     Serial.print ("canal  ");
      Serial.print (channel);
       Serial.print ("\tinstrumento ");
      Serial.print (instrument);
      Serial.print ("\t >> ");
      break;
    }
  }
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
  mySerial.write(cmd);
  mySerial.write(data1);

  //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes 
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  if( (cmd & 0xF0) <= 0xB0)
    mySerial.write(data2);
}



/*

void cleanChannels()
{
  unsigned long milliseconds = millis();
  
  for (int i = 0; i<12; i++)
  {
    if (channelBusy[i])
    {
      if (milliseconds > channelToff[i]) 
      {
        noteOff(i, channelNote[i], 127);
      }
      if (milliseconds > (channelToff[i]+200)) channelBusy[i] = false;
    }
  }
}

*/


