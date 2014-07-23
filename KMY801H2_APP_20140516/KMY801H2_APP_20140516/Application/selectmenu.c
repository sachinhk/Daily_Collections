#include "GlobalVar.h"
#include "kmy_keydrv.h"
//int selectmenu(char *menu1,char *menu2,char *menu3,int no)
int selectmenu(char *menu1,char *menu2,int no)
{
	
	int x1=6,y1=12,x2=126,y2=36,yfill1=15,n1=1;
	clear_area (5, 12, 121, 48);
	clear_area (xstart,yfill1,xend,15);	
	if(strlen(menu1)>0)
	kmy_LcdTextOut(xstart,15,menu1);
	if(strlen(menu2)>0)
	kmy_LcdTextOut(xstart,30,menu2);
	//if(strlen(menu3)>0)
	//kmy_LcdTextOut(xstart,45,menu3);
	reverse_area(xstart,yfill1,xend,15,2);
	myprintf("\n Select Menu Is called\n");
	Keydata=0;
	//	while(Keydata!=KEY_L1 && Keydata!=KEY_R1)
	while(Keydata!=KEY_ReturnPrevious && Keydata!=KEY_Enter)
	{
		Keydata=0;
		Keydata=kmy_GetKeyValue();
		if(Keydata==KEY_ReturnPrevious)
		return 0;
		if(Keydata==KEY_DOWN && n1<no)
		{
			n1++;
			clear_area (xstart,yfill1,xend,15);	
			if(strlen(menu1)>0)
			kmy_LcdTextOut(xstart,15,menu1);
			if(strlen(menu2)>0)
			kmy_LcdTextOut(xstart,30,menu2);
		//	if(strlen(menu3)>0)
		//	kmy_LcdTextOut(xstart,45,menu3);
			yfill1=yfill1+15;
			reverse_area(xstart,yfill1,xend,15,2);
		}else if(Keydata==KEY_UP && n1>1)
		{	n1--;
			clear_area (xstart,yfill1,xend,15);	
			yfill1=yfill1-15;	
			if(strlen(menu1)>0)
			kmy_LcdTextOut(xstart,15,menu1);
			if(strlen(menu2)>0)
			kmy_LcdTextOut(xstart,30,menu2);
		//	if(strlen(menu3)>0)
			//kmy_LcdTextOut(xstart,45,menu3);
			reverse_area(xstart,yfill1,xend,15,2);
		}
		
		
		
	}
	printf("\nyfill in selectmenu()=%d\n",yfill1);
	x1=yfill1/15;

	return x1;
	

}
