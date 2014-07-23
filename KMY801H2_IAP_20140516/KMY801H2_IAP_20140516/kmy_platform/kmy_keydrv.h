#ifndef __KMY_KEYDRV_H
#define __KMY_KEYDRV_H

// for kmy801h--key define
#define KEY_DEL				        0x01	// Delete
#define KEY3				        0x02	// 3
#define KEYXING			            0x05	// *
#define KEY7				        0x04	// 7
#define KEY5				        0x03	// 5

#define KEY_UP				        0x08	// UP
#define KEY_R3				        0x07	// M1
#define KEY8				        0x0a	// 8
#define KEY0				        0x0b	// 0
#define KEY6				        0x09	// 6

#define KEY_R4				        0x0d	// M2
#define KEY1				        0x0E	// 1
#define KEY_DOWN			        0x0F	// DOWN
#define KEY9				        0x10	// 9
#define KEYJING			            0x11	// #

#define KEY_ReturnInitInterface	    0x13    // KEY_EXIT
#define KEY2				        0x14	// 2
#define KEY4				        0x15	// 4
#define KEY_ReturnPrevious	        0x16	// Return
#define KEY_Enter			        0x17	// ENTER

#define KEY_NONE			        0x00
#define	KEY_R1	                    KEY_Enter
#define	KEY_R2	                    KEY_ReturnInitInterface

#define KEY_pseudo					255			//pseudo key
#define KEY_WaitTimeout				254			//wait timeout pseudo key

#define KEY_LEFT			        KEY_R3
#define KEY_RIGHT			        KEY_R4
#define KEY_R6				        0x19


void kmy_KeyInit(void);
//�������ܣ�key ��ʼ��
//������������
//����ֵ����
//ע�������


unsigned char kmy_GetKeyValue(void);
//�������ܣ�ȡ����ֵ
//������������
//����ֵ��
//	���水���궨��
//ע�������


void kmy_AutoOffLcdLight(unsigned long seconds);
//�������ܣ����ö������ر�lcd �ı���
//����������
//	seconds: ʱ�������
//����ֵ����
//ע�����
// 	1 ���seconds==0ȡ�����õ�����
// 	2 ���û��ʱ�����������������
// 	֮ǰ���õ�������ȡ�����Ա��ε�
// 	���������¼���


void kmy_AutoPostKey(unsigned long seconds);
//�������ܣ����ö�������Զ�����KEY_WaitTimeout
//����������
//	seconds: ʱ�������
//����ֵ����
//ע�����
// 	1 ���seconds==0ȡ�����õ�����
// 	2 ���û��ʱ�����������������
// 	֮ǰ���õ�������ȡ�����Ա��ε�
// 	���������¼���
// 	3 �����������������kmy_GetKeyValue()����
// 	�����������ϻ�õ�KEY_WaitTimeout ����ֵ


void kmy_PostKey(unsigned char key);
//�������ܣ�ֱ���ü�ֵ
//����������
//	key: ����ֵ�������ļ��ĺ궨��
//����ֵ����
//ע�����
//	1 ��ִ���������֮����kmy_GetKeyValue()����
// 	�����������ϻ�õ��β�key �� ����ֵ
// 	2 ���������һЩ����ĳ����ã�һ���û������õ�



void kmy_EnterPowerDownMode(void);
//�������ܣ�����͹��ĵ�ֹͣģʽ
//������������
//����ֵ����
//ע�����
//	����˺�������Ļ�����κ���ʾ
//	��������˳��͹��ĵ�ֹͣģʽ


#endif
