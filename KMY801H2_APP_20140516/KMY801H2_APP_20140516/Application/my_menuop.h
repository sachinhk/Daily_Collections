
//simple and easy to use
struct my_MenuFuc
{
	unsigned char *MenuStr;		//menu string
	char (*MenuFuc)(char *msg);	//menu fuction
};

struct my_MenuOp{
	unsigned char MenuTotal;		//Total menus
	unsigned char Menucnt;				//menu count
	unsigned char oldFlashRow;			//current hightlight row
	unsigned char curFlashRow;			//current hightlight row
	unsigned char StartLine;		//start display row
	unsigned char EndLine;		//end display row
	unsigned char flashflag;			//flash flag
};

struct my_MenuOperate{
	struct my_MenuFuc *Menu;				//menu
	struct my_MenuOp op;
	unsigned char RetVal;				//return value
};




#define FlushAllFlag			0x01
#define FlushOnlyHightRowFlag	0x02
#define FlushUpkeyFlag			0x04
#define FlushDownkeyFlag		0x08
#define FlushRightkeyFlag		0x10
#define FlushLeftkeyFlag		0x20





void MenuOperate_response_keyLeft(struct my_MenuOp *op);
void MenuOperate_response_keyRight(struct my_MenuOp *op);
void MenuOperate_response_keyDown(struct my_MenuOp *op);
void MenuOperate_response_keyUp(struct my_MenuOp *op);
void my_DisplayMenuPercent(struct my_MenuOp *op);
unsigned char MenuOperate_response_key1_9(struct my_MenuOp *op,unsigned char keyval);

void my_MenuHandler(struct my_MenuOperate *MenuOp);




