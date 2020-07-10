
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <Wire.h>  
#include <SPI.h>
#include "SSD1306Wire.h"
#include "SH1106.h"
#include "Rapbit32_Motor_drive.h"
#include "Rapbit32_Servo_lib.h"
#include "Rapbit32_IO.h"
#include "Adafruit_NeoPixel.h"
#include "irremote/IRremote.h"


SSD1306Wire display(0x3c, 21, 22);
Adafruit_NeoPixel pixels(4, 13, NEO_GRB + NEO_KHZ800);

IRrecv irrecv(14);
decode_results results;

#define M1A 17
#define M1B 16
#define M2A 18
#define M2B 5
#define _sw 23
int sw1(){
  pinMode(_sw,INPUT);
  return digitalRead(_sw);
}
int SW1(){
  pinMode(_sw,INPUT);
  return digitalRead(_sw);
}
int sw_ok(){
  pinMode(_sw,INPUT);
  return digitalRead(_sw);
}

int state_IMU = 0;
void draw_pixel(int16_t x, int16_t y)
{
  display.setColor(WHITE);
  display.setPixel(x, y);
}

void clear_pixel(int16_t x, int16_t y)
{
  display.setColor(BLACK);
  display.setPixel(x, y);
}
void Rapbit32(){
  Serial.begin(115200);
  pinMode(23,INPUT);
  pinMode(25,OUTPUT);
  pinMode(19,OUTPUT);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
  display.drawString(15,0,"Rapbit32");
  display.drawString(15,30,"Welcome");
  display.display();
  delay(700);
  display.clear();
  display.display();
  pixels.begin();
  pixels.setBrightness(50);
  //pixels.setPixelColor(0, pixels.Color(100, 0, 0));
  pixels.show();
  irrecv.enableIRIn();
  pinMode(M1A,OUTPUT);
  pinMode(M1B,OUTPUT);
  pinMode(M2A,OUTPUT);
  pinMode(M2B,OUTPUT);
  ledcSetup(6, 5000, 8);
  ledcSetup(7, 5000, 8);
  ledcSetup(4, 5000, 8);
  ledcSetup(5, 5000, 8);
  ledcAttachPin(M1A, 6);
  ledcAttachPin(M1B, 7);
  ledcAttachPin(M2A, 4);
  ledcAttachPin(M2B, 5);
  analogReadResolution(10);
  
}
int Knob_sensor(){
  return analogRead(36);
}
void beep(){
  int _buzzer = 4;
  pinMode(_buzzer,OUTPUT);
  digitalWrite(_buzzer,HIGH);
  delay(200); 
  digitalWrite(_buzzer,LOW);
}
void beep(int _delay){
  int _buzzer = 4;
  pinMode(_buzzer,OUTPUT);
  digitalWrite(_buzzer,HIGH);
  delay(_delay);
  digitalWrite(_buzzer,LOW);
}
float voltage_sensor(){
  return analogRead(14)*0.00464;
}
void beep_on(){
  int _buzzer = 4;
  pinMode(_buzzer,OUTPUT);
  digitalWrite(_buzzer,HIGH);
}
void beep_off(){
  int _buzzer = 4;
  pinMode(_buzzer,OUTPUT);
  digitalWrite(_buzzer,LOW);
}
void wait(){
  pinMode(23,INPUT);
  display.clear();
  display.setFont(ArialMT_Plain_24);
  display.drawString(15,0,"Rapbit32");
  display.drawString(15,30,"Welcome");
  display.display();
  delay(700);
  while(digitalRead(23) == 1){
	  	display.clear();
	  	display.drawString(30,0,"Ready");
      display.drawString(10,30,"press_OK");
	  	display.display();
	  	delay(100);
      display.clear();
      display.display();
      delay(100);
  }
  beep();
  display.clear();
  display.display();
}
float ultrasonic(){
  int ECHO = 27;
  int TRIG = 26;
  pinMode(ECHO,INPUT);
  pinMode(TRIG,OUTPUT);
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH);
  // Calculating the distance
  return duration*0.034/2;
}

void Rapbit32_setBrightness(uint8_t num){
  pixels.setBrightness(num);
  pixels.show();
}
void Rapbit32_setColor(int R,int G,int B){
  for(int i = 0;i<5;i++){
    pixels.setPixelColor(i, pixels.Color(R, G, B));
  }
  pixels.show();
}
void Rapbit32_setPixel(int num,int R,int G,int B){
  if(num == 0){
    Rapbit32_setColor(R,G,B);
  }
  else{
    pixels.setPixelColor(num-1, pixels.Color(R, G, B));
    pixels.show();
  }
  
}
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos; 
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  
  if (WheelPos < 170) {
    WheelPos -= 85;return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } 
  
  WheelPos -= 170; return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
} 

void Rapbit32_rainbow(int wait) {

  uint16_t i, j;
  for (j = 0; j < 256; j++) {
   for (i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, Wheel((i + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}
long irremote_sensor(){
  long d = 0;
  if (irrecv.decode(&results)) {
     d = results.value;
    irrecv.resume();
  }
  delay(100);
  return d;
}