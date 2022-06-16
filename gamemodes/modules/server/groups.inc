#include <YSI_Coding/y_hooks> 

new Iterator:groups<MAX_GROUPS>;

hook OnGameModeInit() {
    #if defined DEBUG
        print("hook OnGameModeInit()_groups has been called");
    #endif

    mysql_tquery(gSQL, "SELECT * FROM groups", "loadGroups", "");

    return true;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    #if defined DEBUG
        printf("hook OnPlayerKeyStateChange(%d, %d, %d)_groups has been called", playerid, newkeys, oldkeys);
    #endif  

    if(IsKeyJustDown(KEY_CROUCH, newkeys, oldkeys)) {
        foreach(new x : groups) {
			if(IsPlayerInRangeOfPoint(playerid, 2.0, groupVariables[x][gGroupExteriorPos][0], groupVariables[x][gGroupExteriorPos][1], groupVariables[x][gGroupExteriorPos][2])) {
				if(groupVariables[x][gGroupHQLockStatus] == 1) {
					SendClientMessage(playerid, COLOR_GREY, "This HQ is locked.");
					if(playerVariables[playerid][pAdminLevel] >= 1 && playerVariables[playerid][pAdminDuty] >= 1) {
					    SetPVarInt(playerid, "gE", x); // I'd create a variable for this, but seeing as we'll only ever use this for one thing, this will be better for optimization.
					    Dialog_Show(playerid, DIALOG_GROUP_ENTER, DIALOG_STYLE_MSGBOX, "SERVER: Group HQ", "This Group HQ is locked.\r\nAs an administrator, you can breach this lock and enter. Would you like to do so?", "Yes", "No");
					}
 					else if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1 && playerVariables[playerid][pGroup] != 0) {
					    SetPVarInt(playerid, "gE", x);
					    Dialog_Show(playerid, DIALOG_GROUP_ENTER, DIALOG_STYLE_MSGBOX, "SERVER: Group HQ", "This Group HQ is locked.\r\nAs a law enforcement officer, you can breach this lock and enter. Would you like to do so?", "Yes", "No");
					}
				}
				else {
					SetPlayerPos(playerid, groupVariables[x][gGroupInteriorPos][0], groupVariables[x][gGroupInteriorPos][1], groupVariables[x][gGroupInteriorPos][2]);
					SetPlayerInterior(playerid, groupVariables[x][gGroupHQInteriorID]);
					SetPlayerVirtualWorld(playerid, GROUP_VIRTUAL_WORLD+x);
				}
				return 1;
			}
			if(IsPlayerInRangeOfPoint(playerid, 2.0, groupVariables[x][gGroupInteriorPos][0], groupVariables[x][gGroupInteriorPos][1], groupVariables[x][gGroupInteriorPos][2]) && GetPlayerVirtualWorld(playerid) == GROUP_VIRTUAL_WORLD+x) {
				SetPlayerPos(playerid, groupVariables[x][gGroupExteriorPos][0], groupVariables[x][gGroupExteriorPos][1], groupVariables[x][gGroupExteriorPos][2]);
				SetPlayerInterior(playerid, 0);
				SetPlayerVirtualWorld(playerid, 0);
				return 1;
			}
		}
    }
    if(IsKeyJustDown(KEY_SECONDARY_ATTACK, newkeys, oldkeys)) {
		if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1) {
			if(IsPlayerInRangeOfPoint(playerid, 3.0, 232.899703, 109.074996, 1009.211791))
			{
				if(playerVariables[playerid][pGroup] == 1 && playerVariables[playerid][pGroupRank] >= 5) {
					MoveDynamicObject(LSPDObjs[0][0],232.89999390,105.57499695,1009.21179199, 3.5); //commander south
					MoveDynamicObject(LSPDObjs[0][1],232.89941406,112.57499695,1009.21179199, 3.5); //commander north
					LSPDObjs[0][2] = 1;
					PlayerPlaySoundEx(1083, 232.899703, 109.074996, 1009.211791);
					SetTimerEx("ShutUp",4000,false,"d",0);
				}
			}
			else if(IsPlayerInRangeOfPoint(playerid, 3.0, 275.750000, 117.399414, 1003.617187))
			{
				MoveDynamicObject(LSPDObjs[1][0],275.75000000,120.89941406,1003.61718750, 3.5); // interrogation north
				MoveDynamicObject(LSPDObjs[1][1],275.75000000,118.89941406,1003.61718750, 3.5); // interrogation south
				LSPDObjs[1][2] = 1;
				PlayerPlaySoundEx(1083, 275.750000, 117.399414, 1003.617187);
				SetTimerEx("ShutUp",4000,false,"d",1);
			}
			else if(IsPlayerInRangeOfPoint(playerid, 3.0, 253.201660, 109.099609, 1002.220703))
			{
				MoveDynamicObject(LSPDObjs[2][0],253.20410156,105.59960938,1002.22070312,3.5); // north west lobby door
				MoveDynamicObject(LSPDObjs[2][1],253.19921875,112.59960938,1002.22070312,3.5); // north east lobby door
				LSPDObjs[2][2] = 1;
				PlayerPlaySoundEx(1083, 253.201660, 109.099609, 1002.220703);
				SetTimerEx("ShutUp",4000,false,"d",2);
			}
			else if(IsPlayerInRangeOfPoint(playerid, 3.0, 239.566894, 117.599609, 1002.220703))
			{
				MoveDynamicObject(LSPDObjs[3][0],239.56933594,114.09960938,1002.22070312,3.5); // south west lobby door
				MoveDynamicObject(LSPDObjs[3][1],239.56445312,121.09960938,1002.22070312,3.5); // south east lobby door
				LSPDObjs[3][2] = 1;
				PlayerPlaySoundEx(1083, 239.566894, 117.599609, 1002.220703);
				SetTimerEx("ShutUp",4000,false,"d",3);
			}
			else if(IsPlayerInRangeOfPoint(playerid, 2.0, 265.951171, 115.826660, 1003.622863))
			{
				MoveDynamicObject(LSPDObjs[4][0],263.45,115.82421875,1003.62286377,3.5); // cam room
				MoveDynamicObject(LSPDObjs[4][1],268.75,115.82910156,1003.62286377, 3.5); // cam room
				LSPDObjs[4][2] = 1;
				PlayerPlaySoundEx(1083, 265.951171, 115.826660, 1003.622863);
				SetTimerEx("ShutUp",4000,false,"d",4);
			}
			else if(IsPlayerInRangeOfPoint(playerid, 2.0, 265.820007, 112.530761, 1003.622863))
			{
				MoveDynamicObject(LSPDObjs[5][0],268.8,112.53222656,1003.62286377, 3.5); // locker
				MoveDynamicObject(LSPDObjs[5][1],263.3,112.52929688,1003.62286377, 3.5); // locker
				LSPDObjs[5][2] = 1;
				PlayerPlaySoundEx(1083, 265.820007, 112.530761, 1003.622863);
				SetTimerEx("ShutUp",4000,false,"d",5);
			}
			else if(IsPlayerInRangeOfPoint(playerid, 3.0, 231.099609, 119.532226, 1009.224426)) // Chief of Police
			{
				if(playerVariables[playerid][pGroup] == 1 && playerVariables[playerid][pGroupRank] == 6) {
					MoveDynamicObject(LSPDObjs[6][0],227.0,119.52929688,1009.22442627, 3.5);
					MoveDynamicObject(LSPDObjs[6][1],229.75,119.53515625,1009.22442627, 3.5);
					LSPDObjs[6][2] = 1;
					PlayerPlaySoundEx(1083, 231.099609, 119.532226, 1009.224426);
					SetTimerEx("ShutUp",4000,false,"d",6);
				}
			}
			else if(IsPlayerInRangeOfPoint(playerid, 3.0, 217.800003, 116.529647, 998.015625)) // Cells
			{
				MoveDynamicObject(LSPDObjs[7][0],220.5,116.52999878,998.01562500,3.5);
				MoveDynamicObject(LSPDObjs[7][1],215.3,116.52929688,998.01562500,3.5);
				LSPDObjs[7][2] = 1;
				PlayerPlaySoundEx(1083, 217.800003, 116.529647, 998.015625);
				SetTimerEx("ShutUp",4000,false,"d",7);
			}
		}
    }

    return true;
}

hook OnPlayerDisconnect(playerid, reason) {
    #if defined DEBUG
        printf("hook OnPlayerDisconnect(%d, %d)_groups has been called", playerid, reason);
    #endif

	if(playerVariables[playerid][pGroup] >= 1) {
		switch(reason) {
			case 0: SendToGroup(playerVariables[playerid][pGroup], COLOR_GENANNOUNCE, "%s from your group has disconnected (timeout).", playerVariables[playerid][pNormalName]);
			case 1: SendToGroup(playerVariables[playerid][pGroup], COLOR_GENANNOUNCE, "%s from your group has disconnected (quit).", playerVariables[playerid][pNormalName]);
			case 2: SendToGroup(playerVariables[playerid][pGroup], COLOR_GENANNOUNCE, "%s from your group has disconnected (banned/kicked).", playerVariables[playerid][pNormalName]);
		} 
	}

	return true;
}

function loadGroups() { 
    #if defined DEBUG
        print("function loadGroups()_groups has been called");
    #endif 

    new x = 0;
    for ( new i, j = cache_num_rows(); i != j; ++i ) {
	    cache_get_value_int(i, "groupID", x);
	    cache_get_value_name(i, "groupMOTD", groupVariables[x][gGroupMOTD], 128);
	    cache_get_value_name(i, "groupRankName1", groupVariables[x][gGroupRankName1], 64);
	    cache_get_value_name(i, "groupRankName2", groupVariables[x][gGroupRankName2], 64);
	    cache_get_value_name(i, "groupRankName3", groupVariables[x][gGroupRankName3], 64);
	    cache_get_value_name(i, "groupRankName4", groupVariables[x][gGroupRankName4], 64);
	    cache_get_value_name(i, "groupRankName5", groupVariables[x][gGroupRankName5], 64);
	    cache_get_value_name(i, "groupRankName6", groupVariables[x][gGroupRankName6], 64);
        cache_get_value_name(i, "groupName", groupVariables[x][gGroupName], 128);
        
	    cache_get_value_int(i, "groupType", groupVariables[x][gGroupType]);	  
        cache_get_value_int(i, "groupHQInteriorID", groupVariables[x][gGroupHQInteriorID]);	   
	    cache_get_value_int(i, "groupHQLockStatus", groupVariables[x][gGroupHQLockStatus]);	   
	    cache_get_value_int(i, "groupSafeMoney", groupVariables[x][gSafe][0]);	    
	    cache_get_value_int(i, "groupSafeMats", groupVariables[x][gSafe][1]);	  

	    cache_get_value_float(i, "groupHQExteriorPosX", groupVariables[x][gGroupExteriorPos][0]);	    
	    cache_get_value_float(i, "groupHQExteriorPosY", groupVariables[x][gGroupExteriorPos][1]);
	    cache_get_value_float(i, "groupHQExteriorPosZ", groupVariables[x][gGroupExteriorPos][2]);	
	    cache_get_value_float(i, "groupHQInteriorPosX", groupVariables[x][gGroupInteriorPos][0]);	  
	    cache_get_value_float(i, "groupHQInteriorPosY", groupVariables[x][gGroupInteriorPos][1]);
	    cache_get_value_float(i, "groupHQInteriorPosZ", groupVariables[x][gGroupInteriorPos][2]);	    
	    cache_get_value_float(i, "groupSafePosX", groupVariables[x][gSafePos][0]);	    
	    cache_get_value_float(i, "groupSafePosY", groupVariables[x][gSafePos][1]);
	    cache_get_value_float(i, "groupSafePosZ", groupVariables[x][gSafePos][2]);

        switch(groupVariables[x][gGroupHQLockStatus]) {
            case 0: format(gString, sizeof(gString), "%s's HQ\n\nPress ~k~~PED_DUCK~ to enter.", groupVariables[x][gGroupName]);
            case 1: format(gString, sizeof(gString), "%s's HQ\n\n(locked)", groupVariables[x][gGroupName]);
        }

        groupVariables[x][gGroupPickupID] = CreateDynamicPickup(1239, 23, groupVariables[x][gGroupExteriorPos][0], groupVariables[x][gGroupExteriorPos][1], groupVariables[x][gGroupExteriorPos][2], 0, -1, -1, 10);
        groupVariables[x][gGroupLabelID] = CreateDynamic3DTextLabel(gString, COLOR_YELLOW, groupVariables[x][gGroupExteriorPos][0], groupVariables[x][gGroupExteriorPos][1], groupVariables[x][gGroupExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 25.0);

        format(gString, sizeof(gString), "%s\nGroup Safe", groupVariables[x][gGroupName]);

        groupVariables[x][gSafePickupID] = CreateDynamicPickup(1239, 23, groupVariables[x][gSafePos][0], groupVariables[x][gSafePos][1], groupVariables[x][gSafePos][2], GROUP_VIRTUAL_WORLD+x, groupVariables[x][gGroupHQInteriorID], -1, 10);
        groupVariables[x][gSafeLabelID] = CreateDynamic3DTextLabel(gString, COLOR_YELLOW, groupVariables[x][gSafePos][0], groupVariables[x][gSafePos][1], groupVariables[x][gSafePos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, GROUP_VIRTUAL_WORLD+x, groupVariables[x][gGroupHQInteriorID], -1, 20.0);
    
        Iter_Add(groups, x);
        // x++; de testat
    } 

    printf("[SQL] Groups loaded: %d", Iter_Count(groups)); 
}

function saveGroup(id) {
	if(strlen(groupVariables[id][gGroupName]) >= 1) {
		format(gLargeString, sizeof(gLargeString), "UPDATE groups SET groupName = '%s', groupHQExteriorPosX = '%f', groupHQExteriorPosY = '%f', groupHQExteriorPosZ = '%f'", groupVariables[id][gGroupName], groupVariables[id][gGroupExteriorPos][0], groupVariables[id][gGroupExteriorPos][1], groupVariables[id][gGroupExteriorPos][2]);
		format(gLargeString, sizeof(gLargeString), "%s, groupHQInteriorID = '%d', groupHQLockStatus = '%d', groupHQInteriorPosX = '%f', groupHQInteriorPosY = '%f', groupHQInteriorPosZ = '%f', groupSafeMoney = '%d', groupSafeMats = '%d', groupMOTD = '%s'", gLargeString, groupVariables[id][gGroupHQInteriorID],
		groupVariables[id][gGroupHQLockStatus], groupVariables[id][gGroupInteriorPos][0], groupVariables[id][gGroupInteriorPos][1], groupVariables[id][gGroupInteriorPos][2], groupVariables[id][gSafe][0], groupVariables[id][gSafe][1], groupVariables[id][gGroupMOTD]);
		format(gLargeString, sizeof(gLargeString), "%s, groupRankName1 = '%s', groupRankName2 = '%s', groupRankName3 = '%s', groupRankName4 = '%s', groupRankName5 = '%s', groupRankName6 = '%s'", gLargeString, groupVariables[id][gGroupRankName1], groupVariables[id][gGroupRankName2], groupVariables[id][gGroupRankName3], groupVariables[id][gGroupRankName4], groupVariables[id][gGroupRankName5], groupVariables[id][gGroupRankName6]);
		format(gLargeString, sizeof(gLargeString), "%s, groupSafePosX = '%f', groupSafePosY = '%f', groupSafePosZ = '%f', groupType = '%d' WHERE groupID = '%d'", gLargeString, groupVariables[id][gSafePos][0], groupVariables[id][gSafePos][1], groupVariables[id][gSafePos][2], groupVariables[id][gGroupType], id);
		mysql_tquery(gSQL, gLargeString, "", "");
	}
	else {
		return false;
	}

	return true; 
}

stock SendToGroupType(type, colour, const fmat[], {Float, _}:...) {
    new str[146];
    va_format(str, sizeof (str), fmat, va_start<3>);

	foreach(new i : groups)
	    if(groupVariables[i][gGroupType] == type)
	        return SendToGroup(i, colour, str);
	
	return true;
}

stock sendDepartmentMessage(colour, const fmat[], {Float, _}:...) {
    new str[146];
    va_format(str, sizeof (str), fmat, va_start<2>);

	foreach(new i : Player)
	    if(playerVariables[i][pStatus] == 1 && (groupVariables[playerVariables[i][pGroup]][gGroupType] == 1 || groupVariables[playerVariables[i][pGroup]][gGroupType] == 2)) 
	        return SendClientMessage(i, colour, str);
	
	return true;
} 

stock SendToGroup(groupid, colour, const fmat[], {Float, _}:...) {
    new str[146];
    va_format(str, sizeof (str), fmat, va_start<3>);
	
    foreach(new i : Player)
        if(playerVariables[i][pStatus] == 1 && playerVariables[i][pGroup] == groupid) 
            return SendClientMessage(i, colour, str);
	
	return true;
} 

Dialog:DIALOG_GROUP_ENTER(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        new
            x = GetPVarInt(playerid, "gE"); // So we don't have to access it each and every time.

        DeletePVar(playerid, "gE");
        nearByMessage(playerid, COLOR_PURPLE, 12.0, "* %s breaks down the door and enters the building.", getName(playerid));

        if(playerVariables[playerid][pAdminDuty] < 1 && groupVariables[x][gGroupHQLockStatus] == 1) {
            groupVariables[x][gGroupHQLockStatus] = 0;
            format(gString, sizeof(gString), "%s's HQ\n\nPress ~k~~PED_DUCK~ to enter.", groupVariables[x][gGroupName]);
            UpdateDynamic3DTextLabelText(groupVariables[x][gGroupLabelID], COLOR_YELLOW, gString);
        }

        SetPlayerPos(playerid, groupVariables[x][gGroupInteriorPos][0], groupVariables[x][gGroupInteriorPos][1], groupVariables[x][gGroupInteriorPos][2]);
        SetPlayerInterior(playerid, groupVariables[x][gGroupHQInteriorID]);
        SetPlayerVirtualWorld(playerid, GROUP_VIRTUAL_WORLD+x);
    }
}