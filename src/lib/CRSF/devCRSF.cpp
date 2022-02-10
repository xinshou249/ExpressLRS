#include "targets.h"
#include "common.h"
#include "device.h"

#include "CRSF.h"

#ifdef CRSF_TX_MODULE
static int start()
{
    CRSF::Begin();
#if defined(DEBUG_TX_FREERUN)
    CRSF::CRSFstate = true;
    extern void UARTconnected();
    UARTconnected();
#endif
    return DURATION_IMMEDIATELY;
}

static int timeout()
{
    CRSF::handleUARTin();
    return DURATION_IMMEDIATELY;
}

device_t CRSF_device = {
    .initialize = nullptr,
    .start = start,
    .event = nullptr,
    .timeout = timeout
};
#endif
