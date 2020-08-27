#include <SoftwareSerial.h>
SoftwareSerial mySerial (2, 3); //Soft TX on 3, we don't use RX in this code

#define VS_RESET 8  //Tied to VS1053 Reset line
#define VS_GPIO1 4  // Tied to VS1053 GPIO1

int note = 0; //The MIDI note value to be played
int instrument = 0;
unsigned long proximaNota;
int notas[10] = {67 - 12, 76 - 12, 81 - 12, 84 - 12, 86 - 12, 67, 76, 81, 84, 86};
//int notas[5] = {67,76,81,84,86};

boolean channelBusy[16];
int channelNote[16];
unsigned long channelToff[16];

int proximoInstrumento = 0;

void setup() {
  mySerial.begin(31250);
  Serial.begin(57600);

  //Reset the VS1053
  pinMode(VS_RESET, OUTPUT);
  pinMode(VS_GPIO1, OUTPUT);
  digitalWrite (VS_GPIO1, HIGH);

  digitalWrite(VS_RESET, LOW);
  delayMicroseconds(1);
  digitalWrite(VS_RESET, HIGH);

  delay(1000);

  //set channel volume to near max (127)
  talkMIDI(0xB0, 0x07, 80); //0xB0 is channel message
  delay (100);
  // talkMIDI(0xB0, 0x00, 0x00); //Default bank GM1
  talkMIDI(0xB0, 0x00, 0x7f); //Default bank GM1

  proximaNota = millis() + random (1000, 2000);
  instrument = 8;

  Serial.println ();
  Serial.println ("  **  Iniciando MIDI rt MODE  **");
  Serial.println ();

  int instrument = 53;
  /*
  talkMIDI(0xC0, instrument, 0); //Set instrument number. 0xC0 is a 1 data byte command
  //Play fancy sounds from 'High Q' to 'Open Surdo [EXC 6]'
  for (note = 27 ; note < 87 ; note= note + 1) {
    Serial.print("N:");
    Serial.println(note, DEC);

    //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
    noteOn(0, note, 60);
    delay(500);

    //Turn off the note with a given off/release velocity
    noteOff(0, note, 60);
    delay(500);
  }
  */
  
  for (instrument = 0 ; instrument < 127 ; instrument++) {
    talkMIDI(0xC0, instrument, 0); //Set instrument number. 0xC0 is a 1 data byte command
    Serial.print("Instrument:");
    Serial.println(instrument, DEC);

    //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
    noteOn(0, 62, 60);
    delay(500);

    //Turn off the note with a given off/release velocity
    noteOff(0, 62, 60);
    delay(500);
  }

  
  delay (1000);

}

void loop() {
  rayGenerator();
}


void rayGenerator()
{
  static unsigned long nextRayTime = 0;
  static unsigned long rayOffTime  = 0;
  unsigned long millisec = millis();
  static boolean rayIsOn = false;

  boolean OkButtonOn   = digitalRead(10);
  boolean noOkButtonOn = digitalRead(11);

  if ((millisec > nextRayTime)) {
    rayON();
    nextRayTime = nextRaymillis();
    rayOffTime  = millisec + 1000;
    rayIsOn = true;
  }

  if (millisec < rayOffTime) {
    if (OkButtonOn) {
      // OkButton();   rayOffTime = 0;
    }
    if (noOkButtonOn) {
      // noOkButton(); rayOffTime = 0;
    }
  }

  if ((millisec > rayOffTime) and rayIsOn)
  {
    rayOff();
    rayIsOn = false;
  }
}

void rayON()
{
  int ledPin = 13;
  digitalWrite (ledPin, HIGH);
  talkMIDI(0xC0, 128, 0);
  noteOn(0, 67, 127);
  Serial.println ("ray ON");
}

void rayOff()
{
  int ledPin = 13;
  digitalWrite (ledPin, LOW);
  talkMIDI(0xC0, 94, 0);
  noteOn(0, 79, 127);
  Serial.println ("      ** ray OFF");
}

void OkButton()
{
  int ledPin = 13;
  digitalWrite (ledPin, LOW);
  talkMIDI(0xC0, 127, 0);
  noteOn(0, 55, 127);
  delay (25);
  noteOn(0, 59, 127);
  delay (25);
  noteOn(0, 63, 127);
  delay (25);
  Serial.println ("Button OK");
}

void noOkButton()
{
  int ledPin = 13;
  digitalWrite (ledPin, LOW);
  talkMIDI(0xC0, 1, 0);
  noteOn(0, 40, 127);
  Serial.println ("Button noOK");
}


unsigned long nextRaymillis()
{
  return millis() + random (2000, 4000);
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
  if ( (cmd & 0xF0) <= 0xB0)
    mySerial.write(data2);
}


/*

  //Demo Basic MIDI instruments, GM1
  //=================================================================
  Serial.println("Basic Instruments");
  talkMIDI(0xB0, 0x00, 0x79); //Default bank GM1

  //Change to different instrument
  for(instrument = 0 ; instrument < 127 ; instrument++) {

    Serial.print(" Instrument: ");
    Serial.println(instrument, DEC);

    talkMIDI(0xC0, instrument, 0); //Set instrument number. 0xC0 is a 1 data byte command

    //Play notes from F#-0 (30) to F#-5 (90):
    for (note = 30 ; note < 40 ; note++) {
      Serial.print("N:");
      Serial.println(note, DEC);

      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      noteOn(0, note, 127);
      delay(200);

      //Turn off the note with a given off/release velocity
      noteOff(0, note, 127);
      delay(50);
    }

    delay(100); //Delay between instruments
  }

*/
//=================================================================
/*
  //Demo GM2 / Fancy sounds
  //=================================================================
  Serial.println("Demo Fancy Sounds");
  talkMIDI(0xB0, 0, 0x78); //Bank select drums

  //For this bank 0x78, the instrument does not matter, only the note
  for(instrument = 30 ; instrument < 31 ; instrument++) {

    Serial.print(" Instrument: ");
    Serial.println(instrument, DEC);

    talkMIDI(0xC0, byte(instrument), 0); //Set instrument number. 0xC0 is a 1 data byte command

    //Play fancy sounds from 'High Q' to 'Open Surdo [EXC 6]'
    for (note = 27 ; note < 87 ; note++) {
      Serial.print("N:");
      Serial.println(note, DEC);

      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      noteOn(0, note, 60);
      delay(50);

      //Turn off the note with a given off/release velocity
      noteOff(0, note, 60);
      delay(50);
    }

    delay(100); //Delay between instruments
  }

  //Demo Melodic
  //=================================================================
  Serial.println("Demo Melodic? Sounds");
  talkMIDI(0xB0, 0, 0x79); //Bank select Melodic
  //These don't sound different from the main bank to me

  //Change to different instrument
  for(instrument = 27 ; instrument < 87 ; instrument++) {

    Serial.print(" Instrument: ");
    Serial.println(instrument, DEC);

    talkMIDI(0xC0, instrument, 0); //Set instrument number. 0xC0 is a 1 data byte command

    //Play notes from F#-0 (30) to F#-5 (90):
    for (note = 30 ; note < 40 ; note++) {
      Serial.print("N:");
      Serial.println(note, DEC);

      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      noteOn(0, note, 60);
      delay(50);

      //Turn off the note with a given off/release velocity
      noteOff(0, note, 60);
      delay(50);
    }

    delay(100); //Delay between instruments
  }
*/



