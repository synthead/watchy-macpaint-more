#ifndef MACPAINT_MORE_H
#define MACPAINT_MORE_H

#include <Watchy.h>
#include "macpaint-graphics.h"
#include "icons.h"
#include "Teko_Regular12pt7b.h"

class WatchyMacPaintMore : public Watchy {
  using Watchy::Watchy;

  public:
    void drawWatchFace();
    void clearScreen();
    void drawBackground();
    void drawTime();
    void drawDate();
    void drawBattery();
    void drawWeather();
};

#endif
