#ifndef __KMY_LCDDRV_H
#define __KMY_LCDDRV_H



#define mode_White		1	//填白模式
#define mode_Black		2	//填黑模式
#define mode_Reverse	3	//反显模式
#define mode_OnlyEdge	5	//边框模式



/*
    Function: LCD initialization
    Function parameter:no
    Return value: no
*/
void kmy_LcdInit(void);


/*
    Function: clear LCD
    Function parameter:no
    Return value: no
*/
void kmy_LcdClear(void);


/*
    Function: Turn on LCD backlight
    Function parameter:no
    Return value: no
*/
void kmy_LcdBlackLightOn(void);


/*
    Function: Turn off LCD backlight
    Function parameter:no
    Return value: no
*/
void kmy_LcdBlackLightOff(void);


/*
    Function: LCD draw bit map
    Function parameter:Bitmap:bitmap data
                      xpos: bitmap x axis
                      ypos: bitmap y axis
                      BmWidth:bitmap width
                      BmHigh:bitmap height
    
                Return value: no
                Notice:bitmap format is longitudinal mod,byte in reverse order
*/   
void kmy_LcdDrawBitMap(const unsigned char *Bitmap,unsigned char xpos,unsigned char ypos,unsigned char BmWidth,unsigned char BmHigh);


/*
    Function: display character string
    Function parameter:xpos: display byte's x axis  
                     ypos: display byte's y axis
                     *Str: character string buffer
    Return value: no
*/         
void kmy_LcdTextOut(unsigned char xpos,unsigned char ypos,const unsigned char *Str);


/*
    Function: draw line
    Function parameter:start_xpos:line's starting coordinates on x axis
                    start_ypos:line's starting coordinates on y axis
                    end_xpos:line's ending coordinates on x axis
                    end_ypos: line's ending coordinates on y axis
    Return value: no
*/
void kmy_LcdDrawLine(unsigned char start_xpos,unsigned char start_ypos,unsigned char end_xpos,unsigned char end_ypos);


/*
    Function: draw rectangle
    Function parameter:start_xpos:rectangle's starting coordinates on x axis
                    start_ypos:rectangle's starting coordinates on y axis
                    end_xpos:rectangle's ending coordinates on x axis
                    end_ypos: rectangle's ending coordinates on y axis
    Return value: no
    　　Notice:    mode==mode_White ,clear content in rectangle
    　　		  mode==mode_Black,turn content in rectangle to balck
    　　		  mode==mode_Reverse,reverse rectangle content
    　　		  mode==mode_OnlyEdge,draw only edge
*/
void kmy_LcdDrawRect(unsigned char start_xpos,unsigned char start_ypos,unsigned char end_xpos,unsigned char end_ypos,unsigned char mode);


/*
　　Function:set contrast of  LCD
　　Function parameter: level: contrast(value area 0---100)
*/
void kmy_LcdSetContrast (unsigned char level);



#endif


