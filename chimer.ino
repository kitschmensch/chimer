#include <SevSegShift.h>
#include <Wire.h>
#include <RTClib.h>
#include <BfButton.h>
#include "KY040Encoder.h"
#include "ClockTimeMath.h"
#include <Arduino.h>

//Display Setup
#define SHIFT_PIN_SHCP 9
#define SHIFT_PIN_STCP 8
#define SHIFT_PIN_DS   7
SevSegShift sevseg(SHIFT_PIN_DS, SHIFT_PIN_SHCP, SHIFT_PIN_STCP, 1, true);

//KY-040 Encoder Setup
const int CLK_PIN = 4;
const int DT_PIN = 5;
const int SW_PIN = 6;
KY040Encoder encoder(CLK_PIN, DT_PIN, SW_PIN);

//Rotary button
BfButton btn(BfButton::STANDALONE_DIGITAL, SW_PIN, true, LOW);

//Solenoid pin
int solenoidPin = 2;

//Initialize clock
RTC_DS1307 rtc;

//Hourly Chime Settings
const bool hourlyChimeEnabled = true;
const int hourlyChimeStart = 6;
const int hourlyChimeEnd = 21;
bool bellRungThisHour = false;

//Seven Seg Config
byte numDigits = 4;
byte digitPins[] = {10, 11, 12, 13}; // These are the PINS of the ** Arduino **
byte segmentPins[] = {0, 1, 2, 3, 4, 5, 6, 7}; // these are the PINs of the ** Shift register **
bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
byte hardwareConfig = COMMON_CATHODE; // See README.md for options
bool updateWithDelays = false; // Default 'false' is Recommended
bool leadingZeros = true; // Use 'true' if you'd like to keep the leading zeros
bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected

//Globals
int currentHour = 0;
int currentMinute = 0;
int time = 0;
int change = 0;
int timer = 0;
DateTime timerStartedAt;

enum DisplayState {
  DisplayClock,
  DisplayClockSet,
  DisplayTimer,
  DisplayTimerSet,
  Debug
};

DisplayState displayState = DisplayClock;
long lastStateChange = 0;

void changeDisplayState(DisplayState newValue) {
  displayState = newValue;
  lastStateChange = millis();
}

void pressHandler(BfButton* btn, BfButton::press_pattern_t pattern) {
  switch (pattern) {
    case BfButton::SINGLE_PRESS:
      switch (displayState) {
        case DisplayClock:
          changeDisplayState(DisplayTimerSet);
          break;
        case DisplayClockSet:
          rtc.adjust(DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), currentHour, currentMinute, 0));
          changeDisplayState(DisplayClock);
          break;
        case DisplayTimerSet:
          ring();
          timerStartedAt = rtc.now();
          changeDisplayState(DisplayTimer);
          break;
        case DisplayTimer:
          ring();
          changeDisplayState(DisplayClock);
          break;
      }
      break;

    case BfButton::LONG_PRESS:
      if (displayState == DisplayClock) {
        changeDisplayState(DisplayClockSet);
      }
      break;
  }
}


long millisSinceStateChange() {
  return millis() - lastStateChange;
}

void clockState() {
  DateTime now = rtc.now();
  currentHour = now.hour();
  currentMinute = now.minute();
  time = now.hour() * 100 + now.minute();
  if (hourlyChimeEnabled) {
    hourlyChime(now);
  }
  sevseg.setNumber(time, blink(now.second()));
}

void hourlyChime(DateTime now) {
  int chimeTimes = now.hour();
  if (chimeTimes > 12){
    chimeTimes = chimeTimes - 12;
  }
  if (now.hour() >= hourlyChimeStart && now.hour() <= hourlyChimeEnd) {
    if (now.minute() == 0 && !bellRungThisHour) {
      for (int i = 0; i < chimeTimes; i++) {
        ring();
        //This is a stupid way to implement the chime because the delay causes the display to wig out
        //fortunately, because time is set by the RTC module, the actual time is not delayed.
        delay(2000);
      }
      bellRungThisHour = true;
    } else if (now.minute() > 0) {
      bellRungThisHour = false;
    }
  }
}

void clockSetState() {
  DateTime now = rtc.now();
  change = encoder.getDirection();
  int* result = ClockTimeMath::calculateClockTime(currentHour, currentMinute, change);
  currentHour = result[0];
  currentMinute = result[1];
  time = currentHour * 100 + currentMinute;
  sevseg.setNumber(time, blinkInterval(100));
}

int blink(int seconds) {
  bool even = seconds % 2 == 0;
  if (even) {
    return 2;
  } else {
    return -1;
  }
}

int blinkInterval(int interval) {
  unsigned long currentMillis = millis();

  if (currentMillis / interval % 2 == 0) {
    return 2;
  } else {
    return -1;
  }
}

void timerSetState() {
  timer = encoder.readRange(1, 60);
  sevseg.setNumber(timer, -1);
}

void timerState() {
  DateTime now = rtc.now();
  TimeSpan elapsed = now - timerStartedAt;
  int minutesRemaining = timer - elapsed.minutes();
  sevseg.setNumber(minutesRemaining, blink(now.second()));
  if (elapsed.totalseconds() >= timer * 60){
    ring();
    timer = 0;
    changeDisplayState(DisplayClock);
  }
}

void ring() {
  digitalWrite(solenoidPin, HIGH);      //Switch Solenoid ON
  delay(10);                          //Short delay
  digitalWrite(solenoidPin, LOW); 
}

void setup() {
  //Serial.begin(9600);
  //RTC Config
  Wire.begin();
  rtc.begin();

  //Button config
  btn.onPress(pressHandler)
    .onPressFor(pressHandler, 1000);

  //Solenoid config
  pinMode(solenoidPin, OUTPUT);  
  
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(10);
}



void loop() {
  sevseg.refreshDisplay();

  btn.read();
  switch (displayState) {
    case DisplayClock:
      clockState();
      break;

    case DisplayClockSet:
      clockSetState();
      break;

    case DisplayTimerSet:
      timerSetState();
      break;
    case DisplayTimer:
      timerState();
      break;
  }
}
