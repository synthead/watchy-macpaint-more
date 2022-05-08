#include "macpaint-more.h"

#define TIME_POS_X 30
#define TIME_POS_Y 43

#define DATE_POS_X 70
#define DATE_POS_Y 114

#define BATTERY_POS_X 147
#define BATTERY_POS_Y 130

#define WEATHER_ICON_POS_X 45
#define WEATHER_ICON_POS_Y 118

#define TEMPERATURE_POS_X 100
#define TEMPERATURE_POS_Y 148

#define BATTERY_SEGMENT_WIDTH 7
#define BATTERY_SEGMENT_HEIGHT 11
#define BATTERY_SEGMENT_SPACING 9

#define WEATHER_ICON_WIDTH 48
#define WEATHER_ICON_HEIGHT 32

const unsigned char *numbers[10] = {numbers0, numbers1, numbers2, numbers3, numbers4, numbers5, numbers6, numbers7, numbers8, numbers9};

void WatchyMacPaintMore::drawWatchFace() {
  clearScreen();

  drawBackground();
  drawTime();
  drawDate();
  drawBattery();
  drawWeather();
}

void WatchyMacPaintMore::clearScreen() {
  display.fillScreen(GxEPD_WHITE);
}

void WatchyMacPaintMore::drawBackground() {
  display.drawBitmap(0, 0, window, DISPLAY_WIDTH, DISPLAY_HEIGHT, GxEPD_BLACK);
}

void WatchyMacPaintMore::drawTime() {
  uint8_t hour = currentTime.Hour % 12;

  if (hour == 0) {
    hour = 12;
  }

  if (hour >= 10) {
    // First hour digit.
    display.drawBitmap(TIME_POS_X, TIME_POS_Y, numbers[1], 38, 50, GxEPD_BLACK);
  }

  // Second hour digit.
  display.drawBitmap(TIME_POS_X + 35, TIME_POS_Y, numbers[hour % 10], 38, 50, GxEPD_BLACK);

  // Colon.
  display.drawBitmap(TIME_POS_X + 65, TIME_POS_Y + 10, colon, 11, 31, GxEPD_BLACK);

  // First minute digit.
  display.drawBitmap(TIME_POS_X + 80, TIME_POS_Y, numbers[currentTime.Minute / 10], 38, 50, GxEPD_BLACK);

  // Second minute digit.
  display.drawBitmap(TIME_POS_X + 118, TIME_POS_Y, numbers[currentTime.Minute % 10], 38, 50, GxEPD_BLACK);
}

void WatchyMacPaintMore::drawBattery() {
  display.drawBitmap(BATTERY_POS_X, BATTERY_POS_Y, battery, 37, 21, GxEPD_BLACK);
  display.fillRect(BATTERY_POS_X + 5, BATTERY_POS_Y + 5, 27, BATTERY_SEGMENT_HEIGHT, GxEPD_WHITE);

  float voltage = getBatteryVoltage();
  uint8_t batteryLevel;

  if (voltage > 4.1) {
    batteryLevel = 3;
  } else if (voltage > 3.95) {
    batteryLevel = 2;
  } else if (voltage > 3.80) {
    batteryLevel = 1;
  } else {
    batteryLevel = 0;
  }

  for (uint8_t segment = 0; segment < batteryLevel; segment++) {
    uint8_t posX = BATTERY_POS_X + 5 + (segment * BATTERY_SEGMENT_SPACING);
    uint8_t posY = BATTERY_POS_Y + 5;

    display.fillRect(posX, posY, BATTERY_SEGMENT_WIDTH, BATTERY_SEGMENT_HEIGHT, GxEPD_BLACK);
  }
}

void WatchyMacPaintMore::drawDate() {
  display.setFont(&Teko_Regular12pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(DATE_POS_X, DATE_POS_Y);

  char dateText[15];

  sprintf(
    dateText,
    "%s %04u-%02u-%02u",
    dayShortStr(currentTime.Wday),
    currentTime.Year + 1970,
    currentTime.Month,
    currentTime.Day
  );

  display.print(dateText);
}

void WatchyMacPaintMore::drawWeather() {
  weatherData currentWeather = getWeatherData();

  display.setFont(&Teko_Regular12pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(TEMPERATURE_POS_X, TEMPERATURE_POS_Y);

  display.print(currentWeather.temperature);
  display.print(currentWeather.isMetric ? "C" : "F");

  // https://openweathermap.org/weather-conditions

  uint16_t condition = currentWeather.weatherConditionCode;
  const unsigned char* weatherIcon;

  if (condition > 801) {
    weatherIcon = cloudy;
  } else if (condition == 801) {
    weatherIcon = cloudsun;
  } else if (condition == 800) {
    weatherIcon = sunny;
  } else if (condition >= 700) {
    weatherIcon = atmosphere;
  } else if (condition >= 600) {
    weatherIcon = snow;
  } else if (condition >= 500) {
    weatherIcon = rain;
  } else if (condition >= 300) {
    weatherIcon = drizzle;
  } else if (condition >= 200) {
    weatherIcon = thunderstorm;
  } else {
    return;
  }

  display.drawBitmap(WEATHER_ICON_POS_X, WEATHER_ICON_POS_Y, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, GxEPD_BLACK);
}
