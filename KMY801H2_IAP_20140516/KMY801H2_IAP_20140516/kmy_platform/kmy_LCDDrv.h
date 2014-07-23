#ifndef __KMY_LCDDRV_H
#define __KMY_LCDDRV_H



#define mode_White		1	//填白模式
#define mode_Black		2	//填黑模式
#define mode_Reverse	3	//反显模式
#define mode_OnlyEdge	5	//边框模式





void kmy_LcdInit(void);
//函数功能：lcd 初始化
//函数参数：无
//返回值：无
//注意事项：无


void kmy_LcdClear(void);
//函数功能：清除lcd
//函数参数：无
//返回值：无
//注意事项：无


void kmy_LcdBlackLightOn(void);
//函数功能：开启lcd  背光
//函数参数：无
//返回值：无
//注意事项：无


void kmy_LcdBlackLightOff(void);
//函数功能：关闭lcd  背光
//函数参数：无
//返回值：无
//注意事项：无


void kmy_LcdDrawBitMap(const unsigned char *Bitmap,unsigned char xpos,unsigned char ypos,unsigned char BmWidth,unsigned char BmHigh);
//函数功能：lcd  画位图
//函数参数：
//	Bitmap: 位图数据
//	xpos: 位图的x轴坐标
//	ypos: 位图的y 轴坐标
//	Width:位图的宽度
//	BmHigh:位图的高度
//返回值：无
//注意事项：
//位图格式为:单色点阵液晶字模，纵向取模，字节倒序


void kmy_LcdTextOut(unsigned char xpos,unsigned char ypos,const unsigned char *Str);
//函数功能：显示字符串
//函数参数：
//	xpos: 显示字符的x轴坐标
//	ypos: 显示字符的y 轴坐标
//	Str: 字符串缓冲区
//返回值：无
//注意事项：无


void kmy_LcdDrawLine(unsigned char start_xpos,unsigned char start_ypos,unsigned char end_xpos,unsigned char end_ypos);
//函数功能：画线
//函数参数：
//	start_xpos: 线的起始x轴坐标
//	start_ypos: 线的起始y轴坐标
//	end_xpos: 线的结束x轴坐标
//	end_ypos: 线的结束y轴坐标
//返回值：无
//注意事项：无


void kmy_LcdDrawRect(unsigned char start_xpos,unsigned char start_ypos,unsigned char end_xpos,unsigned char end_ypos,unsigned char mode);
//函数功能：画方框
//函数参数：
//	start_xpos: 方框的起始x轴坐标
//	start_ypos: 方框的起始y轴坐标
//	end_xpos:方框的结束x轴坐标
//	end_ypos:方框的结束y轴坐标
//	mode: 方框的显示方式
//返回值：无
//注意事项：
//	mode==mode_White清除方框的内容
//	mode==mode_Black填黑方框的内容
//	mode==mode_Reverse反显方框的内容
//	mode==mode_OnlyEdge只画方框的边框


//void kmy_LcdSetContrast (unsigned char level);
//函数功能：设置lcd 的对比度
//函数参数：
//	level: 对比度(取值范围0---100)
//返回值：无
//注意事项：无



#endif


