#include <Servo.h>
#include "RTClib.h"
#include <Wire.h>
#include <TimeLib.h>
#include <TimeAlarms.h>

int servoPin = 3;
Servo servo;
const int trigpin = 9;
const int echopin = 10;
long duration;
int distance;
RTC_DS3231 rtc;
int cat = 1;
int relay_pin = 8;
AlarmId id;



void setup() {
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  setTime(9,01,0,10,30,19);
  Alarm.timerRepeat(3600, OnceOnly);
  servo.attach(servoPin);
  servo.write(0);
  pinMode(relay_pin, OUTPUT);
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
}

void loop(){
  Alarm.delay(10);
  DateTime now = rtc.now();
  
  int tund = now.hour();
  int minut = now.minute();
  if ((tund == 06 && minut == 00) || (tund == 17 && minut == 00)) {
    servo.write(45);
    delay(3000);
    servo.write(0);
    delay(60000);
  }
  
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  duration = pulseIn(echopin, HIGH);
  Serial.println(duration);
  Serial.println(cat);
  
  int water = analogRead(A0);
  if (water <= 500){
    digitalWrite(relay_pin, HIGH);
    delay(1500);
    digitalWrite(relay_pin, LOW);
    delay(100);
  }
  
  if (cat == 1){
    if (duration < 500){
      servo.write(45);
      delay(300);
      servo.write(0);
      delay(100);
      OnceOnly();
      cat = 2;
  }
}
}
void OnceOnly(){
  cat = 1;
}
