#include "imgui.h"
#include <vector>
#include <string>
#include <cstdarg>
#include <sys/time.h>  // For gettimeofday()
#include <time.h>
#include <math.h>
#include <unistd.h>

static bool logsEnabled = false;
static bool viewLogWindow = false;

// Log storage
std::vector<std::string> logMessages;


const char* secsToDatetime(long int* tv_sec) {
    // Number of seconds in each month for a non-leap year
    const int days_in_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Constants
    const int SECONDS_IN_A_DAY = 86400; // 24 * 60 * 60
    const int SECONDS_IN_A_HOUR = 3600; // 60 * 60
    const int SECONDS_IN_A_MINUTE = 60; // 60

    // Dereference tv_sec to get the time value in seconds
    long int seconds = *tv_sec;

    // Assume UTC+1 (CET) as default, add 1 hour for DST (CEST)
    // Adjust for daylight saving time (DST) by adding 1 hour (3600 seconds) from March to October
    const int timezone_offset_seconds = 1 * SECONDS_IN_A_HOUR; // CET (UTC+1)
    
    // Adjust the seconds by adding the timezone offset
    seconds += timezone_offset_seconds;

    // Calculate year
    int year = 1970;
    while (seconds >= SECONDS_IN_A_DAY * (365 + (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)))) {
        seconds -= SECONDS_IN_A_DAY * (365 + (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))); 
        year++;
    }

    // Calculate month and day
    int month = 0;
    while (seconds >= SECONDS_IN_A_DAY * days_in_months[month] && month < 12) {
        // Adjust for February during leap years
        if (month == 1 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) {
            // Add an extra day for leap year February
            if (seconds >= SECONDS_IN_A_DAY * (days_in_months[month] + 1)) {
                seconds -= SECONDS_IN_A_DAY * (days_in_months[month] + 1);
                month++;
            } else {
                break;
            }
        } else {
            seconds -= SECONDS_IN_A_DAY * days_in_months[month];
            month++;
        }
    }

    int day = seconds / SECONDS_IN_A_DAY;
    seconds %= SECONDS_IN_A_DAY;

    // Calculate hours, minutes, seconds
    int hour = seconds / SECONDS_IN_A_HOUR;
    seconds %= SECONDS_IN_A_HOUR;
    int minute = seconds / SECONDS_IN_A_MINUTE;
    seconds %= SECONDS_IN_A_MINUTE;
    int second = seconds;

    // Prepare formatted string
    static char formattedTime[80];
    snprintf(formattedTime, sizeof(formattedTime), "%04d:%02d:%02d %02d:%02d:%02d", 
             year, month + 1, day + 1, hour, minute, second);

    return formattedTime;
}


// Function to add messages to log
void AddLog(const char* fmt, ...) {

  if (!logsEnabled) return;

  struct timeval tv;
  gettimeofday(&tv, NULL);

  int millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
  if (millisec>=1000) { // Allow for rounding up to nearest second
    millisec -=1000;
    tv.tv_sec++;
  }
  const char* dateTime = secsToDatetime(&tv.tv_sec);

  char timeStrMs[32];
  snprintf(timeStrMs, sizeof(timeStrMs), "%s.%03d | ", dateTime, millisec);

  char msg[512];  // Buffer for message formatting
  va_list args;
  va_start(args, fmt);
  vsnprintf(msg, sizeof(msg), fmt, args);
  va_end(args);

  char buffer[576]; // Enough space for timeStr + buf
  snprintf(buffer, sizeof(buffer), "%s%s", timeStrMs, msg);
  logMessages.push_back(buffer);  // Store log message
}

void AddLogVector2(const char* name, Vector2& v) {
  
  AddLog("%s: [x=%.1f, y=%.1f]\n", name, v.x, v.y);
}
// ImGui Log Window
void ShowLogWindow() {
  if (ImGui::Begin("Log Console")) {
    if (ImGui::Button("Clear Log")) logMessages.clear();

    ImGui::BeginChild("LogRegion", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

    // Display each log message
    for (const auto& msg : logMessages) {
      ImGui::TextUnformatted(msg.c_str());
    }

    // Auto-scroll to bottom
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
      ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();
  }
  ImGui::End();
}
