float vol = 0.0,l_minute;
unsigned char flowsensor = 10; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
unsigned long flow_frequency;

void flow () // Interrupt function to increment flow
{
   flow_frequency++;
}
void setup()
{
   Serial.begin(9600);
   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt
   

   Serial.println("fuel FlowMeter");

   delay(500);
   currentTime = millis();
   cloopTime = currentTime;
}

void loop ()
{
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
//      Serial.write()
      Serial.print(vol, 'L');
//      Serial.print(" L");
//      flow_frequency = 0; // Reset Counter
//      Serial.print(l_minute, DEC); // Print litres/hour
//      Serial.println(" L/Sec");
    }
    else {
      Serial.println(0,'l');
    }
   }
}
