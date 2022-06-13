#include <YSI_Coding/y_hooks>

hook OnPlayerDisconnect(playerid) {
	#if defined DEBUG
        printf("hook OnPlayerDisconnect(%d)_admin has been called", playerid);
    #endif 

    if(Iter_Contains(Admin, playerid))
        Iter_Remove(Admin, playerid);
}

function adminSecurity(playerid) {
    #if defined DEBUG
        printf("function adminSecurity(%d) has been called", playerid);
    #endif 

    if(playerVariables[playerid][pAdminLevel] > 0) { 
        inline checkAdminSecurity() {
            #if defined DEBUG
                printf("inline checkAdminSecurity(%d) has been called", playerid);
            #endif 

            if(!cache_num_rows()) {
                if(GetPVarInt(playerid, "pAdminPIN") == 0)
                    return true;
                    
                SetPVarInt(playerid, "pAdminFrozen", 1);
                
                Dialog_Show(playerid, DIALOG_ADMIN_PIN, DIALOG_STYLE_INPUT, "SERVER: Admin authentication verification", "The system has recognised that you have connected with an IP that you've never used before.\n\nPlease confirm your admin PIN to continue:", "OK", "Cancel");
            } 
        }  
        MySQL_TQueryInline(gSQL, using inline checkAdminSecurity, "SELECT conIP from playerconnections WHERE conPlayerID = %d AND conIP = '%s'", playerVariables[playerid][pInternalID], playerVariables[playerid][pConnectionIP]);
        
        if(GetPVarInt(playerid, "pAdminPIN") == 0)
            Dialog_Show(playerid, DIALOG_SET_ADMIN_PIN, DIALOG_STYLE_INPUT, "SERVER: Admin PIN creation", "The system has detected you do not yet have an admin PIN set.\n\nThis is a new compulsory security measure.\n\nPlease set a four digit pin:", "OK", "");

        if(playerVariables[playerid][pAdminDuty] > 1)
            SetPlayerName(playerid, playerVariables[playerid][pAdminName]);
    }

    if(playerVariables[playerid][pAdminDuty] == 1 && playerVariables[playerid][pAdminLevel] < 1) {
        playerVariables[playerid][pAdminLevel] = 0;
        playerVariables[playerid][pAdminDuty] = 0;
        playerVariables[playerid][pAdminName][0] = '*';
        SendClientMessage(playerid, COLOR_HOTORANGE, "You're no longer an administrator.");
    }      

    return true;
} 

stock submitToAdmins(colour, const fmat[], {Float, _}:...) {
    new str[146];
    va_format(str, sizeof (str), fmat, va_start<2>);

    printf("[ADMINS]: %s", str);

    foreach(new playerid : Admin)
        return SendClientMessage(playerid, colour, str); 

    return true;
}  

Dialog:DIALOG_ADMIN_PIN(playerid, response, listitem, inputtext[]) {
    if(strlen(inputtext) != 4)
        return Dialog_Show(playerid, DIALOG_ADMIN_PIN, DIALOG_STYLE_INPUT, "SERVER: Admin authentication verification", "Incorrect PIN!\n\nPlease confirm your admin PIN to continue:", "OK", "Cancel");

    if(strval(inputtext) == GetPVarInt(playerid, "pAdminPIN")) {
        DeletePVar(playerid, "pAdminFrozen");  
        SendClientMessage(playerid, COLOR_GENANNOUNCE, "SERVER:{FFFFFF} You've entered the correct PIN.");
    } else {
        SetPVarInt(playerid, "LA", GetPVarInt(playerid, "LA") + 1);

        if(GetPVarInt(playerid, "LA") > MAX_LOGIN_ATTEMPTS) {
            SendClientMessage(playerid, COLOR_RED, "You have used all available login attempts.");
            submitToAdmins(COLOR_HOTORANGE, "AdmWarn: {FFFFFF}Admin %s has been banned (%d failed 3 admin PIN attempts).", szPlayerName, MAX_LOGIN_ATTEMPTS);

            // scriptBan(playerid, "Maximum admin PIN attempts exceeded.");
            return true;
        } else {
            Dialog_Show(playerid, DIALOG_ADMIN_PIN, DIALOG_STYLE_INPUT, "SERVER: Admin authentication verification", "Incorrect PIN!\n\nThe system has recognised that you have connected with an IP that you've never used before.\n\nPlease confirm your admin PIN to continue:", "OK", "Cancel");
            va_SendClientMessage(playerid, COLOR_HOTORANGE, "Incorrect admin PIN. You have %d remaining login attempts left.", MAX_LOGIN_ATTEMPTS - GetPVarInt(playerid, "LA"));
            
            return true;
        }
    }

    return true;
}