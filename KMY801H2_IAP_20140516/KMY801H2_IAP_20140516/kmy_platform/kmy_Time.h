#ifndef __KMY_Time_H
#define __KMY_Time_H








void kmy_TimeInit(void);
//�������ܣ� ʱ���ʼ��
//������������
//����ֵ����
//ע�������


void kmy_RCCInit(void);
//�������ܣ� ϵͳRCC  ��ʼ��
//������������
//����ֵ����
//ע�������



unsigned long kmy_TimeGetMilliseconds(void);
//�������ܣ� ȡ��ǰϵͳ(�ӿ���������)  �ĺ�����
//������������
//����ֵ����ǰ������
//ע�������


//unsigned long kmy_TimeGetRtcSeconds(void);
//�������ܣ� ȡ��ǰϵͳRTC ʱ��
//������������
//����ֵ����ǰRTC ������
//ע�������

/*
void kmy_TimeGetTime(unsigned int *year, unsigned char *mon,
unsigned char *day, unsigned char *hour,
unsigned char *min, unsigned char *sec,unsigned char *weekday);
*/
//�������ܣ� �õ�rtcʱ��
//����������
//	year: ��
//	mon: ��
//	day: ��
//	hour: ʱ
//	min: ��
//	sec: ��
//	weekday:����
//����ֵ����
//ע�������


/*
void kmy_TimeSetTime(const unsigned int year, const unsigned char mon,
const unsigned char day, const unsigned char hour,
const unsigned char min, const unsigned char sec);
*/
//�������ܣ� ����rtcʱ��
//����������
//	year: ��
//	mon: ��
//	day: ��
//	hour: ʱ
//	min: ��
//	sec: ��
//����ֵ����
//ע�������





void sleep(unsigned long seconds);
//�������ܣ� �ȴ���ʱ
//����������
//seconds: ��ʱ������
//����ֵ����
//ע�������ʱ����Ϊ+-2ms




void msleep(unsigned long ms);
//�������ܣ� �ȴ���ʱ
//����������
//ms: ��ʱ�ĺ�����
//����ֵ����
//ע�������ʱ����Ϊ-1��ms������
//msleep(1) ʵ����ʱʱ��Ϊ0.01-1ms��
//msleep(2)ʵ����ʱʱ��Ϊ1.01-2ms




//void usleep(unsigned long us);
//�������ܣ� �ȴ���ʱ
//����������
//us: ��ʱ��΢����
//����ֵ����
//ע�������ʱ����Ϊ+-2us





#endif
