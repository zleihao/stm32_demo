/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "bsp_spi_flash.h"

/* Definitions of physical drive number for each drive */
// #define SD_CARD		0	/* Example: Map Ramdisk to physical drive 0 */
// #define SPI_FLASH		1	/* Example: Map MMC/SD card to physical drive 1 */
// #define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

#define SD_CARD    0
#define SPI_FLASH  1

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv) {
		case SD_CARD:

			// translate the reslut code here

			return stat;

		case SPI_FLASH:
			if (SPI_FLASH_ReadID() == 0xef4017) {
				return 0;
			}
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv) {
		case SD_CARD:
			// result = RAM_disk_initialize();

			// translate the reslut code here

			return stat;

		case SPI_FLASH:
			SPI_FLASH_Init();

			// translate the reslut code here
			return disk_status(SPI_FLASH);
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;

	switch (pdrv) {
		case SD_CARD:
			// translate the arguments here

			// result = RAM_disk_read(buff, sector, count);

			// translate the reslut code here

			return res;

		case SPI_FLASH:
			// translate the arguments here
			SPI_FLASH_BufferRead(buff, sector * 4096, count * 4096);
			res = RES_OK;
			// translate the reslut code here

			return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;

	switch (pdrv) {
		case SD_CARD:
			// translate the arguments here

			// translate the reslut code here

			return res;

		case SPI_FLASH:
			SPI_FLASH_SectorErase(sector * 4096);
			// translate the arguments here
			SPI_FLASH_BufferWrite((u8 *)buff, sector * 4096, count * 4096);
			res = RES_OK;
			// translate the reslut code here

			return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;

	switch (pdrv) {
		case SD_CARD:

			// Process of the command for the RAM drive

			return res;

		case SPI_FLASH:
			switch (cmd) {
				case GET_SECTOR_COUNT:
					*(DWORD *)buff = 2048;
					break;

				case GET_SECTOR_SIZE:
					*(WORD *)buff = 4096;
					break;

				case GET_BLOCK_SIZE:
					*(DWORD *)buff = 1;
					break;

				default:
					break;
			}
			res = RES_OK;
			return res;
	}

	return RES_PARERR;
}

DWORD get_fattime(void)
{
	return 0;
}