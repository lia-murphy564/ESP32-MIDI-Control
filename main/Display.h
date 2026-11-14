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


private:
  const uint8_t *bmp_ = nullptr;
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