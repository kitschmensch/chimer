#ifndef KY040Encoder_h
#define KY040Encoder_h

#include <Arduino.h>

class KY040Encoder {
public:
  KY040Encoder(int clkPin, int dtPin, int swPin);
  void begin();
  int read();
  bool buttonPressed();
  int readRange(int minVal, int maxVal, bool loop = false);
  void setPosition(int newPosition);
  int getDirection();
private:
  int clkPin;
  int dtPin;
  int swPin;
  int encoderPos;
  int lastEncoderPos;
  bool lastCLKState;
};

#endif
