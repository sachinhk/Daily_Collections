#include <string.h>
#include <stdio.h>

#include "BarCode.h"
#include "kmy_PrinterDrv.h"


/*Code128编码表*/
unsigned short  CodingSheet[107]=
{
	0xD980, 0xCD80, 0xCCC0, 0x9300, 0x9180, 0x8980, 0x9900, 0x9880, 
	0x8C80, 0xC900, 0xC880, 0xC480, 0xB380, 0x9B80, 0x99C0, 0xB980, 
	0x9D80, 0x9CC0, 0xCE40, 0xCB80, 0xC9C0, 0xDC80, 0xCE80, 0xEDC0, 
	0xE980, 0xE580, 0xE4C0, 0xEC80, 0xE680, 0xE640, 0xDB00, 0xD8C0, 
	0xC6C0, 0xA300, 0x8B00, 0x88C0, 0xB100, 0x8D00, 0x8C40, 0xD100, 
	0xC500, 0xC440, 0xB700, 0xB1C0, 0x8DC0, 0xBB00, 0xB8C0, 0x8EC0, 
	0xEEC0, 0xD1C0, 0xC5C0, 0xDD00, 0xDC40, 0xDDC0, 0xEB00, 0xE8C0, 
	0xE2C0, 0xED00, 0xEC40, 0xE340, 0xEF40, 0xC840, 0xF140, 0xA600, 
	0xA180, 0x9600, 0x90C0, 0x8580, 0x84C0, 0xB200, 0xB080, 0x9A00, 
	0x9840, 0x8680, 0x8640, 0xC240, 0xCA00, 0xF740, 0xC280, 0x8F40, 
	0xA780, 0x9780, 0x93C0, 0xBC80, 0x9E80, 0x9E40, 0xF480, 0xF280, 
	0xF240, 0xDBC0, 0xDEC0, 0xF6C0, 0xAF00, 0xA3C0, 0x8BC0, 0xBD00, 
	0xBC40, 0xF500, 0xF440, 0xBBC0, 0xBDC0, 0xEBC0, 0xF5C0, 0xD080, 
	0xD200, 0xD380, 0xC758
};

#define USE_CODE128	   0x00
#define USE_CODE128A 0x01
#define USE_CODE128B 0x02


int BarCode128(unsigned char *lpOut, char *lpIn, int inlen)
{
	int i = 0;
	int bitlen = 0;
	unsigned usecode = USE_CODE128;
	unsigned char id;
	int verify;
	int j = 0;
	int tmpbitcount;

	/*检查字符的合法性*/
	for(i = 0; i < inlen; i++)
	{
		if(lpIn[i] < 0)
		{
			return -1;//字符非法
		}
		else if(lpIn[i] <= 31)
		{
			/*Code128A*/
			if(usecode == USE_CODE128B)
			{
				return -1;
			}
			else
			{
				usecode = USE_CODE128A;
			}
		}
		else if(lpIn[i] >= 96)
		{
			/*Code128A*/
			if(usecode == USE_CODE128A)
			{
				return -1;
			}
			else
			{
				usecode = USE_CODE128B;
			}
		}
	}

	if(usecode == USE_CODE128)
	{
		usecode = USE_CODE128A;/*默认按照code128A来编码*/
	}

	bitlen = 0;
	if(usecode == USE_CODE128A)
	{
		id = 103;
	}
	else
	{
		id = 104;
	}

	verify = id;
	lpOut[bitlen / 8] = (CodingSheet[id] >> 8) & 0x00FF;
	lpOut[bitlen / 8 + 1] = CodingSheet[id] & 0x00FF;
	bitlen += 11;

	for(i = 0; i < inlen + 2; i++)
	{
		if(usecode == USE_CODE128B)
		{
			if(i < inlen)
			{
				id = lpIn[i] - 32;
			}
			else if(i == inlen)
			{
				id = verify % 103;
			}
			else if(i == inlen + 1)
			{
				id = 106;
			}
		}
		else
		{
			if(i < inlen)
			{
				if(lpIn[i] >= 32)
				{
					id = lpIn[i] - 32;
				}
				else
				{
					id = lpIn[i] + 96 - 32;
				}
			}
			else if(i == inlen)
			{
				id = verify % 103;
			}
			else if(i == inlen + 1)
			{
				id = 106;
			}
		}
		tmpbitcount = 0;
		verify += ((int)id) * (i + 1);
		j = 8 - (bitlen % 8);
		lpOut[bitlen / 8] &= (0xFF << j);
		lpOut[bitlen / 8] |= (CodingSheet[id] >> (16 - j));
		tmpbitcount += j;
		lpOut[bitlen / 8 + 1] = (CodingSheet[id] << tmpbitcount) >> 8;
		tmpbitcount += 8;
		if(id < 106 && tmpbitcount < 11)
		{

			lpOut[bitlen / 8 + 2] = (CodingSheet[id] << tmpbitcount) >> 8;
		}
		else if(tmpbitcount < 13)
		{
			lpOut[bitlen / 8 + 2] = (CodingSheet[id] << tmpbitcount) >> 8;
		}
		if(id < 106)
		{
			bitlen += 11;
		}
		else
		{
			bitlen += 13;
		}
	}

	return bitlen;
}

void Middle(char *D_buff, char *S_buff,unsigned char NumPerLine)
{
    unsigned char i,len;

	len = (NumPerLine - strlen(S_buff))/2;
    if(len>=NumPerLine/2)
	{
		strcat(D_buff,S_buff);
		return;
	}
	for(i=0;i<len;i++)
	{
		strcat(D_buff," ");
	}	
	strcat(D_buff,S_buff);
}

char PrintBarCode(char *Databuff)
{
    unsigned char BarCode[100]={0};
	unsigned char BmpHex[3000]={0};
	unsigned char printbuff[100]={0};
	unsigned char retval;
	int bitlen=0;
	int i,j=-1;
	int n=0;

	if(Databuff==NULL)return 0;
	if(strlen(Databuff)>10)return 0;

	bitlen=BarCode128(BarCode,Databuff,10);
	printf("bitlen:<%d>\r\n",bitlen);

	for(i=0;i<bitlen;i++)
	{
		if((i%8)==0)j++;
		if((n%8)==0)n=0;
	        if(BarCode[j]&(0x80>>n))
		{
			BmpHex[2*i]=0xff;
			BmpHex[2*i+1]=0xff;
	        }
		else
		{
			BmpHex[2*i]=0x00;
			BmpHex[2*i+1]=0x00;
		}
		n++;
	}
	for(i=1;i<10;i++)
	{
		memcpy(BmpHex+2*i*bitlen,BmpHex,2*bitlen);
	}
	retval= kmy_PrinterBitmap(BmpHex,2*bitlen,80,(384-2*bitlen)/2);
	if(retval!=Printer_OK)return retval;
	Middle((char*)printbuff,(char*)Databuff,16);
	strcat((char*)printbuff,"\n");
	kmy_PrinterSetcommand(WidthZoom2,sizeof(WidthZoom2));
	kmy_PrinterSetcommand(HighZoom1,sizeof(HighZoom1));
	return kmy_PrinterString(printbuff);
}


