//SISTEMA DE ALIMENTACION CONTROLADO REMOTAMENTE PARA GRANJA PISCICULTURA DOSIFICACION Y CANTIDAD DE ALIMENTO

#include <Servo.h>
Servo servo3;

const int pwmPin3 = 3; // Un servo en el pin 3

char val;
String grados;
int angulo;

//Codigo para medir la cantidad del alimento en la tolva
float rango;
char dato_ultrasonido;
int led=13;

char enviar_dato;
char buffer (10);
//Variables para sensor de distancia
#define pEcho 2  //Conecta pin echo
#define pTrig 3  //Conecta el trigger
int duracion;       //Captura el pulso que emite el echo
int ultrasonico=0;      //Captura la distancia
//int i=0;            //Variable para control del ciclo

void setup() {
Serial.begin(9600);
pinMode (pwmPin3, OUTPUT); 

servo3.attach(pwmPin3);

}

void loop() {  
if( Serial.available() ) {
val = Serial.read();
if (isDigit(val)) { grados += (char)val; }

if (val == '\n') {
  
angulo = grados.toInt();
Serial.println(angulo);

for( angulo == 0; angulo < 180; angulo += 1 )
{
servo3.write(angulo);
delay (15);
}
for(angulo = 180; angulo>=1; angulo-=1)
{
  servo3.write(angulo);
  delay(15);
}







grados = "";

}
} 

} // => Fin Loop
