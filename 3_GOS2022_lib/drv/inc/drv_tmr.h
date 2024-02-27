/*
 * Includes
 */
#include <gos.h>

/*
 * Type definitions
 */
/**
 * TODO
 */
typedef void_t (*drv_tmrTimeoutTimerElapsedCallback_t)(void_t);

/**
 * TODO
 */
typedef u8_t drv_tmrTimeoutTimerId;

/**
 * TODO
 */
typedef struct
{
    u32_t timeoutMs;
    u32_t ticks;
    bool_t enabled;
    drv_tmrTimeoutTimerId id;
    drv_tmrTimeoutTimerElapsedCallback_t callback;
}drv_tmrTimeoutTimerDesc_t;

/*
 * Function prototypes
 */
gos_result_t drv_tmrTimeoutTimerInit (void_t);

gos_result_t drv_tmrRegisterTimeoutTimer (drv_tmrTimeoutTimerDesc_t* pDesc);

gos_result_t drv_tmrUnregisterTimeoutTimer (drv_tmrTimeoutTimerId id);

gos_result_t drv_tmrStartTimeoutTimer (drv_tmrTimeoutTimerId id);

gos_result_t drv_tmrResetTimeoutTimer (drv_tmrTimeoutTimerId id);
