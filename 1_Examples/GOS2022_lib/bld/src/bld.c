


/*
 * Includes
 */
#include <bld.h>
#include <bld_cfg.h>
#include <string.h>
#include <drv_crc.h>
#include "stm32f4xx.h"

/*
 * Type definitions
 */
typedef struct
{
    u32_t  initSp;;
    void_t (*resetHandler)(void_t);
}bld_boot_vect_table_t;

/*
 * Function: bld_getVersion
 */
gos_result_t bld_getVersion (lib_version_t* pBldVersion)
{
    /*
     * Local variables.
     */
    gos_result_t getVersionResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (pBldVersion != NULL)
    {
        pBldVersion->major       = BLD_VERSION_MAJOR;
        pBldVersion->minor       = BLD_VERSION_MINOR;
        pBldVersion->build       = BLD_VERSION_BUILD;
        pBldVersion->date.years  = BLD_VERSION_DATE_YEAR;
        pBldVersion->date.months = BLD_VERSION_DATE_MONTH;
        pBldVersion->date.days   = BLD_VERSION_DATE_DAY;

        (void_t) strcpy((char_t*)pBldVersion->name, BLD_NAME);
        (void_t) strcpy((char_t*)pBldVersion->description, BLD_DESCRIPTION);
        (void_t) strcpy((char_t*)pBldVersion->author, BLD_AUTHOR);

        getVersionResult = GOS_SUCCESS;
    }
    else
    {
        // Target is NULL.
    }

    return getVersionResult;
}

/*
 * Function: bld_checkApplication
 */
gos_result_t bld_checkApplication (bld_appData_t* pAppData, bool_t bootMode)
{
    /*
     * Local variables.
     */
    gos_result_t appCheckResult = GOS_ERROR;

    /*
     * Function code.
     */
    // Check application size.
    if (pAppData->size > 0u && pAppData->size <= BLD_APP_ROM_SIZE)
    {
    	// Start address must be the defined start address.
    	if (pAppData->startAddress == BLD_APP_ROM_START_ADDRESS)
    	{
            if (drv_crcCheckCrc32((u8_t*)pAppData->startAddress, pAppData->size, pAppData->crc, NULL) == DRV_CRC_CHECK_OK)
            {
                appCheckResult = GOS_SUCCESS;
            }
            else
            {
                // Application check fail.
            	if (bootMode == GOS_TRUE)
            	{
            		(void_t) gos_traceTrace(GOS_FALSE, "Application CRC error.\r\n");
            	}
            	else
            	{
                    (void_t) gos_traceTraceFormattedUnsafe("Application CRC error.\r\n");
            	}
            }
    	}
    	else
		{
    		if (bootMode == GOS_TRUE)
    		{
        		(void_t) gos_traceTrace(GOS_FALSE, "Application start address error.\r\n");
    		}
    		else
    		{
        		(void_t) gos_traceTraceFormattedUnsafe("Application start address error.\r\n");
    		}
		}
    }
    else
    {
		if (bootMode == GOS_TRUE)
		{
			(void_t) gos_traceTrace(GOS_FALSE, "Application size error.\r\n");
		}
		else
		{
			(void_t) gos_traceTraceFormattedUnsafe("Application size error.\r\n");
		}
    }

    return appCheckResult;
}

/*
 * Function: bld_jumpToApplication
 */
gos_result_t bld_jumpToApplication (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t jumpResult = GOS_ERROR;
    u16_t irqIdx = 0u;

    /*
     * Function code.
     */
    if (BLD_APP_ROM_START_ADDRESS % 0x200 == 0u)
    {
        /* Disable all interrupts */
        __disable_irq();

        /* Disable Systick timer */
        SysTick->CTRL = 0;

        /* Set the clock to the default state */
        HAL_RCC_DeInit();
        HAL_DeInit();

        for (irqIdx = 0; irqIdx < sizeof(NVIC->ICER) / sizeof(NVIC->ICER[0]); irqIdx++)
        {
            NVIC->ICER[irqIdx] = 0xFFFFFFFF;
            NVIC->ICPR[irqIdx] = 0xFFFFFFFF;
        }

        SYSCFG->MEMRMP = 0x01;

        SCB->VTOR = BLD_APP_ROM_START_ADDRESS;

        // Set the MSP
        __set_MSP(((bld_boot_vect_table_t*)BLD_APP_ROM_START_ADDRESS)->initSp);

        // Jump to app firmware
        ((bld_boot_vect_table_t*)BLD_APP_ROM_START_ADDRESS)->resetHandler();
    }
    else
    {
        // Address is incorrect.
    }

    return jumpResult;
}
