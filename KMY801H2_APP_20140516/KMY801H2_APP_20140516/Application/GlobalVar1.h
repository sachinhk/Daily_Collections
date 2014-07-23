/***********************************
Author:Jyotiba Vaijinath kadam
Date:2012-01-12 17:01
Remarks:global variable decleration 
LastModified:Date:2012-01-12 17:01
Remarks:global variable decleration
************************************/
/*----------------------------------------------------------------------------
VARS.H

Note: #define VAR_DECLS 1 before including this file to DECLARE and INITIALIZE
global variables.  Include this file without defining VAR_DECLS to extern
these variables.
----------------------------------------------------------------------------*/
#ifndef VAR_DEFS          // Make sure this file is included only once
#define VAR_DEFS 1

/*----------------------------------------------
Setup variable declaration macros.
----------------------------------------------*/
#ifndef VAR_DECLS1
# define _DECL extern
# define _INIT(x)
#else
# define _DECL
# define _INIT(x)  = x
#endif

/*----------------------------------------------
Declare variables as follows:

_DECL [standard variable declaration] _INIT(x);

where x is the value with which to initialize
the variable.  If there is no initial value for
the varialbe, it may be declared as follows:

_DECL [standard variable declaration];
----------------------------------------------*/

_DECL int page _INIT(0),printvalid _INIT(0);
_DECL int chklen,flagkey;
_DECL int xstart _INIT(5);
_DECL int xend _INIT(121)
_DECL int ystart _INIT(14)
_DECL int yend _INIT(46)
_DECL int mainmenupos _INIT(1),numitem _INIT(5),yfill _INIT(15),menucounter _INIT(0),chkdateflag _INIT(0),recnum _INIT(0);
_DECL char menus[15][21];
_DECL char bufmenu[2];
_DECL int mmenu _INIT(0),smenu1 _INIT(0),smenu2 _INIT(0),smenu3 _INIT(0),mfillpos _INIT(15),fillpos _INIT(15),fillpos1 _INIT(15),fillpos2 _INIT(15),rbotton _INIT(0),ok _INIT(1);
_DECL int back _INIT(0),keydatalen _INIT(0);
_DECL int redcount _INIT(0),endofdata _INIT(1),counterin _INIT(0),inboxfd,yfillinbox;
_DECL unsigned char c3 = '3';
_DECL int flaginbox _INIT(0) ,charlen _INIT(0);
#endif
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/



