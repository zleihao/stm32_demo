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
#include "string.h"
#include "bsp_spi_flash.h"
#include "bsp_sdio_sdcard.h"

/* 为每个设备定义一个物理编号 */
#define ATA			           0     // SD卡
#define SPI_FLASH		       1     // 预留外部SPI Flash使用

#define SD_BLOCKSIZE     512 

extern  SD_CardInfo SDCardInfo;

/*-----------------------------------------------------------------------*/
/* 获取设备状态                                                          */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status(
	BYTE pdrv		/* 物理编号 */
)
{
	DSTATUS status = STA_NOINIT;

	switch (pdrv) {
		case ATA:	/* SD CARD */
			status &= ~STA_NOINIT;
			break;

		case SPI_FLASH:        /* SPI Flash */
			break;

		default:
			status = STA_NOINIT;
	}
	return status;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize(
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS status = STA_NOINIT;
	switch (pdrv) {
		case ATA:	         /* SD CARD */
			if (SD_Init() == SD_OK) {
				status &= ~STA_NOINIT;
			} else {
				status = STA_NOINIT;
			}

			break;

		case SPI_FLASH:    /* SPI Flash */
			break;

		default:
			status = STA_NOINIT;
	}
	return status;
}



/*-----------------------------------------------------------------------*/
/* 读扇区：读取扇区内容到指定存储区                                                      */
/*-----------------------------------------------------------------------*/
DRESULT disk_read(
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT status = RES_PARERR;
	SD_Error SD_state = SD_OK;

	switch (pdrv) {
		case ATA:	/* SD CARD */
			if ((DWORD)buff & 3) {
				DRESULT res = RES_OK;
				DWORD scratch[SD_BLOCKSIZE / 4];

				while (count--) {
					res = disk_read(ATA, (void *)scratch, sector++, 1);

					if (res != RES_OK) {
						break;
					}
					memcpy(buff, scratch, SD_BLOCKSIZE);
					buff += SD_BLOCKSIZE;
				}
				return res;
			}

			SD_state = SD_ReadMultiBlocks(buff, (uint64_t)sector * SD_BLOCKSIZE, SD_BLOCKSIZE, count);
			if (SD_state == SD_OK) {
				/* Check if the Transfer is finished */
				SD_state = SD_WaitReadOperation();
				while (SD_GetStatus() != SD_TRANSFER_OK);
			}
			if (SD_state != SD_OK)
				status = RES_PARERR;
			else
				status = RES_OK;
			break;

		case SPI_FLASH:
			break;

		default:
			status = RES_PARERR;
	}
	return status;
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
	DRESULT status = RES_PARERR;
	SD_Error SD_state = SD_OK;

	if (!count) {
		return RES_PARERR;		/* Check parameter */
	}

	switch (pdrv) {
		case ATA:	/* SD CARD */
			if ((DWORD)buff & 3) {
				DRESULT res = RES_OK;
				DWORD scratch[SD_BLOCKSIZE / 4];

				while (count--) {
					memcpy(scratch, buff, SD_BLOCKSIZE);
					res = disk_write(ATA, (void *)scratch, sector++, 1);
					if (res != RES_OK) {
						break;
					}
					buff += SD_BLOCKSIZE;
				}
				return res;
			}

			SD_state = SD_WriteMultiBlocks((uint8_t *)buff, (uint64_t)sector * SD_BLOCKSIZE, SD_BLOCKSIZE, count);
			if (SD_state == SD_OK) {
				/* Check if the Transfer is finished */
				SD_state = SD_WaitWriteOperation();

				/* Wait until end of DMA transfer */
				while (SD_GetStatus() != SD_TRANSFER_OK);
			}
			if (SD_state != SD_OK)
				status = RES_PARERR;
			else
				status = RES_OK;
			break;

		case SPI_FLASH:
			break;

		default:
			status = RES_PARERR;
	}
	return status;
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
	DRESULT status = RES_PARERR;
	switch (pdrv) {
		case ATA:	/* SD CARD */
			switch (cmd) {
				// Get R/W sector size (WORD) 
				case GET_SECTOR_SIZE:
					*(WORD *)buff = SD_BLOCKSIZE;
					break;
					// Get erase block size in unit of sector (DWORD)
				case GET_BLOCK_SIZE:
					*(DWORD *)buff = 1;
					break;

				case GET_SECTOR_COUNT:
					*(DWORD *)buff = SDCardInfo.CardCapacity / SDCardInfo.CardBlockSize;
					break;
				case CTRL_SYNC:
					break;
			}
			status = RES_OK;
			break;

		case SPI_FLASH:
			break;

		default:
			status = RES_PARERR;
	}
	return status;
}

DWORD get_fattime(void)
{
	return 0;
}
