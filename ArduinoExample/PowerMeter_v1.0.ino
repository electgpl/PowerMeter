#define ISENSE A0
#define VSENSE A1
#define CONST_CORRIENTE 2
#define CONST_TENSION 530
#define MUESTRAS 200
#define MUESTRAS_CAL 100
float tension, corriente, potencia;
int mVperAmp = 185;
double tensionACS = 0;
double amperes = 0;
int ACSoffset = 0; 
int i = 0;
int j = 0;
int RawValue= 0;
int ADCvRef = 0;
float relativo = 0;
float cero = 0;
double volts = 0;
double tensionBAT = 0;
float atenuacion = 1;
float valorMedio = 2.5;
//-----------------------------------------------------------
//Funcion de Setup
//-----------------------------------------------------------
void setup(){
   Serial.begin(9600);
   Serial.println("Medidor de Potencia");
   ADCvRef = vRefADC();
   Serial.print("Tension de Alimentacion:  ");
   Serial.print(ADCvRef);
   Serial.println(" mV");
   ACSoffset = relativoACS();
   Serial.println("Relativisar Corriente - 2000ms");
   delay(2000);
}
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
//Funcion de calculo de cero relativo
//-----------------------------------------------------------
float relativoACS(){
   cero = 0;
   for(j=0;j<MUESTRAS_CAL;j++){
      RawValue = analogRead(ISENSE);
      cero = cero + ((RawValue / 1023.0) * ADCvRef);
      delay(25);
   }
   cero = cero / MUESTRAS_CAL;
   relativo = cero;
   Serial.print("Relativizando cero: ");
   Serial.print(relativo);
   Serial.println(" mV");
   return relativo;
}
//-----------------------------------------------------------
//Funcion de medicion con el sensor
//-----------------------------------------------------------
float mideCorrienteCarga(){
   amperes = 0;
   for(i=0;i<MUESTRAS;i++){
      vRefADC();
      RawValue = analogRead(ISENSE);
      tensionACS = (RawValue / 1023.0) * ADCvRef;
      amperes = amperes + ((tensionACS - ACSoffset) / mVperAmp);
      delay(1);
   }
   corriente = 1.3792 * (amperes / MUESTRAS);
   return corriente;
}
//-----------------------------------------------------------
//Funcion de medicion de Tension con divisor
//-----------------------------------------------------------
float mideTensionBat(){
   volts = 0;
   for(i=0;i<MUESTRAS;i++){
      vRefADC();
      RawValue = analogRead(VSENSE);
      tensionBAT = (RawValue / 1023.0) * ADCvRef;
      volts = volts + tensionBAT;
      delay(1);
   }
   tension = ((volts / MUESTRAS) / 1000) - valorMedio;
   return tension / atenuacion;
}
//-----------------------------------------------------------
//Funcion principal
//-----------------------------------------------------------
void loop(){
   mideCorrienteCarga();
   mideTensionBat();
   potencia = tension * corriente; 
   delay(1000);
   Serial.print("V:");
   Serial.print(tension);
   Serial.print(" | ");
   Serial.print("A:");
   Serial.print(corriente);
   Serial.print(" | ");
   Serial.print("W:");
   Serial.println(potencia);
   Serial.println("");
}