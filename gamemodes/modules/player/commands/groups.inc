CMD:listmygroup(playerid, params[]) {
	if(playerVariables[playerid][pGroup] < 1 && playerVariables[playerid][pGroupRank] < 4) return true;

    SendClientMessage(playerid, COLOR_TEAL, "----------------------------------------------------------------");

    foreach(new i : Player) {
        if(IsPlayerAuthed(i) && playerVariables[i][pGroup] == playerVariables[playerid][pGroup] && playerVariables[i][pAdminDuty] < 1) { 
            switch(playerVariables[i][pGroupRank]) {
                case 1: va_SendClientMessage(playerid, COLOR_WHITE, "* (%d) %s %s", playerVariables[i][pGroupRank], groupVariables[playerVariables[i][pGroup]][gGroupRankName1], playerVariables[i][pNormalName]);
                case 2: va_SendClientMessage(playerid, COLOR_WHITE, "* (%d) %s %s", playerVariables[i][pGroupRank], groupVariables[playerVariables[i][pGroup]][gGroupRankName2], playerVariables[i][pNormalName]);
                case 3: va_SendClientMessage(playerid, COLOR_WHITE, "* (%d) %s %s", playerVariables[i][pGroupRank], groupVariables[playerVariables[i][pGroup]][gGroupRankName3], playerVariables[i][pNormalName]);
                case 4: va_SendClientMessage(playerid, COLOR_WHITE, "* (%d) %s %s", playerVariables[i][pGroupRank], groupVariables[playerVariables[i][pGroup]][gGroupRankName4], playerVariables[i][pNormalName]);
                case 5: va_SendClientMessage(playerid, COLOR_WHITE, "* (%d) %s %s", playerVariables[i][pGroupRank], groupVariables[playerVariables[i][pGroup]][gGroupRankName5], playerVariables[i][pNormalName]);
                case 6: va_SendClientMessage(playerid, COLOR_WHITE, "* (%d) %s %s", playerVariables[i][pGroupRank], groupVariables[playerVariables[i][pGroup]][gGroupRankName6], playerVariables[i][pNormalName]);
            } 
        }
    }
    SendClientMessage(playerid, COLOR_TEAL, "----------------------------------------------------------------");
	
	return 1;
}

CMD:gmotd(playerid, params[]) {
	if(playerVariables[playerid][pGroup] < 1 && playerVariables[playerid][pGroupRank] < 5) return true;
    if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/gmotd [text]");
 
    va_SendClientMessage(playerid, COLOR_WHITE, "You have changed the group MOTD to %s.", params); 
    SendToGroup(playerVariables[playerid][pGroup], COLOR_GENANNOUNCE, "%s has changed the group MOTD to '%s'.", getName(playerid), params);

    gString[0] = (EOS);
    mysql_escape_string(params, gString); 
    strcpy(groupVariables[playerVariables[playerid][pGroup]][gGroupMOTD], gString, 128);
            
	return true;
}

CMD:gsafepos(playerid, params[]) {
	if(playerVariables[playerid][pGroup] < 1 && playerVariables[playerid][pGroupRank] < 6) return true;

    GetPlayerPos(playerid, groupVariables[playerVariables[playerid][pGroup]][gSafePos][0], groupVariables[playerVariables[playerid][pGroup]][gSafePos][1], groupVariables[playerVariables[playerid][pGroup]][gSafePos][2]);
    DestroyDynamicPickup(groupVariables[playerVariables[playerid][pGroup]][gSafePickupID]);
    DestroyDynamic3DTextLabel(groupVariables[playerVariables[playerid][pGroup]][gSafeLabelID]);

    gString[0] = (EOS);
    format(gString, sizeof(gString), "%s\nGroup Safe", groupVariables[playerVariables[playerid][pGroup]][gGroupName]);

    groupVariables[playerVariables[playerid][pGroup]][gSafePickupID] = CreateDynamicPickup(1239, 23, groupVariables[playerVariables[playerid][pGroup]][gSafePos][0], groupVariables[playerVariables[playerid][pGroup]][gSafePos][1], groupVariables[playerVariables[playerid][pGroup]][gSafePos][2], GROUP_VIRTUAL_WORLD+playerVariables[playerid][pGroup], groupVariables[playerVariables[playerid][pGroup]][gGroupHQInteriorID], -1, 50);
    groupVariables[playerVariables[playerid][pGroup]][gSafeLabelID] = CreateDynamic3DTextLabel(gString, COLOR_YELLOW, groupVariables[playerVariables[playerid][pGroup]][gSafePos][0], groupVariables[playerVariables[playerid][pGroup]][gSafePos][1], groupVariables[playerVariables[playerid][pGroup]][gSafePos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, GROUP_VIRTUAL_WORLD+playerVariables[playerid][pGroup], groupVariables[playerVariables[playerid][pGroup]][gGroupHQInteriorID], -1, 50.0);

    SendClientMessage(playerid, COLOR_WHITE, "You have adjusted the position of your group's safe.");

	return true;
}

CMD:gname(playerid, params[]) {
	if(playerVariables[playerid][pGroup] < 1 && playerVariables[playerid][pGroupRank] < 6) return true;
    if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/gname [group name]");
 
    va_SendClientMessage(playerid, COLOR_WHITE, "You have changed the group name to %s.", params);

    gString[0] = (EOS);
    mysql_real_escape_string(params, gString);
    strcpy(groupVariables[playerVariables[playerid][pGroup]][gGroupName], gString, 64);

    gString[0] = (EOS);
    switch(groupVariables[playerVariables[playerid][pGroup]][gGroupHQLockStatus]) {
        case 0: format(gString, sizeof(gString), "%s's HQ\n\nPress ~k~~PED_DUCK~ to enter.", groupVariables[playerVariables[playerid][pGroup]][gGroupName]);
        case 1: format(gString, sizeof(gString), "%s's HQ\n\n(locked)", groupVariables[playerVariables[playerid][pGroup]][gGroupName]);
    }
    UpdateDynamic3DTextLabelText(groupVariables[playerVariables[playerid][pGroup]][gGroupLabelID], COLOR_YELLOW, gString);

    format(gString, sizeof(gString), "%s\nGroup Safe", groupVariables[playerVariables[playerid][pGroup]][gGroupName]);
    UpdateDynamic3DTextLabelText(groupVariables[playerVariables[playerid][pGroup]][gSafeLabelID], COLOR_YELLOW, gString);

	return true;
}

CMD:showmotd(playerid, params[]) {
	if(playerVariables[playerid][pGroup] < 1) return true;
    
    va_SendClientMessage(playerid, COLOR_GENANNOUNCE, "Group MOTD: {FFFFFF}%s", groupVariables[playerVariables[playerid][pGroup]][gGroupMOTD]);
	
	return true;
}

CMD:invite(playerid, params[]) {
    if(playerVariables[playerid][pGroup] < 1 && playerVariables[playerid][pGroupRank] < 5) return true;
    new userID;

    if(sscanf(params, "u", userID)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/invite [playerid]");
    if(!IsPlayerConnected(userID)) return SendClientMessage(playerid, COLOR_GREY, "The specified userID/name is not connected.");
    if(playerVariables[userID][pGroup] > 0) return SendClientMessage(playerid, COLOR_GREY, "That player is already in a group.");

    if(playerVariables[userID][pLevel] < assetVariables[2][aAssetValue]) { 
        va_SendClientMessage(playerid, COLOR_GREY, "You can't invite a player below level %d.", assetVariables[2][aAssetValue]);
 
        return va_SendClientMessage(userID, COLOR_GREY, "You have been invited to a group, but you can't accept the invite. You must be at least level %d, you've got %d levels to go!", assetVariables[2][aAssetValue], assetVariables[2][aAssetValue]-playerVariables[userID][pLevel]);
    }
 
    va_SendClientMessage(playerid, COLOR_WHITE, "You have invited %s to join your group.", getName(userID)); 
    va_SendClientMessage(userID, COLOR_NICESKY, "%s has invited you to join group %s (to accept the invitation, type '/accept invite').", getName(playerid), groupVariables[playerVariables[playerid][pGroup]][gGroupName]);

    SetPVarInt(userID, "invID", playerVariables[playerid][pGroup]); // Storing in a PVar as it's something that won't be used frequently, saving memory. Also, keeping the variable names short, as they're stored in memory and literally kill!!1

    return true;
}

CMD:uninvite(playerid, params[]) {
	if(playerVariables[playerid][pGroup] < 1 && playerVariables[playerid][pGroupRank] < 5) return true;
    new
        userID;

    if(sscanf(params, "u", userID)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/uninvite [playerid]");
    if(!IsPlayerConnected(userID)) return SendClientMessage(playerid, COLOR_GREY, "The specified userID/name is not connected.");
    if(playerVariables[playerid][pGroup] != playerVariables[userID][pGroup]) return SendClientMessage(playerid, COLOR_GREY, "That player isn't in your group.");
    if(playerVariables[playerid][pGroupRank] <= playerVariables[userID][pGroupRank]) return SendClientMessage(playerid, COLOR_GREY, "You can't uninvite this person.");
 
    va_SendClientMessage(userID, COLOR_NICESKY, "%s has removed you from the %s.", getName(playerid), groupVariables[playerVariables[userID][pGroup]][gGroupName]); 
    va_SendClientMessage(playerid, COLOR_WHITE, "You have removed %s from your group.", getName(userID)); 
    SendToGroup(playerVariables[playerid][pGroup], COLOR_GENANNOUNCE, "%s has left the group (uninvited).", getName(userID));

    playerVariables[userID][pGroup] = 0;
    playerVariables[userID][pGroupRank] = 0;
        
	return true;
}

CMD:lockhq(playerid, params[]) {
	if(playerVariables[playerid][pGroup] < 1 && playerVariables[playerid][pGroupRank] < 5) return true;

    gString[0] = (EOS);
    switch(groupVariables[playerVariables[playerid][pGroup]][gGroupHQLockStatus]) {
        case 1: {
            SendClientMessage(playerid, COLOR_WHITE, "HQ unlocked.");
            groupVariables[playerVariables[playerid][pGroup]][gGroupHQLockStatus] = 0;
            format(gString, sizeof(gString), "%s's HQ\n\nPress ~k~~PED_DUCK~ to enter.", groupVariables[playerVariables[playerid][pGroup]][gGroupName]);
        }
        case 0: {
            SendClientMessage(playerid, COLOR_WHITE, "HQ locked.");
            groupVariables[playerVariables[playerid][pGroup]][gGroupHQLockStatus] = 1;
            format(gString, sizeof(gString), "%s's HQ\n\n(locked)", groupVariables[playerVariables[playerid][pGroup]][gGroupName]);
        }
    }

    UpdateDynamic3DTextLabelText(groupVariables[playerVariables[playerid][pGroup]][gGroupLabelID], COLOR_YELLOW, gString);

	return true;
}

CMD:changerank(playerid, params[]) {
	if(playerVariables[playerid][pGroup] >= 1 && playerVariables[playerid][pGroupRank] >= 5) {
	    new
			rank,
	        userID;

        if(sscanf(params, "ud", userID, rank)) {
            SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/changerank [playerid] [rank]");
        }
        else {
            if(!IsPlayerConnected(userID)) return SendClientMessage(playerid, COLOR_GREY, "The specified userID/name is not connected.");
			else if(rank < 1 || rank > 6) return SendClientMessage(playerid, COLOR_GREY, "Invalid rank specified.");
			else if(playerVariables[playerid][pGroup] != playerVariables[userID][pGroup]) return SendClientMessage(playerid, COLOR_GREY, "That player isn't in your group.");
			else if(playerVariables[playerid][pGroupRank] <= rank) return SendClientMessage(playerid, COLOR_GREY, "You can't promote to this rank.");
			else if(playerVariables[playerid][pGroupRank] <= playerVariables[userID][pGroupRank]) return SendClientMessage(playerid, COLOR_GREY, "You can't alter this person's rank.");
			else if(playerVariables[userID][pGroupRank] == rank) return SendClientMessage(playerid, COLOR_GREY, "That person is already of that rank.");
			else {

				new
					messageString[128];

				GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
				if(rank > playerVariables[userID][pGroupRank]) switch(rank) {

					case 1: format(messageString, sizeof(messageString), "%s has promoted you to the rank of %s (1).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName1]);
					case 2: format(messageString, sizeof(messageString), "%s has promoted you to the rank of %s (2).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName2]);
					case 3: format(messageString, sizeof(messageString), "%s has promoted you to the rank of %s (3).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName3]);
					case 4: format(messageString, sizeof(messageString), "%s has promoted you to the rank of %s (4).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName4]);
					case 5: format(messageString, sizeof(messageString), "%s has promoted you to the rank of %s (5).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName5]);
					case 6: format(messageString, sizeof(messageString), "%s has promoted you to the rank of %s (6).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName6]);

				}
				else switch(rank) {

					case 1: format(messageString, sizeof(messageString), "%s has demoted you to the rank of %s (1).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName1]);
					case 2: format(messageString, sizeof(messageString), "%s has demoted you to the rank of %s (2).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName2]);
					case 3: format(messageString, sizeof(messageString), "%s has demoted you to the rank of %s (3).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName3]);
					case 4: format(messageString, sizeof(messageString), "%s has demoted you to the rank of %s (4).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName4]);
					case 5: format(messageString, sizeof(messageString), "%s has demoted you to the rank of %s (5).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName5]);
					case 6: format(messageString, sizeof(messageString), "%s has demoted you to the rank of %s (6).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName6]);
				}
				SendClientMessage(userID, COLOR_NICESKY, messageString);

				GetPlayerName(userID, szPlayerName, MAX_PLAYER_NAME);

				if(rank > playerVariables[userID][pGroupRank]) switch(rank) {

					case 1: format(messageString, sizeof(messageString), "You have promoted %s to the rank of %s (1).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName1]);
					case 2: format(messageString, sizeof(messageString), "You have promoted %s to the rank of %s (2).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName2]);
					case 3: format(messageString, sizeof(messageString), "You have promoted %s to the rank of %s (3).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName3]);
					case 4: format(messageString, sizeof(messageString), "You have promoted %s to the rank of %s (4).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName4]);
					case 5: format(messageString, sizeof(messageString), "You have promoted %s to the rank of %s (5).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName5]);
					case 6: format(messageString, sizeof(messageString), "You have promoted %s to the rank of %s (6).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName6]);

				}
				else switch(rank) {

					case 1: format(messageString, sizeof(messageString), "You have demoted %s to the rank of %s (1).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName1]);
					case 2: format(messageString, sizeof(messageString), "You have demoted %s to the rank of %s (2).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName2]);
					case 3: format(messageString, sizeof(messageString), "You have demoted %s to the rank of %s (3).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName3]);
					case 4: format(messageString, sizeof(messageString), "You have demoted %s to the rank of %s (4).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName4]);
					case 5: format(messageString, sizeof(messageString), "You have demoted %s to the rank of %s (5).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName5]);
					case 6: format(messageString, sizeof(messageString), "You have demoted %s to the rank of %s (6).", szPlayerName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName6]);
				}
				SendClientMessage(playerid, COLOR_WHITE, messageString);

				playerVariables[userID][pGroupRank] = rank;
			}
        }
	}
	return 1;
}

CMD:granknames(playerid, params[]) {
    if(playerVariables[playerid][pGroup] >= 1 && playerVariables[playerid][pGroupRank] >= 6) {
		new
		    rankName[32],
		    rankID;

	    if(sscanf(params, "ds[32]", rankID, rankName)) {
			return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/granknames [rankid (1-6)] [rank title]");
		}
	    else {
	        new
				messageString[128];

	        switch(rankID) {
				case 1: {
				    mysql_real_escape_string(rankName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName1]);

				    format(messageString, sizeof(messageString), "You have changed the title of Rank 1 to '%s'.", rankName);
				    SendClientMessage(playerid, COLOR_WHITE, messageString);
				}
				case 2: {
				    mysql_real_escape_string(rankName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName2]);

				    format(messageString, sizeof(messageString), "You have changed the title of Rank 2 to '%s'.", rankName);
				    SendClientMessage(playerid, COLOR_WHITE, messageString);
				}
				case 3: {
				    mysql_real_escape_string(rankName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName3]);

				    format(messageString, sizeof(messageString), "You have changed the title of Rank 3 to '%s'.", rankName);
				    SendClientMessage(playerid, COLOR_WHITE, messageString);
				}
				case 4: {
				    mysql_real_escape_string(rankName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName4]);

				    format(messageString, sizeof(messageString), "You have changed the title of Rank 4 to '%s'.", rankName);
				    SendClientMessage(playerid, COLOR_WHITE, messageString);
				}
				case 5: {
				    mysql_real_escape_string(rankName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName5]);

				    format(messageString, sizeof(messageString), "You have changed the title of Rank 5 to '%s'.", rankName);
				    SendClientMessage(playerid, COLOR_WHITE, messageString);
				}
				case 6: {
				    mysql_real_escape_string(rankName, groupVariables[playerVariables[playerid][pGroup]][gGroupRankName6]);

				    format(messageString, sizeof(messageString), "You have changed the title of Rank 6 to '%s'.", rankName);
				    SendClientMessage(playerid, COLOR_WHITE, messageString);
				}
			}
	    }
    }
	return 1;
} 

CMD:m(playerid, params[]) {
	return cmd_megaphone(playerid, params);
}

CMD:megaphone(playerid, params[]) { 
    if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] != 1) return SendClientMessage(playerid, COLOR_GREY, "You're not a law enforcement officer.");
    if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/megaphone [message]");
    if (!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You're not in a vehicle."); 
        
    nearByMessage(playerid, COLOR_HOTORANGE, 50.0, "(megaphone) %s says: %s", getName(playerid), params);

    gString[0] = (EOS);
    mysql_escape_string(gString, gString);

    mysql_format(gSQL, gQuery, sizeof(gQuery), "INSERT INTO chatlogs (value, playerinternalid) VALUES('%s', '%d')", gString, playerVariables[playerid][pInternalID]);
    mysql_tquery(gSQL, gQuery, "", ""); 
	
    return true;
} 

CMD:quitgroup(playerid, params[]) {
	if(playerVariables[playerid][pGroup] == 0) return SendClientMessage(playerid, COLOR_WHITE, "You don't have a group to quit.");
 
    SendToGroup(playerVariables[playerid][pGroup], COLOR_GENANNOUNCE, "%s has left the group (quit).", getName(playerid)); 
    va_SendClientMessage(playerid, COLOR_WHITE, "You have left the %s.",groupVariables[playerVariables[playerid][pGroup]][gGroupName]);

    playerVariables[playerid][pGroup] = 0;
    playerVariables[playerid][pGroupRank] = 0;
    
	return true;
}
/*
CMD:elevator(playerid, params[]) {
	if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1) {
		if(IsPlayerInRangeOfPoint(playerid, 1, 276.0980, 122.1232, 1004.6172)) { // Interior
			ShowPlayerDialog(playerid, DIALOG_ELEVATOR3, DIALOG_STYLE_LIST, "Elevator", "Upper Roof\nLower Roof\nGarage", "Select", "Cancel");
		}
		else if(IsPlayerInRangeOfPoint(playerid, 1, 1568.6676, -1689.9708, 6.2188)) { // Garage
			ShowPlayerDialog(playerid, DIALOG_ELEVATOR2, DIALOG_STYLE_LIST, "Elevator", "Upper Roof\nLower Roof\nInterior", "Select", "Cancel");
		}
		else if(IsPlayerInRangeOfPoint(playerid, 1, 1564.8, -1666.2, 28.3)) { // Lower roof
			ShowPlayerDialog(playerid, DIALOG_ELEVATOR1, DIALOG_STYLE_LIST, "Elevator", "Upper Roof\nInterior\nGarage", "Select", "Cancel");
		}
		else if(IsPlayerInRangeOfPoint(playerid, 1, 1564.6584,-1670.2607,52.4503)) { // Upper roof
			ShowPlayerDialog(playerid, DIALOG_ELEVATOR4, DIALOG_STYLE_LIST, "Elevator", "Lower Roof\nInterior\nGarage", "Select", "Cancel");
		}
	}
	return 1;
}
*/

CMD:gate(playerid, params[]) {
	if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1) {
		if(IsPlayerInRangeOfPoint(playerid, 15.0, 1544.6, -1630.8, 13.0)) switch(LSPDGates[0][1]) {
			case 0: {
				SetDynamicObjectRot(LSPDGates[0][0],0.0, 0, 90.0);
				LSPDGates[0][1] = 1;
			}
			case 1: {
				SetDynamicObjectRot(LSPDGates[0][0],0.0, 90.0, 90.0);
				LSPDGates[0][1] = 0;
			}
		}
		else if(IsPlayerInRangeOfPoint(playerid, 15.0, 1589.19995117,-1637.98498535,14.69999981)) switch (LSPDGates[1][1]) {
			case 0: {
				MoveDynamicObject(LSPDGates[1][0] ,1589.19995117,-1637.98498535,9.69999981, 1.0);
				LSPDGates[1][1] = 1;
				PlayerPlaySoundEx(1035, 1589.19995117,-1637.98498535,14.69999981);
			}
			case 1: {
				MoveDynamicObject(LSPDGates[1][0],1589.19995117,-1637.98498535,14.69999981, 1.0);
				LSPDGates[1][1] = 0;
				PlayerPlaySoundEx(1035, 1589.19995117,-1637.98498535,14.69999981);
			}
		}
	}
	return 1;
}

CMD:cancelbackup(playerid, params[]) {
	if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1) {
	    DeletePVar(playerid, "rB");
	    SendClientMessage(playerid, COLOR_WHITE, "You have canceled your backup request.");
	}
	return 1;
}

CMD:backup(playerid, params[]) {
	if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1) { 
	    SendToGroup(playerVariables[playerid][pGroup], COLOR_RADIOCHAT, "Dispatch: %s is requesting for immediate backup (( '/acceptbackup %d' to take the call )).", getName(playerid), playerid);
	    SetPVarInt(playerid, "rB", 1); // Unlike the backup var (which will be called repeatedly) this will only be looked up when someone uses /acceptbackup.
	}
	return true;
}

CMD:acceptbackup(playerid, params[]) {
	if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1) {
		if(sscanf(params, "u", iTarget))
			SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/acceptbackup [playerid]");

		else if(playerVariables[iTarget][pStatus] == 1) {
			if(playerVariables[playerid][pCheckpoint] == 0 || playerVariables[playerid][pCheckpoint] == 5) {
				if(GetPVarInt(iTarget, "rB") == 1) {
					SendClientMessage(playerid, COLOR_WHITE, "You have responded to %s's backup call.", getName(iTarget);
					SendClientMessage(iTarget, COLOR_WHITE, "%s has responded to your backup call.", getName(playerid));

					playerVariables[playerid][pCheckpoint] = 5;
					playerVariables[playerid][pBackup] = iTarget;
 
				}
				else SendClientMessage(playerid, COLOR_GREY, "Invalid backup call specified.");
			}
			else { 
				va_SendClientMessage(playerid, COLOR_GREY, "You already have an active checkpoint (%s), reach it first, or /killcheckpoint.", getPlayerCheckpointReason(playerid));
			}
		}
		else SendClientMessage(playerid, COLOR_GREY, "Invalid backup call specified.");
	}
	return 1;
}

CMD:g(playerid, params[]) {
	return cmd_group(playerid, params);
}

CMD:group(playerid, params[]) {
	if(playerVariables[playerid][pStatus] != 1 || playerVariables[playerid][pGroup] < 1) return SendClientMessage(playerid, COLOR_GREY, "Your group data is invalid.");
	if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1 || groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 2) return SendClientMessage(playerid, COLOR_GREY, "This group does not have an OOC chat.");
	if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/g(roup) [message]");
	if(playerVariables[playerid][pFreezeType] > 0) return SendClientMessage(playerid, COLOR_GREY, "You can't use this command while cuffed, tazed, or frozen."); 

	switch(playerVariables[playerid][pGroupRank]) {
		case 1:	SendToGroup(playerVariables[playerid][pGroup], COLOR_DCHAT, "(Group Chat) %s %s: %s", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName1], getName(playerid), params);
		case 2:	SendToGroup(playerVariables[playerid][pGroup], COLOR_DCHAT, "(Group Chat) %s %s: %s", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName2], getName(playerid), params);
		case 3:	SendToGroup(playerVariables[playerid][pGroup], COLOR_DCHAT, "(Group Chat) %s %s: %s", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName3], getName(playerid), params);
		case 4:	SendToGroup(playerVariables[playerid][pGroup], COLOR_DCHAT, "(Group Chat) %s %s: %s", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName4], getName(playerid), params);
		case 5:	SendToGroup(playerVariables[playerid][pGroup], COLOR_DCHAT, "(Group Chat) %s %s: %s", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName5], getName(playerid), params);
		case 6:	SendToGroup(playerVariables[playerid][pGroup], COLOR_DCHAT, "(Group Chat) %s %s: %s", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName6], getName(playerid), params);
		default: SendToGroup(playerVariables[playerid][pGroup], COLOR_DCHAT, "(Group Chat) %s %s: %s", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName1], getName(playerid), params);
	}
    
	return true;
}

CMD:r(playerid, params[]) return cmd_radio(playerid, params);
CMD:radio(playerid, params[])  {
	if(playerVariables[playerid][pStatus] != 1 || playerVariables[playerid][pGroup] < 1) return SendClientMessage(playerid, COLOR_GREY, "Your group data is invalid.");
	if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] != 1 && groupVariables[playerVariables[playerid][pGroup]][gGroupType] != 2) return SendClientMessage(playerid, COLOR_GREY, "This group does not have an official radio frequency.");
	if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/g(roup) [message]");
	if(playerVariables[playerid][pFreezeType] > 0) return SendClientMessage(playerid, COLOR_GREY, "You can't use this command while cuffed, tazed, or frozen.");

	switch(playerVariables[playerid][pGroupRank]) {
		case 1:	SendToGroup(playerVariables[playerid][pGroup], COLOR_RADIOCHAT, "** %s %s: %s, over.", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName1], getName(playerid), params);
		case 2:	SendToGroup(playerVariables[playerid][pGroup], COLOR_RADIOCHAT, "** %s %s: %s, over.", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName2], getName(playerid), params);
		case 3:	SendToGroup(playerVariables[playerid][pGroup], COLOR_RADIOCHAT, "** %s %s: %s, over.", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName3], getName(playerid), params);
		case 4:	SendToGroup(playerVariables[playerid][pGroup], COLOR_RADIOCHAT, "** %s %s: %s, over.", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName4], getName(playerid), params);
		case 5:	SendToGroup(playerVariables[playerid][pGroup], COLOR_RADIOCHAT, "** %s %s: %s, over.", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName5], getName(playerid), params);
		case 6:	SendToGroup(playerVariables[playerid][pGroup], COLOR_RADIOCHAT, "** %s %s: %s, over.", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName6], getName(playerid), params);
		default: SendToGroup(playerVariables[playerid][pGroup], COLOR_RADIOCHAT, "** %s %s: %s, over.", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName1], getName(playerid), params);
	}

    // SetPlayerChatBubble(playerid, szMessage, COLOR_CHATBUBBLE, 10.0, 10000);

	return true;
}

CMD:d(playerid, params[]) return cmd_department(playerid, params);
CMD:department(playerid, params[]) {
	if(playerVariables[playerid][pStatus] != 1 || playerVariables[playerid][pGroup] < 1) return SendClientMessage(playerid, COLOR_GREY, "Your group data is invalid.");
	if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] != 1 && groupVariables[playerVariables[playerid][pGroup]][gGroupType] != 2) return SendClientMessage(playerid, COLOR_GREY, "This group does not have an official radio frequency.");
	if(playerVariables[playerid][pFreezeType] > 0) return SendClientMessage(playerid, COLOR_GREY, "You can't use this command while cuffed, tazed, or frozen.");
	if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/d(epartment) [message]");

	switch(playerVariables[playerid][pGroupRank]) {
		case 1:	sendDepartmentMessage(COLOR_DCHAT, "*** %s %s: %s, over.", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName1], getName(playerid), params);
		case 2:	sendDepartmentMessage(COLOR_DCHAT, "*** %s %s: %s, over.", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName2], getName(playerid), params);
		case 3:	sendDepartmentMessage(COLOR_DCHAT, "*** %s %s: %s, over.", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName3], getName(playerid), params);
		case 4:	sendDepartmentMessage(COLOR_DCHAT, "*** %s %s: %s, over.", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName4], getName(playerid), params);
		case 5:	sendDepartmentMessage(COLOR_DCHAT, "*** %s %s: %s, over.", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName5], getName(playerid), params);
		case 6:	sendDepartmentMessage(COLOR_DCHAT, "*** %s %s: %s, over.", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName6], getName(playerid), params);
		default: sendDepartmentMessage(COLOR_DCHAT, "*** %s %s: %s, over.", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName1], getName(playerid), params);
	}

    // SetPlayerChatBubble(playerid, szMessage, COLOR_CHATBUBBLE, 10.0, 10000);
	return true;
}
/*
CMD:unsuspendbank(playerid, params[]) {
	if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 2 && playerVariables[playerid][pGroup] != 0) {
		if(playerVariables[playerid][pGroupRank] > 4) {
		    if(isnull(params))
		        return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/unsuspendbank [player name]");

			strcpy(szPlayerName, params, MAX_PLAYER_NAME);
			mysql_real_escape_string(szPlayerName, szPlayerName);

		    iTarget = getIdFromName(szPlayerName);

			if(iTarget == -1)
				return SendClientMessage(playerid, COLOR_GREY, "Error attempting to retrieve an ID from the name.");

			format(szQueryOutput, sizeof(szQueryOutput), "DELETE FROM `banksuspensions` WHERE `playerID` = %d", iTarget);
			mysql_query(szQueryOutput);

			foreach(Player, x) {
				if(playerVariables[x][pInternalID] == iTarget) {
					DeletePVar(x, "BSuspend");
					DeletePVar(x, "BSuspendee");
				}
			}

			format(szMessage, sizeof(szMessage), "You've unsuspended %s's account.", szPlayerName);
			SendClientMessage(playerid, COLOR_WHITE, szMessage);
		}
	}
	return 1;
}


CMD:suspendbank(playerid, params[]) {
	if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 2 && playerVariables[playerid][pGroup] != 0) {
		if(playerVariables[playerid][pGroupRank] > 4) {
		    new
		        szReason[64];

		    if(sscanf(params, "us[64]", iTarget, szReason))
		        return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/suspendbank [playerid] [reason]");

            if(playerVariables[iTarget][pGroupRank] > 4 && groupVariables[playerVariables[iTarget][pGroup]][gGroupType] != 0)
                return SendClientMessage(playerid, COLOR_GREY, "Clearance failure.");

			mysql_real_escape_string(szReason, szReason);
			format(szQueryOutput, sizeof(szQueryOutput), "INSERT INTO `banksuspensions` (`suspendeeID`, `playerID`, `suspensionReason`) VALUES(%d, %d, '%e')", playerVariables[playerid][pInternalID], playerVariables[iTarget][pInternalID], szReason);
			mysql_query(szQueryOutput);

			SetPVarString(iTarget, "BSuspend", szReason);

			GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
			SetPVarString(iTarget, "BSuspendee", szPlayerName);

			GetPlayerName(iTarget, szPlayerName, MAX_PLAYER_NAME);
			format(szMessage, sizeof(szMessage), "You have successfully suspended the bank account of %s.", szPlayerName);
			SendClientMessage(playerid, COLOR_WHITE, szMessage);
		}
	}

	return 1;
}
*/
CMD:taxrate(playerid, params[]) {
	if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 2 && playerVariables[playerid][pGroup] != 0) {
		if(playerVariables[playerid][pGroupRank] > 4) {
			if(!isnull(params)) {

			    new rate = strval(params);

			    if(rate > 0 && rate <= 50) {
					va_SendClientMessage(playerid, COLOR_WHITE, "You have set the tax rate to %d percent.",rate);
				    assetVariables[1][aAssetValue] = rate;

					saveAsset(1);
			    }
			    else SendClientMessage(playerid, COLOR_GREY, "The tax rate must be between 1 and 50 percent.");
			}
			else SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/taxrate [percentage]");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You're not authorised to do this.");
	}
	return 1;
}

CMD:gov(playerid, params[]) {
	if(isnull(params)) SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/gov [message]");
	else if((groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1 || groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 2) && playerVariables[playerid][pGroupRank] > 4)
	{ 
		va_SendClientMessageToAll(COLOR_TEAL, "------ Government Announcement (%s) ------", groupVariables[playerVariables[playerid][pGroup]][gGroupName]);
 
		switch(playerVariables[playerid][pGroupRank]) {
			case 5: va_SendClientMessageToAll(COLOR_WHITE, "* %s %s: %s", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName5], getName(playerid), params);
			case 6: va_SendClientMessageToAll(COLOR_WHITE, "* %s %s: %s", groupVariables[playerVariables[playerid][pGroup]][gGroupRankName6], getName(playerid), params);
		}

		va_SendClientMessageToAll(COLOR_TEAL, "------ Government Announcement (%s) ------", groupVariables[playerVariables[playerid][pGroup]][gGroupName]);
	}
	return true;
}

CMD:gdeposit(playerid, params[]) {
    if(playerVariables[playerid][pStatus] != 1) return true;
    if(playerVariables[playerid][pGroup] == 0) return true;

    new item[9], string[64], amount;

    if(sscanf(params, "sd", item, amount)) {
        SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/gdeposit [money/materials] [amount]"); 
        va_SendClientMessage(playerid, COLOR_GREY, "Safe balance: $%d, %d materials.", groupVariables[playerVariables[playerid][pGroup]][gSafe][0], groupVariables[playerVariables[playerid][pGroup]][gSafe][1]);
    } else {
        if(amount > 0) {
            if(!IsPlayerInRangeOfPoint(playerid, 5.0, groupVariables[playerVariables[playerid][pGroup]][gSafePos][0], groupVariables[playerVariables[playerid][pGroup]][gSafePos][1], groupVariables[playerVariables[playerid][pGroup]][gSafePos][2])) 
                return SendClientMessage(playerid, COLOR_WHITE, "You must be at your group safe to do this.");

            if(strcmp(item, "money", true) == 0) {
                if(playerVariables[playerid][pMoney] < amount) return SendClientMessage(playerid, COLOR_WHITE, "You don't have that amount of money.");
                
                playerVariables[playerid][pMoney] -= amount;
                groupVariables[playerVariables[playerid][pGroup]][gSafe][0] += amount;
                
                va_SendClientMessage(playerid, COLOR_WHITE, "You have deposited $%d in your group safe.", amount);
                nearByMessage(playerid, COLOR_PURPLE, 12.0, "* %s deposits $%d in their group safe.", getName(playerid), amount);
                saveGroup(playerVariables[playerid][pGroup]);
            }
            else if(strcmp(item, "materials", true) == 0 ) {
                if(playerVariables[playerid][pMaterials] < amount) return SendClientMessage(playerid, COLOR_WHITE, "You don't have that amount of materials.");
                
                playerVariables[playerid][pMaterials] -= amount;
                groupVariables[playerVariables[playerid][pGroup]][gSafe][1] += amount;
                
                va_SendClientMessage(playerid, COLOR_WHITE, "You have deposited %d materials in your group safe.", amount);
                nearByMessage(playerid, COLOR_PURPLE, 12.0, "* %s deposits %d materials in their group safe.", getName(playerid), amount);
                saveGroup(playerVariables[playerid][pGroup]); 
            }
        }
    }

	return true;
}

//aici
CMD:confiscate(playerid, params[])
{
	if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1) {
		new
			targetID,
			string[128],
			item[12],
			playerNames[2][MAX_PLAYER_NAME];

		if(sscanf(params, "us[12]", targetID, item)) {
			SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/confiscate [playerid] [item]");
			SendClientMessage(playerid, COLOR_GREY, "Items: Materials, Phone, Weapons");
		}
		else if(IsPlayerAuthed(targetID)){
			if(IsPlayerInRangeOfPlayer(playerid, targetID, 3.0)) {
				if(playerVariables[targetID][pFreezeType] == 2 || playerVariables[targetID][pFreezeType] == 4 || (GetPlayerSpecialAction(targetID) == SPECIAL_ACTION_HANDSUP && playerVariables[targetID][pFreezeType] == 0)) {
					if(!strcmp(item, "materials", true)) {
						if(playerVariables[targetID][pMaterials] >= 1) {

							GetPlayerName(playerid, playerNames[0], MAX_PLAYER_NAME);
							GetPlayerName(targetID, playerNames[1], MAX_PLAYER_NAME);

							format(string, sizeof(string), "* %s has confiscated %d materials from %s.", playerNames[0], playerVariables[targetID][pMaterials], playerNames[1]);
							nearByMessage(playerid, COLOR_PURPLE, string);

							format(string, sizeof(string), "%s has confiscated your materials.", playerNames[0]);
							SendClientMessage(targetID, COLOR_WHITE, string);

							format(string, sizeof(string), "You have confiscated %s's materials (%d).", playerNames[1], playerVariables[targetID][pMaterials]);
							SendClientMessage(playerid, COLOR_WHITE, string);

							playerVariables[playerid][pMaterials] += playerVariables[targetID][pMaterials];
							playerVariables[targetID][pMaterials] = 0;
						}
						else SendClientMessage(playerid, COLOR_GREY, "This player has no materials to confiscate.");

					}
					else if(!strcmp(item, "weapons", true)) {

						GetPlayerName(playerid, playerNames[0], MAX_PLAYER_NAME);
						GetPlayerName(targetID, playerNames[1], MAX_PLAYER_NAME);

						format(string, sizeof(string), "* %s has confiscated %s's weapons.", playerNames[0], playerNames[1]);
						nearByMessage(playerid, COLOR_PURPLE, string);

						format(string, sizeof(string), "%s has confiscated your weapons.", playerNames[0]);
						SendClientMessage(targetID, COLOR_WHITE, string);

						format(string, sizeof(string), "You have confiscated %s's weapons.", playerNames[1]);
						SendClientMessage(playerid, COLOR_WHITE, string);

						validResetPlayerWeapons(targetID);
					}
					else if(!strcmp(item, "phone", true)) {
						if(playerVariables[targetID][pPhoneNumber] != -1) {

							GetPlayerName(playerid, playerNames[0], MAX_PLAYER_NAME);
							GetPlayerName(targetID, playerNames[1], MAX_PLAYER_NAME);

							format(string, sizeof(string), "* %s has confiscated %s's phone.", playerNames[0], playerNames[1]);
							nearByMessage(playerid, COLOR_PURPLE, string);

							format(string, sizeof(string), "%s has confiscated your phone.", playerNames[0]);
							SendClientMessage(targetID, COLOR_WHITE, string);

							format(string, sizeof(string), "You have confiscated %s's phone.", playerNames[1]);
							SendClientMessage(playerid, COLOR_WHITE, string);

							playerVariables[targetID][pPhoneNumber] = -1;
						}
						else SendClientMessage(playerid, COLOR_GREY, "This player has no phone to confiscate.");
					}
					else SendClientMessage(playerid, COLOR_GREY, "Invalid item specified.");
				}
				else SendClientMessage(playerid, COLOR_GREY, "That person must first be subdued, or have their hands up.");
			}
			else SendClientMessage(playerid, COLOR_GREY, "You're too far away.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
	}
	return 1;
}

CMD:gwithdraw(playerid, params[]) {
    if(playerVariables[playerid][pStatus] != 1) return 1;
    if(playerVariables[playerid][pGroup] != 0 && playerVariables[playerid][pGroupRank] >= 5) {

		new
			item[9],
			string[64],
			amount;

		if(sscanf(params, "sd", item, amount)) {
			SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/gwithdraw [money/materials] [amount]");

			format(string, sizeof(string), "Safe balance: $%d, %d materials.", groupVariables[playerVariables[playerid][pGroup]][gSafe][0], groupVariables[playerVariables[playerid][pGroup]][gSafe][1]);
			SendClientMessage(playerid, COLOR_GREY, string);
		}
		else {
		    if(amount > 0) {
			    if(IsPlayerInRangeOfPoint(playerid, 5.0, groupVariables[playerVariables[playerid][pGroup]][gSafePos][0], groupVariables[playerVariables[playerid][pGroup]][gSafePos][1], groupVariables[playerVariables[playerid][pGroup]][gSafePos][2])) {
			        if(strcmp(item, "money", true) == 0) {
						if(groupVariables[playerVariables[playerid][pGroup]][gSafe][0] >= amount) {
					    	playerVariables[playerid][pMoney] += amount;
					    	groupVariables[playerVariables[playerid][pGroup]][gSafe][0] -= amount;
					    	format(string, sizeof(string), "You have withdrawn $%d from your group safe.", amount);
				    		SendClientMessage(playerid, COLOR_WHITE, string);
				    		saveGroup(playerVariables[playerid][pGroup]);

							GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
							format(string, sizeof(string), "* %s withdraws $%d from their group safe.", szPlayerName, amount);
							nearByMessage(playerid, COLOR_PURPLE, string);
						}
						else {
					    	SendClientMessage(playerid, COLOR_WHITE, "You don't have that amount of money in your group safe.");
						}
					}
					else if(strcmp(item, "materials", true) == 0 ) {
						if(groupVariables[playerVariables[playerid][pGroup]][gSafe][1] >= amount) {
					    	playerVariables[playerid][pMaterials] += amount;
					    	groupVariables[playerVariables[playerid][pGroup]][gSafe][1] -= amount;
					    	format(string, sizeof(string), "You have withdrawn %d materials from your group safe.", amount);
				    		SendClientMessage(playerid, COLOR_WHITE, string);
				    		saveGroup(playerVariables[playerid][pGroup]);

							GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
							format(string, sizeof(string), "* %s withdraws %d materials from their group safe.", szPlayerName, amount);
							nearByMessage(playerid, COLOR_PURPLE, string);
						}
						else {
					    	SendClientMessage(playerid, COLOR_WHITE, "Your don't have that amount of materials in your group safe.");
						}
					}
				}
				else {
				    SendClientMessage(playerid, COLOR_WHITE, "You must be at your group safe to do this.");
				}
			}
		}
	}
	return 1;
}


CMD:swatinv(playerid,params[]) {
	if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1 && playerVariables[playerid][pGroup] != 0) {

		if(playerVariables[playerid][pGroupRank] > 3) {
			new string[64];
			GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
			if(groupVariables[playerVariables[playerid][pGroup]][gswatInv] == 0) {
				groupVariables[playerVariables[playerid][pGroup]][gswatInv] = 1;
				format(string, sizeof(string), "The SWAT inventory has been enabled by %s.", szPlayerName);
				SendToGroup(playerVariables[playerid][pGroup], COLOR_HOTORANGE, string);
			}
			else {
				groupVariables[playerVariables[playerid][pGroup]][gswatInv] = 0;
				format(string, sizeof(string), "The SWAT inventory has been disabled by %s.", szPlayerName);
				SendToGroup(playerVariables[playerid][pGroup], COLOR_HOTORANGE, string);
			}
		}
	}
	return 1;
}

CMD:tazer(playerid, params[]) {
	if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1 && playerVariables[playerid][pGroup] != 0) {
	    switch(playerVariables[playerid][pTazer]) {
			case 0: {
			    givePlayerValidWeapon(playerid, 22);
			    playerVariables[playerid][pTazer] = 1;
			}
			case 1: {
			    removePlayerWeapon(playerid, 22);
			    playerVariables[playerid][pTazer] = 0;
			}
		}
	}
	return 1;
}

CMD:taser(playerid, params[]) {
	return cmd_tazer(playerid, params);
}


CMD:fingerprint(playerid, params[]) {

	new
		targetID,
		string[106],
		dates[3],
		playerNames[2][MAX_PLAYER_NAME];

	if(sscanf(params, "u", targetID)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/fingerprint [playerid]");

	else if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1) {
		if(IsPlayerAuthed(targetID)) {
			if(playerVariables[targetID][pFreezeType] == 2 || playerVariables[targetID][pFreezeType] == 4 || (GetPlayerSpecialAction(targetID) == SPECIAL_ACTION_HANDSUP && playerVariables[targetID][pFreezeType] == 0)) {
				if(IsPlayerInRangeOfPlayer(playerid, targetID, 2.0)) {

					GetPlayerName(targetID, playerNames[0], MAX_PLAYER_NAME);
					GetPlayerName(playerid, playerNames[1], MAX_PLAYER_NAME);

					getdate(dates[0], dates[1], dates[2]);

					format(string, sizeof(string), "* %s grabs ahold of %s's finger, and places it on the scanner.", playerNames[1], playerNames[0]);
					nearByMessage(playerid, COLOR_PURPLE, string);

					SendClientMessage(playerid, COLOR_TEAL, "----------------------------------------------------------------");

					if(playerVariables[targetID][pCrimes] > 0 || playerVariables[targetID][pArrests] > 0 || playerVariables[targetID][pWarrants] > 0) {

						format(string, sizeof(string), "Citizen's registered name: %s", playerNames[0]);
						SendClientMessage(playerid, COLOR_WHITE, string);
						format(string, sizeof(string), "Citizen's age: %d (born %d)", dates[0] - playerVariables[targetID][pAge], playerVariables[targetID][pAge]);
						SendClientMessage(playerid, COLOR_WHITE, string);

						switch(playerVariables[targetID][pGender]) {
							case 1: SendClientMessage(playerid, COLOR_WHITE, "Citizen's gender: Male");
							case 2: SendClientMessage(playerid, COLOR_WHITE, "Citizen's gender: Female");
							default: SendClientMessage(playerid, COLOR_WHITE, "Citizen's gender: Unknown");
						}
						if(playerVariables[targetID][pPhoneNumber] == -1) {
							SendClientMessage(playerid, COLOR_WHITE, "Citizen's phone number: None");
						}
						else {
							format(string, sizeof(string), "Citizen's phone number: %d", playerVariables[targetID][pPhoneNumber]);
							SendClientMessage(playerid, COLOR_WHITE, string);
						}
					}
					else SendClientMessage(playerid, COLOR_WHITE, "No results found.");

					SendClientMessage(playerid, COLOR_TEAL, "----------------------------------------------------------------");
				}
				else SendClientMessage(playerid, COLOR_GREY, "You're too far away.");
			}
			else SendClientMessage(playerid, COLOR_GREY, "The person you wish to fingerprint must be restrained first (cuffed, or tied).");
		}
		else SendClientMessage(playerid, COLOR_GREY, "The specified player is either not connected or has not authenticated.");
	}
	return 1;
}

CMD:wanted(playerid, params[]) {
	if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1) {

		new
			dString[128],
			x;

		SendClientMessage(playerid, COLOR_TEAL, "---------------------------------------------------------------------------------------------------------------------------------");
		SendClientMessage(playerid, COLOR_WHITE, "Active Felons:");
		foreach(Player, i) {
			if(playerVariables[i][pWarrants] >= 1) {

				GetPlayerName(i, szPlayerName, MAX_PLAYER_NAME);

				if(x == 0)
					format(dString, sizeof(dString), "%s (%d)", szPlayerName, playerVariables[i][pWarrants]);
				else
					format(dString, sizeof(dString), "%s | %s (%d)", dString, szPlayerName, playerVariables[i][pWarrants]);

				if(x == 3) {
					SendClientMessage(playerid, COLOR_WHITE, dString);
					x = 0;
				}

				else x++;
			}
		}

		if(x < 3 && x > 0)
			SendClientMessage(playerid, COLOR_WHITE, dString);

		if(x == 0)
			SendClientMessage(playerid, COLOR_WHITE, "No active felons found.");

		SendClientMessage(playerid, COLOR_TEAL, "---------------------------------------------------------------------------------------------------------------------------------");
	}

	return 1;
}

CMD:ticket(playerid, params[]) {

	new
		targetID,
		price,
		string[96],
		playerName[2][MAX_PLAYER_NAME];

	if(sscanf(params, "ud", targetID, price)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/ticket [playerid] [price]");

	else if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1) {
		if(IsPlayerAuthed(targetID)) {
			if(playerid != targetID) {
				if(IsPlayerInRangeOfPlayer(playerid, targetID, 3.0)) {
					if(price >= 1 && price <= 100000) {

						GetPlayerName(playerid, playerName[0], MAX_PLAYER_NAME);
						GetPlayerName(targetID, playerName[1], MAX_PLAYER_NAME);

						format(string, sizeof(string), "* %s writes up a ticket, and hands it to %s.", playerName[0], playerName[1]);
						nearByMessage(playerid, COLOR_PURPLE, string);

						format(string, sizeof(string), "You have issued %s a ticket costing $%d.", playerName[1], price);
						SendClientMessage(playerid, COLOR_WHITE, string);

						format(string, sizeof(string), "%s has issued you a ticket costing $%d - /accept ticket to pay the fine.", playerName[0], price);
						SendClientMessage(targetID, COLOR_GENANNOUNCE, string);

						SetPVarInt(targetID, "tP", price);
						SetPVarInt(targetID, "tID", playerid + 1);
					}
					else SendClientMessage(playerid, COLOR_GREY, "Invalid price specified (must be between $1 and $100,000).");
				}
				else SendClientMessage(playerid, COLOR_GREY, "You're too far away.");
			}
			else SendClientMessage(playerid, COLOR_GREY, "You can't ticket yourself.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "The specified player is either not connected or has not authenticated.");
	}
	return 1;
}

CMD:suspect(playerid, params[]) {
	return cmd_su(playerid, params);
}

CMD:su(playerid, params[]) {

	new
		targetID,
		string[128],
		crime[96],
		playerName[2][MAX_PLAYER_NAME];

	if(sscanf(params, "us", targetID, crime))
		return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/su [playerid] [offence]");

	else if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1) {
		if(IsPlayerAuthed(targetID)) {
			if(groupVariables[playerVariables[targetID][pGroup]][gGroupType] != 1) {

				GetPlayerName(playerid, playerName[0], MAX_PLAYER_NAME);
				GetPlayerName(targetID, playerName[1], MAX_PLAYER_NAME);

				format(string, sizeof(string), "Dispatch: %s has issued an arrest warrant on %s (%s).", playerName[0], playerName[1], crime);
				SendToGroup(playerVariables[playerid][pGroup], COLOR_RADIOCHAT, string);

				playerVariables[targetID][pWarrants]++;
				if(playerVariables[targetID][pWarrants] < 7)
					SetPlayerWantedLevel(targetID, playerVariables[targetID][pWarrants]);
			}
			else SendClientMessage(playerid, COLOR_GREY, "You can't place an arrest warrant on this person.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "The specified player is either not connected or has not authenticated.");
	}
	return 1;
}

CMD:arrest(playerid, params[]) {

	new
		string[128],
		playerName[2][MAX_PLAYER_NAME],
		targetID,
		arrestInfo[3];

	if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] != 1) return SendClientMessage(playerid, COLOR_GREY, "You're not a law enforcement officer.");

	else if(sscanf(params, "udd", targetID, arrestInfo[0], arrestInfo[1])) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/arrest [playerid] [time] [price]");

	else if(targetID == playerid) return SendClientMessage(playerid, COLOR_GREY, "You can't arrest yourself.");

	else if((IsPlayerInRangeOfPoint(playerid,5, 1528.5240,-1678.2472,5.8906) && IsPlayerInRangeOfPoint(targetID,5, 1528.5240,-1678.2472,5.8906)) || (IsPlayerInRangeOfPoint(playerid, 20.0, 221.25, 110.0, 999.02) && IsPlayerInRangeOfPoint(targetID, 20.0, 221.25, 110.0, 999.02))) {
		if(playerVariables[targetID][pFreezeType] == 2) {
			if(arrestInfo[0] <= 60 && arrestInfo[0] > 0 && arrestInfo[1] <= 30000 && arrestInfo[1] >= 0) {

				GetPlayerName(playerid, playerName[0], MAX_PLAYER_NAME);
				GetPlayerName(targetID, playerName[1], MAX_PLAYER_NAME);

				validResetPlayerWeapons(targetID);

				playerVariables[targetID][pMoney] -= arrestInfo[1];
				playerVariables[targetID][pFreezeTime] = 0;
				playerVariables[targetID][pFreezeType] = 0;
				playerVariables[targetID][pPrisonID] = 3;
				playerVariables[targetID][pPrisonTime] = arrestInfo[0] * 60;
				playerVariables[targetID][pArmour] = 0;
				playerVariables[targetID][pArrests]++;
				playerVariables[targetID][pCrimes] += playerVariables[targetID][pWarrants];
				playerVariables[targetID][pWarrants] = 0;

				SetPlayerArmour(targetID, 0);
				TogglePlayerControllable(targetID, true);

				groupVariables[playerVariables[playerid][pGroup]][gSafe][0] += arrestInfo[1];

				format(string, sizeof(string),"You have been arrested by %s for %d minutes, and issued a fine of $%d.", playerName[0], arrestInfo[0], arrestInfo[1]);
				SendClientMessage(targetID, COLOR_NICESKY, string);

				format(string, sizeof(string),"Dispatch: %s has processed suspect %s, issuing a fine of $%d with a sentence of %d minutes.", playerName[0], playerName[1], arrestInfo[1], arrestInfo[0]);
				SendToGroup(playerVariables[playerid][pGroup], COLOR_RADIOCHAT, string);

				SetPlayerInterior(targetID, 10);
				SetPlayerVirtualWorld(targetID, GROUP_VIRTUAL_WORLD+1);

				arrestInfo[2] = random(sizeof(JailSpawns));

				SetPlayerPos(targetID, JailSpawns[arrestInfo[2]][0], JailSpawns[arrestInfo[2]][1], JailSpawns[arrestInfo[2]][2]);
				SetPlayerFacingAngle(targetID, 0);
			}
			else SendClientMessage(playerid, COLOR_GREY, "Fine price must be between $0 and $30,000; time must be 60 minutes or less.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "The person you wish to arrest must be restrained first (cuffed).");
	}
	else SendClientMessage(playerid, COLOR_GREY, "Both you and the person you wish to arrest must be at the arrest point.");
	return 1;
}

CMD:frisk(playerid, params[]) {

	new
		targetID,
		string[128],
		playerNames[2][MAX_PLAYER_NAME],
		count;

	if(sscanf(params, "u", targetID)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/frisk [playerid]");

	else if(IsPlayerInRangeOfPlayer(playerid, targetID, 2.0)) {
		if(playerVariables[targetID][pFreezeType] == 2 || (GetPlayerSpecialAction(targetID) == SPECIAL_ACTION_HANDSUP && playerVariables[targetID][pFreezeType] == 0) || playerVariables[targetID][pFreezeType] == 4) {

			GetPlayerName(playerid, playerNames[0], MAX_PLAYER_NAME);
			GetPlayerName(targetID, playerNames[1], MAX_PLAYER_NAME);

			format(string, sizeof(string), "* %s has frisked %s.", playerNames[0], playerNames[1]);
			nearByMessage(playerid, COLOR_PURPLE, string);

			SendClientMessage(playerid, COLOR_TEAL, "----------------------------------------------------------------");

			for(new x; x < 13; x++) { // Retrieve all weapons in slots, get their names, shove them into one string.
				if(playerVariables[targetID][pWeapons][x] > 0) {
					if(count == 0) format(string, sizeof(string), "* Weapons: %s", WeaponNames[playerVariables[targetID][pWeapons][x]]);
					else format(string, sizeof(string), "%s, %s", string, WeaponNames[playerVariables[targetID][pWeapons][x]]);
					count++;
				}
			}
			if(count >= 1) SendClientMessage(playerid, COLOR_GREY, string);

			if(playerVariables[targetID][pMaterials] >= 1) {
				format(string, sizeof(string), "* Materials: %d", playerVariables[targetID][pMaterials]);
				SendClientMessage(playerid, COLOR_GREY, string);
				count++;
			}
			if(playerVariables[targetID][pPhoneNumber] != -1) {
				SendClientMessage(playerid, COLOR_GREY, "Phone");
			}
			if(count == 0) SendClientMessage(playerid, COLOR_GREY, "No items found.");

			SendClientMessage(playerid, COLOR_TEAL, "----------------------------------------------------------------");
	    }
		else SendClientMessage(playerid, COLOR_GREY, "That person must first be subdued, or have their hands up.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You're too far away.");
	return 1;
}

CMD:cuff(playerid, params[]) {

	new
		string[128],
		playerName[2][MAX_PLAYER_NAME],
		target,
		Float:Pos[3];

    if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] != 1) return SendClientMessage(playerid, COLOR_GREY, "You're not a law enforcement officer.");

	else if(sscanf(params, "u", target)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/cuff [playerid]");

	else if(target == playerid) return SendClientMessage(playerid, COLOR_GREY, "You can't cuff yourself.");

    else {
	    if(IsPlayerInRangeOfPlayer(playerid, target, 3.0)) {
            if(playerVariables[target][pFreezeType] == 5 || playerVariables[target][pFreezeType] == 1 || (GetPlayerSpecialAction(target) == SPECIAL_ACTION_HANDSUP && playerVariables[target][pFreezeType] == 0) || playerVariables[target][pFreezeType] == 4) { // CAN NEVAR BE EXPLOITED!1 Means admin-frozen people can't be exploited out with cuffs.

				GetPlayerName(playerid, playerName[0], MAX_PLAYER_NAME);
				GetPlayerName(target, playerName[1], MAX_PLAYER_NAME);

				TogglePlayerControllable(target, 0);
				playerVariables[target][pFreezeTime] = 900;
				playerVariables[target][pFreezeType] = 2;
				GameTextForPlayer(target,"~n~~r~Handcuffed!",4000,4);

				GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);

				format(string, sizeof(string), "* %s has handcuffed %s.", playerName[0], playerName[1]);
				nearByMessage(playerid, COLOR_PURPLE, string);
				format(string, sizeof(string),"You have handcuffed %s.", playerName[1]);
				SendClientMessage(playerid, COLOR_NICESKY, string);

				PlayerPlaySoundEx(1145, Pos[0], Pos[1], Pos[2]);
				ApplyAnimation(target, "PED", "cower", 1, 1, 0, 0, 0, 0, 1);
    	    }
    	    else SendClientMessage(playerid, COLOR_GREY, "That person must first be subdued, or have their hands up.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You're too far away.");
	}
	return 1;
} 

CMD:uncuff(playerid, params[]) {
	new
		string[128],
		playerName[2][MAX_PLAYER_NAME],
		target;

	if(sscanf(params, "u", target)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/uncuff [playerid]");

    if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] != 1) {
        SendClientMessage(playerid, COLOR_GREY, "You're not a law enforcement officer.");
    }

	else if(target == playerid) SendClientMessage(playerid, COLOR_GREY, "You can't uncuff yourself.");

    else {
	    if(IsPlayerInRangeOfPlayer(playerid, target, 4.0)) {
            if(playerVariables[target][pFreezeType] == 2) {

				GetPlayerName(playerid, playerName[0], MAX_PLAYER_NAME);
				GetPlayerName(target, playerName[1], MAX_PLAYER_NAME);

				playerVariables[target][pFreezeTime] = 0;
				playerVariables[target][pFreezeType] = 0;

				TogglePlayerControllable(target, 1);
				PlayerPlaySound(playerid, 1145, 0.0, 0.0, 0.0);
				ClearAnimations(target);
				ApplyAnimation(target, "CARRY", "crry_prtial", 4.0, 0, 0, 0, 0, 0);
				GameTextForPlayer(target,"~n~~g~Uncuffed!",4000,4);

				format(string, sizeof(string), "* %s has uncuffed %s.", playerName[0], playerName[1]);
				nearByMessage(playerid, COLOR_PURPLE, string);
				format(string, sizeof(string),"You have uncuffed %s.", playerName[1]);
				SendClientMessage(playerid, COLOR_NICESKY, string);
				PlayerPlaySound(playerid, 1145, 0.0, 0.0, 0.0);

				if(playerVariables[target][pDrag] != -1) {
					format(string, sizeof(string), "You have stopped dragging %s.", playerName[1]);
					SendClientMessage(playerid, COLOR_WHITE, string);

					playerVariables[target][pDrag] = -1;

					format(string, sizeof(string), "* %s has stopped dragging %s, releasing their grip.", playerName[0], playerName[1]);

					return nearByMessage(playerid, COLOR_PURPLE, string);
				}
    	    }
    	    else SendClientMessage(playerid, COLOR_GREY, "That person is not cuffed.");
		}
	}
	return 1;
}


CMD:lspd(playerid,params[]) {
    if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1 && playerVariables[playerid][pGroup] != 0) {
	    new string[64];
	 	if(IsPlayerInRangeOfPoint(playerid, 5, 264.1055,109.8094,1004.6172) && GetPlayerInterior(playerid) == 10) {
   			format(string, sizeof(string), "%s Menu", groupVariables[playerVariables[playerid][pGroup]][gGroupName]);
	    	ShowPlayerDialog(playerid, DIALOG_LSPD, DIALOG_STYLE_LIST, string, "Equipment\nRelease Suspect\nClothing\nClear Suspect", "Select", "Cancel");
		}
    }
	return 1;
}

