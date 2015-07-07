#include "hw_config.h"
#include "sst25vf_spi.h"

uint32_t NbrOfPage = 0;
uint16_t Flash_Update_Index = 0;
uint32_t External_Flash_Address = 0;
static uint32_t External_Flash_Start_Address = 0;

uint32_t OTA_FlashAddress()
{
    return FLASH_FW_ADDRESS;
}

#define FLASH_MAX_SIZE          (int32_t)(FLASH_LENGTH - FLASH_FW_ADDRESS)

uint32_t OTA_FlashLength()
{
    return FLASH_MAX_SIZE;
}

void FLASH_Begin(uint32_t sFLASH_Address, uint32_t fileSize)
{
    //OTA_FLASHED_Status_SysFlag = 0x0000;
    //Save_SystemFlags();

    Flash_Update_Index = 0;
    External_Flash_Start_Address = sFLASH_Address;
    External_Flash_Address = External_Flash_Start_Address;

    /* Define the number of External Flash pages to be erased */
    NbrOfPage = FLASH_PagesMask(fileSize);

    /* Erase the SPI Flash pages */
    for (int EraseCounter = 0; (EraseCounter < NbrOfPage); EraseCounter++)
    {
        sFLASH_EraseSector(External_Flash_Start_Address + (sFLASH_PAGESIZE * EraseCounter));
    }
}

uint32_t FLASH_PagesMask(uint32_t fileSize)
{
    //Calculate the number of flash pages that needs to be erased
    uint32_t numPages = 0x0;

    if ((fileSize % sFLASH_PAGESIZE) != 0)
    {
        numPages = (fileSize / sFLASH_PAGESIZE) + 1;
    }
    else
    {
        numPages = fileSize / sFLASH_PAGESIZE;
    }

    return numPages;
}

uint16_t FLASH_Update(uint8_t *pBuffer, uint32_t bufferSize)
{
    uint8_t *writeBuffer = pBuffer;
    uint8_t readBuffer[bufferSize];

    /* Write Data Buffer to SPI Flash memory */
    sFLASH_WriteBuffer(writeBuffer, External_Flash_Address, bufferSize);

    /* Read Data Buffer from SPI Flash memory */
    sFLASH_ReadBuffer(readBuffer, External_Flash_Address, bufferSize);

    /* Is the Data Buffer successfully programmed to SPI Flash memory */
    if (0 == memcmp(writeBuffer, readBuffer, bufferSize))
    {
        External_Flash_Address += bufferSize;
        Flash_Update_Index += 1;
    }
    else
    {
        /* Erase the problematic SPI Flash pages and back off the chunk index */
        External_Flash_Address = ((uint32_t)(External_Flash_Address / sFLASH_PAGESIZE)) * sFLASH_PAGESIZE;
        sFLASH_EraseSector(External_Flash_Address);
        Flash_Update_Index = (uint16_t)((External_Flash_Address - External_Flash_Start_Address) / bufferSize);
    }

    return Flash_Update_Index;
}

void FLASH_End(void)
{
	uint32_t fw_len = (uint32_t)(External_Flash_Address - External_Flash_Start_Address);

	uint8_t byte1 = sFLASH_ReadSingleByte(0x0101);
	simple_uart_putstring("Size of app: ");
	char str1[80];
	sprintf(str1, "%d", fw_len);
	simple_uart_putstring(str1);
	simple_uart_putstring("\n");

	//erase the sector for now, so we can store the FW size and set the flag
	sFLASH_EraseSector(FLASH_FW_STATUS);

	uint8_t length1 = (uint8_t)((fw_len & 0xFF0000) >> 16);
	uint8_t length2 = (uint8_t)((fw_len & 0xFF00) >> 8);
	uint8_t length3 = (uint8_t)(fw_len & 0xFF);


	//TO DO: Set a flag in external flash to notify bootloader there is a FW update
	sFLASH_WriteSingleByte(FLASH_FW_STATUS, 0x01);
	sFLASH_WriteSingleByte(FLASH_FW_LENGTH1, length1);
	sFLASH_WriteSingleByte(FLASH_FW_LENGTH2, length2);
	sFLASH_WriteSingleByte(FLASH_FW_LENGTH3, length3);

	simple_uart_putstring("Size of 2nd byte: ");
	char str2[80];
	sprintf(str2, "%d", length2);
	simple_uart_putstring(str2);
	simple_uart_putstring("\n");


	uint8_t byte17 = sFLASH_ReadSingleByte(FLASH_FW_STATUS);
	simple_uart_putstring("Byte at 0x0101: ");
	char str3[80];
	sprintf(str3, "%d", byte17);
	simple_uart_putstring(str3);
	simple_uart_putstring("\n");


    NVIC_SystemReset();
}
