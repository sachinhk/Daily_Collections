
#ifndef FormInput_h
#define FormInput_h




struct sFormInput
{
	const unsigned char *Title;	//������
	unsigned char *TitleResult;	//������뻺����
	unsigned char MaxInputSize;	//������������ֽ������м���ʹ��С��TitleResult  ��������С��ֵ
};





#define FORMINPUT_TITLE_TO_LONG				0x01
#define FORMINPUT_TITLE_NUMBER_ERROR		0x02
#define FORMINPUT_KEY_ReturnPrevious		0x03
#define FORMINPUT_KEY_ReturnInitInterface	0x04
#define FORMINPUT_FIRST_CONFIRM_RETURN		0x05
#define FORMINPUT_SECOND_CONFIRM_RETURN		0x06




#define INPUT_MODE_123		0x01
#define INPUT_MODE_abc		0x02
#define INPUT_MODE_ABC		0x04
#define INPUT_MODE_BiHua	0x08
#define INPUT_MODE_PY		0x10









//�������ܣ��������
//����������
//		1. pFI: �������ṹ��ָ��
//		2. TitleNumber: �������Ŀ
//����ֵ��
//		1. ���ͣ�ڵ�һ��������ť����ȷ�ϼ�����FORMINPUT_FIRST_CONFIRM_RETURN
//		2. ���ͣ�ڵڶ���������ť����ȷ�ϼ�����FORMINPUT_SECOND_CONFIRM_RETURN
//		3. ��KEY_ReturnPrevious������FORMINPUT_KEY_ReturnPrevious
//		4. ��KEY_ReturnInitInterface������FORMINPUT_KEY_ReturnInitInterface
//		5. ��������С��3 ����FORMINPUT_TITLE_NUMBER_ERROR
//		6. ��ʾ����̫��(����16��ascii ��)����FORMINPUT_TITLE_TO_LONG
//ע�����
//		1. �˺���������������������Ϊ������ť����ʾ�����һ��
//		2. �ṹ���ԱMaxInputSize ����С�ڽṹ���ԱTitleResult ��������С
//		3. ���غ�����������ַ�����ڣ��ṹ���ԱTitleResult  ��
//		4. ���뺯��ʱ����ѳ�ԱTitleResult ��ʾ�ڱ���ڣ����н���֮ǰ
//		    ����Գ�ԱTitleResult  ���г�ʼ��
unsigned char FormInput(struct sFormInput *pFI,unsigned char TitleNumber);






#endif


