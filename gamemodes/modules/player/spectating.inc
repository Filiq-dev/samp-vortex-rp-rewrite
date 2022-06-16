#include <YSI_Coding/y_hooks>

hook OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid) {
	#if defined DEBUG
	    printf("hook OnPlayerInteriorChange(%d, %d, %d)_spectating has called", playerid, newinteriorid, oldinteriorid);
	#endif 

	if(playerVariables[playerid][pSpectating] == INVALID_PLAYER_ID && playerVariables[playerid][pEvent] == 0) {
		playerVariables[playerid][pInterior] = newinteriorid;
		playerVariables[playerid][pVirtualWorld] = GetPlayerVirtualWorld(playerid);
	}

	return true;
}

hook OnPlayerDisconnect(playerid, reason) {
	#if defined DEBUG
	    printf("hook OnPlayerDisconnect(%d)_spectating has called", playerid);
	#endif 

    foreach(new x : Player) {
        if(playerVariables[x][pSpectating] == playerid) {
            spectateOff(x);
            SendClientMessage(x, COLOR_GREY, "The player you were spectating has disconnected.");
        }
    }

    return true;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	#if defined DEBUG
	    printf("hook OnPlayerKeyStateChange(%d, %d, %d)_spectating", playerid, newkeys, oldkeys);
	#endif  

    if(IsKeyJustDown(KEY_WALK, newkeys, oldkeys)) {
        if(playerVariables[playerid][pSpectating] != INVALID_PLAYER_ID && playerVariables[playerid][pAdminLevel] >= 1) {
            spectateOff(playerid);
            return true;
        }
    }

    return true;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source) {
	#if defined DEBUG
	    printf("public OnPlayerClickPlayer(%d, %d, %d)_spectating", playerid, clickedplayerid, source);
	#endif
	
    if(Iter_Contains(Admin, playerid)) 
        spectatePlayer(playerid, clickedplayerid);
            
	return true;
} 

YCMD:spec(playerid, params[], help) {
    if(!Iter_Contains(Admin, playerid)) return true;
    new userID;

    if(sscanf(params, "u", userID)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/spec [playerid]");
    
    spectatePlayer(playerid, userID);
    
	return true;
}

function spectatePlayer(adminid, playerid) {
    if(!IsPlayerAuthed(playerid)) return SendClientMessage(adminid, COLOR_GREY, "The specified player is not connected, or has not authenticated.");
    if(playerVariables[playerid][pTutorial] >= 1) return SendClientMessage(adminid, COLOR_GREY, "This player is currently in the tutorial.");
    if(playerVariables[adminid][pSpectating] == INVALID_PLAYER_ID) { // Will only save pos/etc if they're NOT spectating. This will stop the annoying death/pos/int/VW/crash bugs everyone's experiencing...
        GetPlayerPos(adminid, playerVariables[adminid][pPos][0], playerVariables[adminid][pPos][1], playerVariables[adminid][pPos][2]);
        playerVariables[adminid][pInterior] = GetPlayerInterior(adminid);
        playerVariables[adminid][pVirtualWorld] = GetPlayerVirtualWorld(adminid);
        playerVariables[adminid][pSkin] = GetPlayerSkin(adminid);

        if(playerVariables[adminid][pAdminDuty] == 0) {
            GetPlayerHealth(adminid, playerVariables[adminid][pHealth]);
            GetPlayerArmour(adminid, playerVariables[adminid][pArmour]);
        }
    }

    playerVariables[adminid][pSpectating] = playerid;
    TogglePlayerSpectating(adminid, true); 
    SetPlayerInterior(adminid, GetPlayerInterior(playerid));
    SetPlayerVirtualWorld(adminid, GetPlayerVirtualWorld(playerid));

    if(IsPlayerInAnyVehicle(playerid)) PlayerSpectateVehicle(adminid, GetPlayerVehicleID(playerid)); 
    else PlayerSpectatePlayer(adminid, playerid); 

    TextDrawShowForPlayer(adminid, textdrawVariables[4]);
}

function spectateOff(adminid) {
    playerVariables[adminid][pSpectating] = INVALID_PLAYER_ID;

    TogglePlayerSpectating(adminid, false);
    SetCameraBehindPlayer(adminid);

    SetPlayerPos(adminid, playerVariables[adminid][pPos][0], playerVariables[adminid][pPos][1], playerVariables[adminid][pPos][2]);
    SetPlayerInterior(adminid, playerVariables[adminid][pInterior]);
    SetPlayerVirtualWorld(adminid, playerVariables[adminid][pVirtualWorld]);

    TextDrawHideForPlayer(adminid, textdrawVariables[4]);
}