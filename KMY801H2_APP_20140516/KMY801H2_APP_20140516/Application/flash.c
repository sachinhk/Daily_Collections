
#include "flash.h"

#include "stm32f10x.h"



//GPIOA define
#define FLASH_PORT			GPIOA
#define FLASH_SPI 			SPI1
#define FLASH_CS_PIN		GPIO_Pin_4
#define FLASH_SCK_PIN		GPIO_Pin_5
#define FLASH_MISO_PIN		GPIO_Pin_6
#define FLASH_MOSI_PIN		GPIO_Pin_7

/**
* @brief  M25P SPI Flash supported commands
*/  
#define FLASH_CMD_WRITE          0x02  /*!< Write to Memory instruction */
#define FLASH_CMD_WRSR           0x01  /*!< Write Status Register instruction */
#define FLASH_CMD_WREN           0x06  /*!< Write enable instruction */
#define FLASH_CMD_READ           0x03  /*!< Read from Memory instruction */
#define FLASH_CMD_RDSR           0x05  /*!< Read Status Register instruction  */
#define FLASH_CMD_RDID           0x9F  /*!< Read identification */
#define FLASH_CMD_SE             0xD8  /*!< Sector Erase instruction */
#define FLASH_CMD_BE             0xC7  /*!< Bulk Erase instruction */

#define FLASH_WIP_FLAG           0x01  /*!< Write In Progress (WIP) flag */

#define FLASH_DUMMY_BYTE         0xA5
#define FLASH_PAGESIZE			 0x100

#define FLASH_M25P16_ID          0x202015


#define FLASH_CS_LOW()       GPIO_ResetBits(FLASH_PORT, FLASH_CS_PIN);usleep(1)
#define FLASH_CS_HIGH()      GPIO_SetBits(FLASH_PORT, FLASH_CS_PIN);usleep(1)

#define pin_SpiCs_SetLow()		GPIO_ResetBits(GPIOA, GPIO_Pin_4);usleep(1)
#define pin_SpiCs_SetHigh()		GPIO_SetBits(GPIOA, GPIO_Pin_4);usleep(1)

#define pin_SpiSck_SetLow()		GPIO_ResetBits(GPIOA, GPIO_Pin_5);usleep(1)
#define pin_SpiSck_SetHigh()	GPIO_SetBits(GPIOA, GPIO_Pin_5);usleep(1)

#define pin_SpiMosi_SetLow()	GPIO_ResetBits(GPIOA, GPIO_Pin_7);usleep(1)
#define pin_SpiMosi_SetHigh()	GPIO_SetBits(GPIOA, GPIO_Pin_7);usleep(1)


#define pin_SpiMISO_IsLow()		(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)==0)
#define pin_SpiMISO_IsHigh()	(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)==1)



/***************************************************************************************
读取一个字节   
*****************************************************************************************/
uint8 Read_abyte(uint32 address)
{
	uint8 i;
	uint8 read_data;
	uint8 command;


	//while((Read_RDSR())&0x01);//等待空闲。

	CS_0;

	command=0x03;	//读取数据命令。
	for(i=0;i<8;i++)	//写入命令。
	{
		SCLK_0;
		if(0x80&command)		//高位优先。
		SO_1;
		else
		SO_0;
		SCLK_1;
		command<<=1;
	}


	for(i=0;i<24;i++)	//写入24位读取地址值。
	{
		SCLK_0;
		if(0x00800000&address)
		SO_1;
		else
		SO_0;
		SCLK_1;
		address<<=1;
	}

	for(i=0;i<8;i++)	//读取数据值。
	{
		SCLK_0;
		SCLK_1;
		read_data<<=1;
		if(in_SI)read_data++;
	}

	SO_0;
	CS_1;
	SCLK_1;

	return(read_data);
}

/***************************************************************************************
写入一个字节   
*****************************************************************************************/
void Write_adata(uint32 address,uint8 write_data)
{
	uint8 i;
	uint8 command;

	while((Read_RDSR())&0x01);

	WriteEN();

	command=0x02;
	CS_1;
	SCLK_1;
	CS_0;
	SCLK_0;
	for(i=0;i<8;i++)
	{
		SCLK_0;
		if(0x80&command)
		SO_1;
		else
		SO_0;
		SCLK_1;
		command<<=1;
	}


	for(i=0;i<24;i++)
	{
		SCLK_0;
		if(0x00800000&address)
		SO_1;
		else
		SO_0;
		SCLK_1;
		address<<=1;
	}


	for(i=0;i<8;i++)
	{
		SCLK_0;
		if(0x80&write_data)
		SO_1;
		else
		SO_0;
		SCLK_1;
		write_data<<=1;
	}


	SCLK_0;
	CS_1;
}

/***************************************************************************************
写入写使能命令 
*****************************************************************************************/
void WriteEN(void)
{
	uint8 i;
	uint8 command;
	command=0x06;	//使能命令。
	CS_1;
	SCLK_1;
	CS_0;
	for(i=0;i<8;i++)	//写入命令。
	{
		SCLK_0;
		if(0x80&command)
		SO_1;
		else
		SO_0;
		SCLK_1;
		command<<=1;
	}
	SCLK_0;
	CS_1;
}


/***************************************************************************************
写入写禁止命令 
*****************************************************************************************/
void WriteDisable(void)
{
	uint8 i;
	uint8 command;
	command=0x04;	//写入禁止命令。
	CS_1;
	SCLK_1;
	CS_0;
	SCLK_0;
	for(i=0;i<8;i++)	//写入命令。
	{
		SCLK_0;
		if(0x80&command)
		SO_1;
		else
		SO_0;
		SCLK_1;
		command<<=1;
	}
	SCLK_0;
	CS_1;
}
/***************************************************************************************
写入段擦除使能命令 
*****************************************************************************************/
void Sector_Erase(uint32 address)
{
	uint8 i;
	uint8 command;
	WriteEN();
	while((Read_RDSR())&0x01);
	command=0xd8;	//段擦除命令。
	CS_1;
	SCLK_1;
	CS_0;
	SCLK_0;
	for(i=0;i<8;i++)
	{
		SCLK_0;
		if(0x80&command)
		SO_1;
		else
		SO_0;
		SCLK_1;
		command<<=1;
	}
	for(i=0;i<24;i++)
	{
		SCLK_0;
		if(0x00800000&address)
		SO_1;
		else
		SO_0;
		SCLK_1;
		address<<=1;
	}
	SCLK_0;
	CS_1;
}

/***************************************************************************************
写入片擦除使能命令 
*****************************************************************************************/
void Bulk_Erase(void)
{
	uint8 i;
	uint8 command;
	command=0xc7;	//片擦除命令。
	while((Read_RDSR())&0x01);
	WriteEN();
	CS_1;
	SCLK_1;
	CS_0;
	for(i=0;i<8;i++)
	{
		SCLK_0;
		if(0x80&command)
		SO_1;
		else
		SO_0;
		SCLK_1;
		command<<=1;
	}
	SO_0;
	SCLK_0;
	CS_1;
}

/***************************************************************************************
延时X/100ms
*****************************************************************************************/
void DelayX100ms(uint32 x)
{
	uint32 i;
	while(x--)
	{
		i=1275;
		while(i--);
	}
}

/***************************************************************************************
读取状态寄存器。
*****************************************************************************************/
uint8 Read_RDSR(void)
{
	uint8 i,j;
	uint8 read_data;
	uint8 command;


	//CS_1;
	//SCLK_1;
	CS_0;
	//SCLK_0;

	command=0x05;
	for(i=0;i<8;i++)
	{
		SCLK_0;
		if(0x80&command)
		SO_1;
		else
		SO_0;
		SCLK_1;
		command<<=1;
	}

	//SO_0;

	for(i=0;i<8;i++)
	{
		SCLK_0;
		SCLK_1;
		read_data<<=1;
		if(in_SI)read_data++;
	}

	//SCLK_0;

	CS_1;

	return(read_data);
}
/**************************************************************
写入一串字节，芯片最多支持写入256个字节。
****************************************************************/
void Write_string(uint8 *string,uint32 address,uint8 n)
{
	uint8 i;
	uint8 command;
	while((Read_RDSR())&0x01);
	WriteEN();
	command=0x02;
	CS_1;
	SCLK_1;
	CS_0;
	SCLK_0;
	for(i=0;i<8;i++)
	{
		SCLK_0;
		if(0x80&command)
		SO_1;
		else
		SO_0;
		SCLK_1;
		command<<=1;
	}
	for(i=0;i<24;i++)
	{
		SCLK_0;
		if(0x00800000&address)
		SO_1;
		else
		SO_0;
		SCLK_1;
		address<<=1;
	}
	while(n--)
	{
		for(i=0;i<8;i++)
		{
			SCLK_0;
			if(0x80&(*string))
			SO_1;
			else
			SO_0;
			SCLK_1;
			(*string)<<=1;
		}
		string++;
	}
	SCLK_0;
	CS_1;
}
/**************************************************************
写保护
****************************************************************/
void Write_Protect()
{
	uint8 i;
	uint8 command;
	uint8 write_data;
	while((Read_RDSR())&0x01);
	WriteEN();
	command=0x01;
	write_data=0x9c;
	CS_1;
	SCLK_1;
	CS_0;
	SCLK_0;
	for(i=0;i<8;i++)
	{
		SCLK_0;
		if(0x80&command)
		SO_1;
		else
		SO_0;
		SCLK_1;
		command<<=1;
	}
	for(i=0;i<8;i++)
	{
		SCLK_0;
		if(0x80&write_data)
		SO_1;
		else
		SO_0;
		SCLK_1;
		write_data<<=1;
	}
	SCLK_0;
	CS_1;
}


//================================API=====================================
u8 spi_WriteAndRead(unsigned char spi_dat)  
{
	unsigned  char  i;
	u8 rec;

	//pin_SpiCs_SetLow();

	for(i=0;i<8;i++){
		pin_SpiSck_SetLow();

		if((spi_dat & 0x80)==0x80){pin_SpiMosi_SetHigh();}
		else{pin_SpiMosi_SetLow();}

		pin_SpiSck_SetHigh();

		rec<<=1;
		if(pin_SpiMISO_IsHigh())rec++;

		spi_dat<<=1;
	}

	//pin_SpiCs_SetHigh();

	return rec;
}


void spi_Write(unsigned char spi_dat)  
{
	unsigned  char  i;

	//spi_Cs_Set=0;
	pin_SpiCs_SetLow();
	for(i=0;i<8;i++)  
	{
		//spi_clk=0;
		pin_SpiSck_SetLow();
		if((spi_dat & 0x80)==0x80){
			//spi_di=1;
			pin_SpiMosi_SetHigh();
		}
		else{
			//spi_di=0;
			pin_SpiMosi_SetLow();
		}
		//spi_clk=1;
		pin_SpiSck_SetHigh();
		spi_dat<<=1;
	}
	//spi_Cs_Set=1;
	pin_SpiCs_SetHigh();
} 



#if 1
void my_FlashInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	FLASH_CS_HIGH();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/*!< Configure FLASH_SPI pins: SCK */
	GPIO_InitStructure.GPIO_Pin = FLASH_SCK_PIN|FLASH_CS_PIN|FLASH_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(FLASH_PORT, &GPIO_InitStructure);

	/*!< Configure FLASH_SPI pins: MISO */
	GPIO_InitStructure.GPIO_Pin = FLASH_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_Init(FLASH_PORT, &GPIO_InitStructure);

	FLASH_CS_LOW();
	spi_Write(FLASH_CMD_WRSR);
	spi_Write(0x02);
	FLASH_CS_HIGH();
}


void my_FlashReadBuffer(unsigned char* pBuffer, unsigned int ReadAddr, unsigned int NumByteToRead)
{
	/*!< Select the FLASH: Chip Select low */
	FLASH_CS_LOW();

	/*!< Send "Read from Memory " instruction */
	spi_Write(FLASH_CMD_READ);

	/*!< Send ReadAddr high nibble address byte to read from */
	spi_Write((ReadAddr & 0xFF0000) >> 16);
	/*!< Send ReadAddr medium nibble address byte to read from */
	spi_Write((ReadAddr& 0xFF00) >> 8);
	/*!< Send ReadAddr low nibble address byte to read from */
	spi_Write(ReadAddr & 0xFF);

	while (NumByteToRead--) 	/*!< while there is data to be read */
	{
		/*!< Read a byte from the FLASH */
		*pBuffer = spi_WriteAndRead(FLASH_DUMMY_BYTE);
		/*!< Point to the next location where the byte read will be saved */
		pBuffer++;
	}

	/*!< Deselect the FLASH: Chip Select high */
	FLASH_CS_HIGH();
}



static void FlashWaitForWriteEnd(void)
{
	uint8_t flashstatus = 0;

	FLASH_CS_LOW();
	spi_Write(FLASH_CMD_RDSR);

	do{
		flashstatus = spi_WriteAndRead(FLASH_DUMMY_BYTE);
	}
	while ((flashstatus & FLASH_WIP_FLAG) == SET);

	FLASH_CS_HIGH();
}


void my_FlashWritePage(const unsigned char* pBuffer, unsigned int WriteAddr, unsigned int NumByteToWrite)
{
	/*!< Enable the write access to the FLASH */
	//kmy_FlashWriteEnable();
	FLASH_CS_LOW();
	spi_Write(FLASH_CMD_WREN);
	FLASH_CS_HIGH();

	/*!< Select the FLASH: Chip Select low */
	FLASH_CS_LOW();
	/*!< Send "Write to Memory " instruction */
	spi_Write(FLASH_CMD_WRITE);
	/*!< Send WriteAddr high nibble address byte to write to */
	spi_Write((WriteAddr & 0xFF0000) >> 16);
	/*!< Send WriteAddr medium nibble address byte to write to */
	spi_Write((WriteAddr & 0xFF00) >> 8);
	/*!< Send WriteAddr low nibble address byte to write to */
	spi_Write(WriteAddr & 0xFF);

	/*!< while there is data to be written on the FLASH */
	while (NumByteToWrite--){
		/*!< Send the current byte */
		spi_Write(*pBuffer);
		/*!< Point on the next byte to be written */
		pBuffer++;
	}

	/*!< Deselect the FLASH: Chip Select high */
	FLASH_CS_HIGH();

	/*!< Wait the end of Flash writing */
	FlashWaitForWriteEnd();
}
#else
/*void kmy_FlashInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	FLASH_CS_HIGH();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//!< Configure FLASH_SPI pins: SCK 
	GPIO_InitStructure.GPIO_Pin = FLASH_SCK_PIN|FLASH_CS_PIN|FLASH_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(FLASH_PORT, &GPIO_InitStructure);

	//!< Configure FLASH_SPI pins: MISO 
	GPIO_InitStructure.GPIO_Pin = FLASH_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_Init(FLASH_PORT, &GPIO_InitStructure);

	FLASH_CS_LOW();
	spi_Write(FLASH_CMD_WRSR);
	spi_Write(0x02);
	FLASH_CS_HIGH();
} */


void kmy_FlashReadBuffer(unsigned char* pBuffer, unsigned int ReadAddr, unsigned int NumByteToRead)
{
	FLASH_CS_LOW();
	spi_Write(FLASH_CMD_READ);
	spi_Write((ReadAddr & 0xFF0000) >> 16);
	spi_Write((ReadAddr& 0xFF00) >> 8);
	spi_Write(ReadAddr & 0xFF);

	while (NumByteToRead--)
	{
	*pBuffer = spi_WriteAndRead(FLASH_DUMMY_BYTE);
	pBuffer++;
	}

	FLASH_CS_HIGH(); sat/

	while (NumByteToRead){
		* pBuffer=Read_abyte(ReadAddr);
		ReadAddr++;
		pBuffer++;
		NumByteToRead--;
	}
} 

/* 
static void FlashWaitForWriteEnd(void)
{
	uint8_t flashstatus = 0;

	FLASH_CS_LOW();
	spi_Write(FLASH_CMD_RDSR);

	do{
		flashstatus = spi_WriteAndRead(FLASH_DUMMY_BYTE);
	}
	while ((flashstatus & FLASH_WIP_FLAG) == SET);

	FLASH_CS_HIGH();
}*/



/*void kmy_FlashWritePage(const unsigned char* pBuffer, unsigned int WriteAddr, unsigned int NumByteToWrite)
{
	//!< Enable the write access to the FLASH 
	//kmy_FlashWriteEnable();
	FLASH_CS_LOW();
	spi_Write(FLASH_CMD_WREN);
	FLASH_CS_HIGH();

	//!< Select the FLASH: Chip Select low 
	FLASH_CS_LOW();
	//!< Send "Write to Memory " instruction 
	spi_Write(FLASH_CMD_WRITE);
	//!< Send WriteAddr high nibble address byte to write to 
	spi_Write((WriteAddr & 0xFF0000) >> 16);
	//!< Send WriteAddr medium nibble address byte to write to 
	spi_Write((WriteAddr & 0xFF00) >> 8);
	//!< Send WriteAddr low nibble address byte to write to 
	spi_Write(WriteAddr & 0xFF);

	//!< while there is data to be written on the FLASH 
	while (NumByteToWrite--){
		//!< Send the current byte 
		spi_Write(*pBuffer);
		//!< Point on the next byte to be written 
		pBuffer++;
	}

	//!< Deselect the FLASH: Chip Select high 
	FLASH_CS_HIGH();

	//!< Wait the end of Flash writing 
	FlashWaitForWriteEnd();
} */

/*void kmy_FlashTest(void)
{
	// Private define ------------------------------------------------------------
	#define  FLASH_WriteAddress     0
	#define  FLASH_ReadAddress      FLASH_WriteAddress
	#define  FLASH_SectorToErase    FLASH_WriteAddress

	#define  BufferSize (countof(Tx_Buffer)-1)

	// Private macro -------------------------------------------------------------
	#define countof(a) (sizeof(a) / sizeof(*(a)))
	unsigned char Tx_Buffer[] = "STM32F10x SPI Firmware Library Example: communication with an M25P SPI FLASH";
	unsigned char  Rx_Buffer[BufferSize]={0};

	printf("Flash Test\r\n");
	
	memset(Rx_Buffer, 0, sizeof(Rx_Buffer));
	kmy_FlashReadBuffer(Rx_Buffer, FLASH_ReadAddress, BufferSize);
	printf("Flash Read: \r\n");
	kmy_USART1SendData(Rx_Buffer, BufferSize);


}  */

#endif

