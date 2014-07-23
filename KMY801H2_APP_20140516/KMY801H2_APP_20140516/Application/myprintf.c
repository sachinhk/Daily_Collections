/***********************************
Author:Jyotiba Vaijinath kadam
Date:2012-02-01 14:26
Remarks:print consol from here flag ==1
LastModified:Date:2012-02-01 14:26
Remarks:print consol from here flag ==1
************************************/
#include <stdlib.h>
#include <stdarg.h>
#define myprint 0
#include "GlobalVar.h"
void myprintf (char *frmt, ...)
{
  int i,integer=0;
  double floattype=0.0;
  char *chareter,*p,stringvar[50],*integerdata;
  va_list argp;
  va_start (argp, frmt);
  if(myprint)
  {
	printf("\n");
  for (p = frmt; *p != '\0'; p++)
    {
      if (*p != '%')
	{
			if(*p==',')
			{
				continue;
			}
			 putchar(*p);
			 continue;
	}
     	p++;
      switch (*p)
		{
			memset(stringvar,'\0',sizeof(stringvar));
			case 'd':
				 i=va_arg (argp, int);
				 if(i<0)
				    {
					     i=-i;
					     putchar('-');
				     }
				  integerdata=convert(i,10);
				  puts(integerdata);
				  break;
			case 'f':
				  floattype=va_arg (argp,double);
				  if( floattype<0)
				    {
					      floattype=- floattype;
					     putchar('-');
				     }
                                  sprintf(stringvar,"%f",floattype);
				  puts(stringvar);
				  break;
			case 'c':
				  integer=va_arg (argp,int);
				  putchar(integer);
				  puts("");
				  break;
			case 's':
				  chareter=(char *) va_arg (argp, char *);
				  puts(chareter);
				  break;
			case '%':
				  putchar ('%');
				  break;
			
		}//close switch
	}//close for
   }//macro close
}//close funtion
char *convert (unsigned int num, int base)
{
  static char buff[33];
  char *ptr;
  ptr = &buff[sizeof (buff)-1];
  *ptr = '\0';
  do
    {
      *--ptr = "0123456789abcdef"[num % base];
      num /= base;
    }
  while (num>0);
  return (ptr);
}


