/***********************************
Author:Jyotiba Vaijinath kadam
Date:2011-12-01 15:02
Remarks:display popup message 
LastModified:Date:2012-02-03 14:14
Remarks:
************************************/
#include "GlobalVar.h"
#include "kmy_KeyDrv.h"
#include "popup.h"
popup (char msg[30])
{
  long StTime,EnTime;
  clear_area (2, 12, 124, 48);
  printf ("\n Entered in popup");

  kmy_LcdDrawLine(5, 15, 120, 15, 2);
  kmy_LcdDrawLine(5, 57, 120, 57, 2);
  kmy_LcdDrawLine(5, 15, 5, 57, 2);
  kmy_LcdDrawLine(120, 15, 120, 57, 2);
  kmy_LcdTextOut(10, 30, msg);
  reverse_area(7, 17, 112, 39, 2);
  Keydata=0;
 /* StTime=AuroGetTime();
  while(Keydata!=KEY_R1 && difftime1(EnTime,StTime)<5)
  {
        EnTime=AuroGetTime();
   		Keydata=kmy_GetKeyValue();

  }
  return;	*/
  while(Keydata!=KEY_R1)
    {
        Keydata=kmy_GetKeyValue();

  }
  return;

}
popup2(char msg[30], char msg1[20])
{

  clear_area (2, 12, 124, 48);
  printf ("\n Entered in popup");

  kmy_LcdDrawLine(5, 15, 120, 15, 2);
  kmy_LcdDrawLine(5, 57, 120, 57, 2);
  kmy_LcdDrawLine (5, 15, 5, 57, 2);
  kmy_LcdDrawLine(120, 15, 120, 57, 2);
  kmy_LcdTextOut (10, 25, msg);
  kmy_LcdTextOut (10, 40, msg1);
  reverse_area (7, 17, 112, 39, 2);
  

}
popup1 (char msg[30], char msg1[20])
{

  clear_area (2, 12, 124, 48);
  printf ("\n Entered in popup");
  kmy_LcdDrawLine (5, 15, 120, 15, 2);
 kmy_LcdDrawLine(5, 57, 120, 57, 2);
 kmy_LcdDrawLine (5, 15, 5, 57, 2);
 kmy_LcdDrawLine (120, 15, 120, 57, 2);
  kmy_LcdTextOut (10, 25, msg);
  kmy_LcdTextOut(22, 40, msg1);
  reverse_area (7, 17, 112, 39, 2);
  	Keydata=0;
  while(Keydata!=KEY_R1)
  {
  		Keydata=kmy_GetKeyValue();
  }
  	 return;

}
