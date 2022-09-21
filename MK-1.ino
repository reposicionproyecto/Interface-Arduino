//SISTEMA DE ALIMENTACION CONTROLADO REMOTAMENTE PARA GRANJA PISCICULTURA

  //#define DEBUG

  #include <String.h>

  #include <Adafruit_GFX.h> // Libreria para los graficos y actualización

  #include <Adafruit_TFTLCD.h> // Instalacion y actualizacion de la Libreria de LCD

  #include <SD.h> // Instalacion de la Libreria de tarjeta SD y actualizacion

  #include <SPI.h>  // Instalacion y Actualizacion de la Libreria bus SPI



  #include <avr/pgmspace.h>

  #include <TouchScreen.h>  // Libreria del panel tactil

  #include <EEPROM.h>

  #include <Time.h> // librerias para manejar hora y fecha

  #include <Wire.h>

  #include <DS1307RTC.h> // a basic DS1307 library that returns time as a time_t



  #include <SoftwareSerial.h> // LIBRERIA PARA EL GSM

  SoftwareSerial SIM900(22,23);



  #define LCD_CS A3 // Definimos los pines del LCD

  #define LCD_CD A2 // para poder visualizar elementos graficos

  #define LCD_WR A1

  #define LCD_RD A0

  #define LCD_RESET A4
 






  #if defined     AVR_ATmega2560  // Para Arduino Uno/Duemilanove, conectamos la tarjeta SD en los pines del puerto SPI
  #define SD_SCK 13 // que se corresponden con los pines MOSI -> 11, MISO -> 12 y SCK -> 13
  #define SD_MISO 12

  #define SD_MOSI 11

  #endif



  // Chip Select del bus SPI correspondiente a la conexion con la tarjeta SD

  #define SD_CS 10



  // En la tarjeta SD debemos colocar imagenes en formato BMP de 24 Bits!

  // Otro tipo de formato de imagen no se puede visualizar por pantalla.



  Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, A4); // Instancia LCD



  // Pines necesarios para los 4 pines del panel tactil

  #define YP A1 // Pin analogico A1 para ADC

  #define XM A2 // Pin analogico A2 para ADC

    ///°°°!!!!! EL PIN "A2 y A1" SE REPITEN EN LAS DEFINICIONES UTILIZAR OTROS PIN,,, COMO???
  #define YM 7

  #define XP 6
 






  // Definimos la presion máxima y minima que podemos realizar sobre el panel

  #define MINPRESSURE 1

  #define MAXPRESSURE 1000



  // Para mejor precision de la presion realizada, es necesario

  // medir la resistencia entre los pines X+ y X-.

  // En Shield TFT 2.4" LCD se mide entre los pines A2 y 6

  // Instancia del panel tactil (Pin XP, YP, XM, YM, Resistencia del panel)

  TouchScreen ts = TouchScreen(XP, YP, XM, YM, 346);// ESTABA 364



  short TS_MINX = 206; // 150 Coordenadas del panel tactil para delimitar

  short TS_MINY = 89; // 120 el tamaño de la zona donde podemos presionar

  short TS_MAXX = 910; //850 y que coincida con el tamaño del LCD

  short TS_MAXY = 950; //891



  #define
BL
ACK 0x0000 // Definimos los colores

  #define
BL
UE  0x001F // que utilizaremos para

  #define
RE
D 0xF800 // el texto y los elementos graficos
 




  #define
GR
EEN 0x07E0

  #define CYAN  0x07FF

  #define CELESTE 0x075F

  #define MAGENTA 0xF81F

  #define YELLOW 0xFFE0

  #define NARANJA 0xFBC0

  #define ROSA  0xFBEA

  #define WHITE  0xFFFF

  #define FONDO  0x0005

  ////>>>>> DEFINICION DEL PIN PARA GSM <<<<///

  #define pwrkey 24



  ////>>>>> DEFINICION DE PINES PARA ULTRASÓNICO <<<<///

  #define trigPin 30

  #define echoPin 31



  /////((((( DEFINICION DE VARIABLES, CONSTANTES Y TIPOS DE DATOS
)))))))/////

  // Variables que almacenaran la coordenada

  //long lmax;

  int X,Y; // X, Y donde presionemos y la variable Z

  int Z; // almacenara la presion realizada
 






  byte op,op_del,ban1=0,hm=0,hora=0,minuto=0,dia=1,mes=12,backlight=52, i_etiqueta;

  word lmax,lmin,anio=2016;

  boolean a=1,bancell=0; // variable "flag" para control rebotes a

  String cell,aux, aux1;

  tmElements_t tm,t1;

  word matbot[39][4]={{1, 75, 75, 45}, //BOTON 0

  {81, 75, 75, 45},

  {161, 75, 75, 45},

  {1, 125, 75, 45},

  {81, 125, 75, 45},

  {161, 125, 75, 45},

  {1, 175, 75, 45},

  {81, 175, 75, 45},

  {161, 175, 75, 45},

  {1, 225, 75, 45},

  {81, 225, 75, 45},//BOTON 10

  {161, 225, 75, 45},

  {1, 275, 238, 45},//BOTON OK

  {10, 258, 105, 60},

  {125, 258, 105, 60},

  {40,140,160,90}, // START <<---//BOTON 15

  {95,45, 135, 60},
 




  {95, 110, 135, 60},//192

  {95, 175, 135, 60},//258

  {125, 258, 105, 60},//258

  {10, 125, 135, 60}, // BOTON 20

  {10, 190, 135, 60},

  {125, 258, 105, 60},

  {125, 258, 105, 60},

  {10, 45, 135, 60},

  {10, 110, 135, 60}, // BOTON 25

  {10, 175, 135, 60},

  {125, 258, 105, 60},

  {10, 192, 60, 60},

  {80, 192, 60, 60},

  {165, 192, 60, 60}, //BOTON 30

  {10, 258, 105, 60},

  {125, 258, 105, 60},

  {10, 60, 135, 60},

  {10, 125, 135,60},

  {125, 258, 105, 60},//BOTON 35

  {10, 258, 105, 60}, // HORARIO

  {125, 258, 105, 60}, //BOTON RESULTADOS

  {40,140,160,90}}; // STOP <<---


 




  char *nomb[]= {"1","2","3","4","5","6","7","8","9",

  "BACK","0","DEL","OK",

  "MENU","DATOS","START",

  "$$ COMIDA","ESTADO","MODIFICAR","ATRAS",

  "# ANIMALES","EDAD","ATRAS",

  "OK",

  "PRODUCCION","HORA/FECHA","# CELULAR","ATRAS",

  "+","-","-->","OK","ATRAS",

  "EQUIPO","PROPIETARIO","ATRAS",

  "HORARIO","RESUMEN","STOP"};





  void setup()

  {

  Serial.begin(9600);

  Serial.println("inicio");

  tft.reset();

  tft.begin(0x9341); // Iniciamos el LCD especificando el controlador ILI9341.

    // (Otros controladores disponibles: HX8357D, HX8347G, ILI9328, ILI9325)




  //#endif //////////////////°°°°° FIN DEL SETUP DE SD°°°°°////////


 




  //////////////////++++++++ INICIO DEL SETUP DE TOUCH ++++++////////

  //tft.fillScreen(0xFBC0);/////--- PONE LA PANTALLA NARANJA

  //pinMode(13, OUTPUT);

  tft.setRotation(0); // Establecemos la posición de la pantalla Vertical u Horizontal



  ///<<<< SETUP PARA EL RELOJ >>>

  Wire.begin();// Inicia el puerto I2C

  //RTC.begin();// Inicia la comunicación con el RTC

    //RTC.adjust(DateTime( DATE  ,  TIME  ));  //  Establece  la  fecha  y  hora (Comentar una vez establecida la hora)


  setSyncProvider(RTC.get); // Vamos a usar el RTC

  //setTime(hora,minuto,00,dia,mes,2015); // Las 0:0:00 del dia 1 de Dic de 2015

  //RTC.write(hora,minuto,00,dia,mes,2015);

  if (timeStatus() != timeSet)

  tft.println("Unable to sync with the RTC");

  else

  {

  tft.println("RTC has set the system time");

  }



  ///<<<< SETUP PARA EL GSM >>>

  pinMode(24, OUTPUT);

  SIM900.begin(19200);//INICIAMOS EL GSM
 






  ///<<<< SETUP PARA EL ULTRASÓNICO >>>

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output

  pinMode(echoPin, INPUT); // Sets the echoPin as an Input



  //PARA LIMPIAR LA EEPROM ///

  //for(byte i=1;i<14;i++)

  // eepromClear(i);

  // PONE EL COLOR DEL FONDO

  tft.fillScreen(FONDO);

  /* pinMode(backlight,OUTPUT);

  digitalWrite(backlight,HIGH);

  delay(200);

  digitalWrite(backlight,LOW);*/

  }////////////++++++++ FIN SETUP +++++////////



  void loop()

  {

    makeScreen(1,BLACK,WHITE); ///;;;; CREA LA PANTALLA DENTRO DE INICIO [MENU, DATOS]
  // digitalWrite(backlight,HIGH);

  do

  {

  printclock(0);
 




  leeBoton(1);

  } while(!(op >= 13 && op <= 15));



  switch (op)

  {

  case 13: do //;;; MANTIENE EN LA PANTALLA ACTUAL

  {

    ///;;;; CREA  LA  PANTALLA  DENTRO  DE  MENU [NUEVO,ESTADO,MODIFICAR,ATRAS]
  clrprintclock(); //:: BORRA EL RELOJ

  clearBoton(op_del); //:: BORRA LOS BOTONES

  makeScreen(2,BLACK,WHITE);

  do

  {

  leeBoton(2);

  } while(!(op >= 16 && op <= 19));

  switch(op)

  {

  case 16: clearBoton(op_del); //:: BORRA LOS BOTONES

    makeScreen(0,BLACK,WHITE);///;;;; CREA LA PANTALLA DE TECLADO NUMERICO
  i_etiqueta = 4;

  //aux = "0.00";//String(precio);

  do //;;; MANTIENE EN LA PANTALLA ACTUAL (TECLADO)
 




  {

  ///<<< INGRESA EL PRECIO DE LAS COMIDAS 1/16, 3/32, 1/8, 1/4
>>>///

    etiquetaTeclado(i_etiqueta);  /// HACE QUE SE IMPRIMAN LAS ETIQUETAS DE TAMAÑO COMIDA
  do

  {

  leeTeclado(4);

    } while(!(op == 9 || op == 12)); //SALE EN CASO DE PRESIONAR 'BACK' U 'OK'
  if(op == 12)

  {

  writeEeprom(i_etiqueta); //>>> GUARDA EN LA EEPROM

  if(i_etiqueta != 7)

  op=16;

  }

  i_etiqueta += 1;

  }while (!(i_etiqueta > 7 || op == 9));

    ///;;; SALE DEL do while CUANDO SE HAYA PRESIONADO 'BACK' O YA SE HAYA INGRESADO LOPRECIOS DE TODAS LAS COMIDAS
  tft.fillScreen(FONDO);//::: LIMPIA LA PANTALLA

  break;

  case 17: ///;;;; CREA LA PANTALLA DENTRO DE ESTADO [OK]

  clearBoton(op_del); //:: BORRA LOS BOTONES

  makeScreen(4,BLACK,WHITE);
 




  informativo(1);

  do //;;; MANTIENE EN LA PANTALLA ACTUAL

  {

  leeBoton(4);

    } while (op != 23);///;;; SALE DEL do while CUAN DO SE HAYA PRESIONADO EL BOTON 'OK'
  tft.fillScreen(FONDO);

  break;

    case  18: //  CREA LA PANTALLA DENTRO DE MODIFICAR [PRODUCCION, HORA/FECHA, #CELULAR, ATRAS]
  do //;;; MANTIENE EN LA PANTALLA ACTUAL

  {



    ///;;;; CREA  LA  PANTALLA  DENTRO  DE  MODIFICAR [PRODUCCION, HORA/FECHA,#CELULAR,ATRAS]
  clearBoton(op_del); //:: BORRA LOS BOTONES

  makeScreen(5,BLACK,WHITE);

  do

  {

  leeBoton(5);

  } while(!(op >= 24 && op <= 27));

  switch (op)

  {

    case 24: //LLAMA A PANTALLA DE PRODUCCION [# ANNIMALES, EDAD, ATRAZ]
 






  do //;;; MANTIENE EN LA PANTALLA ACTUAL

  {

    ///;;;; CREA LA PANTALLA DENTRO DE PRODUCCIOM [# DE ANIMALES, EDAD, ATRAS]
  clearBoton(op_del); //:: BORRA LOS BOTONES

  makeScreen(3,BLACK,WHITE);

  do

  {

  leeBoton(3);

  } while(!(op >= 20 && op <= 22));

    //if(op!=21) //CONTROLA QUE NO HAYA SIDO PRESIONADO ATRAS


  switch (op)

  {

  ///<<< INGRESA EL # DE ANIMALES
///

    case 20: clearBoton(op_del); //:: BORRA LOS BOTONES
  makeScreen(0,BLACK,WHITE);///;;;;
CREA LA PANTALLA DE TECLADO NUMERICO



  etiquetaTeclado(1);

  do
 




  {

  leeTeclado(1);

  } while(!(op == 9 || op == 12)); //SALE
EN CASO DE ´RESIONAR 'BACK' U 'OK'

  if(op==12)

  writeEeprom(1);

  tft.fillScreen(FONDO);//::: LIMPIA LA
PANTALLA

  break;

  ///<<<  INGRESA LA  EDAD  DE
ANIMALES ///

    case 21: clearBoton(op_del); //:: BORRA LOS BOTONES
  makeScreen(0,BLACK,WHITE);///;;;;
CREA LA PANTALLA DE TECLADO NUMERICO



  etiquetaTeclado(2);

  do

  {

  leeTeclado(2);

  } while(!(op == 9 || op == 12)); //SALE
EN CASO DE ´RESIONAR 'BACK' U 'OK'

  if(op==12)

  {

  writeEeprom(2);
 




  }


PANTALLA    
tft.fillScreen(FONDO);//::: LIMPIA LA
    break;
  } 

    } while (op != 22);///;;; SALE DEL do while CUAN DO SE HAYA PRESIONADO EL BOTON 'ATRAZ'
  clearBoton(op_del); //:: BORRA LOS BOTONES

  break;

  case 25: //LLAMA A PANTALLA DE HORA/FECHA

  ban1=0;

  hm=0;

    ///;;;; CREA LA PANTALLA DENTRO DE HORA FECHA [+, -, -->]
  clearBoton(op_del); //:: BORRA LOS BOTONES

  makeScreen(6,BLACK,WHITE);



    etiquetaClock(0,WHITE);// PONE  EL  TITULO 'HORA'
    updateclok(); // ACTUALIZA LOS VALORES DE LAS VARIABLES hora, minuto, dia, mes, anio, A LOS ACTUALES
    clock(1,0,WHITE,5);// HORA REAL/ HORA DE AJUSTE, HORA/FECHA, COLOR DE TEXTO, TAMAÑO DE TEXTO
  do

  {
 




  leeClock();

  }while(!(op == 32 || ban1 == 2));

    ///**** SI op = 32, ES 'ATRAS', SI ban = 2 HAY QUE GUARDAR EL VALOR
  if(ban1 == 2)

  {

  clock(1,1,FONDO,3); //BORRA EL RELOJ

    etiquetaClock(1,FONDO); //BORRA LA ETIQUETA HORA/FECHA
    settime(); //<<<  <<< PONE HORA Y FECHA INGRESADA EN EL RTC
  }

  else

    if(ban1 == 1) // SI PRESIONO OK EN HORA Y ATRAS EN FECHA
  {

  clock(1,1,FONDO,3);//BORRA EL RELOJ

    etiquetaClock(1,FONDO);//BORRA  LA ETIQUETA HORA/FECHA
  }

  else  // SI PRESIONO ATRAS EN HORA

  {

  clock(1,0,FONDO,5);//BORRA EL RELOJ

    etiquetaClock(0,FONDO);//BORRA  LA ETIQUETA HORA/FECHA
 




  }

  clearBoton(op_del); //:: BORRA LOS BOTONES

  break;

  case 26: //LLAMA A PANTALLA DE #CELULAR

  clearBoton(op_del);

    makeScreen(0,BLACK,WHITE);///;;;; CREA  LA PANTALLA DE TECLADO NUMERICO
  etiquetaTeclado(3);



  do

  {

  leeTeclado(3);

    } while(!(op == 9 || op == 12)); //SALE EN CASO DE ´RESIONAR 'BACK' U 'OK'
  if(op==12)

  {

  //eepromClear(3);

  writeEeprom(3);

  }

    // si op = 12 guardar el valor ingresado en aux en la eeprom <<<  ****
  tft.fillScreen(FONDO); //::: LIMPIA LA PANTALLA

  break;

  }
 




    } while (op != 27);///;;; SALE DEL do while CUANDO SE HAYA PRESIONADO EL BOTON 'ATRAS'
  clearBoton(op_del); //:: BORRA LOS BOTONES

  break;// FIN case 18

  } /// FIN 1er switch DEL case 13

  }while(op!= 19); // CONDICION DEL 1er do DEL case 13



  break;// FIN case 13



  case 14: //LLAMA A PANTALLA DE DATOS

  do

  {

  clrprintclock();

  clearBoton(op_del); //:: BORRA LOS BOTONES

  clearBoton(8); //:: BORRA EL BOTON START/STOP

  makeScreen(7,BLACK,WHITE);

  do

  {

  leeBoton(7);

  } while(!(op >= 33 && op <= 35));// SALE CUANDO op = 'OK'



  if(op != 35)

  {

  clearBoton(op_del);
 




  makeScreen(4,BLACK,WHITE);

  switch(op)

  {

    case 33: informativo(2); ///<<< LLAMA AL LA PANTALLA DE INFORMACIÓN
  do //;;; MANTIENE EN LA PANTALLA ACTUAL

  {

  leeBoton(4);

    } while (op != 23);///;;; SALE DEL do while CUAN DO SE HAYA PRESIONADO EL BOTON 'OK'
  break;

  case 34: informativo(3);

  do //;;; MANTIENE EN LA PANTALLA ACTUAL

  {

  leeBoton(4);

    } while (op != 23);///;;; SALE DEL do while CUAN DO SE HAYA PRESIONADO EL BOTON 'OK'
  break;

  }

  if(op == 23)

  tft.fillScreen(FONDO);

  }

  }while(op != 35);

  break;
 




  case 15: //LLAMA A LA PANTALLA STOP

  do

  {

  clearBoton(op_del);

  makeScreen(8,BLACK,WHITE);



  digitalWrite(backlight,LOW);

  do

  {

  printclock(1);

  // a_dormir();

  leeBoton(8);

  }while(!(op >= 36 && op <= 38));



  if(op != 38)

  {

  clrprintclock();

  clearBoton(op_del);

  makeScreen(4,BLACK,WHITE);

  switch(op)

  {

    case 36: informativo(4); ///<<< LLAMA AL LA PANTALLA DE HORARIO
  do //;;; MANTIENE EN LA PANTALLA ACTUAL
 




  {

  leeBoton(4);

    } while (op != 23);///;;; SALE DEL do while CUAN DO SE HAYA PRESIONADO EL BOTON 'OK'
  break;

    case 37: informativo(5); ///<<< LLAMA AL LA PANTALLA DE RESUMEN
  do //;;; MANTIENE EN LA PANTALLA ACTUAL

  {

  leeBoton(4);

    } while (op != 23);///;;; SALE DEL do while CUAN DO SE HAYA PRESIONADO EL BOTON 'OK'
  break;

  }

  if(op == 23)

  tft.fillScreen(FONDO);

  }

  }while(op != 38);

  break;

  /* default:

  tft.fillScreen(0xFBC0);/////--- PONE LA PANTALLA NARANJA

  makeScreen(op);

  op=3;

  do
 




  {

  leeBoton(0);

  } while(!(op >= 0 && op < 3)); */

  a=1;

  } //FIN DEL 1er switch

  clearBoton(op_del); //:: BORRA LOS BOTONES

  }

  //////////+++++ FIN DEL LOOP++++++/////
