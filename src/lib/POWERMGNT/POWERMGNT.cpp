#include "POWERMGNT.h"
#include "DAC.h"
#include "targets.h"


#if defined(Regulatory_Domain_AU_915) || defined(Regulatory_Domain_EU_868) || defined(Regulatory_Domain_FCC_915) || defined(Regulatory_Domain_AU_433) || defined(Regulatory_Domain_EU_433)
extern SX127xDriver Radio;
#elif Regulatory_Domain_ISM_2400
extern SX1280Driver Radio;
#endif

PowerLevels_e POWERMGNT::CurrentPower = (PowerLevels_e)DefaultPowerEnum;

PowerLevels_e POWERMGNT::incPower()
{
    if (CurrentPower < MaxPower)
    {
        setPower((PowerLevels_e)((uint8_t)CurrentPower + 1));
    }
    return CurrentPower;
}

PowerLevels_e POWERMGNT::decPower()
{
    if (CurrentPower > 0)
    {
        setPower((PowerLevels_e)((uint8_t)CurrentPower - 1));
    }
    return CurrentPower;
}

PowerLevels_e POWERMGNT::currPower()
{
    return CurrentPower;
}

uint8_t POWERMGNT::powerToCrsfPower(PowerLevels_e Power)
{
    // Crossfire's power levels as defined in opentx:radio/src/telemetry/crossfire.cpp
    //static const int32_t power_values[] = { 0, 10, 25, 100, 500, 1000, 2000, 250 };
    switch (Power)
    {
    case PWR_10mW: return 1;
    case PWR_25mW: return 2;
    case PWR_100mW: return 3;
    case PWR_250mW: return 7;
    case PWR_500mW: return 4;
    case PWR_1000mW: return 5;
    case PWR_2000mW: return 6;
    case PWR_50mW:
    default:
        return 0;
    }
}

void POWERMGNT::init()
{
#if DAC_IN_USE
    TxDAC.init();
#endif // DAC_IN_USE
#ifdef TARGET_R9M_LITE_PRO_TX
    //initialize both 12 bit DACs
    pinMode(GPIO_PIN_RFamp_APC1, OUTPUT);
    pinMode(GPIO_PIN_RFamp_APC2, OUTPUT);
    analogWriteResolution(12);
#endif
#if defined(GPIO_PIN_FAN_EN) && (GPIO_PIN_FAN_EN != UNDEF_PIN)
    pinMode(GPIO_PIN_FAN_EN, OUTPUT);
#endif
    CurrentPower = PWR_COUNT;
}

void POWERMGNT::setDefaultPower()
{
    setPower((PowerLevels_e)DefaultPowerEnum);
}

PowerLevels_e POWERMGNT::setPower(PowerLevels_e Power)
{
    if (Power == CurrentPower)
        return CurrentPower;

    if (Power > MaxPower)
    {
        Power = (PowerLevels_e)MaxPower;
    }

#ifdef GPIO_PIN_FAN_EN
    digitalWrite(GPIO_PIN_FAN_EN, (Power >= PWR_250mW) ? HIGH : LOW);
#endif

#if DAC_IN_USE
    // DAC is used e.g. for R9M, ES915TX and Voyager
    Radio.SetOutputPower(0b0000);
    TxDAC.setPower((DAC_PWR_)Power);

#elif defined(POWER_250mw_Voltage1)
    Radio.SetOutputPower(0b0000);
    //Set DACs PA5 & PA4
    switch (Power)
    {
    case PWR_250mW:
        analogWrite(GPIO_PIN_RFamp_APC1, POWER_250mw_Voltage1); //0-4095 2.7V
        analogWrite(GPIO_PIN_RFamp_APC2, POWER_250mw_Voltage2); //0-4095 870mV this is actually 200mw
        CurrentPower = PWR_250mW;
        break;
    case PWR_500mW:
        analogWrite(GPIO_PIN_RFamp_APC1, POWER_500mw_Voltage1); //0-4095 2.7V
        analogWrite(GPIO_PIN_RFamp_APC2, POWER_500mw_Voltage2); //0-4095 1.093V
        CurrentPower = PWR_500mW;
        break;
    case PWR_1000mW:
        analogWrite(GPIO_PIN_RFamp_APC1, POWER_1000mw_Voltage1); //0-4095 2.7V
        analogWrite(GPIO_PIN_RFamp_APC2, POWER_1000mw_Voltage2); //0-4095 1.493V
        CurrentPower = PWR_1000mW;
        break;
    case PWR_100mW:
    default:
        CurrentPower = PWR_100mW;
        analogWrite(GPIO_PIN_RFamp_APC1, POWER_100mw_Voltage1); //0-4095 2.7V
        analogWrite(GPIO_PIN_RFamp_APC2, POWER_100mw_Voltage2);  //0-4095 590mV
        break;
    }
#elif defined(TARGET_TX)
    switch (Power)
    {
    case PWR_10mW:
        Radio.SetOutputPower(POWER_10mw_VALUE);
        break;
    case PWR_25mW:
        Radio.SetOutputPower(POWER_25mw_VALUE);
        break;
    case PWR_50mW:
        Power = PWR_50mW;
        Radio.SetOutputPower(POWER_50mw_VALUE);
        break;
    case PWR_100mW:
        #ifdef POWER_100mw_VALUE
        Radio.SetOutputPower(POWER_100mw_VALUE);
        #endif
        break;
    case PWR_250mW:
        #ifdef POWER_250mw_VALUE
        Radio.SetOutputPower(POWER_250mw_VALUE);
        #endif
        break;
    case PWR_500mW:
        #ifdef POWER_500mW_VALUE
        Radio.SetOutputPower(POWER_500mw_VALUE); 
        #endif
        break;
    case PWR_1000mW:
        #if defined(POWER_1000mW_VALUE)
        Radio.SetOutputPower(POWER_1000mw_VALUE);
        #endif 
        break;
    default: 
        switch (DefaultPowerEnum)
        {
            case PWR_10mW:
                Radio.SetOutputPower(POWER_10mw_VALUE);
                break;
            case PWR_25mW:
                Radio.SetOutputPower(POWER_25mw_VALUE);
                break;
            case PWR_50mW:
                Radio.SetOutputPower(POWER_50mw_VALUE);
                break;
        }
    
    }

#elif defined(TARGET_RX)
    #ifdef TARGET_SX1280
        Radio.SetOutputPower(13); //default is max power (12.5dBm for SX1280 RX)
    #else
        Radio.SetOutputPower(0b1111); //default is max power (17dBm for SX127x RX@)
    #endif
#else
    #error "[ERROR] Unknown power management!"
#endif
    CurrentPower = Power;
    return Power;
}
