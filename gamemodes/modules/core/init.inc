#include <a_samp>
#include <a_mysql>
#include <fixes>
#include <omp>
#include <easyDialog>  
#include <streamer>
#include <a_zones>
#include <sscanf2>

#define YSI_YES_HEAP_MALLOC

#include <YSI_Data/y_iterate>
#include <YSI_Extra\y_inline_mysql>
#include <YSI_Coding/y_timers>
#include <YSI_Coding\y_va>
#include <YSI_Core\y_master>
#include <YSI_Visual\y_commands>

#include "modules/core/colors.inc"
#include "modules/core/globalVariables.inc"
#include "modules/player/playerVariables.inc"

#include "modules/server/globalTimers.inc"
#include "modules/server/settings.inc"
#include "modules/server/weather.inc"

#include "modules/core/usefullFunctions.inc"
#include "modules/core/sql.inc"

#include "modules/server/assets.inc"
#include "modules/server/groups.inc"
#include "modules/server/jobs.inc"
#include "modules/server/atms.inc"
#include "modules/server/business.inc"
#include "modules/server/house.inc"

#include "modules/player/tutorial.inc"
#include "modules/player/login.inc"
#include "modules/player/admin.inc"
#include "modules/player/groups.inc"

#include "modules/player/commands/jobs.inc"
// #include "modules/player/playerfunctions.inc"

// #include "modules/player/commands/player.inc"
