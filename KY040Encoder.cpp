#include "KY040Encoder.h"

KY040Encoder::KY040Encoder(int clkPin, int dtPin, int swPin)
  : clkPin(clkPin), dtPin(dtPin), swPin(swPin), encoderPos(0), lastEncoderPos(0), lastCLKState(LOW) {}

void KY040Encoder::begin() {
  pinMode(clkPin, INPUT_PULLUP);
  pinMode(dtPin, INPUT_PULLUP);
  pinMode(swPin, INPUT_PULLUP);
}

int KY040Encoder::read() {
  boolean clkState = digitalRead(clkPin);

  if (clkState != lastCLKState) {
    if (digitalRead(dtPin) != clkState) {
      encoderPos--;
    } else {
      encoderPos++;
    }
  }

  lastCLKState = clkState;
  return encoderPos;
}

int KY040Encoder::readRange(int minVal, int maxVal, bool loop) {
  int pos = read();
  
  if (pos < minVal) {
    if (loop) {
      setPosition(maxVal);
      return maxVal;
    } else {
      encoderPos = minVal;
      return minVal;
    }
  } else if (pos > maxVal) {
    if (loop) {
      setPosition(minVal);
      return minVal;
    } else {
      encoderPos = maxVal;
      return maxVal;
    }
  }
  
  return pos;
}

int KY040Encoder::getDirection() {
  int pos = read();
  int direction = pos - lastEncoderPos;
  lastEncoderPos = pos;
  
  if (direction > 0) {
    return 1; // Clockwise
  } else if (direction < 0) {
    return -1; // Counter-clockwise
  } else {
    return 0; // No change
  }
}

void KY040Encoder::setPosition(int newPosition) {
  encoderPos = newPosition;
}

bool KY040Encoder::buttonPressed() {
  return (digitalRead(swPin) == LOW);
}
