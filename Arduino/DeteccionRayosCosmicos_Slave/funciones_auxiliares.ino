void imprime_datos (byte dato_0 ,byte dato_1 , byte switchVar1, byte switchVar2)
{
  boolean prueba;

  // Impresión de datos alternativa
  Serial.print ("rayos   ");
    for (int i=7; i>=4; i--) {prueba = 1 & (dato_0<<i); Serial.print (prueba,BIN);}
    Serial.print (" ");
    for (int i=3; i>=0; i--) {prueba = 1 & (dato_0<<i); Serial.print (prueba,BIN);}
     
  Serial.print (" - ");
    for (int i=7; i>=4; i--) {prueba = 1 & (dato_1>>i); Serial.print (prueba,BIN);}
    Serial.print (" ");
    for (int i=3; i>=0; i--) {prueba = 1 & (dato_1>>i); Serial.print (prueba,BIN);}
     
  Serial.print (" --- ");
  Serial.print ("pisadas ");
    for (int i=7; i>=4; i--) {prueba = 1 & (switchVar1>>i); Serial.print (prueba,BIN);}
    Serial.print (" ");
    for (int i=3; i>=0; i--) {prueba = 1 & (switchVar1>>i); Serial.print (prueba,BIN);}

  Serial.print (" - ");
    for (int i=7; i>=4; i--) {prueba = 1 & (switchVar2>>i); Serial.print (prueba,BIN);}
    Serial.print (" ");
    for (int i=3; i>=0; i--) {prueba = 1 & (switchVar2>>i); Serial.print (prueba,BIN);}

   Serial.println ();     
}


/***********************************************************************/
/***      void envia_datos (unsigned int patron_envio)           *******
 Envía datos a través del puerto software serial BTSlave del arduino micro
 Se envían dos veces una secuencia del byte alto y el byte bajo 
     de la palabra de 16 bits.
************************************************************************/
void envia_datos (unsigned int patron_envio)
{
  for (int i=0; i<4; i++)
  {
    BTSlave.write(highByte(patron_envio));
    delay(1);
    BTSlave.write(lowByte(patron_envio));
    delay (1);
  }
  delay(75);
}



/***********************************************************************/
/***      unsigned int lee_dato ()                **********************
 Lee datos en el puerto software serial BTSlave del arduino micro
 Los datos corresponden a el byte bajo y el byte alto de una
     una palabra de 16 bits. Los datos se envían y reciben dos veces 
     para corregir errores de comunicación
************************************************************************/
unsigned int lee_dato ()
{
  unsigned int respuesta = 0;
    
  byte datos [8] = {0,0,0,0,0,0,0,0};
   
  if (BTSlave.available() >= 4)
  {
    BTSlave.readBytes (datos, 4);
    respuesta = (datos[0] & datos[2]) *256 + (datos [1] & datos [3]);

    // Vacía el buffer del puerto serie
    while (BTSlave.available())   // Limpia el buffer del puerto serie
    {
      BTSlave.read();
      Serial.print (".");
    }
  }  
  return respuesta;
}



int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

// https://learn.adafruit.com/memories-of-an-arduino/optimizing-sram#f-those-strings

