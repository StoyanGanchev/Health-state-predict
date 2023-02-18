#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS     1000
// Create a PulseOximeter object
PulseOximeter pox;

// Time at which the last beat occurred
uint32_t tsLastReport = 0;
int red = 3, green = 4;
#define ntc_pin A0         // Pin,to which the voltage divider is connected
#define vd_power_pin 6        // 5V for the voltage divider
#define nominal_resistance 10000       //Nominal resistance at 25⁰C
#define nominal_temeprature 25   // temperature for nominal resistance (almost always 25⁰ C)
#define samplingrate 5    // Number of samples
#define beta 3950  // The beta coefficient or the B value of the thermistor (usually 3000-4000) check the datasheet for the accurate value.
#define Rref 10000   //Value of  resistor used for the voltage divider
  float temperature;
  int samples = 0;   //array to store the samples
  



  
void setup() {
  if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
    
  // Configure sensor to use 7.6mA for LED drive
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
    Serial.begin(9600);   //initialize serial communication at a baud rate of 9600
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
    pinMode(vd_power_pin, OUTPUT);
}


void loop() {
  // Read from the sensor
    pox.update();
  
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print(pox.getHeartRate());
        Serial.print(",");
        Serial.print(pox.getSpO2());
        Serial.print(",");
        tsLastReport = millis();
    }
    
  uint8_t i;
  float average;
  samples = 0; 
    
  // take voltage readings from the voltage divider
  digitalWrite(vd_power_pin, HIGH);
  for (i = 0; i < samplingrate; i++) {
    samples += analogRead(ntc_pin);
    delay(10);
  }
  digitalWrite(vd_power_pin, LOW);
  average = 0;
  average = samples / samplingrate;
  // Calculate NTC resistance
  average = 1023 / average - 1;
  average = Rref / average;

  temperature = average / nominal_resistance;     // (R/Ro)
  temperature = log(temperature);                  // ln(R/Ro)
  temperature /= beta;                   // 1/B * ln(R/Ro)
  temperature += 1.0 / (nominal_temeprature + 273.15); // + (1/To)
  temperature = 1.0 / temperature;                 // Invert
  temperature -= 273.15;                         // convert absolute temp to C
  Serial.println(temperature);
 // Serial.print(",");
  
  
  delay(2000);
}


void Signal(float temperature){
  while (temperature > 37.5){
    Serial.println("red");
    digitalWrite(red,HIGH);
    delay(1000);
    digitalWrite(red,LOW);
    delay(1000);
    }
    while(temperature < 37.5){
      Serial.println("green");
      digitalWrite(green,HIGH);
    delay(1000);
    digitalWrite(green,LOW);
    delay(1000);
      }
  }
