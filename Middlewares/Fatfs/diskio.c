/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
/*-----------------------------------------------------------------------*/
/* 									   Usr header add (begin)        										 */
/*-----------------------------------------------------------------------*/
/*                    在此处添加底层驱动头文件                           */
#include "w25qxx.h"
#include "bsp_sdio.h"
/*-----------------------------------------------------------------------*/
/* 									   Usr header add (end)        										   */
/*-----------------------------------------------------------------------*/
/* Definitions of physical drive number for each drive */
//#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_NOR_FLASH		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv) {
	case DEV_NOR_FLASH :
//		result = RAM_disk_status();
		stat = RES_OK;
		// translate the reslut code here

		return stat;

	case DEV_MMC :
//		stat = SdiocInit();

		// translate the reslut code here

		return stat;

	case DEV_USB :
//		result = USB_disk_status();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv) {
	case DEV_NOR_FLASH :
//		result = RAM_disk_initialize();
		stat = Spi_device_Init(&w25q64);
		// translate the reslut code here

		return stat;

	case DEV_MMC :
//		result = MMC_disk_initialize();
#if USE_SD_FOR_FATFS
		// translate the reslut code here
		stat = SD_Initialize(pdrv);
#endif
		return stat;

	case DEV_USB :
//		result = USB_disk_initialize();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	uint32_t ReadBytesCount = 0;
	switch (pdrv) {
	case DEV_NOR_FLASH :
		ReadBytesCount = count*(w25q64.Sectorsize);
		res = (DRESULT)Spi_device_Read(&w25q64,sector*(w25q64.Sectorsize),buff,ReadBytesCount);
		return res;

	case DEV_MMC :
#if USE_SD_FOR_FATFS
		// translate the arguments here
		res = SD_Read(pdrv, buff, sector, count);
		// translate the reslut code here
#endif
		return res;

	case DEV_USB :
		// translate the arguments here

//		result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	uint32_t ReadBytesCount = 0;
	
	switch (pdrv) {
	case DEV_NOR_FLASH :
		// translate the arguments here
		ReadBytesCount = count*(w25q64.Sectorsize);
		res = (DRESULT)Spi_device_Write(&w25q64,sector*(w25q64.Sectorsize),(BYTE *)buff,ReadBytesCount);
		return res;
//		result = RAM_disk_write(buff, sector, count);

		// translate the reslut code here

	case DEV_MMC :
		// translate the arguments here
#if USE_SD_FOR_FATFS
		res = SD_Write(pdrv, buff, sector, count);
#endif
		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

//		result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	
	switch (pdrv) {
	case DEV_NOR_FLASH :
		switch (cmd)
		{
			/* Make sure that no pending write process */
			case CTRL_SYNC:
				res = RES_OK;
				break;
			/* Get number of sectors on the disk (DWORD) */
			case GET_SECTOR_COUNT :
				*(DWORD*)buff = w25q64.Sectornum;
				res = RES_OK;
				break;
			/* Get R/W sector size (WORD) */
			case GET_SECTOR_SIZE:
				*(WORD*)buff = (uint16_t)w25q64.Sectorsize;
				res = RES_OK;
				break;
			/* Get erase block size in unit of sector (DWORD) */
			case GET_BLOCK_SIZE:
				*(DWORD*)buff = w25q64.Blocksize;
				res = RES_OK;
				break;
			default:
				res = RES_PARERR;
				break;
		}
		// Process of the command for the RAM drive

		return res;

	case DEV_MMC :
#if USE_SD_FOR_FATFS
		// Process of the command for the MMC/SD card
		res = SD_Ioctl(pdrv, cmd, buff);
#endif
		return res;

	case DEV_USB :

		// Process of the command the USB drive

		return res;
	}
	
	return RES_PARERR;
}

//用户添加
__WEAKDEF DWORD get_fattime(void)
{
	return 0;
}


