#ifndef __KMY_WIFI_USARTDRV_H
#define __KMY_WIFI_USARTDRV_H


				   

void kmy_WiFi_USARTInit(unsigned long BaudRate);
//�������ܣ����ڳ�ʼ��
//����������brd: ������
//����ֵ����
//ע�������


void kmy_WiFiOnOff (unsigned char OnOff);
//�������ܣ�WiFi����
//����������OnOff��1-->����0-->��
//����ֵ����
//ע�������



void kmy_WiFi_USARTSendString(const unsigned char *Buff);
//�������ܣ� �����ַ���
//����������
//	Buff: �����ַ�����buff
//����ֵ����
//ע�������




void kmy_WiFi_USARTSendData(const unsigned char *Buff, unsigned int SendSize);
//�������ܣ�  ��������
//����������
//	Buff:  �������ݵ�buff
//	SendSize:�������ݵĳ���
//����ֵ����
//ע�������





void kmy_WiFi_USARTReceiveReset(void);
//�������ܣ� ��WiFi_USART�ѽ��ܵ������ֽڶ���
//������������
//����ֵ����
//ע�������




unsigned int kmy_WiFi_USARTReceiveData(unsigned char *Buff,unsigned int RecSize,unsigned long timeout_MilliSeconds);
//�������ܣ� ���ܴ��ڵ�����
//����������
//	Buff: �������ݴ�ŵĵط�
//	RecSize: ���ܶ��ٸ��ֽ�
//	timeout_MilliSeconds: ��ʱ�ĺ�����
//����ֵ�����ؽ��ܵ����ֽ���
//ע�������

char kmy_WiFi_USARTCheckBuff(char *ExpectString);
void kmy_WiFi_USARTShowBuff(void);
unsigned int kmy_WiFi_USARTReadToEnd(char *Buff, unsigned int BuffSize);
int kmy_WiFi_USARTReadTo(char *Buff, short int BuffSize, char *EndString);

#endif
