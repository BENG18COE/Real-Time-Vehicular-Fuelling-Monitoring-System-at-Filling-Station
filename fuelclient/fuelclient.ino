#include <LiquidCrystal_I2C.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <U8g2lib.h>
#include <SPI.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
const int SENSOR = 2;
// const int BUZ = D5;
//U8g2 Constructor List - https://github.com/olikraus/u8g2/wiki/u8g2setupcpp#introduction
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4);
ESP8266WebServer server(80);

const char *ssid = "poopssid";
const char *password = "pingu4prez";

long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
boolean ledState = LOW;
float calibrationFactor = 90;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned long flowMilliLitres;
unsigned int totalMilliLitres;
float flowLitres;
float totalLitres;
String FLOW_INT;
int show=0;
int sensorInterrupt = 0;  // interrupt 0

// Prototyping function pulseCounter()
void pulseCounter(){
	pulseCount++;
}

void setup(){
	Serial.begin(115200);
	// pinMode(BUZ, OUTPUT);
	lcd.init();                
	lcd.backlight();
	lcd.setCursor(3,0);
	lcd.print("OIL METER");
	lcd.setCursor(0,1);
	lcd.print("Trust true litters");

	pulseCount = 0;
	flowRate = 0.0;
	flowMilliLitres = 0;
	totalMilliLitres = 0;
	previousMillis = 0;
	// sensorInterrupt = digitalPinToInterrupt(SENSOR);

// Enable the interrupt again now that we've finished sending output
	attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
	delay(3000);
	lcd.home(); 
	lcd.clear();

	u8g2.begin();
	u8g2.setFont(u8g2_font_logisoso62_tn);
	u8g2.setFontMode(0);    // enable transparent mode, which is faster

	WiFi.softAP(ssid, password);
	IPAddress myIP = WiFi.softAPIP();
	Serial.print(myIP);
	server.on("/data/", HTTP_GET, handleSentVar); // when the server receives a request with /data/ in the string then run the handleSentVar function
	server.begin();
}


void loop(){
	server.handleClient();
	currentMillis = millis();

	if (currentMillis - previousMillis > interval) {
		// detachInterrupt(sensorInterrupt);
		pulse1Sec = pulseCount;
		pulseCount = 0;
		flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
		previousMillis = millis();

		flowMilliLitres = (flowRate / 60) * 1000;
		flowLitres = (flowRate / 60);

		totalMilliLitres += flowMilliLitres;
		totalLitres += flowLitres;

		// Print the flow rate for this second in litres / minute
		Serial.print("Flow rate: ");
		Serial.print(float(flowRate));  // Print the integer part of the variable
		Serial.print("L/min");
		Serial.print("\t"); 

		// Print the cumulative total of litres flowed since starting
		Serial.print("Fuel measurement: ");
		Serial.print(totalMilliLitres);
		Serial.print("mL / ");
		Serial.print(totalLitres);
		Serial.println("L");
	}
		
	lcd.setBacklight(255);
	lcd.setCursor(0,0);
	lcd.print("Fuel measurement: ");
	lcd.setCursor(0,1);
	lcd.print(FLOW_INT);
	lcd.setCursor(5,1);
	lcd.print("millitres");
	// digitalWrite(BUZ, HIGH);
	delay(3000);
	// digitalWrite(BUZ,LOW);
}

 void handleSentVar() {
 	if (server.hasArg("sensor_reading")) { // this is the variable sent from the client
 		int readingInt = server.arg("sensor_reading").toInt();
 		char readingToPrint[5];
 		itoa(readingInt, readingToPrint, 10); //integer to string conversion for OLED library
 		u8g2.firstPage();
 		u8g2.drawUTF8(0, 64, readingToPrint);
 		u8g2.nextPage();
 		server.send(200, "text/html", "Data received");
 		Serial.print("Data received ");
 		Serial.println(readingToPrint);
 		FLOW_INT = readingToPrint;
 	}
 }
