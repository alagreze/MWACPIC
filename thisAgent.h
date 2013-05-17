//  agent initial definitions 
#ifndef __THIS_AGENT__
#define __THIS_AGENT__
#include "mwac.h"
#define THIS_AGENT_ID  1
#define THIS_AGENT_ROLE NOTHING
#define SERIAL_BAUD_RATE 2400.0
//  global variable defining this agent
#ifdef _DEF_THIS_AGENT_ 
 #define PUBLIC_THIS  
#else
 #define PUBLIC_THIS extern
#endif
PUBLIC_THIS me_T gl_me;
#endif