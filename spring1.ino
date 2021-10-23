#include <Wire.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads1115;
const int AirValue = 19000;  // Medimos valor en seco
const int WaterValue = 9700;  // Medimos valor en agua
const int LackSaultValue = 22400;// Medimos valor sin sal
const int SalinityValue = 25800;// Medimos valor  de una cucharada sal en taza
int salinity_power_pin = 5;
#define LED_ROJO1 16 // llamamos LED_ROJO1 al pin 16
#define LED_ROJO2 13 // llamamos LED_ROJO2 al pin 13

void setup() {
  
  Serial.begin(9600);
  Serial.println("Inicializando...");
  ads1115.begin(); //Initialize ads1115
  Serial.println("Ajustando la ganancia...");
  ads1115.setGain(GAIN_ONE);
  
  Serial.println("Tomando medidas del canal AIN0");
  
  Serial.println("Rango del ADC: +/- 4.096V (1 bit=2mV)");
 pinMode(salinity_power_pin, OUTPUT);
  pinMode(LED_ROJO1, OUTPUT); // configuramos el pin 16 como salida
  pinMode(LED_ROJO2, OUTPUT); // configuramos el pin 13 como salida

}
int medida_humedad(){
  
int16_t adc0;
int16_t humedad;
adc0 = ads1115.readADC_SingleEnded(0);
humedad = 100*AirValue/(AirValue-WaterValue)-adc0*100/(AirValue-WaterValue);

// Serial.print("AIN0: ");
// Serial.println(adc0);
//Serial.print("Porcentaje de humedad (%): ");
//Serial.print(humedad);
//Serial.println("%");
//delay(1000);
  
  return humedad;
}

int medida_salinidad(){
  
  int adc1;
 int input_pin = 5;
 
 int16_t sal;
 int salinity_input_pin = 5;

 digitalWrite( salinity_power_pin, HIGH );

 adc1 = ads1115.readADC_SingleEnded(1);// entrada selecionada la 0 comunicanose con nuestra placa
 sal = 100*LackSaultValue/(LackSaultValue-SalinityValue)-adc1*100/(LackSaultValue-SalinityValue);//formula para sacar porcentaje,(algoritmo)
 
 digitalWrite( salinity_power_pin, LOW );
 return sal;
 //Serial.print("Salinidad: ");
 //Serial.println(adc1);// entrada selecionada la 1 comunicanose con nuestra placa
 //Serial.print("Porcentaje de salinidad (%):");//Esto es el porcentaje que se muestra en el monitor serie 
 //Serial.print(sal);//este es el resultado de el algoritmo 
 //Serial.println("%");
 
 //Serial.println("-----------------------");
 //delay(1000);// tiempo de espera entre medida y medida en ms
}
void loop() {
 int humedad;
 int sal;
 humedad=medida_humedad();
 sal=medida_salinidad();
 Serial.print("Porcentaje de humedad (%): ");
if(humedad >100){
  humedad=100;
}
 if(humedad<0){
  humedad=0;
 }
if(humedad>=80){
  digitalWrite(LED_ROJO1, HIGH);
}
else{
  digitalWrite(LED_ROJO1, LOW);
}
 
Serial.print(humedad);
Serial.println("%");
//Serial.print("Salinidad: ");
//Serial.println(adc1);// entrada selecionada la 1 comunicanose con nuestra placa
 Serial.print("Porcentaje de salinidad (%):");
 //Esto es el porcentaje que se muestra en el monitor serie 
if(sal >100){
  sal=100;
}
 if(sal<0){
  sal=0;
 }
 if(sal>=80){
  digitalWrite(LED_ROJO2, HIGH);
}
else{
  digitalWrite(LED_ROJO2, LOW);
}
 Serial.print(sal);//este es el resultado de el algoritmo 
 Serial.println("%");
 Serial.println("-----------------------");
delay(1000);
}
