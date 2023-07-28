#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SoberThreshold 120    // Define max value that we consider sober
//#define DrunkThreshold 200    // Define min value that we consider drunk
#define MQ3pin A0             // Analog pin for MQ3 alcohol sensor
#define greenLedPin 6         // Pin for the green LED
#define redLedPin 7           // Pin for the red LED
int fanPin = 9;
float sensorValue;            // Variable to store sensor value
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  Serial.begin(9600);        // Sets the serial port to 9600
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  Serial.println("MQ3 warming up!");
  delay(200);                // Allow the MQ3 to warm up
  
  lcd.init();
  Wire.begin();
  lcd.begin(16, 2);
  lcd.clear();
  lcd.backlight();
}

void loop() {
  sensorValue = analogRead(MQ3pin);  // Read analog input pin for MQ3 sensor
  Serial.println(sensorValue);

  lcd.setCursor(0, 0);
  lcd.print("AValue: ");
  lcd.print(sensorValue);
  
  if (sensorValue < SoberThreshold) {
    lcd.setCursor(0, 1);
    lcd.print("Status:NOT DRUNK");
    digitalWrite(greenLedPin, HIGH);   // Turn on green LED
    digitalWrite(redLedPin, LOW);      // Turn off red LED
  } else{
    lcd.setCursor(0, 1);
    lcd.print("Status:DRUNK");
    digitalWrite(greenLedPin, LOW);    // Turn off green LED
    digitalWrite(redLedPin, HIGH);     // Turn on red LED
  } /*else {
    lcd.setCursor(0, 1);
    lcd.print("Status: SOBER");
    digitalWrite(greenLedPin, LOW);    // Turn off green LED
    digitalWrite(redLedPin, LOW);      // Turn off red LED
  }*/
   if (sensorValue < SoberThreshold) {
        // Turn off red LED
          digitalWrite(fanPin, HIGH);
  } else{
      digitalWrite(fanPin, LOW);
  }
  delay(2000);    // Wait 2s for next reading
}
