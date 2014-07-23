
#ifndef FormInput_h
#define FormInput_h




struct sFormInput
{
	const unsigned char *Title;	//表格标题
	unsigned char *TitleResult;	//表格输入缓冲区
	unsigned char MaxInputSize;	//表格容许输入字节数，切记请使用小于TitleResult  缓冲区大小的值
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









//函数功能：表格输入
//函数参数：
//		1. pFI: 表格输入结构体指针
//		2. TitleNumber: 表格总数目
//返回值：
//		1. 表格停在第一个高亮按钮处按确认键返回FORMINPUT_FIRST_CONFIRM_RETURN
//		2. 表格停在第二个高亮按钮处按确认键返回FORMINPUT_SECOND_CONFIRM_RETURN
//		3. 按KEY_ReturnPrevious键返回FORMINPUT_KEY_ReturnPrevious
//		4. 按KEY_ReturnInitInterface键返回FORMINPUT_KEY_ReturnInitInterface
//		5. 标题总数小于3 返回FORMINPUT_TITLE_NUMBER_ERROR
//		6. 显示标题太长(超过16个ascii 码)返回FORMINPUT_TITLE_TO_LONG
//注意事项：
//		1. 此函数把最后两个表格数据作为高亮按钮，显示在最后一行
//		2. 结构体成员MaxInputSize 必须小于结构体成员TitleResult 缓冲区大小
//		3. 返回后，所有输入的字符存放于，结构体成员TitleResult  内
//		4. 进入函数时，会把成员TitleResult 显示于表格内，所有进入之前
//		    必须对成员TitleResult  进行初始化
unsigned char FormInput(struct sFormInput *pFI,unsigned char TitleNumber);






#endif


