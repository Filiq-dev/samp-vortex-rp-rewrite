#include <a_samp>
#include <a_mysql>
#include <fixes>
#include <omp>
#include <easyDialog> 

#define MAX_LOGIN_ATTEMPTS  3 
#define MAX_TEXTDRAWS       10
#define ADMIN_PIN_TIMEOUT   120
 
#define function%0(%1) forward%0(%1); public%0(%1)

native WP_Hash(buffer[], len, const str[]);

#include "modules/core/colors.inc"
#include "modules/core/globalVariables.inc"
#include "modules/core/usefullFunctions.inc"
#include "modules/core/sql.inc"

#include "modules/player/playerVariables.inc"
#include "modules/player/tutorial.inc"
#include "modules/player/login.inc"
