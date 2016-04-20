#ifndef HAL_MCU_H
#define HAL_MCU_H

#define HAL_MCU_CYGWIN

typedef unsigned char halIntState_t;



#define HAL_ENTER_CRITICAL_SECTION(x)   {x=0;}
#define HAL_EXIT_CRITICAL_SECTION(x)    {x=0;}
#define HAL_CRITICAL_STATEMENT(x)  {x=0;}

#define SAVE_AND_DISABLE_GLOBAL_INTERRUPT(x)   {x=0;}
#define RESTORE_GLOBAL_INTERRUPT(x)	{x=0;}





#endif
