#ifndef __INPUT_H
#define __INPUT_H



#define	SRF_abc		0x04
#define	SRF_ABC		0x08
#define	SRF_123		0x10



void text_out_to_row (unsigned char row, const unsigned char *buff);
void PostKey (unsigned char key);
char GetString_abcABC123PYWuBi (unsigned char srfFlag, unsigned char DefaultSrf, unsigned char *receive, unsigned char recSize, unsigned char HaveoldFlag);





#endif

