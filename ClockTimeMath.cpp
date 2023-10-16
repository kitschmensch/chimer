#include "ClockTimeMath.h"

int* ClockTimeMath::calculateClockTime(int hour, int minute, int change) {
    static int result[2];
    // Ensure the hour is between 0 and 23
    if (hour < 0) {
        hour = 0;
    } else if (hour > 23) {
        hour = 23;
    }

    // Ensure the minute is between 0 and 59
    if (minute < 0) {
        minute = 0;
    } else if (minute > 59) {
        minute = 59;
    }

    // Calculate the new minute value
    int newMinute = minute + change;

    // Handle minute rollovers
    if (newMinute < 0) {
        newMinute = 59;
        hour = (hour - 1 + 24) % 24; // Handle hour rollover as well
    } else if (newMinute > 59) {
        newMinute = 0;
        hour = (hour + 1) % 24; // Handle hour rollover
    }
    result[0] = hour;
    result[1] = newMinute;
    return result;
}
