#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


class DisplayData
{
public:
  virtual ~DisplayData() {};
  virtual void draw(Adafruit_GFX &gfx) 
  {
    if (bmp_)
        gfx.drawBitmap(0, 0, bmp_, gfx.width(), gfx.height(), 1);
  };

  void setBitmap(const uint8_t *bmp) { bmp_ = bmp; }

  // TODO: Remove and write a better version
  void setLine(uint8_t i, const char *text, uint8_t size = 1)
  {
    if (i >= 4) return;
    strncpy(lines_[i], text, 22 - 1);
    lines_[i][22 - 1] = '\0';
    sizes_[i] = size;
  }

protected:
  char    lines_[4][22] = {};
  uint8_t sizes_[22] = {1, 1, 1, 1};


private:
  const uint8_t *bmp_ = nullptr;
};

class DisplayDebug : public DisplayData
{
public:
  DisplayDebug() { setLine(0, "L1A - DEBUG"); }

  void draw(Adafruit_GFX &gfx) override
  {
    gfx.setTextColor(1);
    int16_t y = 0;
    for (uint8_t i = 0; i < 4; i++) {
      if (lines_[i][0] == '\0') continue;
      gfx.setTextSize(sizes_[i]);
      gfx.setCursor(0, y);
      gfx.print(lines_[i]);
      y += 8 * sizes_[i];
    }
  }

  void setButtons(bool btn1, bool btn2)
  {
    char buf[22];
    snprintf(buf, sizeof(buf), "BTN1:%d  BTN2:%d", btn1, btn2);
    setLine(1, buf);
  }

  void setPot(int value)
  {
    char buf[22];
    snprintf(buf, sizeof(buf), "POT:%d", value);
    setLine(2, buf);
  }

  void setLed(bool on)
  {
    setLine(3, on ? "LED:ON" : "LED:OFF");
  }
};

// class BitmapDisplay : public DisplayData
// {
// public:
//   explicit BitmapDisplay(const uint8_t *bmp) : bmp_(bmp) {}

// //   void setBitmap(const uint8_t *bmp) { bmp_ = bmp; }

//   void draw(Adafruit_GFX &gfx) override
//   {
//     gfx.drawBitmap(0, 0, bmp_, gfx.width(), gfx.height(), 1);
//   }

// };