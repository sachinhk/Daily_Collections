#ifndef __KMY_LCDDRV_H
#define __KMY_LCDDRV_H



#define mode_White		1	//���ģʽ
#define mode_Black		2	//���ģʽ
#define mode_Reverse	3	//����ģʽ
#define mode_OnlyEdge	5	//�߿�ģʽ





void kmy_LcdInit(void);
//�������ܣ�lcd ��ʼ��
//������������
//����ֵ����
//ע�������


void kmy_LcdClear(void);
//�������ܣ����lcd
//������������
//����ֵ����
//ע�������


void kmy_LcdBlackLightOn(void);
//�������ܣ�����lcd  ����
//������������
//����ֵ����
//ע�������


void kmy_LcdBlackLightOff(void);
//�������ܣ��ر�lcd  ����
//������������
//����ֵ����
//ע�������


void kmy_LcdDrawBitMap(const unsigned char *Bitmap,unsigned char xpos,unsigned char ypos,unsigned char BmWidth,unsigned char BmHigh);
//�������ܣ�lcd  ��λͼ
//����������
//	Bitmap: λͼ����
//	xpos: λͼ��x������
//	ypos: λͼ��y ������
//	Width:λͼ�Ŀ��
//	BmHigh:λͼ�ĸ߶�
//����ֵ����
//ע�����
//λͼ��ʽΪ:��ɫ����Һ����ģ������ȡģ���ֽڵ���


void kmy_LcdTextOut(unsigned char xpos,unsigned char ypos,const unsigned char *Str);
//�������ܣ���ʾ�ַ���
//����������
//	xpos: ��ʾ�ַ���x������
//	ypos: ��ʾ�ַ���y ������
//	Str: �ַ���������
//����ֵ����
//ע�������


void kmy_LcdDrawLine(unsigned char start_xpos,unsigned char start_ypos,unsigned char end_xpos,unsigned char end_ypos);
//�������ܣ�����
//����������
//	start_xpos: �ߵ���ʼx������
//	start_ypos: �ߵ���ʼy������
//	end_xpos: �ߵĽ���x������
//	end_ypos: �ߵĽ���y������
//����ֵ����
//ע�������


void kmy_LcdDrawRect(unsigned char start_xpos,unsigned char start_ypos,unsigned char end_xpos,unsigned char end_ypos,unsigned char mode);
//�������ܣ�������
//����������
//	start_xpos: �������ʼx������
//	start_ypos: �������ʼy������
//	end_xpos:����Ľ���x������
//	end_ypos:����Ľ���y������
//	mode: �������ʾ��ʽ
//����ֵ����
//ע�����
//	mode==mode_White������������
//	mode==mode_Black��ڷ��������
//	mode==mode_Reverse���Է��������
//	mode==mode_OnlyEdgeֻ������ı߿�


//void kmy_LcdSetContrast (unsigned char level);
//�������ܣ�����lcd �ĶԱȶ�
//����������
//	level: �Աȶ�(ȡֵ��Χ0---100)
//����ֵ����
//ע�������



#endif


