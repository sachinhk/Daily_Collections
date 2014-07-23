#ifndef _flash_
#define _flassh_


#define uint8	unsigned char
#define uint16	unsigned short
#define uint32	unsigned int


/*#define SI 4
#define SO 3
#define SCLK 5
#define CS 2
#define WP 1
#define PORT1 PORTB
#define PIN1 PINB
#define DDR1 DDRB

#define SO_0 PORT1&=~(1<<SO)
#define SO_1 PORT1|=(1<<SO)
#define SCLK_0 PORT1&=~(1<<SCLK)
#define SCLK_1 PORT1|=(1<<SCLK)
#define CS_0 PORT1&=~(1<<CS)
#define CS_1 PORT1|=(1<<CS)
#define in_SI ((1<<SI)&PIN1)

*/
//#define WP_0 PORT1&=~(1<<WP)
//#define WP_1 PORT1|=(1<<WP)


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


#define SO_0 GPIO_ResetBits(GPIOA, GPIO_Pin_7)
#define SO_1 GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define SCLK_0 GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define SCLK_1 GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define CS_0  GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define CS_1 GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define in_SI (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)==1)


uint8 Read_abyte(uint32 address);
void Write_adata(uint32 address,uint8 write_data);
void WriteEN(void);
uint8 Read_RDSR(void);
void WriteDisable(void);
void Sector_Erase(uint32 address);
void Write_string(uint8 *string,uint32 address,uint8 n);
void Bulk_Erase(void);
void DelayX100ms(uint32 x);

#endif



