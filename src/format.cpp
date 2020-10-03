#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  int hour, min, sec;
  std::string s_hour, s_min, s_sec;
  
  hour = seconds / 3600;
  seconds = seconds % 3600;
  min = seconds / 60;
  sec = seconds % 60;
  
  if (hour == 0) {
    s_hour = "00";
  } else if (hour < 10) {
    s_hour = "0" + std::to_string(hour);
  } else {
    s_hour = std::to_string(hour);
  }

  if (min == 0) {
    s_min = "00";
  } else if (min < 10) {
    s_min = "0" + std::to_string(min);
  } else {
    s_min = std::to_string(min);
  }
  
  if (sec == 0) {
    s_sec = "00";
  } else if (sec < 10) {
    s_sec = "0" + std::to_string(sec);
  } else {
    s_sec = std::to_string(sec);
  }
  
  return s_hour + ":" + s_min + ":" + s_sec;
}