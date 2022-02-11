#ifndef DEVICE_NAME
#define DEVICE_NAME          "EMAX 900TX"
#endif

// There is some special handling for this target
#define TARGET_TX_EMAX_900

#define USE_OLED_I2C
#define OLED_REVERSED
#define HAS_FIVE_WAY_BUTTON

// GPIO pin definitions
#define GPIO_PIN_NSS            5
#define GPIO_PIN_DIO0           26
#define GPIO_PIN_DIO1           25
#define GPIO_PIN_MOSI           23
#define GPIO_PIN_MISO           19
#define GPIO_PIN_SCK            18
#define GPIO_PIN_RST            14
#define GPIO_PIN_RX_ENABLE      13
#define GPIO_PIN_TX_ENABLE      12
#define GPIO_PIN_RCSIGNAL_RX    2
#define GPIO_PIN_RCSIGNAL_TX    2 // so we don't have to solder the extra resistor, we switch rx/tx using gpio mux
// #define GPIO_PIN_LED_WS2812     27
#define GPIO_PIN_FAN_EN         17
#define GPIO_PIN_RFamp_APC2     25
#define GPIO_PIN_OLED_SDA       22
#define GPIO_PIN_OLED_SCK       21
#define GPIO_PIN_OLED_RST       U8X8_PIN_NONE
#define GPIO_PIN_JOYSTICK       33

// Output Power
#define MinPower                PWR_10mW
#define HighPower               PWR_250mW
#define MaxPower                PWR_2000mW
#define POWER_OUTPUT_DACWRITE
#define POWER_OUTPUT_VALUES     {0,5,15,25,45,105,150,225}

/* Joystick values              {UP, DOWN, LEFT, RIGHT, ENTER, IDLE}*/
#define JOY_ADC_VALUES          {1650, 1070, 580, 2170, 0, 3240}

// #define WS2812_IS_GRB