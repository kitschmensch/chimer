# chimer
A meditation timer and hourly chimer powered by an Arduino UNO R3 and a little solenoid.

**Note: this is my very first Arduino project. Code and diagram may have problems!!**

<img src="https://i.imgur.com/JGQ50rE.jpg" width="500">

## Motivation

There are many smartphone applications that provide meditation timers, but when I am meditating I don't want a phone near me as it would act as a potential distraction. Other timers available use horrendously loud electronic beepers and buzzers. There are analog wind up timers, but these tick, and the wirring bell sound is equally unpleasant. There are some "analog" meditation timers on the market, but they are expensive and seemingly unreliable.

## Usage


To start a meditation timer, click the rotary encoder once. Set a timer by turning the rotary encoder, and click again to start. Click again to cancel the timer and return to the clock.

Click the rotary encoder for 2 seconds to set the time. Click again to confirm.


By default the device will strike the hours on the hour, starting at 6am and ending at 9pm. The hourly range, and whether the hourly chime is enabled, can be set using variables.
```
//Hourly Chime Settings
const bool hourlyChimeEnabled = true;
const int hourlyChimeStart = 6;
const int hourlyChimeEnd = 21;
```

## Parts:
- [Arduino Uno R3 (I use an Elegoo copycat)](https://www.amazon.com/ELEGOO-Board-ATmega328P-ATMEGA16U2-Compliant/dp/B01EWOE0UU/ref=sr_1_3?crid=1D2D6GBO0V7WZ&keywords=r3+uno&qid=1697415380&sprefix=r3+uno%2Caps%2C123&sr=8-3)
- [4-digit 7-segment display](https://protosupplies.com/product/led-7-segment-0-36-cc-4-digit/)
- 5, 220 Ω resistors
- 1 diode
- 1 NPN transistor
- [5V Solenoid](https://www.adafruit.com/product/2776)
- [DS1207 with breakout board](https://www.adafruit.com/product/264)
- [KY-040 rotary encoder with breakout board](https://www.amazon.com/Cylewet-Encoder-15%C3%9716-5-Arduino-CYT1062/dp/B06XQTHDRR)
- [A cheap, miniature tibetan singing bowl](https://www.amazon.com/dp/B08RP81QXR?psc=1&ref=ppx_yo2ov_dt_b_product_details)

## Library Dependencies
- [ButtonFever](https://github.com/mickey9801/ButtonFever)
- [SevSegShift](https://github.com/bridystone/SevSegShift)

## Planned enhancements:
- [ ] Disable display when there is no user input for several seconds.
- [ ] Disable/enable hourly chime via user input.
- [ ] Disable onboard LEDs and LEDs on breakout boards. (Too bright!!)
- [ ] Set start and end times for the hourly chime via user input.
- [ ] Set an alarm.
- [ ] Set an interval chime during a meditation timer (e.g. 20 minute timer, ring every 5 minutes).
- [ ] Use a [display with a backpack](https://www.adafruit.com/product/879) to save pin space. 
- [ ] Use an Arduino Nano!
- [ ] Design a custom PCB shield for the Nano.
- [ ] Design and manufacture an enclosure.

## Acknowledgments
The structure of the code is heavily influenced by [alarm-clock.ino](https://wokwi.com/projects/297787059514376717) by @urish.
Many thanks to all the kind people on Arduino Forums and on YouTube that provide so much educational content.
