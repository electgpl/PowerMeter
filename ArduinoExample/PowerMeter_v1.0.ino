#include <math.h>
#define CONST_CORRIENTE 5
#define CONST_TENSION   100
int i=0, adcZ=0; 
float ADCvRef=0;
float adc1=0, valorMax1=0, adc2=0, valorMax2=0;
float tension, corriente, potencia;
//-----------------------------------------------------------
//Funcion de calculo de tension de alimentacion
//-----------------------------------------------------------
int vRefADC(){
   long result;
   ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
   delay(2);
   ADCSRA |= _BV(ADSC);
   while (bit_is_set(ADCSRA,ADSC));
   result = ADCL;
   result |= ADCH<<8;
   result = 1125300L / result;
   return result;
}
//-----------------------------------------------------------
//Funcion de configuracion
//-----------------------------------------------------------
void setup(){
   Serial.begin(9600);
   ADCvRef = vRefADC();
}
//-----------------------------------------------------------
//Funcion repetitiva
//-----------------------------------------------------------
void loop(){
   do{
      adcZ=analogRead(A1)-512;
      delayMicroseconds(20);
   }while(adcZ<10);
   for(i=0;i<50;i++){
      adc1=(analogRead(A1)*ADCvRef/1023.0)-2500;
      delayMicroseconds(33);
      valorMax1=adc1*adc1+valorMax1;
      adc2=(analogRead(A0)*ADCvRef/1023.0)-2500;
      delayMicroseconds(33);
      valorMax2=adc2*adc2+valorMax2;
   }  
   tension=(sqrt(valorMax1/50)*CONST_TENSION)/1000;
   corriente=(sqrt(valorMax2/50)*CONST_CORRIENTE)/1000;
   potencia=tension*corriente;
   delay(2000);
   potencia = tension * corriente; 
   Serial.print("V:");
   Serial.print(tension);
   Serial.print(" | ");
   Serial.print("A:");
   Serial.print(corriente);
   Serial.print(" | ");
   Serial.print("W:");
   Serial.println(potencia);
   Serial.println("");
   valorMax1=0;
   valorMax2=0;
}
