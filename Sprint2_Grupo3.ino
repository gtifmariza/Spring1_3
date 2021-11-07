#include <Wire.h>
#include <Adafruit_ADS1X15.h> // Incluimos las librerias 

Adafruit_ADS1115 ads1115; // construimos el ads1115 en la direccion 0x48

const int AirValue = 19000;  // Medimos valor en seco
const int WaterValue = 9700;  // Medimos valor en agua
const int LackSaultValue = 22400;// Medimos valor sin sal
const int SalinityValue = 25800;// Medimos valor  de una cucharada sal en taza
float b = 0.786;
float m = 0.0347;
float desviacion = 0.6;// coeficientes para la ecuacion lineal del sensor de temperatura T=((Vo-b)/m)+desviacion

int salinity_power_pin = 5; //llamamos salinity_power_pin al pin 5

#define LED_ROJO1 16 // llamamos LED_ROJO1 al pin 16
#define LED_ROJO2 13 // llamamos LED_ROJO2 al pin 13

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup() {

  Serial.begin(9600); // inicializamos el monitor serie y ponemos la velocidad a la que trabajará

  Serial.println("Inicializando...");

  ads1115.begin(); // inicializa el ads1115
  Serial.println("Ajustando la ganancia...");

  ads1115.setGain(GAIN_ONE); //ponemos la ganancia one del adc que es la de 4.096V
  Serial.println("Tomando medidas del canal AIN0");

  Serial.println("Rango del ADC: +/- 4.096V (1 bit=2mV)");
  pinMode(salinity_power_pin, OUTPUT); //configuramos el pin5 como salida
  pinMode(LED_ROJO1, OUTPUT); // configuramos el pin 16 como salida
  pinMode(LED_ROJO2, OUTPUT); // configuramos el pin 13 como salida

}
//----------------------------------------------------------------------------------------------------------------------------------------------------

int medida_humedad() {  //funcion para la medida de humedad

  int16_t adc0;
  int16_t humedad; //declaramos variables

  adc0 = ads1115.readADC_SingleEnded(0);// el valor que recoja el sensor se almacenará en la variable adc0
  humedad = 100 * AirValue / (AirValue - WaterValue) - adc0 * 100 / (AirValue - WaterValue); // algoritmo para calcular el porcentaje de humedad

  if (humedad > 100) {  //condiciones para arreglar el algoritmo cuando dé valores negativos o superiores a 100
    humedad = 100;
  }
  if (humedad < 0) {
    humedad = 0;
  }

  return humedad;// devolvemos el valor de humedad al loop


} // ()

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

int medida_salinidad() {//funcion para la medida de salinidad

  int adc1;
  int input_pin = 5;
  int sal;
  int salinity_input_pin = 5; //declaracion de variables

  digitalWrite( salinity_power_pin, HIGH );// valor alto a la señal para la medicion del movimiento de iones en el agua

  adc1 = ads1115.readADC_SingleEnded(1);// entrada selecionada la 1 comunicanose con nuestra placa

  sal = 100 * LackSaultValue / (LackSaultValue - SalinityValue) - adc1 * 100 / (LackSaultValue - SalinityValue); //formula para sacar porcentaje,(algoritmo)

  if (sal > 100) {   // estas son unas condiciones para arreglar cuando el algoritmo dé porcentajes negativos o mayores que 100
    sal = 100;
  }
  if (sal < 0) {
    sal = 0;
  }

  digitalWrite( salinity_power_pin, LOW );//damos un valor Low a la señal de pulso

  return sal;//devolvemos el valor de sal al loop


}
//-------------------------------------------------------------------------------------------------------
float medida_temperatura() { // funcion para la medida de temperatura
  int adc2;
  float temperatura;
  float tension;// declaracion de variables

  adc2 = (ads1115.readADC_SingleEnded(2));// entrada selecionada la 2 comunicanose con nuestra placa
  tension = (4.096 * adc2) / 32767;// factor de conversión para obtener la tension de salida
  temperatura = ((tension - b) / m) + desviacion;// algoritmo para obtener la temperatura (recta de calibracion)

  return temperatura;//devolvemos el valor de "temperatura" al loop

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void loop() {
  int humedad;
  int sal;
  int temperatura; //declaramos las variables

  temperatura = medida_temperatura();// llamada a la funcion medida_temperatura
  humedad = medida_humedad();// llamada a la funcion medida_humedad
  sal = medida_salinidad();//llamada a la funcion medida_salinidad

  //------------------------------------------------------------------------------------------------------------temperatura
  Serial.print("Temperatura: ");
  Serial.print(temperatura, 1); //este es el resultado de el algoritmo
  Serial.println("ºC");

  //---------------------------------------------------------------------------------------------humedad
  Serial.print("Porcentaje de humedad (%): ");  //Porcentaje que se muestra en el monitor serie

  if (humedad >= 80) {          //Parte Extra: Led de Alarma
    digitalWrite(LED_ROJO1, HIGH);
  }
  else {
    digitalWrite(LED_ROJO1, LOW);
  }

  Serial.print(humedad);//resultado de la funcion medida_humedad
  Serial.println("%");

  //------------------------------------------------------salinidad
  Serial.print("Porcentaje de salinidad (%):"); //Esto es el porcentaje que se muestra en el monitor serie

  if (sal >= 80) {   //Parte Extra: Led de Alarma
    digitalWrite(LED_ROJO2, HIGH);
  }
  else {
    digitalWrite(LED_ROJO2, LOW);
  }

  Serial.print(sal);//este es el resultado de el algoritmo
  Serial.println("%");
  Serial.println("-----------------------");

  delay(2000);// tiempo de espera

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
