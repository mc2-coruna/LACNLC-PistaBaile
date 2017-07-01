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

int muestreo_tubos ()
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
  
  return salida;
}

void envia_datos (unsigned int patron_rayos)
{
      tiempo1 = millis();
      nuevoRayo = true;

      Serial2.write(highByte(patron_rayos));
      delay(1);
      Serial2.write(lowByte(patron_rayos));
      delay (1);
      Serial2.write(highByte(patron_rayos));
      delay(1);
     
      Serial2.write(lowByte(patron_rayos));
    
      delay(75);
      /*
      //repito el envio
      Serial2.write(bytealto); delay(1);
      Serial2.write(bytebajo); delay(75);
      */

     
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
  
  
unsigned int lee_dato ()
{
  unsigned int respuesta = 0;
    
  byte datos_leidos [4] = {0,0,0,0};
   
  if (Serial2.available())
  {
    longitud = Serial2.readBytes (datos_leidos, 2);
    respuesta = datos_leidos[0] *256 + datos_leidos [1];
  
    
    //nuevaPisada = true;
    
    /************
    if(dato[0]>0 || dato[1]>0)
    {
      TileStep(dato[0], dato[1]);
    }
    *////////////////

    while (Serial2.available())   // Limpia el buffer del puerto serie
    {
      Serial2.read();
      Serial.print (".");
    }

    Serial.print ("El dato cono!     "); Serial.println (respuesta);

  }
  
  return respuesta;
    
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
 }



void LEDs_placa (unsigned int salida, int tiempo_LED_on)
{
  // Gestiona el encendido de los LED de la placa electrónica
  static unsigned long tiempo[16];
  unsigned long milis_ahora = millis();

  for (int i = 0; i < 16; i++) 
  {
    if(bitRead(salida, i)) 
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
