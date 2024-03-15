
#include <bld.h>
#include <bld_data.h>
#include <drv.h>
#include <drv_crc.h>
#include <drv_flash.h>
#include <string.h>

GOS_STATIC bld_data_struct_t bootloaderData;

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
	if (drv_flashRead(BLD_DATA_START_ADDRESS, (void_t*)&bootloaderData, sizeof(bootloaderData)) == GOS_SUCCESS)
	{
		// Force update bootloader version info.
		bld_getVersion(&bootloaderData.bldVersion);

		if (bootloaderData.initPattern == BLD_INIT_PATTERN)
		{
			// Data already initialized.
			// Check data CRC.
			if (drv_crcCheckCrc32((u8_t*)&bootloaderData, sizeof(bootloaderData) - sizeof(u32_t),
				bootloaderData.bldDataCrc, NULL) == DRV_CRC_CHECK_OK)
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
	bootloaderData.initPattern                    = BLD_INIT_PATTERN;
	bootloaderData.appData.appVersion.major       = 0;
	bootloaderData.appData.appVersion.minor       = 0;
	bootloaderData.appData.appVersion.build       = 0;
	bootloaderData.appData.appVersion.date.years  = 1900;
	bootloaderData.appData.appVersion.date.months = GOS_TIME_JANUARY;
	bootloaderData.appData.appVersion.date.days   = 1;
	bootloaderData.appData.appStartAddress        = 0;
	bootloaderData.appData.appCrc                 = 0xFFFFFFFFu;
	bootloaderData.appData.appSize                = 0;

	(void_t) strcpy(bootloaderData.appData.appVersion.author, "N/A");
	(void_t) strcpy(bootloaderData.appData.appVersion.description, "N/A");
	(void_t) strcpy(bootloaderData.appData.appVersion.name, "N/A");

	(void_t) drv_crcGetCrc32((u8_t*)&bootloaderData.appData, sizeof(bootloaderData.appData) - sizeof(bootloaderData.appData.appDataCrc), &bootloaderData.appData.appDataCrc);

	// Get bootloader version info.
	bld_getVersion(&bootloaderData.bldVersion);

	bootloaderData.bldStartAddress                = BLD_ROM_START_ADDRESS;
	bootloaderData.bldSize                        = bld_dataGetBootloaderSize();

	(void_t) drv_crcGetCrc32((u8_t*)BLD_ROM_START_ADDRESS, bootloaderData.bldSize, &bootloaderData.bldCrc);
	(void_t) drv_crcGetCrc32((u8_t*)&bootloaderData, sizeof(bootloaderData) - sizeof(bootloaderData.bldDataCrc), &bootloaderData.bldDataCrc);

	if (drv_flashErase(BLD_DATA_START_ADDRESS, sizeof(bootloaderData)) != GOS_SUCCESS ||
		drv_flashWrite(BLD_DATA_START_ADDRESS, (void_t*)&bootloaderData, sizeof(bootloaderData)) != GOS_SUCCESS)
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
gos_result_t bld_dataGet (bld_data_struct_t* pData)
{
	/*
	 * Local variables.
	 */
	gos_result_t dataGetResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (pData != NULL)
	{
		dataGetResult = drv_flashRead(BLD_DATA_START_ADDRESS, (void_t*)pData, sizeof(*pData));
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
gos_result_t bld_dataSet (bld_data_struct_t* pData)
{
	/*
	 * Local variables.
	 */
	gos_result_t dataSetResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (pData != NULL)
	{
		// Calculate CRC.
		(void_t) drv_crcGetCrc32((u8_t*)pData, sizeof(*pData) - sizeof(pData->bldDataCrc), &pData->bldDataCrc);

		if (drv_flashErase(BLD_DATA_START_ADDRESS, sizeof(*pData)) != GOS_SUCCESS ||
			drv_flashWrite(BLD_DATA_START_ADDRESS, (void_t*)pData, sizeof(*pData)) != GOS_SUCCESS)
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
 * Function: bld_dataPrint
 */
void_t bld_dataPrint (void_t)
{
	/*
	 * Local variables.
	 */
	lib_version_t driverLibVer;

	/*
	 * Function code.
	 */
	drv_getVersion(&driverLibVer);

	// Header.
	(void_t) gos_traceTraceFormattedUnsafe("\r\n");

	// Driver info.
	(void_t) gos_traceTraceFormattedUnsafe(TRACE_BG_BLUE_START "DRIVER: %s" TRACE_FORMAT_RESET "\r\n", driverLibVer.name);
	(void_t) gos_traceTraceFormattedUnsafe("Version:    \t%u.%u.%u\r\n",driverLibVer.major, driverLibVer.minor, driverLibVer.build);
	(void_t) gos_traceTraceFormattedUnsafe("Author:     \t%s\r\n", driverLibVer.author);
	(void_t) gos_traceTraceFormattedUnsafe("Date:       \t%04u-%02u-%02u\r\n", driverLibVer.date.years, driverLibVer.date.months, driverLibVer.date.days);
	(void_t) gos_traceTraceFormattedUnsafe("Description: \t%s\r\n\r\n", driverLibVer.description);

	// Bootloader info.
	(void_t) gos_traceTraceFormattedUnsafe(TRACE_BG_BLUE_START "BOOTLOADER: %s" TRACE_FORMAT_RESET "\r\n", bootloaderData.bldVersion.name);
	(void_t) gos_traceTraceFormattedUnsafe("Version:     \t%u.%u.%u\r\n", bootloaderData.bldVersion.major, bootloaderData.bldVersion.minor, bootloaderData.bldVersion.build);
	(void_t) gos_traceTraceFormattedUnsafe("Author:      \t%s\r\n", bootloaderData.bldVersion.author);
	(void_t) gos_traceTraceFormattedUnsafe("Date:        \t%04u-%02u-%02u\r\n", bootloaderData.bldVersion.date.years, bootloaderData.bldVersion.date.months, bootloaderData.bldVersion.date.days);
	(void_t) gos_traceTraceFormattedUnsafe("Description: \t%s\r\n", bootloaderData.bldVersion.description);
	(void_t) gos_traceTraceFormattedUnsafe("Size:        \t%u bytes\r\n", bootloaderData.bldSize);
	(void_t) gos_traceTraceFormattedUnsafe("Address:     \t0x%08x\r\n", bootloaderData.bldStartAddress);
	(void_t) gos_traceTraceFormattedUnsafe("CRC32:       \t0x%08x\r\n\r\n", bootloaderData.bldCrc);

	// Application info
	(void_t) gos_traceTraceFormattedUnsafe(TRACE_BG_BLUE_START "APPLICATION: %s" TRACE_FORMAT_RESET "\r\n", bootloaderData.appData.appVersion.name);
	(void_t) gos_traceTraceFormattedUnsafe("Version:     \t%u.%u.%u\r\n", bootloaderData.appData.appVersion.major, bootloaderData.appData.appVersion.minor, bootloaderData.appData.appVersion.build);
	(void_t) gos_traceTraceFormattedUnsafe("Author:      \t%s\r\n", bootloaderData.appData.appVersion.author);
	(void_t) gos_traceTraceFormattedUnsafe("Date:        \t%04u-%02u-%02u\r\n", bootloaderData.appData.appVersion.date.years, bootloaderData.appData.appVersion.date.months, bootloaderData.appData.appVersion.date.days);
	(void_t) gos_traceTraceFormattedUnsafe("Description: \t%s\r\n", bootloaderData.appData.appVersion.description);
	(void_t) gos_traceTraceFormattedUnsafe("Size:        \t%u bytes\r\n", bootloaderData.appData.appSize);
	(void_t) gos_traceTraceFormattedUnsafe("Address:     \t0x%08x\r\n", bootloaderData.appData.appStartAddress);
	(void_t) gos_traceTraceFormattedUnsafe("CRC32:       \t0x%08x\r\n\r\n", bootloaderData.appData.appCrc);
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
