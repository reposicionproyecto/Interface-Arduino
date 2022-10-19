//SISTEMA DE ALIMENTACION CONTROLADO REMOTAMENTE PARA GRANJA PISCICULTURA DOSIFICACION Y CANTIDAD DE ALIMENTO

#include <Servo.h>
Servo servo3;

//para el ultrasonido
int Echo= 9; //Declaramos el pin 5 como echo
int Trigger= 8; //Declaramos el pin 6 como trigger

double distancia;
long tiempo;

const int pwmPin3 = 3; // Un servo en el pin 3

char val;
String grados;
int angulo;

void setup() {
Serial.begin(9600);
pinMode (pwmPin3, OUTPUT); 

servo3.attach(pwmPin3);

//para el ultrasonido
pinMode(Trigger, OUTPUT);
pinMode(Echo, INPUT);

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
digitalWrite (Trigger, LOW);
delayMicroseconds(5);
digitalWrite(Trigger, HIGH);
delayMicroseconds(10);
digitalWrite(Trigger, LOW);
tiempo=pulseIn(Echo, HIGH);
distancia= tiempo /(29.2*2);

Serial.println (distancia);
delay(250);

} 
