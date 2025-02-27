//Import libraries
#include "SoftwareSerial.h"       // Sd-card, also mp3 player
#include <Adafruit_NeoPixel.h>    // ringlight
#include <avr/wdt.h>              // restart function
#include "DFRobotDFPlayerMini.h"  // mp3 player
#include <Adafruit_VL53L0X.h>     // laser distance sensor

bool playSound = false;
bool actionSoundPlayed = false;

unsigned long time;  //reading time
unsigned long timeSinceHunt;

// Define pins and numbers
int neoPin = 10;        // pin connected to ringlight
int numberPixels = 24;  // number of pixels in ringlight
int mp3Pin1 = 9;        // pin connected to mp3
int mp3Pin2 = 8;        // other pin connected to mp3
int inactive[3] = { 1, 3, 4 };
int distance;

//Objects
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numberPixels, neoPin, NEO_GRB + NEO_KHZ800);
DFRobotDFPlayerMini mp3Player;
SoftwareSerial softwareSerial(mp3Pin2, mp3Pin1);
Adafruit_VL53L0X lox = Adafruit_VL53L0X();


void setup() {
  //general setup
  softwareSerial.begin(9600);
  Serial.begin(9600);
  Serial.println("Aperture Science");

  // Ringlight setup
  pixels.begin();
  pixels.setBrightness(100);
  pixels.show();
  changeColor(0, 0, 255, 50);

  //mp3 Player setup
  if (!mp3Player.begin(softwareSerial, true, true)) {
    Serial.println(F("Sound no workie"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
  }
  mp3Player.volume(20);  // Set volume for mp3 Player

  if (!lox.begin()) {
    Serial.println("Laser no workie");
  }

  mp3Player.play(10);  //Play sound during startup "Hello friend"
  delay(3000);
  lox.startRangeContinuous();
}



void loop() {

  time = millis();
  delay(100);
  if (lox.isRangeComplete()) {
    distance = lox.readRange();
  }
  Serial.println(distance);
  hunting();
}





void hunting() {

  Serial.println("HEJ");

  if (distance < 100) {
    Serial.println("mindre än 100");
    actionSoundPlayed = false;
    playSound = false;
    mp3Player.stop();
    death();
  }

  else if (distance < 500 && distance > 100) {
    Serial.println("mindre än 500");
    if (!actionSoundPlayed) {
      mp3Player.play(2);
      delay(2000);
      mp3Player.play(4);
      delay(2000);
      actionSoundPlayed = true;
    }
    playSound = false;
    shoot();
  }

  else if (distance > 500) {
    Serial.println("mer än 500");
    mp3Player.stop();
    actionSoundPlayed = false;
    playSound = false;
    //mp3Player.disableLoopAll();
    changeColor(0, 50, 255, 0);
  }

  if (time - timeSinceHunt > 10000 && !playSound) {
    timeSinceHunt = millis();
    playSound = true;
    int sound = rand() % 3;
    mp3Player.play(inactive[sound]);
    delay(3000);
  }
}





void changeColor(int r, int g, int b, int delayLength) {
  for (int i = 0; i < numberPixels; i++) {

    pixels.setPixelColor(i, r, g, b);
    pixels.show();

    delay(delayLength);
  }
}





void shoot() {
  changeColor(255, 0, 1, 0);
  mp3Player.loop(5);
  delay(1000);
  playSound = true;
  timeSinceHunt = millis();
}




void death() {

  changeColor(0, 50, 255, 0);
  mp3Player.play(8);

  for (int i = 255; i > 0; i -= 17) {
    changeColor(0, 0, i, 0);
    delay(100);
  }

  wdt_enable(WDTO_15MS);  // Restart
  while (true) {
    Serial.println("restarting");
  }
}
