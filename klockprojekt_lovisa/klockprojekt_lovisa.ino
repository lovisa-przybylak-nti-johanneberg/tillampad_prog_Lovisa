//inkludera olika libraries
#include <RTClib.h>
#include <Wire.h>
#include <U8glib.h>

// Init constants
const int potPin = A1;

// Init global variables
//RGB-dioden
int redPin = 13;
int greenPin = 12;
int bluePin = 11;

// construct objects
RTC_DS3231 rtc;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);


void setup() {
  // Deklarerar outputs och inputs
  pinMode(potPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  //seriellmonitorn
  Serial.begin(9600);
  
  // Init Hardware
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  u8g.setFont(u8g_font_unifont);
}

void loop() {

  Serial.println(getThresh());
  String currentTime = getTime();
  float currentTemp = getTemp();

  // Draw both time and temperature in a single display update
  u8g.firstPage();
  do {
    // Display time
    u8g.drawStr(0, 20, "Time:");
    u8g.drawStr(50, 20, currentTime.c_str());
    // Display temperature
    u8g.drawStr(0, 40, "Temp:");
    u8g.drawStr(50, 40, String(currentTemp).c_str());
    u8g.drawStr(100, 40, "'C");
  } while (u8g.nextPage());
  delay(200);

  if(getTemp() > getThresh()){
    RGB_color(255, 0, 0);

  }else{
   RGB_color(0, 255, 0);
  }
}


/*
*This function reads time from an ds3231 module and package the time as a String
*Parameters: Void
*Returns: time in hh:mm:ss as String
*/
String getTime() {
  DateTime now = rtc.now();

  String newTime = String(now.hour());
  if (now.minute() < 10) {
    newTime = newTime + ":" + "0" + String(now.minute());
  }
  else {
    newTime = newTime + ":" + String(now.minute());
  }

  if (now.second() < 10) {
    newTime = newTime + ":" + "0" + String(now.second());
  }
  else {
    newTime = newTime + ":" + String(now.second());
  }

  return newTime;
}

/*
*This function reads temperature from an ds3231 module and package the temp as float
*Parameters: Void
*Returns: temprature as float */
float getTemp(){
  return rtc.getTemperature();
}

/*
*This function reads the potentiometer and sets the span of 0-1023 to 15-30
*Parameter:
*Returns: threshhold for temperature*/

int getThresh() {
  return map(analogRead(potPin), 0, 1023, 15, 30);
  }

/*
*This function sets the RGB values for the RGB diode
*Parameter:
*/
void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(redPin, red_light_value);
  analogWrite(greenPin, green_light_value);
  analogWrite(bluePin, blue_light_value);
 }

