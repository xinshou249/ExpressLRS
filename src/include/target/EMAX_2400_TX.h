#ifndef DEVICE_NAME
#define DEVICE_NAME          "EMAX 2400TX"
#endif

// There is some special handling for this target
#define TARGET_TX_EMAX_2400_V1

// GPIO pin definitions
#define GPIO_PIN_NSS            5
#define GPIO_PIN_BUSY           21
#define GPIO_PIN_DIO0           -1
#define GPIO_PIN_DIO1           4
#define GPIO_PIN_MOSI           23
#define GPIO_PIN_MISO           19
#define GPIO_PIN_SCK            18
#define GPIO_PIN_RST            14
#define GPIO_PIN_RX_ENABLE      27
#define GPIO_PIN_TX_ENABLE      26
#define GPIO_PIN_RCSIGNAL_RX    13
#define GPIO_PIN_RCSIGNAL_TX    13
#define GPIO_PIN_LED_BLUE       17
#define GPIO_PIN_LED_GREEN      16
//#define GPIO_PIN_BUTTON         25
#define GPIO_PIN_FAN_EN         22
#define GPIO_PIN_OLED_SDA       12
#define GPIO_PIN_OLED_SCK       15
#define GPIO_PIN_OLED_RST       U8X8_PIN_NONE
#define GPIO_PIN_KEY_UP         33
#define GPIO_PIN_KEY_DOWN       32
#define GPIO_PIN_KEY_LEFT       34
#define GPIO_PIN_KEY_RIGHT      25
#define GPIO_PIN_KEY_ENTER      35

// Output Power
#define MinPower PWR_10mW
#define MaxPower PWR_1000mW
#define POWER_OUTPUT_VALUES {-19,-15,-12,-9,-4,-1,3}

#define Regulatory_Domain_ISM_2400 1
