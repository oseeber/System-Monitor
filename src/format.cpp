#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    long hours, minutes;
    string hourString, minString, secString, retString;

    hours = floor(seconds/3600);
    hourString = std::to_string(hours);
    if (hours < 10) {
        hourString = "0" + hourString;
    }
    minutes = floor(floor(seconds/60) - hours*60);
    minString = std::to_string(minutes);
    if (minutes < 10) {
        minString = "0" + minString;
    }
    seconds = floor(seconds - hours*3600 - minutes*60);
    secString = std::to_string(seconds);
    if (seconds < 10) {
        secString = "0" + secString;
    }

    retString = hourString + ":" + minString + ":" + secString;
    return retString;

 }