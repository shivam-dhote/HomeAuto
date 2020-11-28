/*========================================== [EASS solutions] Smart Switch Board ==========================================*\
   main app

 * Summery:- cyclic running main state machine of smart switch board feature
\*=========================================================================================================================*/

#include "port.h"
#include "comm.h"
#include "monitor.h"
#include "control.h"
#include "switch_table.h"
#include <stdio.h>

typedef enum{
   IDL,
   MONITOR,
   CONTROL
}app_state_t;

static void app_task(void);

void app_main(void)
{
   while (1)
   {
      app_task();
   }
}

/**
 * Main cyclic task
 * 
 * @param void
 * @return void
 */
static void app_task(void)
{
   static app_state_t state = IDL;
   static switchtable_t switchtable_buff = { 0 };

   switch (state)
   {
      case IDL:
         {
            /* loading switch table from Non Volatile Storage */
            SwitchTable_load(&switchtable_buff);
            state = MONITOR;
            break;
         }

      case MONITOR:
         {
            Get_HardSwitch_Status();

            if(Is_Any_Toggle_Detected())
            {
               SwitchTable_Update();
               SendUpdate_ToNw();
               if(Is_HardRegl_Toggle())
               {
                  SendUpdate_ToReglMicro();
               }
            }

            state = CONTROL;
            break;
         }

      case CONTROL:
         {
            if(Is_Automation_on())
            {
               Control_Switches();
            }
            state = MONITOR;
            break;
         }

      default:
         break;
   }

return;
}

/*================================================== Revision History =====================================================*\
   Developer       Rev Date       Comments
 * Shivam          08-11-2020     Initial Version
\*=========================================================================================================================*/