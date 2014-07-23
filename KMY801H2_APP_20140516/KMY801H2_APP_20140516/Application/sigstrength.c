#include "GlobalVar.h"
#include "string.h"
#include "GlobalVariable.h"
void signalstrenth(char signaldata[20])
{
 			char *ptr1,*sig;
			static flagtog=0;
			char datebuf1[20],year1[8];
			struct Auro_time timeobj;
			int stren;
			AuroGetDateTime(&timeobj);
			memset(datebuf1,'\0',sizeof(datebuf1));
			if(flagtog==0)
			{
				memset(year1,'\0',sizeof(year1));
				sprintf(year1,"%d",timeobj.mon);
				strcpy(datebuf1,year1);
				memset(year1,'\0',sizeof(year1));
				sprintf(year1,"/%d",timeobj.day);
				strcat(datebuf1,year1);
				memset(year1,'\0',sizeof(year1));
				sprintf(year1,"/%d",timeobj.year);
				strcat(datebuf1,year1);
			flagtog=1;
			}else
			{			
				memset(year1,'\0',sizeof(year1));
				sprintf(year1,"%d",timeobj.hour);
				strcpy(datebuf1,year1);
				memset(year1,'\0',sizeof(year1));
				sprintf(year1,":%d",timeobj.min);
				strcat(datebuf1,year1);
				memset(year1,'\0',sizeof(year1));
				sprintf(year1,":%d",timeobj.sec);
				strcat(datebuf1,year1);
				flagtog=0;
			}
			if((ptr1=strstr(signaldata,":"))!=NULL)
			{
				 if((ptr1=strstr(signaldata,","))!=NULL)
				 {
				  		sig=strtok(&signaldata[5],",");
						
						stren=atoi(sig);
						printf("\n signal value==%d\n",stren);
						clear_area (0, 0, 126, 10, 1);
					   
						
						
					    if (stren >= 0 || stren < 6)
						kmy_LcdDrawLine (117, 6, 117, 8);
					    if (stren >= 6)
						kmy_LcdDrawLine (119, 5, 119, 8);
					    if (stren >= 12)
						kmy_LcdDrawLine (121, 4, 121, 8);
					    if (stren >= 18)
						kmy_LcdDrawLine (123, 3, 123, 8);
					    if (stren >= 24)
						kmy_LcdDrawLine (125, 2, 125, 8);
						 kmy_LcdDrawLine (115, 0, 115, 8);
					    kmy_LcdDrawLine (112, 0, 119, 0);
					    kmy_LcdDrawLine (112, 0, 115, 3);
					    kmy_LcdDrawLine (118, 0, 115, 3);
				}
			 	
			}
				kmy_LcdTextOut(0,0,datebuf1);
			//	drawline1();
}