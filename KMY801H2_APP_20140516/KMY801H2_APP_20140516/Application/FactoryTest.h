#ifndef FactoryTest_h
#define FactoryTest_h


#define TESTITEMCOUNT 9/*≤‚ ‘œÓ ˝ƒø*/

#define LED_BEEP 0x01
#define LED_DATA_LED 0x02
#define LED_PAPER_LED 0x03
#define LED_COMPLETE 0x04

#define TEST_FACTORY 0x01
#define TEST_SELF 0x02

typedef void(*LED_test_fun)(void);

struct strTestData
{
	unsigned char m_title[15];
	unsigned char m_result;/*test result 0:success;1:fail;2:unknow*/
	unsigned int m_commentindex;/*test comment,data index in buf*/
	unsigned int m_commentlen;
};

char start_factory_test(char *msg);
extern char ProcStandbyKey(char *msg);
void check_rtc(void);


#endif
