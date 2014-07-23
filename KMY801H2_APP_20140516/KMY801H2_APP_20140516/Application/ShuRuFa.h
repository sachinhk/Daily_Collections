#ifndef ShuRuFa_H
#define ShuRuFa_H


#include "my_stdint.h"



#define	SRF_PY		0x01
#define	SRF_BiHua	0x02
#define	SRF_abc		0x04
#define	SRF_ABC		0x08
#define	SRF_123		0x10


typedef struct 
{
	const u8 *OtherTitle;
	const u8 *LocalTitle;
	u8 *receive;
	u8 recSize;
	u8 xingflag;
	u8 clearlcdflag;
	u8 keyval;
	u8 DiplayRow;
}PasswordInputArgument;


typedef struct 
{
	const u8 *OtherTitle;
	const u8 *LocalTitle;
	u8 *receive;
	u8 recSize;
	u8 srfFlag;
	u8 DefaultSrf;
	u8 clearlcdflag;
	u8 HaveoldFlag;
}abcABC123PYWuBiArgument;


typedef struct 
{
	const u8 *EnglishTitle;
	const u8 *PolishTitle;
	u8 srfFlag;
	u8 DefaultSrf;
	u8 *receive;
	u8 recSize;
	u8 HaveoldFlag;
}ABCabc123BHPYInputArgument;


void flashcursor(signed int x,signed int y);
void abcABC123PYWuBiArgDeInit(abcABC123PYWuBiArgument *arg,u8 *buff,u8 Size);
u8 GetString_abcABC123PYWuBi2(abcABC123PYWuBiArgument *arg);
u8 GetString_abcABC123PYWuBi(const u8 *Title,u8 srfFlag,u8 DefaultSrf,u8 *receive,u8 recSize,u8 HaveoldFlag);
u8 GetString_abcABC123PYWuBi_Language2(const u8 *LocalTitle,const u8 *OtherTitle,u8 srfFlag,u8 DefaultSrf,u8 *receive,u8 recSize,u8 HaveoldFlag);
void PassWordArgDeInit(PasswordInputArgument *arg,u8 *buff,u8 Size);
u8 PassWordInput(PasswordInputArgument *arg);
u8 PassWordInput_MiddledDisplay(PasswordInputArgument *arg);
u8 PassWordInput_XposStartDisplay(PasswordInputArgument *arg,u8 Xpos);
u8 PassWordInput2(const u8 *Title,u8 *receive,u8 recSize,u8 xingflag,u8 clearlcdflag,u8 keyval,u8 DiplayRow);



#endif

