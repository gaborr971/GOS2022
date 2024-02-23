#include <drv_tmr.h>
#include "stm32f4xx_hal.h"

#define DRV_TMR_MAX_TIMEOUT_TIMERS ( 2u )

GOS_STATIC TIM_HandleTypeDef htim3  = {0};

GOS_STATIC drv_tmrTimeoutTimerDesc_t timeoutTimers [DRV_TMR_MAX_TIMEOUT_TIMERS];

gos_result_t drv_tmrTimeoutTimerInit (void_t)
{
	gos_result_t timeoutTimerInitResult = GOS_ERROR;
    /*
     * Function code.
     */
    __HAL_RCC_TIM3_CLK_ENABLE();
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim3.Instance               = TIM3;
    htim3.Init.Prescaler         = 50-1;
    htim3.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim3.Init.Period            = 1000-1;
    htim3.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_NVIC_EnableIRQ(TIM3_IRQn);

    if (HAL_TIM_Base_Init(&htim3)  == HAL_OK)
    {
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) == HAL_OK &&
        		HAL_TIM_Base_Start_IT(&htim3) == HAL_OK)
        {
        	timeoutTimerInitResult = GOS_SUCCESS;
        }
    }

    return timeoutTimerInitResult;
}

gos_result_t drv_tmrRegisterTimeoutTimer (drv_tmrTimeoutTimerDesc_t* pDesc)
{
	u8_t index;

	for (index = 0u; index < DRV_TMR_MAX_TIMEOUT_TIMERS; index++)
	{
		if (timeoutTimers[index].callback == NULL)
		{
			timeoutTimers[index].callback  = pDesc->callback;
			timeoutTimers[index].timeoutMs = pDesc->timeoutMs;
			timeoutTimers[index].ticks     = 0u;
			timeoutTimers[index].enabled   = GOS_FALSE;
			pDesc->id = index;
			return GOS_SUCCESS;
		}
	}

	return GOS_ERROR;
}

gos_result_t drv_tmrUnregisterTimeoutTimer (drv_tmrTimeoutTimerId id)
{
	if (timeoutTimers[id].callback != NULL)
	{
		timeoutTimers[id].callback  = NULL;
		timeoutTimers[id].timeoutMs = 0u;
		timeoutTimers[id].ticks     = 0u;
		timeoutTimers[id].enabled   = GOS_FALSE;
		return GOS_SUCCESS;
	}
	return GOS_ERROR;
}

gos_result_t drv_tmrStartTimeoutTimer (drv_tmrTimeoutTimerId id)
{
	if (id < DRV_TMR_MAX_TIMEOUT_TIMERS && timeoutTimers[id].enabled == GOS_FALSE)
	{
		timeoutTimers[id].ticks   = 0;
		timeoutTimers[id].enabled = GOS_TRUE;
	}
}

gos_result_t drv_tmrResetTimeoutTimer (drv_tmrTimeoutTimerId id)
{
	if (id < DRV_TMR_MAX_TIMEOUT_TIMERS && timeoutTimers[id].enabled == GOS_TRUE)
	{
		timeoutTimers[id].ticks   = 0;
	}
}

void_t TIM3_IRQHandler (void_t)
{
	HAL_TIM_IRQHandler(&htim3);
}

void_t HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim3)
	{
		for (u8_t index = 0u; index < DRV_TMR_MAX_TIMEOUT_TIMERS; index++)
		{
			if (timeoutTimers[index].enabled == GOS_TRUE)
			{
				timeoutTimers[index].ticks++;

				if (timeoutTimers[index].ticks == timeoutTimers[index].timeoutMs)
				{
					timeoutTimers[index].enabled = GOS_FALSE;

					if (timeoutTimers[index].callback != NULL)
					{
						timeoutTimers[index].callback();
					}
				}
			}
		}
	}
}
