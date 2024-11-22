// include the library code: 
#include <LiquidCrystal.h> // library for LCD 
#include "Arduino_SensorKit.h"

// initialize the library with the numbers of the interface pins 
LiquidCrystal lcd(13, 12, 11, 10, 9, 8); 

// Measuring Current Using ACS712
const int Sensor_Pin = A0; // Connect current sensor with A0 of Arduino
int sensitivity = 60; // use 100 for 20A Module and 66 for 30A Module
int offsetvoltage = 2500; 

// Measuring Temperature Using LM35
const int tempPin = A1; // Connect LM35 to A1

unsigned long previousMillis = 0; // To store the last time Serial output was sent
const long interval = 2000; // Interval at which to send Serial output (60 seconds)

void setup() 
{
  pinMode(7, OUTPUT); // Relay Pin as OUTPUT
  lcd.begin(20, 4); // set up the LCD's number of columns and rows:
  lcd.setCursor(0, 0);
  lcd.print("  BATCH 12 SPNK    "); // heading
  Serial.begin(9600); // Initialize Serial communication
}

void loop()
{
  //******************* Measure Current ****************************
  unsigned int temp = 0;
  float maxpoint = 0;
  for(int i = 0; i < 500; i++)
  {
    temp = analogRead(Sensor_Pin);
    if(temp > maxpoint)
    {
      maxpoint = temp;
    }
  }
  float ADCvalue = maxpoint; 
  double Voltage = (ADCvalue / 1024.0) * 5000; // Gets mV
  double Current = ((Voltage - offsetvoltage) / sensitivity);
  double AC_Current = Current / sqrt(2);

  // Display current on the LCD
  lcd.setCursor(0, 1);
  lcd.print("  CURRENT = ");
  lcd.print(AC_Current, 2);
  lcd.print("A          "); // unit for the current

  //******************* Measure Temperature *************************
  int tempValue = analogRead(tempPin); 
  float voltageTemp = (tempValue*0.20828125); // Convert analog value to mV (0 - 5000 mV)
  float temperatureC = voltageTemp/10;// LM35 gives 10mV per degree Celsius
  
  // Display temperature on the LCD
  lcd.setCursor(0, 2);
  lcd.print("   TEMP = ");
  lcd.print(temperatureC);
  lcd.print("C        ");

  //********** Combined Overload Protection (Current or Temperature) **********************
  String status;
  if(AC_Current >2 || temperatureC >500) // Check if either current > 10A or temp > 80°C
  {
    lcd.setCursor(0, 3);
    lcd.print("     OVERLOADED       ");
    digitalWrite(7, HIGH);  // Shut Down Transformer/System (Turn Off Relay)
    status = "OVERLOADED";
  }
  else
  {
    lcd.setCursor(0, 3);
    lcd.print("       NORMAL     ");
    digitalWrite(7, LOW);  // System Normal (Relay On)
    status = "NORMAL";
  }

  // Wait 1 second before the next reading
  delay(100);

  // Check if it's time to send data to the Serial Monitor
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Save the last time we sent the Serial output
    // Output current, temperature, and status to Serial Monitor
    Serial.print("Current: ");
    Serial.print(AC_Current, 2);
    Serial.print(" A, Temperature: ");
    Serial.print(temperatureC);
    Serial.print(" C, Status: ");
    Serial.println(status);
  }
}
