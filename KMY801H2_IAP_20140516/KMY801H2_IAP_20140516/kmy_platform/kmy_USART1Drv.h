#ifndef __KMY_USART1DRV_H
#define __KMY_USART1DRV_H




void kmy_USART1Init(unsigned long BaudRate);
//�������ܣ����ڳ�ʼ��
//����������
//	brd: ������
//����ֵ����
//ע�������




void kmy_USART1SendString(const unsigned char *Buff);
//�������ܣ� �����ַ���
//����������
//	Buff: �����ַ�����buff
//����ֵ����
//ע�������



void kmy_USART1SendData(const unsigned char *Buff, unsigned int SendSize);
//�������ܣ�  ��������
//����������
//	Buff:  �������ݵ�buff
//	SendSize:�������ݵĳ���
//����ֵ����
//ע�������





//void kmy_USART1ReceiveReset(void);
//�������ܣ� ��USART1 �ѽ��ܵ������ֽڶ���
//������������
//����ֵ����
//ע�������




//unsigned int kmy_USART1ReceiveData(unsigned char *Buff,unsigned int RecSize,unsigned long timeout_MilliSeconds);
//�������ܣ� ���ܴ��ڵ�����
//����������
//	Buff: �������ݴ�ŵĵط�
//	RecSize: ���ܶ��ٸ��ֽ�
//	timeout_MilliSeconds: ��ʱ�ĺ�����
//����ֵ�����ؽ��ܵ����ֽ���
//ע�������


#endif
