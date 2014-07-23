#ifndef MenuOperate_H
#define MenuOperate_H




//simple and easy to use
struct FunctionMenuAddIco 
{
	const unsigned char *Ico;				//the ico ahead the string
	unsigned char *MenuStr;			//menu string
	unsigned short size;						//
	unsigned char (*MenuFuc)(unsigned char *,unsigned short);	//menu fuction
};
struct OperateAddIco
{
	struct FunctionMenuAddIco *Menu;	//menu
	const unsigned char MenuTotal;		//Total menus
	unsigned char Menucnt;				//menu count
	unsigned char curFlashRow;			//current hightlight row
	unsigned char StartLine;			//start display row
	unsigned char EndLine;				//end display row
	unsigned char flashflag;			//flash flag
	unsigned char RetVal;				//return value
	unsigned char FucRetVal;			//fuction return value
};

struct MenuFuc_Language2_Pic
{
	unsigned char *MenuStrLocal;		//menu string
	unsigned char *MenuStrOther;	//menu string2
	char (*MenuFuc)(char *msg);	//menu fuction
	const unsigned char *pPic;
};
struct MenuOperate_Language2_Pic{
	struct MenuFuc_Language2_Pic *Menu;		//menu
	unsigned char MenuTotal;		//Total menus
	unsigned char Menucnt;				//menu count
	unsigned char curFlashRow;			//current hightlight row
	unsigned char StartLine;			//start display row
	unsigned char EndLine;				//end display row
	unsigned char flashflag;			//flash flag
	unsigned char RetVal;				//return value
	unsigned char FucRetVal;			//fuction return value
};

//simple and easy to use
struct FunctionMenuAddIco2 
{
	const unsigned char *Ico;			//the ico ahead the string
	const unsigned char *MenuStr;		//menu string
	char (*MenuFuc)(char *msg);			//menu fuction
};
struct OperateAddIco2{
	struct FunctionMenuAddIco2 *Menu;	//menu
	const unsigned char MenuTotal;		//Total menus
	unsigned char Menucnt;				//menu count
	unsigned char curFlashRow;			//current hightlight row
	unsigned char StartLine;			//start display row
	unsigned char EndLine;				//end display row
	unsigned char flashflag;			//flash flag
	unsigned char RetVal;				//return value
	unsigned char FucRetVal;			//fuction return value
};


//simple and easy to use
struct MenuFuc
{
	unsigned char *MenuStr;			//menu string
	char (*MenuFuc)(char *msg);		//menu fuction
};
struct MenuOperate
{
	struct MenuFuc *Menu;				//menu
	unsigned char MenuTotal;			//Total menus
	unsigned char Menucnt;				//menu count
	unsigned char curFlashRow;			//current hightlight row
	unsigned char StartLine;			//start display row
	unsigned char EndLine;				//end display row
	unsigned char flashflag;			//flash flag
	unsigned char RetVal;				//return value
	unsigned char FucRetVal;			//fuction return value
};

//simple and easy to use
struct MenuFuc_Language2
{
	unsigned char *MenuStrLocal;		//menu string
	unsigned char *MenuStrOther;	//menu string2
	char (*MenuFuc)(char *msg);	//menu fuction
};
struct MenuOperate_Language2{
	struct MenuFuc_Language2 *Menu;		//menu
	const unsigned char MenuTotal;		//Total menus
	unsigned char Menucnt;				//menu count
	unsigned char curFlashRow;			//current hightlight row
	unsigned char StartLine;			//start display row
	unsigned char EndLine;				//end display row
	unsigned char flashflag;			//flash flag
	unsigned char RetVal;				//return value
	unsigned char FucRetVal;			//fuction return value
};

//simple and easy to use
struct MenuFucSameFuc
{
	unsigned char *MenuStr;		//menu string
};

struct MenuOperateSameFuc
{
	struct MenuFucSameFuc *Menu;	//menu
	unsigned char MenuTotal;		//Total menus
	unsigned char Menucnt;				//menu count
	unsigned char curFlashRow;			//current hightlight row
	unsigned char StartLine;			//start display row
	unsigned char EndLine;				//end display row
	unsigned char flashflag;			//flash flag
	unsigned char RetVal;				//return value
	unsigned char FucRetVal;			//fuction return value
	unsigned char *Ico;				//the start of ico
};




//simple and easy to use
struct OperateDisplay{
	unsigned char MenuTotal;			//Total menus
	unsigned char Menucnt;				//menu count
	unsigned char curFlashRow;			//current hightlight row
	unsigned char StartLine;			//start display row
	unsigned char EndLine;				//end display row
	unsigned char flashflag;			//flash flag
};



void MenuOperateDeInit(struct MenuOperate *MenuOp,struct MenuFuc *Menu,unsigned char MenuTotal,unsigned char StartLine,unsigned char EndLine);
void MenuHandler(struct MenuOperate *MenuOp);
void MenuHandler_Language2(struct MenuOperate_Language2 *MenuOp);
void MenuHandler_Language2_Pic(struct MenuOperate_Language2_Pic *MenuOp);
void MenuHandler_AddIco2(struct OperateAddIco2 *MenuOp);
void MenuHandler_AddIco(struct OperateAddIco *MenuOp);
void MenuOperateSameFucHandler(struct MenuOperateSameFuc *MenuOp);
//unsigned char PleaseSelect(const unsigned char *StrLocal,const unsigned char *StrOther,unsigned char TotalObs,const unsigned char *Obs,...);
unsigned char GetKeyNumVal(unsigned char keyval);
void DisplayMenuPercent(struct MenuOperate *MenuOp);


void PostKey(unsigned char key);
void KeyEventPost(void);

#endif


