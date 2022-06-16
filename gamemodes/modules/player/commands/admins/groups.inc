CMD:listgroups(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 1) {
        for(new xf = 0; xf < MAX_GROUPS; xf++) {
			if(strlen(groupVariables[xf][gGroupName]) >= 1 && strcmp(groupVariables[xf][gGroupName], "None", true)) {
				format(szMessage, sizeof(szMessage), "ID: %d | Group Name: %s | Group Type: %d", xf, groupVariables[xf][gGroupName], groupVariables[xf][gGroupType]);
				SendClientMessage(playerid, COLOR_WHITE, szMessage);
			}
		}
	}
	return 1;
}
CMD:movehq(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 3) {
	    new
	        ID,
	        subject[32],
			string[128];

		if(sscanf(params, "ds[32]", ID, subject)) {
		    SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/movehq [group ID] [exterior/interior]");
		}
		else {
		    if(ID < 1 || ID > MAX_GROUPS) return SendClientMessage(playerid, COLOR_GREY, "Invalid group ID.");

            if(strcmp(subject, "exterior", true) == 0) {
			    GetPlayerPos(playerid, groupVariables[ID][gGroupExteriorPos][0], groupVariables[ID][gGroupExteriorPos][1], groupVariables[ID][gGroupExteriorPos][2]);

			    DestroyDynamic3DTextLabel(groupVariables[ID][gGroupLabelID]);
			    DestroyDynamicPickup(groupVariables[ID][gGroupPickupID]);

				new
    				labelString[96];

				switch(groupVariables[ID][gGroupHQLockStatus]) {
			    	case 0: format(labelString, sizeof(labelString), "%s's HQ\n\nPress ~k~~PED_DUCK~ to enter.", groupVariables[ID][gGroupName]);
			    	case 1: format(labelString, sizeof(labelString), "%s's HQ\n\n(locked)", groupVariables[ID][gGroupName]);
			    }

				groupVariables[ID][gGroupLabelID] = CreateDynamic3DTextLabel(labelString, COLOR_YELLOW, groupVariables[ID][gGroupExteriorPos][0], groupVariables[ID][gGroupExteriorPos][1], groupVariables[ID][gGroupExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 50.0);
				groupVariables[ID][gGroupPickupID] = CreateDynamicPickup(1239, 23, groupVariables[ID][gGroupExteriorPos][0], groupVariables[ID][gGroupExteriorPos][1], groupVariables[ID][gGroupExteriorPos][2], 0, -1, -1, 50);
			}
			else if(strcmp(subject, "interior", true) == 0){
			    GetPlayerPos(playerid, groupVariables[ID][gGroupInteriorPos][0], groupVariables[ID][gGroupInteriorPos][1], groupVariables[ID][gGroupInteriorPos][2]);
			    groupVariables[ID][gGroupHQInteriorID] = GetPlayerInterior(playerid);

			}
			format(string,sizeof(string),"You have successfully moved the %s of the %s group (ID %d).",subject,groupVariables[ID][gGroupName],ID);
			SendClientMessage(playerid, COLOR_WHITE, string);
		}
	}

	return 1;
}

CMD:gtype(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 4) {
	    new
	        groupID,
	        groupType;

        if(sscanf(params, "dd", groupID, groupType))
			return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/gtype [groupid] [grouptypeid]");

		if(groupID > 0 && groupID < MAX_GROUPS) {
            format(szMessage, sizeof(szMessage), "You have set group %s's group type to %d.", groupVariables[groupID][gGroupName], groupType);
            SendClientMessage(playerid, COLOR_WHITE, szMessage);

            groupVariables[groupID][gGroupType] = groupType;
  		} else return SendClientMessage(playerid, COLOR_GREY, "Invalid Group ID!");
	}
	return 1;
}