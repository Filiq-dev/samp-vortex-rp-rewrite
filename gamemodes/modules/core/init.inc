#include <a_samp>
#include <a_mysql>
#include <fixes>
#include <omp>
#include <easyDialog>  
#include <streamer>

#define YSI_YES_HEAP_MALLOC

#include <YSI_Data/y_iterate>
#include <YSI_Extra\y_inline_mysql>
#include <YSI_Coding/y_timers>
#include <YSI_Coding\y_va>

#include "modules/core/globalVariables.inc"
#include "modules/player/playerVariables.inc"

#include "modules/server/settings.inc"
#include "modules/server/globalTimers.inc"
#include "modules/server/weather.inc"

#include "modules/core/colors.inc"
#include "modules/core/usefullFunctions.inc"
#include "modules/core/sql.inc"

#include "modules/player/tutorial.inc"
#include "modules/player/login.inc"
#include "modules/player/admin.inc"
