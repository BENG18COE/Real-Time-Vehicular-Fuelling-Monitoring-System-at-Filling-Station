#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x20,16,2); 
float vol = 0.0,l_minute;
unsigned char flowsensor = 10; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
unsigned long flow_frequency;

void flow () // Interrupt function to increment flow
{
   flow_frequency++;
}

void setup() {

  Serial.begin(9600);
  lcd.init();                 //Init the LCD
  lcd.backlight();            //Activate backlight     
  lcd.home();

   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt
   

   Serial.println("Fuel Flow Meter");

   delay(500);
   currentTime = millis();
   cloopTime = currentTime;

  lcd.clear();
  lcd.print("Fuel Flow Measure");
  delay(1000);
  lcd.clear();

}

void loop() {
     currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
    cloopTime = currentTime; // Updates cloopTime
    if(flow_frequency != 0)
    {
      
       l_minute = (flow_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      l_minute = l_minute/60;
      
      vol = vol +l_minute;
//      Serial.print("   sensing...  ");
//      Serial.write();
        Serial.print(vol);
//      Serial.print(" L");
//      flow_frequency = 0; // Reset Counter
//      Serial.print(l_minute, DEC); // Print litres/hour
//      Serial.println(" L/Sec");
    }
    else {
      Serial.println(0,'L');
    }
   }
  lcd.setCursor(0,0);
  lcd.print("Fuel Flow Measure");
  
    lcd.setCursor(1,1);
    lcd.print(vol);      
  
}
