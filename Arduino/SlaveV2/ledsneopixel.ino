void ResetPista(){
   
   //Inicializo  los neopixel en apagado
   for(int i=0; i<NUMPIXELS; i++){
      pixels_1.setPixelColor(i, pixels_1.Color(0,0,0));
      pixels_2.setPixelColor(i, pixels_1.Color(0,0,0));
      pixels_3.setPixelColor(i, pixels_1.Color(0,0,0));
      pixels_4.setPixelColor(i, pixels_1.Color(0,0,0));
   }
   pixels_1.show();
   pixels_2.show();
   pixels_3.show();
   pixels_4.show();
}
void PruebaPista(){
  
   //Hago prueba de encendido directo  por filas desde el arduino micro
  for(int i=0; i<NUMPIXELS; i++){
      pixels_1.setPixelColor(i, pixels_1.Color(70,70,0));
      pixels_2.setPixelColor(i, pixels_1.Color(70,0,70));
      pixels_3.setPixelColor(i, pixels_1.Color(50,50,50));
      pixels_4.setPixelColor(i, pixels_1.Color(0,70,70));
   }
   pixels_1.show();
   delay(500);
   pixels_2.show();
   delay(500);
   pixels_3.show();
   delay(500);
   pixels_4.show();
   delay(500);
}

void iluminapista( ){

/*  int numLEDSbaldosa_1[4] = {32, 36, 32, 36};  //rombo, circulo, cuadrado,  triangulo
  int numLEDSbaldosa_2[4] = {36, 32, 32, 36};  //T, C, R , circulo
  int numLEDSbaldosa_3[4] = {36, 32, 36, 32};  //Cir, C, T, R
  int numLEDSbaldosa_4[4] = {32, 36, 36, 32};  //C, T, Cir, R
  
  int factor_1[4] = {0, 36, 68, 104};  //1 y 3 empiezan al revés
  int factor_2[4] = {0, 36, 68, 100};
  int factor_3[4] = {0, 32, 68, 100}; //
  int factor_4[4] = {0, 32, 68, 104}; 
*/

  int numLEDSbaldosa_1[4] = {32, 36, 36, 32};  //C, T, circulo, Rombo
  int numLEDSbaldosa_2[4] = {36, 32, 36, 32};  //T, C, R , circulo
  int numLEDSbaldosa_3[4] = {36, 32, 32, 36};  //Cir, C, T, R
  int numLEDSbaldosa_4[4] = {32, 36, 32, 36};  //C, T, Cir, R
  
  int factor_1[4] = {0, 32, 68, 104};  //1 y 3 empiezan al revés
  int factor_2[4] = {0, 32, 68, 100};
  int factor_3[4] = {0, 36, 68, 100}; //
  int factor_4[4] = {0, 36, 68, 104}; 



  int k;
  byte red1[4], red2[4], red3[4], red4[4];
  byte green1[4], green2[4], green3[4], green4[4];
  byte blue1[4], blue2[4], blue3[4], blue4[4];

  
  
/*  Serial.print("Fila1: ");
   Serial.println(fila1, BIN);
   Serial.print("Fila2: ");
   Serial.print(fila2, BIN);
   Serial.println("Fila3: ");
   Serial.println(fila3, BIN);
   Serial.print("Fila4: ");
   Serial.print(fila4, BIN); */

// For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
// Con cada lectura cambio el color

for(int i = 0; i < 4; i++){   
    
      switch(color1[i]){
        case 1: //rojo
        {
          red1[i]   = Qluz1[i];
          green1[i] = 0;
          blue1[i]  = 0;
        }
        break;
        case 2: //verde
        {
          red1[i]   = 0;
          green1[i] = Qluz1[i];
          blue1[i]  = 0;
        }
        break;
        case 3: //azul
        {
          red1[i]   = 0;
          green1[i] = 0;
          blue1[i]  = Qluz1[i];
        }
        break;
        case 4: //rojo + verde
        {
          red1[i]   = Qluz1[i];
          green1[i] = Qluz1[i];
          blue1[i]  = 0;
        }
        break;

         case 5: //verde+azul
        {
          red1[i]   = 0;
          green1[i] = Qluz1[i];
          blue1[i]  = Qluz1[i];
        }
        break;

        case 6: //rojo+azul
        {
          red1[i]   = Qluz1[i];
          green1[i] = 0;
          blue1[i]  = Qluz1[i];
        }
        break;
        
        default://de momento en blanco enciendo
        {
          red1[i]   = 0;
          green1[i] = 0;
          blue1[i]  = 0;
        }
        
      }

      switch(color2[i]){
        case 1: //Rojo
        {
          red2[i]   = Qluz2[i];
          green2[i] = 0;
          blue2[i]  = 0;
        }
        break;
        case 2: //verde
        {
          red2[i]   = 0;
          green2[i] = Qluz2[i];
          blue2[i]  = 0;
        }
        break;
        case 3: //azul
        {
          red2[i]   = 0;
          green2[i] = 0;
          blue2[i]  = Qluz2[i];
        }
        break;
        case 4: //rojo + verde
        {
          red2[i]   = Qluz2[i];
          green2[i] = Qluz2[i];
          blue2[i]  = 0;
        }
        break;

         case 5: //verde+azul
        {
          red2[i]   = 0;
          green2[i] = Qluz2[i];
          blue2[i]  = Qluz2[i];
        }
        break;

        case 6: //rojo+azul
        {
          red2[i]   = Qluz2[i];
          green2[i] = 0;
          blue2[i]  = Qluz2[i];
        }
        break;
        
        default://de momento en blanco enciendo
        {
          red2[i]   = 0;
          green2[i] = 0;
          blue2[i]  = 0;
        }
        
      }

      switch(color3[i]){
        case 1: //Rojo
        {
          red3[i]   = Qluz3[i];
          green3[i] = 0;
          blue3[i]  = 0;
        }
        break;
        case 2: //verde
        {
          red3[i]   = 0;
          green3[i] = Qluz3[i];
          blue3[i]  = 0;
        }
        break;
        case 3: //azul
        {
          red3[i]   = 0;
          green3[i] = 0;
          blue3[i]  = Qluz3[i];
        }
        break;
        case 4: //rojo + verde
        {
          red3[i]   = Qluz3[i];
          green3[i] = Qluz3[i];
          blue3[i]  = 0;
        }
        break;

         case 5: //verde+azul
        {
          red3[i]   = 0;
          green3[i] = Qluz3[i];
          blue3[i]  = Qluz3[i];
        }
        break;

        case 6: //rojo+azul
        {
          red3[i]   = Qluz3[i];
          green3[i] = 0;
          blue3[i]  = Qluz3[i];
        }
        break;
        
        default://de momento en blanco enciendo
        {
          red3[i]   = 0;
          green3[i] = 0;
          blue3[i]  = 0;
        }
        
      }switch(color4[i]){
        case 1: //Rojo
        {
          red4[i]   = Qluz4[i];
          green4[i] = 0;
          blue4[i] = 0;
        }
        break;
        case 2: //verde
        {
          red4[i]   = 0;
          green4[i] = Qluz4[i];
          blue4[i]  = 0;
        }
        break;
        case 3: //azul
        {
          red4[i]   = 0;
          green4[i] = 0;
          blue4[i]  = Qluz4[i];
        }
        break;
        case 4: //rojo + verde
        {
          red4[i]   = Qluz4[i];
          green4[i] = Qluz4[i];
          blue4[i] = 0;
        }
        break;

         case 5: //verde+azul
        {
          red4[i]   = 0;
          green4[i] = Qluz4[i];
          blue4[i]  = Qluz4[i];
        }
        break;

        case 6: //rojo+azul
        {
          red4[i]   = Qluz4[i];
          green4[i] = 0;
          blue4[i]  = Qluz4[i];
        }
        break;
        
        default://de momento en blanco enciendo
        {
          red4[i]   = 0;
          green4[i] = 0;
          blue4[i]  = 0;
        }
        
      }
      
}

//FILA1 
  
  for(int i = 0, k=3 ; i < 4; i++, k--) {
    
    
    //Los enciendo con la cantidad de luz que tenga cada uno
       for(int j=0; j < numLEDSbaldosa_1[i]; j++){
          pixels_1.setPixelColor(factor_1[k]+j, pixels_1.Color(red1[i],green1[i],blue1[i]));
       }
    
  }

   pixels_1.show();

   
//FILA2
  
  for(int i = 0, k=3; i < 4; i++, k--) {
    
       for(int j=0; j < numLEDSbaldosa_2[i]; j++){
          pixels_2.setPixelColor(factor_2[k]+j, pixels_2.Color(red2[i],green2[i],blue2[i]));   
       } 
    }
    
  pixels_2.show();

  
//FILA3  
   
   for(int i = 0, k=3; i < 4; i++, k--) {
    
   
      //Enciendo si bit=1
      for(int j=0; j < numLEDSbaldosa_3[i]; j++){
          pixels_3.setPixelColor(factor_3[k]+j, pixels_3.Color(red3[i],green3[i],blue3[i]));
      }       
    }

  pixels_3.show();


//FILA4
   
   for(int i = 0, k=3; i < 4; i++, k--) {
    
    
      //Enciendo con cantidad de luz desde 100% a 0%
      for(int j=0; j < numLEDSbaldosa_4[i]; j++){
          pixels_4.setPixelColor(factor_4[k]+j, pixels_4.Color(red4[i],green4[i],blue4[i]));
      }       
    }
    
   pixels_4.show();
  
}
