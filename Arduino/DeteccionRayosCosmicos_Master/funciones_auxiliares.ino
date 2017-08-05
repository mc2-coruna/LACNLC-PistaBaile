void imprime_datos (byte dato_0 ,byte dato_1 , byte Var1, byte Var2)
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
    for (int i=7; i>=4; i--) {prueba = 1 & (Var1>>i); Serial.print (prueba,BIN);}
    Serial.print (" ");
    for (int i=3; i>=0; i--) {prueba = 1 & (Var1>>i); Serial.print (prueba,BIN);}

  Serial.print (" - ");
    for (int i=7; i>=4; i--) {prueba = 1 & (Var2>>i); Serial.print (prueba,BIN);}
    Serial.print (" ");
    for (int i=3; i>=0; i--) {prueba = 1 & (Var2>>i); Serial.print (prueba,BIN);}

   Serial.println ();     
}


/***********************************************************************/
/***      unsigned int muestreo_tubos ()                          *******
 Muestrea el estado de la matriz de tubos Geiger, y calcula las 
     coincidencias
 El resultado es una palabra de 16 bit con 1 en las coincidencias
************************************************************************/
unsigned int muestreo_tubos ()
{
  unsigned int salida = 0;
  // byte entradas1 = 0;  //Almaceno lectura de 4 tubos fila1
  // byte entradas2 = 0;  //Almaceno lectura de 4 tubos fila2

    //Lectura del estado de los tubos
    sttubo10 = digitalRead(tubo10);
    sttubo11 = digitalRead(tubo11);
    sttubo12 = digitalRead(tubo12);
    sttubo13 = digitalRead(tubo13);
    sttubo20 = digitalRead(tubo20);
    sttubo21 = digitalRead(tubo21);
    sttubo22 = digitalRead(tubo22);
    sttubo23 = digitalRead(tubo23);

    entradas1 = 0;  //Almaceno lectura de 4 tubos fila1
    bitWrite(entradas1, 0, sttubo10);
    bitWrite(entradas1, 1, sttubo11);
    bitWrite(entradas1, 2, sttubo12);
    bitWrite(entradas1, 3, sttubo13);
    bitWrite(entradas1, 4, 1);  //Lo añado para facilitar lectura en pantalla y ...

    entradas2 = 0;  //Almaceno lectura de 4 tubos fila2
    bitWrite(entradas2, 0, sttubo20);
    bitWrite(entradas2, 1, sttubo21);
    bitWrite(entradas2, 2, sttubo22);
    bitWrite(entradas2, 3, sttubo23);
    bitWrite(entradas2, 4, 1);  //...así empieza por 1 y visualizo los 0
    
    //Miramos coincidencias de deteccion de rayo entre todos los tubos 
    //y generamos una palabra de salida - 16 bits(16 salidas)
  
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
  
  return salida;
}


/***********************************************************************/
/***      void envia_datos (unsigned int patron_envio)           *******
 Envía datos a través del puerto Serial2 del arduino MEGA
 Se envían dos veces una secuencia del byte alto y el byte bajo 
     de la palabra de 16 bits.
************************************************************************/
void envia_datos (unsigned int patron_envio)
{
  for (int i=0; i<4; i++)
  {
    Serial2.write(highByte(patron_envio));
    delay(1);
    Serial2.write(lowByte(patron_envio));
    delay (1);
  }
  delay(75);

}
  

/***********************************************************************/
/***      unsigned int lee_dato ()                **********************
 Lee datos en el puerto Serial2 del arduino MEGA
 Los datos corresponden a el byte bajo y el byte alto de una
     una palabra de 16 bits. Los datos se envían y reciben dos veces 
     para corregir errores de comunicación
************************************************************************/
unsigned int lee_dato ()
{
  unsigned int respuesta = 0;
    
  byte datos [8] = {0,0,0,0,0,0,0,0};
  
  Serial.print ("datos disponibles = "); Serial.println (Serial2.available());
   
  if (Serial2.available() >= 4)
  {
    Serial2.readBytes (datos, 4);
    respuesta = (datos[0] & datos[2]) *256 + (datos [1] & datos [3]);

    // Vacía el buffer del puerto serie
    while (Serial2.available())   // Limpia el buffer del puerto serie
    {
      Serial2.read();
      Serial.print (".");
    }
  }  
  return respuesta;
}



/***********************************************************************/
/***   void LEDs_placa (unsigned int patron_LED, int tiempo_LED_on)  ***
 Maneja el encendido y apagado de la matriz de LEDs de la placa de control
 
 patron_LED:    el patron de LEDs a encender
 tiempo_LED_on: tiempo hasta el apagado del LED (milisegundos)
************************************************************************/
void LEDs_placa (unsigned int patron_LED, int tiempo_LED_on)
{
  // Gestiona el encendido de los LED de la placa electrónica
  static unsigned long tiempo[16];
  unsigned long milis_ahora = millis();

  for (int i = 0; i < 16; i++) 
  {
    if(bitRead(patron_LED, i)) 
    {
      //Serial.print("Tiempo en milis salida distinta a la anterior:");
      //Serial.println(tiempo1);
      digitalWrite(i+22, HIGH);  //Enciendo leds techo
      tiempo[i] = milis_ahora;
    }
    
    //Sólo tengo encendidos los leds un tiempo, luego lo apago
    if ((milis_ahora - tiempo[i]) > tiempo_LED_on)  digitalWrite(i+22, LOW);
  }
}
