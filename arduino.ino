// include the library code: 
#include <LiquidCrystal.h> //library for LCD 

// initialize the library with the numbers of the interface pins 
LiquidCrystal lcd(13, 12, 11, 10, 9, 8); 

//Measuring Current Using ACS712
const int Sensor_Pin = A0; //Connect current sensor with A0 of Arduino
int sensitivity = 66; // use 100 for 20A Module and 66 for 30A Module
int offsetvoltage = 2500; 

//Measuring Temperature Using LM35
const int tempPin = A1; // Connect LM35 to A1

void setup() 
{
  pinMode(7,OUTPUT); //Relay Pin as OUTPUT
  lcd.begin(20, 4); // set up the LCD's number of columns and rows:
  lcd.setCursor(0,0);
  lcd.print("  BATCH 12 SPNK    ");//heading
}

void loop()
{
  //******************* Measure Current ****************************
  unsigned int temp=0;
  float maxpoint = 0;
  for(int i=0;i<500;i++)
  {
    if(temp = analogRead(Sensor_Pin), temp > maxpoint)
    {
      maxpoint = temp;
    }
  }
  float ADCvalue = maxpoint; 
  double Voltage = (ADCvalue / 1024.0) * 5000; // Gets mV
  double Current = ((Voltage - offsetvoltage) / sensitivity);
  double AC_Current = ( Current ) / ( sqrt(2) );

  // Display current on the LCD
  lcd.setCursor(0,1);
  lcd.print("  CURRENT = ");
  lcd.print(AC_Current,2);
  lcd.print("A          "); //unit for the current

  //******************* Measure Temperature *************************
  int tempValue = analogRead(tempPin); 
  float voltageTemp = (tempValue / 1024.0) * 5.0; // Convert analog value to voltage
  float temperatureC = voltageTemp * 100; // LM35 gives 10mV per degree Celsius
  
  // Display temperature on the LCD
  lcd.setCursor(0, 2);
  lcd.print("   TEMP = ");
  lcd.print(temperatureC);
  lcd.print("C        ");

  //********** Combined Overload Protection (Current or Temperature) **********************
  if(AC_Current > 20 || temperatureC > 80) // Check if either current > 20A or temp > 80°C
  {
    lcd.setCursor(0,3);
    lcd.print("     OVERLOADED       ");
    digitalWrite(7,LOW);  //Shut Down Transformer/System (Turn Off Relay)
  }
  else
  {
    lcd.setCursor(0,3);
    lcd.print("       NORMAL     ");
    digitalWrite(7,HIGH);  //System Normal (Relay On)
  }

  // Wait 1 second before the next reading
  delay(1000);
}
