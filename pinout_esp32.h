#pragma once

enum class PinMode : uint8_t
{
  Input         = INPUT,
  Output        = OUTPUT,
  InputPullup   = INPUT_PULLUP,
  InputPulldown = INPUT_PULLDOWN,
};

enum class PinAddress : uint8_t
{
  GPIO_0  = 0,   // strapping, BOOT button
  GPIO_1  = 1,   // UART0 TX (USB serial)
  GPIO_2  = 2,   // strapping, onboard LED
  GPIO_3  = 3,   // UART0 RX (USB serial)
  GPIO_4  = 4,
  GPIO_5  = 5,   // strapping, SPI SS
  GPIO_12 = 12,  // strapping, must be LOW at boot
  GPIO_13 = 13,
  GPIO_14 = 14,
  GPIO_15 = 15,  // strapping
  GPIO_16 = 16,  // UART2 RX
  GPIO_17 = 17,  // UART2 TX
  GPIO_18 = 18,  // SPI SCK
  GPIO_19 = 19,  // SPI MISO
  GPIO_21 = 21,  // I2C SDA
  GPIO_22 = 22,  // I2C SCL
  GPIO_23 = 23,  // SPI MOSI
  GPIO_25 = 25,  // DAC1
  GPIO_26 = 26,  // DAC2
  GPIO_27 = 27,
  GPIO_32 = 32,
  GPIO_33 = 33,
  GPIO_34 = 34,
  GPIO_35 = 35,
  GPIO_36 = 36,
  GPIO_39 = 39,
  
  // Onboard
  LED_INTERNAL = GPIO_2,
  BOOT_BUTTON  = GPIO_0,

  // UART0 (USB serial)
  UART0_TX = GPIO_1,
  UART0_RX = GPIO_3,
 
  // UART2
  UART2_TX = GPIO_17,
  UART2_RX = GPIO_16,
 
  // I2C
  I2C_SDA = GPIO_21,
  I2C_SCL = GPIO_22,
 
  // SPI (VSPI)
  SPI_MOSI = GPIO_23,
  SPI_MISO = GPIO_19,
  SPI_SCK  = GPIO_18,
  SPI_SS   = GPIO_5,
 
  // DAC
  DAC1 = GPIO_25,
  DAC2 = GPIO_26,
 
  // Input-only sensor pins
  SENSOR_VP = GPIO_36,
  SENSOR_VN = GPIO_39,

    // ADC1 (works with WiFi on)
  ADC1_0 = GPIO_36,
  ADC1_3 = GPIO_39,
  ADC1_4 = GPIO_32,
  ADC1_5 = GPIO_33,
  ADC1_6 = GPIO_34,
  ADC1_7 = GPIO_35,
 
  // ADC2 (unusable while WiFi is active)
  ADC2_0 = GPIO_4,
  ADC2_1 = GPIO_0,
  ADC2_2 = GPIO_2,
  ADC2_3 = GPIO_15,
  ADC2_4 = GPIO_13,
  ADC2_5 = GPIO_12,
  ADC2_6 = GPIO_14,
  ADC2_7 = GPIO_27,
  ADC2_8 = GPIO_25,
  ADC2_9 = GPIO_26,
};