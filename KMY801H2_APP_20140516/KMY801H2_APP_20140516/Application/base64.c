
#include "base64.h"



#if 0
static signed char index_64[128] = {	
	0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xff,	
	0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xff,	
	0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xff, 0xff,0xff,0xff,62, 0xff,0xff,0xff,63,	
	52,53,54,55, 56,57,58,59, 60,61,0xff,0xff, 0xff,0xff,0xff,0xff,	
	0xff, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,	
	15,16,17,18, 19,20,21,22, 23,24,25,0xff, 0xff,0xff,0xff,0xff,	
	0xff,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,	
	41,42,43,44, 45,46,47,48, 49,50,51,0xff, 0xff,0xff,0xff,0xff	
} ;
static char basis_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define CHAR64(c)  (((c) < 0 || (c) > 127) ? -1 : index_64[(c)])

unsigned int Base64_Encode(const unsigned char *value,unsigned char *result, unsigned int vlen) {
	unsigned char oval = 0 ; 	
	//char *result = (char *)malloc((vlen * 4) / 3 + 5) ;	
	unsigned char *out = result;

	while (vlen >= 3) {	
		*out++ = basis_64[value[0] >> 2];	
		*out++ = basis_64[((value[0] << 4) & 0x30) | (value[1] >> 4)];	
		*out++ = basis_64[((value[1] << 2) & 0x3C) | (value[2] >> 6)];	
		*out++ = basis_64[value[2] & 0x3F];	
		value += 3;	
		vlen -= 3;	
	}
	if (vlen > 0) {	
		*out++ = basis_64[value[0] >> 2];	
		oval = (value[0] << 4) & 0x30 ;	
		if (vlen > 1) oval |= value[1] >> 4;	
		*out++ = basis_64[oval];	
		*out++ = (vlen < 2) ? '=' : basis_64[(value[1] << 2) & 0x3C];	
		*out++ = '=';	
	}	
	*out = '\0';

	return (unsigned int)(out-result);
}	



unsigned int Base64_Decode(const unsigned char *value,unsigned char *result,unsigned int rlen)	
{		
	int c1, c2, c3, c4; 			
	unsigned char *out = result;	
	
	rlen = 0;
	
	while (1) { 
		if (value[0]==0) {
			out = '\0' ; 
			return rlen;	
		}
		c1 = value[0];	
		if (CHAR64(c1) == -1) goto base64_decode_error;
		c2 = value[1]; 
		if (CHAR64(c2) == -1) goto base64_decode_error;
		c3 = value[2]; 
		if ((c3 != '=') && (CHAR64(c3) == -1)) goto base64_decode_error;
		c4 = value[3]; 
		if ((c4 != '=') && (CHAR64(c4) == -1)) goto base64_decode_error;	
		value += 4;	
		*out++ = (CHAR64(c1) << 2) | (CHAR64(c2) >> 4);	
		rlen += 1;	
		if (c3 != '=') {	
			*out++ = ((CHAR64(c2) << 4) & 0xf0) | (CHAR64(c3) >> 2);	
			rlen += 1; 
			if (c4 != '=') {	
				*out++ = ((CHAR64(c3) << 6) & 0xc0) | CHAR64(c4);	
				rlen += 1; 
			}
		}	
	}	

	base64_decode_error:	
	return rlen;	
}

#endif

#if 1
static const unsigned char index_64[128] = {
	0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xff,	
	0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xff,	
	0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xff, 0xff,0xff,0xff,62, 0xff,0xff,0xff,63,	
	52,53,54,55, 56,57,58,59, 60,61,0xff,0xff, 0xff,0xff,0xff,0xff,	
	0xff, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,	
	15,16,17,18, 19,20,21,22, 23,24,25,0xff, 0xff,0xff,0xff,0xff,	
	0xff,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,	
	41,42,43,44, 45,46,47,48, 49,50,51,0xff, 0xff,0xff,0xff,0xff	
} ;
static const unsigned char basis_64[]={"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};

#define CHAR64(c)  (((c) < 0 || (c) > 127) ? -1 : index_64[(c)])

//src: 需要编码的数据buff
//dest: 编码后存放的buff, 注意buff的长度必需大于srcLen/3*4+3
//srcLen: 需要编码数据的长度
//返回编码后字符串的长度
unsigned int Base64_Encode(const unsigned char *src,unsigned char *dest, unsigned int srcLen)
{
	unsigned char oval = 0;
	unsigned char *out = dest;

	while (srcLen >= 3)
	{
		*out++ = basis_64[src[0] >> 2];
		*out++ = basis_64[((src[0] << 4) & 0x30) | (src[1] >> 4)];
		*out++ = basis_64[((src[1] << 2) & 0x3C) | (src[2] >> 6)];
		*out++ = basis_64[src[2] & 0x3F];
		src += 3;
		srcLen -= 3;
	}

	if (srcLen > 0)
	{
		*out++ = basis_64[src[0] >> 2];
		oval = (src[0] << 4) & 0x30 ;
		if (srcLen > 1) oval |= src[1] >> 4;
		*out++ = basis_64[oval];
		*out++ = (srcLen < 2) ? '=' : basis_64[(src[1] << 2) & 0x3C];
		*out++ = '=';
	}

	*out = '\0';

	return (unsigned int)(out-dest);
}	


//src: 需要解码的buff
//dest: 解码后存放的buff
//srcLen: 需要解码数据的长度
//返回解码后数据的长度
unsigned int Base64_Decode(const unsigned char *src,unsigned char *dest,unsigned int srcLen)	
{		
	int c1, c2, c3, c4;
	unsigned char *out=dest;

	srcLen = 0;

	while(1)
	{
		if (src[0]==0)
		{
			out = '\0' ;
			return srcLen;
		}
		c1 = src[0];
		if (CHAR64(c1) == -1) goto base64_decode_error;
		c2 = src[1];
		if (CHAR64(c2) == -1) goto base64_decode_error;
		c3 = src[2];
		if ((c3 != '=') && (CHAR64(c3) == -1)) goto base64_decode_error;
		c4 = src[3];
		if ((c4 != '=') && (CHAR64(c4) == -1)) goto base64_decode_error;
		src += 4;
		*out++ = (CHAR64(c1) << 2) | (CHAR64(c2) >> 4);
		srcLen += 1;
		if (c3 != '=')
		{
			*out++ = ((CHAR64(c2) << 4) & 0xf0) | (CHAR64(c3) >> 2);	
			srcLen += 1;
			if (c4 != '=')
			{
				*out++ = ((CHAR64(c3) << 6) & 0xc0) | CHAR64(c4);	
				srcLen += 1;
			}
		}
	}	

	base64_decode_error:
	return srcLen;
}
#endif


