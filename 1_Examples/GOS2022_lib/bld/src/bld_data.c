
#include <bld.h>
#include <bld_data.h>
#include <drv.h>
#include <drv_crc.h>
#include <drv_flash.h>
#include <string.h>

#define BLD_DEFAULT_CONN_TMO_MS     ( 3000u     )
#define BLD_DEFAULT_REQ_TMO_MS      ( 60000u    )
#define BLD_DEFAULT_INSTALL_TMO_MS  ( 120000u   )
#define BLD_DEFAULT_CONN_ON_STARTUP ( GOS_TRUE  )

GOS_STATIC bld_bootloaderData_t bootloaderData;
GOS_STATIC bld_appData_t appData;
GOS_STATIC bld_bootloaderConfig_t bootloaderConfig;
GOS_STATIC bld_packedData bldPackedData;

GOS_STATIC u32_t bld_dataGetBootloaderSize (void_t);

/*
 * Function: bld_dataInitialize
 */
gos_result_t bld_dataInitialize (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t dataInitResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (drv_flashRead(BLD_DATA_START_ADDRESS, (void_t*)&bldPackedData, sizeof(bldPackedData)) == GOS_SUCCESS)
    {
    	(void_t) memcpy ((void_t*)&bootloaderData, (void_t*)&bldPackedData.bldData, sizeof(bldPackedData.bldData));
    	(void_t) memcpy ((void_t*)&appData, (void_t*)&bldPackedData.appData, sizeof(bldPackedData.appData));
    	(void_t) memcpy ((void_t*)&bootloaderConfig, (void_t*)&bldPackedData.bldConfig, sizeof(bldPackedData.bldConfig));

        // Force update bootloader version info.
        bld_getVersion(&bootloaderData.version);

        if (bootloaderData.initPattern == BLD_INIT_PATTERN &&
        	bootloaderConfig.initPattern == BLD_INIT_PATTERN)
        {
            // Data already initialized.
            // Check data CRC.
            if (drv_crcCheckCrc32((u8_t*)&bootloaderData, sizeof(bootloaderData) - sizeof(u32_t),
                bootloaderData.dataCrc, NULL) == DRV_CRC_CHECK_OK &&
            	drv_crcCheckCrc32((u8_t*)&bootloaderConfig, sizeof(bootloaderConfig) - sizeof(u32_t),
                bootloaderConfig.dataCrc, NULL) == DRV_CRC_CHECK_OK )
            {
                // CRC OK, initialization successful.
                dataInitResult = GOS_SUCCESS;
            }
            else
            {
                // CRC error.
                (void_t) gos_traceTraceFormattedUnsafe("Bootloader data CRC ERROR.\r\n");
                (void_t) gos_traceTraceFormattedUnsafe("Resetting boorloader data ...\r\n");
                bld_dataReset();
                dataInitResult = GOS_SUCCESS;
            }
        }
        else
        {
            // Data uninitialized, do it now.
            bld_dataReset();
        }
    }
    else
    {
        // Read unsuccessful.
        (void_t) gos_traceTraceFormattedUnsafe("FLASH read ERROR.\r\n");
    }

    return dataInitResult;
}

/*
 * Function: bld_dataReset
 */
gos_result_t bld_dataReset (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t dataResetResult = GOS_ERROR;

    /*
     * Function code.
     */
    bootloaderData.initPattern  = BLD_INIT_PATTERN;
    appData.version.major       = 0;
    appData.version.minor       = 0;
    appData.version.build       = 0;
    appData.version.date.years  = 1900;
    appData.version.date.months = GOS_TIME_JANUARY;
    appData.version.date.days   = 1;
    appData.startAddress        = 0;
    appData.crc                 = 0xFFFFFFFFu;
    appData.size                = 0;

    (void_t) strcpy(appData.version.author, "N/A");
    (void_t) strcpy(appData.version.description, "N/A");
    (void_t) strcpy(appData.version.name, "N/A");

    (void_t) drv_crcGetCrc32((u8_t*)&appData, sizeof(appData) - sizeof(appData.dataCrc), &appData.dataCrc);

    // Get bootloader version info and driver version info.
    bld_getVersion(&bootloaderData.version);
    drv_getVersion(&bootloaderData.driverVersion);

    bootloaderData.startAddress = BLD_ROM_START_ADDRESS;
    bootloaderData.size         = bld_dataGetBootloaderSize();

    bootloaderConfig.initPattern                = BLD_INIT_PATTERN;
    bootloaderConfig.connectionTimeout          = BLD_DEFAULT_CONN_TMO_MS;
    bootloaderConfig.installTimeout             = BLD_DEFAULT_INSTALL_TMO_MS;
    bootloaderConfig.requestTimeout             = BLD_DEFAULT_REQ_TMO_MS;
    bootloaderConfig.waitForConnectionOnStartup = BLD_DEFAULT_CONN_ON_STARTUP;

    (void_t) drv_crcGetCrc32((u8_t*)BLD_ROM_START_ADDRESS, bootloaderData.size, &bootloaderData.crc);
    (void_t) drv_crcGetCrc32((u8_t*)&bootloaderData, sizeof(bootloaderData) - sizeof(bootloaderData.dataCrc), &bootloaderData.dataCrc);
    (void_t) drv_crcGetCrc32((u8_t*)&bootloaderConfig, sizeof(bootloaderConfig) - sizeof(bootloaderConfig.dataCrc), &bootloaderConfig.dataCrc);

    (void_t) memcpy ((void_t*)&bldPackedData.bldData, (void_t*)&bootloaderData, sizeof(bootloaderData));
    (void_t) memcpy ((void_t*)&bldPackedData.appData, (void_t*)&appData, sizeof(appData));
    (void_t) memcpy ((void_t*)&bldPackedData.bldConfig, (void_t*)&bootloaderConfig, sizeof(bootloaderConfig));

    if (drv_flashErase(BLD_DATA_START_ADDRESS, sizeof(bldPackedData)) != GOS_SUCCESS ||
    	drv_flashWrite(BLD_DATA_START_ADDRESS, (void_t*)&bldPackedData, sizeof(bldPackedData)) != GOS_SUCCESS)
    {
        (void_t) gos_traceTraceFormattedUnsafe("Bootloader data initialization ERROR\r\n");
    }
    else
    {
        dataResetResult = GOS_SUCCESS;
    }

    return dataResetResult;
}

/*
 * Function: bld_dataGet
 */
gos_result_t bld_dataGet (bld_bootloaderData_t* pData)
{
    /*
     * Local variables.
     */
    gos_result_t dataGetResult = GOS_ERROR;
    bld_packedData bldPackedData = {0};

    /*
     * Function code.
     */
    if (pData != NULL)
    {
        dataGetResult = drv_flashRead(BLD_DATA_START_ADDRESS, (void_t*)&bldPackedData, sizeof(bldPackedData));
        (void_t*) memcpy((void_t*)pData, (void_t*)&bldPackedData.bldData, sizeof(bldPackedData.bldData));
    }
    else
    {
        // Error.
    }

    return dataGetResult;
}

/*
 * Function: bld_appDataGet
 */
gos_result_t bld_appDataGet (bld_appData_t* pData)
{
    /*
     * Local variables.
     */
    gos_result_t dataGetResult = GOS_ERROR;
    bld_packedData bldPackedData = {0};

    /*
     * Function code.
     */
    if (pData != NULL)
    {
    	dataGetResult = drv_flashRead(BLD_DATA_START_ADDRESS, (void_t*)&bldPackedData, sizeof(bldPackedData));
        (void_t*) memcpy((void_t*)pData, (void_t*)&bldPackedData.appData, sizeof(bldPackedData.appData));
    }
    else
    {
        // Error.
    }

    return dataGetResult;
}

/*
 * Function: bld_configGet
 */
gos_result_t bld_configGet (bld_bootloaderConfig_t* pData)
{
    /*
     * Local variables.
     */
    gos_result_t dataGetResult = GOS_ERROR;
    bld_packedData bldPackedData = {0};

    /*
     * Function code.
     */
    if (pData != NULL)
    {
        dataGetResult = drv_flashRead(BLD_DATA_START_ADDRESS, (void_t*)&bldPackedData, sizeof(bldPackedData));
        (void_t*) memcpy((void_t*)pData, (void_t*)&bldPackedData.bldConfig, sizeof(bldPackedData.bldConfig));
    }
    else
    {
        // Error.
    }

    return dataGetResult;
}

/*
 * Function: bld_dataSet
 */
gos_result_t bld_dataSet (bld_bootloaderData_t* pData)
{
    /*
     * Local variables.
     */
    gos_result_t dataSetResult = GOS_ERROR;
    bld_packedData bldPackedData = {0};

    /*
     * Function code.
     */
    if (pData != NULL && drv_flashRead(BLD_DATA_START_ADDRESS, (void_t*)&bldPackedData, sizeof(bldPackedData)) == GOS_SUCCESS)
    {
        // Calculate CRC.
        (void_t) drv_crcGetCrc32((u8_t*)pData, sizeof(*pData) - sizeof(pData->dataCrc), &pData->dataCrc);

        // Copy bootloader data to packed data.
        (void_t*) memcpy((void_t*)&bldPackedData.bldData, (void_t*)pData, sizeof(*pData));

        // Save updated packed data.
        if (drv_flashErase(BLD_DATA_START_ADDRESS, sizeof(bldPackedData)) != GOS_SUCCESS ||
            drv_flashWrite(BLD_DATA_START_ADDRESS, (void_t*)&bldPackedData, sizeof(bldPackedData)) != GOS_SUCCESS)
        {
            (void_t) gos_traceTraceFormattedUnsafe("Bootloader data set ERROR\r\n");
        }
        else
        {
            dataSetResult = GOS_SUCCESS;
        }
    }
    else
    {
        // Error.
    }

    return dataSetResult;
}

/*
 * Function: bld_appDataSet
 */
gos_result_t bld_appDataSet (bld_appData_t* pData)
{
    /*
     * Local variables.
     */
    gos_result_t dataSetResult = GOS_ERROR;
    bld_packedData bldPackedData = {0};

    /*
     * Function code.
     */
    if (pData != NULL && drv_flashRead(BLD_DATA_START_ADDRESS, (void_t*)&bldPackedData, sizeof(bldPackedData)) == GOS_SUCCESS)
    {
        // Calculate CRC.
        (void_t) drv_crcGetCrc32((u8_t*)pData, sizeof(*pData) - sizeof(pData->dataCrc), &pData->dataCrc);

        // Copy application data to packed data.
        (void_t*) memcpy((void_t*)&bldPackedData.appData, (void_t*)pData, sizeof(*pData));

        // Save updated packed data.
        if (drv_flashErase(BLD_DATA_START_ADDRESS, sizeof(bldPackedData)) != GOS_SUCCESS ||
            drv_flashWrite(BLD_DATA_START_ADDRESS, (void_t*)&bldPackedData, sizeof(bldPackedData)) != GOS_SUCCESS)
        {
            (void_t) gos_traceTraceFormattedUnsafe("App data set ERROR\r\n");
        }
        else
        {
            dataSetResult = GOS_SUCCESS;
        }
    }
    else
    {
        // Error.
    }

    return dataSetResult;
}

/*
 * Function: bld_configSet
 */
gos_result_t bld_configSet (bld_bootloaderConfig_t* pData)
{
    /*
     * Local variables.
     */
    gos_result_t dataSetResult = GOS_ERROR;
    bld_packedData bldPackedData = {0};

    /*
     * Function code.
     */
    if (pData != NULL && drv_flashRead(BLD_DATA_START_ADDRESS, (void_t*)&bldPackedData, sizeof(bldPackedData)) == GOS_SUCCESS)
    {
        // Calculate CRC.
        (void_t) drv_crcGetCrc32((u8_t*)pData, sizeof(*pData) - sizeof(pData->dataCrc), &pData->dataCrc);

        // Copy bootloader config to packed data.
        (void_t*) memcpy((void_t*)&bldPackedData.bldConfig, (void_t*)pData, sizeof(*pData));

        // Save updated packed data.
        if (drv_flashErase(BLD_DATA_START_ADDRESS, sizeof(bldPackedData)) != GOS_SUCCESS ||
            drv_flashWrite(BLD_DATA_START_ADDRESS, (void_t*)&bldPackedData, sizeof(bldPackedData)) != GOS_SUCCESS)
        {
            (void_t) gos_traceTraceFormattedUnsafe("Bootloader config set ERROR\r\n");
        }
        else
        {
            dataSetResult = GOS_SUCCESS;
        }
    }
    else
    {
        // Error.
    }

    return dataSetResult;
}

/*
 * Function: bld_dataPrint
 */
void_t bld_dataPrint (void_t)
{
    /*
     * Function code.
     */
    // Header.
    (void_t) gos_traceTraceFormattedUnsafe("\r\n");

    // Driver info.
    (void_t) gos_traceTraceFormattedUnsafe(TRACE_BG_BLUE_START "DRIVER: %s" TRACE_FORMAT_RESET "\r\n", bootloaderData.driverVersion.name);
    (void_t) gos_traceTraceFormattedUnsafe("Version:    \t%u.%u.%u\r\n",bootloaderData.driverVersion.major, bootloaderData.driverVersion.minor, bootloaderData.driverVersion.build);
    (void_t) gos_traceTraceFormattedUnsafe("Author:     \t%s\r\n", bootloaderData.driverVersion.author);
    (void_t) gos_traceTraceFormattedUnsafe("Date:       \t%04u-%02u-%02u\r\n", bootloaderData.driverVersion.date.years, bootloaderData.driverVersion.date.months, bootloaderData.driverVersion.date.days);
    (void_t) gos_traceTraceFormattedUnsafe("Description: \t%s\r\n\r\n", bootloaderData.driverVersion.description);

    // Bootloader info.
    (void_t) gos_traceTraceFormattedUnsafe(TRACE_BG_BLUE_START "BOOTLOADER: %s" TRACE_FORMAT_RESET "\r\n", bootloaderData.version.name);
    (void_t) gos_traceTraceFormattedUnsafe("Version:     \t%u.%u.%u\r\n", bootloaderData.version.major, bootloaderData.version.minor, bootloaderData.version.build);
    (void_t) gos_traceTraceFormattedUnsafe("Author:      \t%s\r\n", bootloaderData.version.author);
    (void_t) gos_traceTraceFormattedUnsafe("Date:        \t%04u-%02u-%02u\r\n", bootloaderData.version.date.years, bootloaderData.version.date.months, bootloaderData.version.date.days);
    (void_t) gos_traceTraceFormattedUnsafe("Description: \t%s\r\n", bootloaderData.version.description);
    (void_t) gos_traceTraceFormattedUnsafe("Size:        \t%u bytes\r\n", bootloaderData.size);
    (void_t) gos_traceTraceFormattedUnsafe("Address:     \t0x%08x\r\n", bootloaderData.startAddress);
    (void_t) gos_traceTraceFormattedUnsafe("CRC32:       \t0x%08x\r\n\r\n", bootloaderData.crc);

    (void_t) gos_traceTraceFormattedUnsafe(TRACE_BG_BLUE_START "BOOTLOADER CONFIGURATION" TRACE_FORMAT_RESET "\r\n");
    (void_t) gos_traceTraceFormattedUnsafe("Connection on startup:\t%s\r\n", bootloaderConfig.waitForConnectionOnStartup == GOS_TRUE ? "yes" : "no");
    (void_t) gos_traceTraceFormattedUnsafe("Connection timeout:   \t%u ms\r\n", bootloaderConfig.connectionTimeout);
    (void_t) gos_traceTraceFormattedUnsafe("Request timeout:      \t%u ms\r\n", bootloaderConfig.requestTimeout);
    (void_t) gos_traceTraceFormattedUnsafe("Install timeout:      \t%u ms\r\n\r\n", bootloaderConfig.installTimeout);

    // Application info
    (void_t) gos_traceTraceFormattedUnsafe(TRACE_BG_BLUE_START "APPLICATION: %s" TRACE_FORMAT_RESET "\r\n", appData.version.name);
    (void_t) gos_traceTraceFormattedUnsafe("Version:     \t%u.%u.%u\r\n", appData.version.major, appData.version.minor, appData.version.build);
    (void_t) gos_traceTraceFormattedUnsafe("Author:      \t%s\r\n", appData.version.author);
    (void_t) gos_traceTraceFormattedUnsafe("Date:        \t%04u-%02u-%02u\r\n", appData.version.date.years, appData.version.date.months, appData.version.date.days);
    (void_t) gos_traceTraceFormattedUnsafe("Description: \t%s\r\n", appData.version.description);
    (void_t) gos_traceTraceFormattedUnsafe("Size:        \t%u bytes\r\n", appData.size);
    (void_t) gos_traceTraceFormattedUnsafe("Address:     \t0x%08x\r\n", appData.startAddress);
    (void_t) gos_traceTraceFormattedUnsafe("CRC32:       \t0x%08x\r\n\r\n", appData.crc);
}

/**
 * TODO
 * @param
 * @return
 */
GOS_STATIC u32_t bld_dataGetBootloaderSize (void_t)
{
    /*
     * Local variables.
     */
    u32_t address = BLD_ROM_END_ADDRESS;
    u32_t size    = (BLD_ROM_END_ADDRESS - BLD_ROM_START_ADDRESS);

    /*
     * Function code.
     */
    for (address = BLD_ROM_END_ADDRESS; address >= BLD_ROM_START_ADDRESS; address--)
    {
        if ((u8_t)(*((u32_t*)address)) != 0xFFu)
        {
            break;
        }
        else
        {
            size--;
        }
    }

    return size;
}
