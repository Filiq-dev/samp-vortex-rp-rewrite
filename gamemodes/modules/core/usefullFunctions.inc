/*
    if somebody wants to have roleplay names can enable this
function invalidNameChange(playerid) {
	// Anti-spam mechanism to confirm the feature isn't being spammed
    if(gettime()-GetPVarInt(playerid, "namet") < 3) {
        if(GetPVarInt(playerid, "namett") != 0)
            KillTimer(GetPVarInt(playerid, "namett")); // Kill the timer if it already exists and let it create a new one
            
        // Call (self) again in 4 seconds to avoid clogging the server with useless requests
        SetPVarInt(playerid, "namett", SetTimerEx("invalidNameChange", 4000, false, "d", playerid));
        return 1;
    }
        
	format(playerVariables[playerid][pNormalName], MAX_PLAYER_NAME, "NONRPNAME[%d]", playerid);
	SetPlayerName(playerid, playerVariables[playerid][pNormalName]);
	
	SendClientMessage(playerid, COLOR_GREY, "You are being prompted to change your name. You can do this by following the instructions as they are written in the dialog.");
	ShowPlayerDialog(playerid, DIALOG_RP_NAME_CHANGE, DIALOG_STYLE_INPUT, "SERVER: Non RP name change", "This server has a strict name policy.\n\nYou must enter a valid roleplay name, the name must:\n- Be under 20 characters\n- Not contain numbers\n- Contain only two uppercase characters for the forename and surname\n- Be in the format of Forename_Surname", "OK", "Cancel");
	SetPVarInt(playerid, "namet", gettime());
	return 1;
} */

function IsKeyJustDown(key, newkeys, oldkeys) {
	if((newkeys & key) && !(oldkeys & key))
		return true;

	return false;
}