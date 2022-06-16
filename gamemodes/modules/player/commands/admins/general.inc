CMD:unfreeze(playerid, params[]) {
	return cmd_freeze(playerid, params);
}

CMD:freeze(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 2) {
		new
			string[128],
			target;

		if(sscanf(params, "u", target)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/freeze [playerid]");

		else if(playerVariables[playerid][pAdminLevel] >= playerVariables[target][pAdminLevel]) {

			GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);

			switch(playerVariables[target][pFreezeType]) {
				case 3: {

					playerVariables[target][pFreezeTime] = 0;
					playerVariables[target][pFreezeType] = 0;
					TogglePlayerControllable(target, 1);

					format(string, sizeof(string), "You have been unfrozen by Administrator %s.", szPlayerName);
					SendClientMessage(target, COLOR_WHITE, string);

					GetPlayerName(target, szPlayerName, MAX_PLAYER_NAME);
					format(string, sizeof(string), "You have unfrozen %s.", szPlayerName);
					SendClientMessage(playerid, COLOR_WHITE, string);
				}
				default: {

					TogglePlayerControllable(target, 0);
					playerVariables[target][pFreezeTime] = -1;
					playerVariables[target][pFreezeType] = 3;

					format(string, sizeof(string), "You have been frozen by Administrator %s.", szPlayerName);
					SendClientMessage(target, COLOR_WHITE, string);

					GetPlayerName(target, szPlayerName, MAX_PLAYER_NAME);
					format(string, sizeof(string), "You have frozen %s.", szPlayerName);
					SendClientMessage(playerid, COLOR_WHITE, string);
				}
			}
		}
		else SendClientMessage(playerid, COLOR_GREY, "You can't freeze a higher level administrator.");
	}
	return 1;
}

CMD:changename(playerid, params[]) 
{
	if(playerVariables[playerid][pAdminLevel] >= 4) 
	{
		new
			newName[MAX_PLAYER_NAME];

		if(sscanf(params, "us[24]", iTarget, newName)) 
		{
			SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/changename [playerid] [newname]");
		}
		else if(playerVariables[iTarget][pStatus] != 1)
		{
			SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
		}
		else 
		{
			if(getPlayerBusinessID(iTarget) >= 1)
				strcpy(businessVariables[getPlayerBusinessID(iTarget)][bOwner], newName, MAX_PLAYER_NAME);

			if(getPlayerHouseID(iTarget) >= 1)
				strcpy(houseVariables[getPlayerHouseID(iTarget)][hHouseOwner], newName, MAX_PLAYER_NAME);

			new
				playerName[2][MAX_PLAYER_NAME],
				querySz[150];

			format(querySz, sizeof(querySz), "SELECT playerName FROM playeraccounts WHERE playerName = '%s'", newName);
			mysql_query(querySz);
			mysql_store_result();

			if(mysql_num_rows() > 0) {
			    SendClientMessage(playerid, COLOR_GREY, "That name is already taken.");
			    mysql_free_result();
			    return 1;
			}

			mysql_real_escape_string(newName, newName);

			GetPlayerName(playerid, playerName[0], MAX_PLAYER_NAME);
			GetPlayerName(iTarget, playerName[1], MAX_PLAYER_NAME);

			format(querySz, sizeof(querySz), "UPDATE playeraccounts SET playerName = '%s' WHERE playerID = '%d'", newName, playerVariables[iTarget][pInternalID]);
			mysql_query(querySz); // No point in threading a simple response...

			format(querySz, sizeof(querySz), "INSERT INTO namechanges (userid, oldname, newname, adminid) VALUES(%d, '%s', '%s', %d)", playerVariables[iTarget][pInternalID], playerName[1], newName, playerVariables[playerid][pInternalID]);
			mysql_query(querySz);

			format(querySz, sizeof(querySz), "Administrator %s has changed your name to %s.", playerName[0], newName); // Might as well re-use the string...
			SendClientMessage(iTarget, COLOR_WHITE, querySz);

			format(querySz, sizeof(querySz), "You have changed %s (ID: %d)'s name to %s.", playerName[1], iTarget, newName);
			SendClientMessage(playerid, COLOR_WHITE, querySz);

			SetPlayerName(iTarget, newName);

			strcpy(playerVariables[iTarget][pNormalName], newName, MAX_PLAYER_NAME);
		}
	}
	return 1;
}

CMD:listguns(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 1) {
	    new
	        targetid;

		if(sscanf(params, "u", targetid)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/listguns [playerid]");
		if(targetid == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");

		SendClientMessage(playerid, COLOR_TEAL, "--------------------------------------------------------------");

		for(new i = 0; i < 13; i++) {
		    if(playerVariables[targetid][pWeapons][i] >= 1) {
			    format(szMessage, sizeof(szMessage), "Weapon: %s (%d) | Slot: %d", WeaponNames[playerVariables[targetid][pWeapons][i]], playerVariables[targetid][pWeapons][i], i);
			    SendClientMessage(playerid, COLOR_WHITE, szMessage);
		    }
		}

		SendClientMessage(playerid, COLOR_TEAL, "--------------------------------------------------------------");
	}
	return 1;
}

CMD:go(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] > 0) {
		ShowPlayerDialog(playerid, DIALOG_GO, DIALOG_STYLE_LIST, "SERVER: Teleport Locations", "House Interiors\nRace Tracks\nCity Locations\nPopular Locations\nGym Interiors\nOther", "Select", "Cancel");
	}
	return 1;
}

CMD:setadminname(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 4) {
        new
            userID,
            playerNameString[MAX_PLAYER_NAME];

        if(sscanf(params, "us[24]", userID, playerNameString)) {
            return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/setadminname [playerid] [adminname]");
        }
        else {
            if(!IsPlayerConnected(userID))
				return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");

            if(playerVariables[userID][pAdminLevel] >= 1) {
                if(playerVariables[userID][pAdminLevel] > playerVariables[playerid][pAdminLevel]) {
                    return SendClientMessage(playerid, COLOR_GREY, "You can't change the admin name of a higher level administrator.");
                }
                else {
                    new
                        messageString[128];

                    format(messageString, sizeof(messageString), "You have changed %s's admin name to %s.", playerVariables[userID][pAdminName], playerNameString);
                    SendClientMessage(playerid, COLOR_WHITE, messageString);

                    format(messageString, sizeof(messageString), "%s has changed your admin name to %s.", playerVariables[playerid][pAdminName], playerNameString);
                    SendClientMessage(userID, COLOR_WHITE, messageString);

                    format(playerVariables[userID][pAdminName], MAX_PLAYER_NAME, "%s", playerNameString);
                    
                    if(playerVariables[userID][pAdminDuty] >= 1)
						SetPlayerName(userID, playerNameString);
						
                    return 1;
                }
            }
            else {
                return SendClientMessage(playerid, COLOR_GREY, "You can't change a non-admin's admin name.");
            }
        }
	}
	return 1;
}

CMD:setnewbiespawn(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 5) {
        if(GetPVarInt(playerid, "pAdminPINConfirmed") >= 1) {
	        GetPlayerPos(playerid, playerVariables[playerid][pPos][0], playerVariables[playerid][pPos][1], playerVariables[playerid][pPos][2]);
	        format(szLargeString, sizeof(szLargeString), "ALTER TABLE `playeraccounts` CHANGE `playerPosX` `playerPosX` VARCHAR( 255 ) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL DEFAULT '%f',", playerVariables[playerid][pPos][0]);
	        format(szLargeString, sizeof(szLargeString), "%s CHANGE `playerPosY` `playerPosY` VARCHAR( 255 ) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL DEFAULT '%f',", szLargeString, playerVariables[playerid][pPos][1]);
	        format(szLargeString, sizeof(szLargeString), "%s CHANGE `playerPosZ` `playerPosZ` VARCHAR( 255 ) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL DEFAULT '%f',", szLargeString, playerVariables[playerid][pPos][2]);
	        format(szLargeString, sizeof(szLargeString), "%s CHANGE `playerInterior` `playerInterior` INT( 6 ) NOT NULL DEFAULT '%d',", szLargeString, GetPlayerInterior(playerid));
	        format(szLargeString, sizeof(szLargeString), "%s CHANGE `playerSkin` `playerSkin` INT( 6 ) NOT NULL DEFAULT '%d',", szLargeString, GetPlayerSkin(playerid));
	        format(szLargeString, sizeof(szLargeString), "%s CHANGE `playerVirtualWorld` `playerVirtualWorld` INT( 6 ) NOT NULL DEFAULT '%d'", szLargeString, GetPlayerVirtualWorld(playerid));
	        mysql_query(szLargeString, THREAD_CHANGE_SPAWN, playerid);
	        SendClientMessage(playerid, COLOR_GENANNOUNCE, "SERVER:{FFFFFF} You've successfully changed the newbie spawn position.");
        } else {
            forceAdminConfirmPIN(playerid, "setnewbiespawn", params);
        }
    }
	return 1;
}

CMD:setadminlevel(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] < 5)
        return 0;

	if(GetPVarInt(playerid, "pAdminPINConfirmed") >= 1) {
		new
		    iLevel;

		if(sscanf(params, "ud", iTarget, iLevel))
		    return SendClientMessage(playerid, COLOR_GREY, "Syntax: /setadminlevel [playerid] [admin level]");

		if(iTarget == INVALID_PLAYER_ID)
		    return SendClientMessage(playerid, COLOR_GREY, "The specified player is not connected, or has not authenticated.");

		if(playerVariables[iTarget][pAdminLevel] > playerVariables[playerid][pAdminLevel] || iTarget == playerid)
		    return SendClientMessage(playerid, COLOR_GREY, "You can't modify the admin level of someone who retains a higher level of admin.");

		if(playerVariables[iTarget][pAdminLevel] < iLevel) {
		    format(szMessage, sizeof(szMessage), "You've been promoted to level %d admin, by %s.", iLevel, playerVariables[playerid][pNormalName]);
		    SendClientMessage(iTarget, COLOR_YELLOW, szMessage);

		    format(szMessage, sizeof(szMessage), "You've promoted %s to level %d admin.", playerVariables[iTarget][pNormalName], iLevel);
		    SendClientMessage(playerid, COLOR_YELLOW, szMessage);
		} else {
		    format(szMessage, sizeof(szMessage), "You've been demoted to level %d admin, by %s.", iLevel, playerVariables[playerid][pNormalName]);
		    SendClientMessage(iTarget, COLOR_YELLOW, szMessage);

		    format(szMessage, sizeof(szMessage), "You've demoted %s to level %d admin.", playerVariables[iTarget][pNormalName], iLevel);
		    SendClientMessage(playerid, COLOR_YELLOW, szMessage);
		}

	    playerVariables[iTarget][pAdminLevel] = iLevel;
	    
	    
    } else forceAdminConfirmPIN(playerid, "setadminlevel", params);
    
	return 1;
}

CMD:adminduty(playerid, params[]) 
{
	if(playerVariables[playerid][pAdminLevel] >= 1) 
	{
		if(playerVariables[playerid][pAdminName][0] == '*')
		{
			SendClientMessage(playerid, COLOR_GREY, "You don't have an admin name set. Contact a Head Admin (or higher) first.");
		}
		else 
		{
		    switch(playerVariables[playerid][pAdminDuty]) 
			{
				case 0: 
				{
				    playerVariables[playerid][pAdminDuty] = 1;
					GetPlayerHealth(playerid, playerVariables[playerid][pHealth]);
					GetPlayerArmour(playerid, playerVariables[playerid][pArmour]);
					SetPlayerName(playerid, playerVariables[playerid][pAdminName]);
					SetPlayerHealth(playerid, 500000.0);
					format(szMessage, sizeof(szMessage), "Notice: {FFFFFF}Admin %s (%s) is now on administrative duty.", playerVariables[playerid][pAdminName], playerVariables[playerid][pNormalName]);
				}
				case 1: 
				{
				    playerVariables[playerid][pAdminDuty] = 0;
					SetPlayerName(playerid, playerVariables[playerid][pNormalName]);
					SetPlayerHealth(playerid, playerVariables[playerid][pHealth]);
					SetPlayerArmour(playerid, playerVariables[playerid][pArmour]);
					format(szMessage, sizeof(szMessage), "Notice: {FFFFFF}Admin %s (%s) is now off administrative duty.", playerVariables[playerid][pAdminName], playerVariables[playerid][pNormalName]);
				}
			}
			submitToAdmins(szMessage, COLOR_HOTORANGE);
		}
	}
	return 1;
}

CMD:set(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 3) {
        new
            item[32],
            userID,
            amount;

        if(sscanf(params, "us[32]d", userID, item, amount)) {
			SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/set [playerid] [item] [amount]");
			SendClientMessage(playerid, COLOR_GREY, "Items: Health, Armour, Money, BankMoney, Skin, Interior, VirtualWorld, Job, JobSkill1, JobSkill2,");
			SendClientMessage(playerid, COLOR_GREY, "Phone, Materials, Group, GroupRank, Age, Gender");
		}
        else if(IsPlayerAuthed(userID)) {
            if(playerVariables[playerid][pAdminLevel] >= playerVariables[userID][pAdminLevel]) {
				GetPlayerName(userID, szPlayerName, MAX_PLAYER_NAME);

				if(strcmp(item, "health", true) == 0) {
					format(szMessage, sizeof(szMessage), "You have set %s (ID: %d)'s health to %d.", szPlayerName, userID, amount);
					SendClientMessage(playerid, COLOR_WHITE, szMessage);
					SetPlayerHealth(userID, amount);
				}
				else if(strcmp(item, "jobskill2", true) == 0) {
					format(szMessage, sizeof(szMessage), "You have set %s (ID: %d)'s JobSkill2 to %d.", szPlayerName, userID, amount);
					SendClientMessage(playerid, COLOR_WHITE, szMessage);
					playerVariables[userID][pJobSkill][1] = amount;
				}
				else if(strcmp(item, "jobskill1", true) == 0) {
					format(szMessage, sizeof(szMessage), "You have set %s (ID: %d)'s JobSkill1 to %d.", szPlayerName, userID, amount);
					SendClientMessage(playerid, COLOR_WHITE, szMessage);
					playerVariables[userID][pJobSkill][0] = amount;
				}
				else if(strcmp(item, "virtualworld", true) == 0) {
					format(szMessage, sizeof(szMessage), "You have set %s (ID: %d)'s virtual world to %d.", szPlayerName, userID, amount);
					SendClientMessage(playerid, COLOR_WHITE, szMessage);
					SetPlayerVirtualWorld(userID, amount);
					playerVariables[userID][pVirtualWorld] = amount;
				}
				else if(strcmp(item, "interior", true) == 0) {
					format(szMessage, sizeof(szMessage), "You have set %s (ID: %d)'s interior to %d.", szPlayerName, userID, amount);
					SendClientMessage(playerid, COLOR_WHITE, szMessage);
					SetPlayerInterior(userID, amount);
					playerVariables[userID][pInterior] = amount;
				}
				else if(strcmp(item, "job", true) == 0) {
					if(amount >= 0 && amount <= MAX_JOBS) {
						format(szMessage, sizeof(szMessage), "You have set %s (ID: %d)'s job to %d.", szPlayerName, userID, amount);
						SendClientMessage(playerid, COLOR_WHITE, szMessage);
						playerVariables[userID][pJob] = amount;
						playerVariables[userID][pJobDelay] = 0;
					}
					else SendClientMessage(playerid, COLOR_GREY, "Invalid job specified.");
				}
				else if(strcmp(item, "armour", true) == 0) {
					format(szMessage, sizeof(szMessage), "You have set %s (ID: %d)'s armour to %d.", szPlayerName, userID, amount);
					SendClientMessage(playerid, COLOR_WHITE, szMessage);
					SetPlayerArmour(userID, amount);
				}
				else if(strcmp(item, "bankmoney", true) == 0) {
					format(szMessage, sizeof(szMessage), "You have set %s (ID: %d)'s bank balance to %d.", szPlayerName, userID, amount);
					SendClientMessage(playerid, COLOR_WHITE, szMessage);
					playerVariables[userID][pBankMoney] = amount;
				}
				else if(strcmp(item, "money", true) == 0) {
					format(szMessage, sizeof(szMessage), "You have set %s (ID: %d)'s money to %d.", szPlayerName, userID, amount);
					SendClientMessage(playerid, COLOR_WHITE, szMessage);
					playerVariables[userID][pMoney] = amount;
				}
				else if(strcmp(item, "materials", true) == 0) {
					format(szMessage, sizeof(szMessage), "You have set %s (ID: %d)'s materials to %d.", szPlayerName, userID, amount);
					SendClientMessage(playerid, COLOR_WHITE, szMessage);
					playerVariables[userID][pMaterials] = amount;
				}
				else if(strcmp(item, "skin", true) == 0) {
					if(IsValidSkin(amount)) {
						format(szMessage, sizeof(szMessage), "You have set %s (ID: %d)'s skin to %d.", szPlayerName, userID, amount);
						SendClientMessage(playerid, COLOR_WHITE, szMessage);
						SetPlayerSkin(userID, amount);
						if(playerVariables[userID][pEvent] == 1) SendClientMessage(playerid, COLOR_WHITE, "As this player is participating in an event, their original skin will be restored once it has ended.");
						else playerVariables[userID][pSkin] = amount;
					}
					else SendClientMessage(playerid, COLOR_GREY, "Invalid skin specified.");
				}
				else if(strcmp(item, "phone", true) == 0) {
					format(szMessage, sizeof(szMessage), "You have set %s (ID: %d)'s phone number to %d.", szPlayerName, userID, amount);
					SendClientMessage(playerid, COLOR_WHITE, szMessage);
					playerVariables[userID][pPhoneNumber] = amount;
				}
				else if(strcmp(item, "group", true) == 0) {
					if(amount >= 0 && amount <= MAX_GROUPS) {

						format(szMessage, sizeof(szMessage), "You have set %s (ID: %d)'s group to %d.", szPlayerName, userID, amount);
						SendClientMessage(playerid, COLOR_WHITE, szMessage);

						format(szMessage, sizeof(szMessage), "%s has left the group (admin-set).", szPlayerName);
						SendToGroup(playerVariables[userID][pGroup], COLOR_GENANNOUNCE, szMessage);

						playerVariables[userID][pGroup] = amount;

						format(szMessage, sizeof(szMessage), "%s has joined the group (admin-set).", szPlayerName);
						SendToGroup(playerVariables[userID][pGroup], COLOR_GENANNOUNCE, szMessage);
					}
					else SendClientMessage(playerid, COLOR_GREY, "Invalid group specified.");
				}
				else if(strcmp(item, "grouprank", true) == 0) {
					if(amount >= 1 && amount <= 6) {
						format(szMessage, sizeof(szMessage), "You have set %s (ID: %d)'s group rank to %d.", szPlayerName, userID, amount);
						SendClientMessage(playerid, COLOR_WHITE, szMessage);
						playerVariables[userID][pGroupRank] = amount;
					}
					else SendClientMessage(playerid, COLOR_GREY, "Invalid rank specified.");
				}
				else if(strcmp(item, "age", true) == 0) {
					if(amount >= 16 && amount <= 122) {

						new
							dates[3];

						getdate(dates[0], dates[1], dates[2]);
						playerVariables[userID][pAge] = dates[0] - amount;

						format(szMessage, sizeof(szMessage), "You have set %s (ID: %d)'s age to %d (birth year %d).", szPlayerName, userID, amount, playerVariables[userID][pAge]);
						SendClientMessage(playerid, COLOR_WHITE, szMessage);
					}
					else SendClientMessage(playerid, COLOR_GREY, "Invalid age specified (must be between 16 and 122 years old).");
				}
				else if(strcmp(item, "gender", true) == 0) {
					if(amount >= 1 && amount <= 2) {

						playerVariables[userID][pGender] = amount;

						switch(playerVariables[userID][pGender]) {
							case 1: format(szMessage, sizeof(szMessage), "You have set %s (ID: %d)'s gender to male.", szPlayerName, userID);
							case 2: format(szMessage, sizeof(szMessage), "You have set %s (ID: %d)'s gender to female.", szPlayerName, userID);
						}
						SendClientMessage(playerid, COLOR_WHITE, szMessage);
					}
					else SendClientMessage(playerid, COLOR_GREY, "Invalid gender specified; must be 1 (male) or 2 (female).");
				}
			}
			else SendClientMessage(playerid, COLOR_GREY, "You can't set a higher level administrator's statistics.");
        }
		else SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
    }
	return 1;
}

CMD:a(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 1) {
		if(!isnull(params)) {
		    new
		        messageString[128];

		    format(messageString, sizeof(messageString), "* Admin %s (%d) says: %s", playerVariables[playerid][pAdminName], playerVariables[playerid][pAdminLevel], params);
		    submitToAdmins(messageString, COLOR_YELLOW);
		}
		else {
			SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/a [message]");
		}
	}
	return 1;
}

CMD:he(playerid, params[]) {
	if(playerVariables[playerid][pHelper] >= 1 || playerVariables[playerid][pAdminLevel] >= 1) {
		if(!isnull(params)) {
		    new
		        msgSz[128];

			if(playerVariables[playerid][pAdminLevel] >= 1)
				format(msgSz, sizeof(msgSz), "* Administrator %s (%d): %s", playerVariables[playerid][pAdminName], playerVariables[playerid][pAdminLevel], params);

			if(playerVariables[playerid][pHelper] >= 1)
				format(msgSz, sizeof(msgSz), "* Helper %s (%d): %s", playerVariables[playerid][pNormalName], playerVariables[playerid][pHelper], params);


			foreach(Player, x) {
			    if(playerVariables[x][pHelper] >= 1 || playerVariables[x][pAdminLevel] >= 1) {
                    SendClientMessage(x, COLOR_GENANNOUNCE, msgSz);
				}
			}
		}
		else {
			return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/he [message]");
		}
	}
	return 1;
}

CMD:getvehicle(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 1) {
		new
		    iVehicleID = strval(params);

		if(doesVehicleExist(iVehicleID)) {
	        GetPlayerPos(playerid, playerVariables[playerid][pPos][0], playerVariables[playerid][pPos][1], playerVariables[playerid][pPos][2]);
	        SetVehiclePos(iVehicleID, playerVariables[playerid][pPos][0], playerVariables[playerid][pPos][1], playerVariables[playerid][pPos][2]);
	        
	        format(szMessage, sizeof(szMessage), "Vehicle %d has been teleported to your location", iVehicleID);
	        SendClientMessage(playerid, COLOR_WHITE, szMessage);
        }
    }
	return 1;
}

CMD:flipvehicle(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 1) {
        if(!IsPlayerInAnyVehicle(playerid))
            return SendClientMessage(playerid, COLOR_GREY, "You're not in a vehicle.");

        GetVehiclePos(GetPlayerVehicleID(playerid), playerVariables[playerid][pPos][0], playerVariables[playerid][pPos][1], playerVariables[playerid][pPos][2]);
        SetVehiclePos(GetPlayerVehicleID(playerid), playerVariables[playerid][pPos][0], playerVariables[playerid][pPos][1], playerVariables[playerid][pPos][2]);
        SendClientMessage(playerid, COLOR_WHITE, "Your vehicle has been flipped back over.");
    }
	return 1;
}

CMD:spawnweapon(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 3) {
        new
            weaponID,
            userID;

		if(sscanf(params, "ud", userID, weaponID))
			return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/spawnweapon [playerid] [weaponid]");

        if(userID != INVALID_PLAYER_ID) {
			new
				string[63];

			GetPlayerName(userID, szPlayerName, MAX_PLAYER_NAME);
			
			if(weaponID < 1 && weaponID > 48)
			    return SendClientMessage(playerid, COLOR_GREY, "Invalid weapon ID.");

			if(weaponID == 19) {
				if(GetPlayerState(userID) != 1) {
					return SendClientMessage(playerid, COLOR_GREY, "The specified player must be on foot.");
				}
				else {
					format(string, sizeof(string), "You have given %s a jetpack.", szPlayerName);
					SendClientMessage(playerid, COLOR_WHITE, string);
					playerVariables[userID][pJetpack] = 1;
					return SetPlayerSpecialAction(userID, SPECIAL_ACTION_USEJETPACK);
				}
			}
			else {
				format(string, sizeof(string), "You have given %s a %s.", szPlayerName, WeaponNames[weaponID]);
				SendClientMessage(playerid, COLOR_WHITE, string);
				return givePlayerValidWeapon(userID, weaponID);
			}
		}
		else SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
    }
    return 1;
}

CMD:get(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 1) {
        new
            userID;

		if(sscanf(params, "u", userID)) {
			return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/get [playerid]");
		}
        else {
            if(userID == INVALID_PLAYER_ID)
				return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");

			if(playerVariables[playerid][pAdminLevel] >= playerVariables[userID][pAdminLevel]) {
				new
					messageString[64],

					Float: fPos[3];

				GetPlayerPos(playerid, fPos[0], fPos[1], fPos[2]);
				if(GetPlayerState(userID) == 2) {

					SetVehiclePos(GetPlayerVehicleID(userID), fPos[0], fPos[1]+2, fPos[2]);
					LinkVehicleToInterior(GetPlayerVehicleID(userID), GetPlayerInterior(playerid));
					SetVehicleVirtualWorld(GetPlayerVehicleID(userID), GetPlayerVirtualWorld(playerid));
				}

				else SetPlayerPos(userID, fPos[0], fPos[1]+2, fPos[2]); // If they're driving a vehicle, it gets the vehicle; otherwise, it warps them only.

				SetPlayerInterior(userID, GetPlayerInterior(playerid));
				SetPlayerVirtualWorld(userID, GetPlayerVirtualWorld(playerid));

				GetPlayerName(userID, szPlayerName, MAX_PLAYER_NAME);
				format(messageString, sizeof(messageString), "You have teleported %s to you.", szPlayerName);
				SendClientMessage(playerid, COLOR_WHITE, messageString);
			}
			else SendClientMessage(playerid, COLOR_GREY, "You can't teleport a higher level administrator - request them to teleport to you.");
		}
    }

    return 1;
}

CMD:gotoplayervehicle(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 2) {
        new
            userID;

		if(sscanf(params, "u", userID))
			return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/gotoplayervehicle [playerid]");

        if(userID == INVALID_PLAYER_ID)
			return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");

		if(playerVariables[userID][pCarModel] < 1)
			return SendClientMessage(playerid, COLOR_GREY, "That player does not own a vehicle.");

		new
		    messageString[64];

		GetVehiclePos(playerVariables[userID][pCarID], playerVariables[userID][pCarPos][0], playerVariables[userID][pCarPos][1], playerVariables[userID][pCarPos][2]);

		if(GetPlayerState(playerid) == 2) {
			SetVehiclePos(GetPlayerVehicleID(playerid), playerVariables[userID][pCarPos][0], playerVariables[userID][pCarPos][1]+2, playerVariables[userID][pCarPos][2]);
			SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), GetVehicleVirtualWorld(playerVariables[userID][pCarID]));
			LinkVehicleToInterior(GetPlayerVehicleID(playerid), 0);
		}
		else SetPlayerPos(playerid, playerVariables[userID][pCarPos][0], playerVariables[userID][pCarPos][1]+2, playerVariables[userID][pCarPos][2]);

		SetPlayerInterior(playerid, 0);
		SetPlayerVirtualWorld(playerid, GetVehicleVirtualWorld(playerVariables[userID][pCarID]));

		GetPlayerName(userID, szPlayerName, MAX_PLAYER_NAME);

		format(messageString, sizeof(messageString), "You have teleported to %s's %s.", szPlayerName, VehicleNames[playerVariables[userID][pCarModel] - 400]);
		SendClientMessage(playerid, COLOR_WHITE, messageString);
    }
    return 1;
}

CMD:goto(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 1) {
        new
            userID;

		if(sscanf(params, "u", userID)) {
			return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/goto [playerid]");
		}
        else {
            if(!IsPlayerConnected(userID)) return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");

			new
			    messageString[64],

			    Float: fPos[3];

			GetPlayerPos(userID, fPos[0], fPos[1], fPos[2]);

			if(GetPlayerState(playerid) == 2) {

				SetVehiclePos(GetPlayerVehicleID(playerid), fPos[0], fPos[1]+2, fPos[2]);

				LinkVehicleToInterior(GetPlayerVehicleID(playerid), GetPlayerInterior(userID));
				SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), GetPlayerVirtualWorld(userID));
			}

			else SetPlayerPos(playerid, fPos[0], fPos[1]+2, fPos[2]);

			SetPlayerInterior(playerid, GetPlayerInterior(userID));
			SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(userID));

			GetPlayerName(userID, szPlayerName, MAX_PLAYER_NAME);

			format(messageString, sizeof(messageString), "You have teleported to %s.", szPlayerName);
			SendClientMessage(playerid, COLOR_WHITE, messageString);
		}
    }

    return 1;
}

CMD:setleader(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 3) {
        new
            groupID,
            userID;

		if(sscanf(params, "ud", userID, groupID)) {
			return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/setleader [playerid] [groupid]");
		}
        else {
			if(groupID < 1 || groupID > MAX_GROUPS) return SendClientMessage(playerid, COLOR_GREY, "Invalid group ID.");

			playerVariables[userID][pGroup] = groupID;
			playerVariables[userID][pGroupRank] = 6;

			new

			    string[128];

			GetPlayerName(userID, szPlayerName, MAX_PLAYER_NAME);

			format(string, sizeof(string), "You have set %s to lead group %s.", szPlayerName, groupVariables[groupID][gGroupName]);
			SendClientMessage(playerid, COLOR_WHITE, string);

			GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);

			format(string, sizeof(string), "Administrator %s has set you to lead group %s.", szPlayerName, groupVariables[groupID][gGroupName]);
			SendClientMessage(userID, COLOR_WHITE, string);
		}
    }

    return 1;
}    

CMD:gotopoint(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 3) {
	    new
	        interiorID,
	        Float: coordinates[3];

		if(sscanf(params, "fffd", coordinates[0], coordinates[1], coordinates[2], interiorID)) {
		    SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/gotopoint [x] [y] [z] [interior id]");
		}
		else {
		    SetPlayerInterior(playerid, interiorID);
		    SetPlayerPos(playerid, coordinates[0], coordinates[1], coordinates[2]);
		}
	}

	return 1;
}

CMD:reports(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 1) {
		new
			tool[16]; 

		if(sscanf(params, "s[16] ", tool)) {
		    SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/reports [tool]");
		    SendClientMessage(playerid, COLOR_GREY, "Tools: List, Accept, Disregard, Status");
		}
		else {
		    if(strcmp(tool, "List", true) == 0) {
				SendClientMessage(playerid, COLOR_WHITE, "-------------------------------------------------------------------------------------------------------------------------------");

		        new
					string[128],
					reportCount;

		        foreach(Player, i) {
		            if(playerVariables[i][pReport] >= 1) {
		                GetPlayerName(i, szPlayerName, MAX_PLAYER_NAME);
		                format(string, sizeof(string), "[ACTIVE] %s [%d] has reported: %s", szPlayerName, i, playerVariables[i][pReportMessage]);
		                SendClientMessage(playerid, COLOR_YELLOW, string);
		                reportCount++;
		            }
		        }

		        format(string, sizeof(string), "ACTIVE REPORTS: %d.", reportCount);
		        SendClientMessage(playerid, COLOR_WHITE, string);

				SendClientMessage(playerid, COLOR_WHITE, "-------------------------------------------------------------------------------------------------------------------------------");
		    }
		    else if(strcmp(tool, "Accept", true) == 0)
		    {
		        new
					userID;

		        if(sscanf(params, "s[16]u", tool, userID)) {
		            SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/reports accept [playerid]");
		        }
				else if(playerVariables[userID][pStatus] != 1)
		        {
					SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
		        }
		        else if(playerVariables[userID][pReport] == 0)
		        {
					SendClientMessage(playerid, COLOR_GREY, "That player doesn't have an active report.");
				}
				else
				{
					new
						string[128];

					GetPlayerName(userID, szPlayerName, MAX_PLAYER_NAME);

		            format(string, sizeof(string), "You have accepted %s's report (%s)", szPlayerName, playerVariables[userID][pReportMessage]);
		            SendClientMessage(playerid, COLOR_WHITE, string);

		            playerVariables[userID][pReport] = 0;
					
		            GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);

		            format(string, sizeof(string), "Thank you for your report! Administrator %s is now reviewing your report.", szPlayerName);
		            SendClientMessage(userID, COLOR_YELLOW, string);

		            SetPVarInt(playerid, "aR", 1);
		            SetPVarInt(playerid, "aRf", userID);
					
					ShowPlayerDialog(playerid, DIALOG_REPORT, DIALOG_STYLE_LIST, "Report System", "Teleport\nSpectate\nTake no action", "Select", "Exit");
		        }
		    }
		    else if(strcmp(tool, "Disregard", true) == 0) {
		        new
					userID,
					string[128];

		        if(sscanf(params, "s[16]u", tool, userID)) {
		            SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/reports disregard [playerid]");
		        }
				else if(playerVariables[userID][pStatus] != 1)
		        {
					SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
		        }
				else if(playerVariables[userID][pReport] == 0)
		        {
					SendClientMessage(playerid, COLOR_GREY, "That player doesn't have an active report.");
				}
		        else 
				{
					GetPlayerName(userID, szPlayerName, MAX_PLAYER_NAME);

		            playerVariables[userID][pReport] = 0;

		            format(string, sizeof(string), "You have disregarded %s's report.", szPlayerName);
		            SendClientMessage(playerid, COLOR_WHITE, string);
		        }
		    }
		    else if(strcmp(tool, "Status", true) == 0) {
		        if(playerVariables[playerid][pAdminLevel] >= 4) {
			        if(systemVariables[reportSystem] == 0) {
			            systemVariables[reportSystem] = 1;
			            SendClientMessage(playerid, COLOR_WHITE, "You have disabled the report system.");
			            SendClientMessageToAll(COLOR_YELLOW, "The report system has been temporarily disabled.");
			        }
			        else {
			            systemVariables[reportSystem] = 0;
			            SendClientMessage(playerid, COLOR_WHITE, "You have enabled the report system.");
			            SendClientMessageToAll(COLOR_YELLOW, "The report system has been re-enabled.");
			        }
		        }
		        else {
					return SendClientMessage(playerid, COLOR_GREY, "You need to be a Head Administrator or above to use this command.");
				}
 		    }
		    else {
			    SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/reports [tool]");
			    SendClientMessage(playerid, COLOR_GREY, "TOOLS: List, Accept, Disregard, Status");
		    }
		}
	}

	return 1;
}

CMD:announce(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 1) {
        if(!isnull(params)) {
			GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
			format(szMessage, sizeof(szMessage), "(( Announcement from %s: %s ))", szPlayerName, params);
			SendClientMessageToAll(COLOR_LIGHTRED, szMessage);
		}
		else {
		    return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/announce [text]");
		}
	}
	return 1;
}

CMD:disableooc(playerid, params[]) 
{
    if(playerVariables[playerid][pAdminLevel] >= 2) 
	{
        if(systemVariables[OOCStatus] == 0) 
		{
		    systemVariables[OOCStatus] = 1;
		    SendClientMessageToAll(COLOR_LIGHTRED, "The OOC chat channel has been disabled.");
        }
        else 
		{
			SendClientMessage(playerid, COLOR_GREY, "OOC is already disbled.");
		}
    }
	return 1;
}

CMD:enableooc(playerid, params[]) 
{
    if(playerVariables[playerid][pAdminLevel] >= 2) 
	{
        if(systemVariables[OOCStatus] == 1) 
		{
		    systemVariables[OOCStatus] = 0;
		    SendClientMessageToAll(COLOR_LIGHTRED, "The OOC chat channel has been enabled.");
        }
        else 
		{
			SendClientMessage(playerid, COLOR_GREY, "OOC is already enabled.");
		}
    }
	return 1;
}

CMD:namechanges(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 1) {
        if(sscanf(params, "u", iTarget))
            return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/namechanges [playerid]");

		if(iTarget == INVALID_PLAYER_ID || playerVariables[playerid][pStatus] < 1)
		    return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");

		format(szQueryOutput, sizeof(szQueryOutput), "SELECT namechangeid, oldname, newname, time FROM namechanges WHERE userid = %d ORDER BY namechangeid ASC", playerVariables[iTarget][pInternalID]);
		mysql_query(szQueryOutput, THREAD_CHECK_PLAYER_NAMES, playerid);
    }
	return 1;
}



CMD:ahelp(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 1) {
        SendClientMessage(playerid, COLOR_TEAL, "--------------------------------------------------------------------------------------------------------------------------------");

	    SendClientMessage(playerid, COLOR_WHITE, "Level 1: /ban, /kick, /check, /reports, /announce, /warn, /listgroups, /a, /adminduty, /vrespawn, /fixcar, /flipvehicle");
		SendClientMessage(playerid, COLOR_WHITE, "Level 1: /go, /get, /goto, /spec, /slap, /listguns, /gotoplayervehicle, /serverstats, /closestcar, /namechanges, /getvehicle");

	    if(playerVariables[playerid][pAdminLevel] >= 2) {
	        SendClientMessage(playerid, COLOR_GREY, "Level 2: /enableooc, /disableooc, /prison, /jail, /release, /mute, /omute, /fine, /unfreeze, /freeze, /forcelogout");
	    }

	    if(playerVariables[playerid][pAdminLevel] >= 3) {
	        SendClientMessage(playerid, COLOR_WHITE, "Level 3: /unban, /unbanip, /veh, /despawnavehicles, /spawnweapon, /gotopoint, /setleader, /movehouse, /asellhouse");
	        SendClientMessage(playerid, COLOR_WHITE, "Level 3: /set, /hprice, /bprice, /vehname, /gunname, /explode, /gotohouse, /gotobusiness");
			SendClientMessage(playerid, COLOR_WHITE, "Level 3: /eventproperties, /startevent, /endevent, /setplayervehicle, /setweather, /vdespawn");
	    }

	    if(playerVariables[playerid][pAdminLevel] >= 4) {
	        SendClientMessage(playerid, COLOR_GREY, "Level 4: /setadminname, /createhouse, /createbusiness, /btype, /gtype");
			SendClientMessage(playerid, COLOR_GREY, "Level 4: /savevehicle, /vgroup, /vcolour, /vmove, /vmodel, /vmassrespawn, /changename");
	    }

	    if(playerVariables[playerid][pAdminLevel] >= 5) {
	        SendClientMessage(playerid, COLOR_WHITE, "Level 5: /savedata, /gmx, /sethelper, /listassets, /setnewbiespawn, /setadminlevel");
	    }

		SendClientMessage(playerid, COLOR_TEAL, "--------------------------------------------------------------------------------------------------------------------------------");
	}

	return 1;
}

CMD:ah(playerid, params[]) {
	return cmd_ahelp(playerid, params);
}

CMD:jail(playerid, params[]) 
{
    if(playerVariables[playerid][pAdminLevel] >= 2) 
	{
        new
            minutes,
            userID,
            reason[64];

        if(sscanf(params, "uds[64]", userID, minutes, reason)) 
		{
			SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/jail [playerid] [minutes] [reason]");
		}
		else if(playerVariables[playerid][pStatus] != 1)
		{
			SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
		}
		else 
		{
			if(minutes == 0) 
			{
	            GetPlayerName(userID, szPlayerName, MAX_PLAYER_NAME);

	            format(szMessage, sizeof(szMessage), "Release: %s has been released from prison by %s, reason: %s", szPlayerName, playerVariables[playerid][pAdminName], reason);
	            SendClientMessageToAll(COLOR_LIGHTRED, szMessage);

	            playerVariables[userID][pPrisonID] = 0;
	            playerVariables[userID][pPrisonTime] = 0;

	            SendClientMessage(userID, COLOR_WHITE, "Your time is up! You have been released from jail/prison.");
				SetPlayerPos(userID, 738.9963, -1417.2211, 13.5234);
				SetPlayerInterior(userID, 0);
				SetPlayerVirtualWorld(userID, 0);
			}

			if(playerVariables[playerid][pAdminLevel] >= playerVariables[userID][pAdminLevel])
			{
				GetPlayerName(userID, szPlayerName, MAX_PLAYER_NAME);
			    format(szMessage, sizeof(szMessage), "Jail: %s has been jailed by %s, reason: %s (%d minutes).", szPlayerName, playerVariables[playerid][pAdminName], reason, minutes);
				SendClientMessageToAll(COLOR_LIGHTRED, szMessage);

				playerVariables[userID][pPrisonTime] = minutes * 60;
				playerVariables[userID][pPrisonID] = 2;

				SetPlayerPos(userID, 264.58, 77.38, 1001.04);
				SetPlayerInterior(userID, 6);
				SetPlayerVirtualWorld(userID, 0);
			}
			else 
			{
				SendClientMessage(playerid, COLOR_GREY, "You can't jail a higher level administrator.");
			}
		}
	}
	return 1;
}

CMD:release(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 2) {
        new
            reason[64],
            targetid;

        if(sscanf(params, "us[64]", targetid, reason))
		{
            SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/release [playerid] [reason]");
        }
		else if(playerVariables[targetid][pStatus] != 1)
        {
			SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
		}
		else
		{
            GetPlayerName(targetid, szPlayerName, MAX_PLAYER_NAME);

            format(szMessage, sizeof(szMessage), "Release: %s has been released from prison by %s, reason: %s", szPlayerName, playerVariables[playerid][pAdminName], reason);
            SendClientMessageToAll(COLOR_LIGHTRED, szMessage);

            playerVariables[targetid][pPrisonID] = 0;
            playerVariables[targetid][pPrisonTime] = 0;

            SendClientMessage(targetid, COLOR_WHITE, "Your time is up! You have been released from jail/prison.");
			SetPlayerPos(targetid, 738.9963, -1417.2211, 13.5234);
			SetPlayerInterior(targetid, 0);
			SetPlayerVirtualWorld(targetid, 0);

			return 1;
		}
	}
	return 1;
}

CMD:prison(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 2) {
        new
            minutes,
            userID,
            reason[64];

        if(sscanf(params, "uds[64]", userID, minutes, reason)) {
			return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/prison [playerid] [minutes] [reason]");
		}
		else {
			if(!IsPlayerConnected(userID)) return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
			
			if(minutes == 0) {
	            GetPlayerName(userID, szPlayerName, MAX_PLAYER_NAME);

	            format(szMessage, sizeof(szMessage), "Release: %s has been released from prison by %s, reason: %s", szPlayerName, playerVariables[playerid][pAdminName], reason);
	            SendClientMessageToAll(COLOR_LIGHTRED, szMessage);

	            playerVariables[userID][pPrisonID] = 0;
	            playerVariables[userID][pPrisonTime] = 0;

	            SendClientMessage(userID, COLOR_WHITE, "Your time is up! You have been released from jail/prison.");
				SetPlayerPos(userID, 738.9963, -1417.2211, 13.5234);
				SetPlayerInterior(userID, 0);
				SetPlayerVirtualWorld(userID, 0);
			}
			
			if(playerVariables[playerid][pAdminLevel] >= playerVariables[userID][pAdminLevel]) {
				GetPlayerName(userID, szPlayerName, MAX_PLAYER_NAME);
			    format(szMessage, sizeof(szMessage), "Prison: %s has been prisoned by %s, reason: %s (%d minutes).", szPlayerName, playerVariables[playerid][pAdminName], reason, minutes);
				SendClientMessageToAll(COLOR_LIGHTRED, szMessage);

				playerVariables[userID][pPrisonTime] = minutes*60;
				playerVariables[userID][pPrisonID] = 1;

				SetPlayerPos(userID, -26.8721, 2320.9290, 24.3034);
				SetPlayerInterior(userID, 0);
				SetPlayerVirtualWorld(userID, 0);
			}
			else {
				return SendClientMessage(playerid, COLOR_GREY, "You can't prison a higher level administrator.");
			}
		}
	}
	return 1;
}

CMD:slap(playerid,params[])
{
	if(playerVariables[playerid][pAdminLevel] >= 1) 
	{
	    new
	        userID;

		if(sscanf(params, "u", userID))
			SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/slap [playerid]");
		else if(playerVariables[userID][pStatus] != 1)
			SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
	    else if(playerVariables[playerid][pAdminLevel] >= playerVariables[userID][pAdminLevel]) 
		{
            new
                string[64],

                Float: playerHealth,
                Float: fPos[3];

            GetPlayerName(userID, szPlayerName, MAX_PLAYER_NAME);

			GetPlayerPos(userID, fPos[0], fPos[1], fPos[2]);
			PlayerPlaySoundEx(1190, fPos[0], fPos[1], fPos[2]);
			SetPlayerPos(userID, fPos[0], fPos[1], fPos[2]+5);

			GetPlayerHealth(userID, playerHealth);
	    	SetPlayerHealth(userID, playerHealth-5);

		    format(string, sizeof(string), "You have slapped %s.", szPlayerName);
		    SendClientMessage(playerid, COLOR_WHITE, string);
		}
	}
	return 1;
}

CMD:check(playerid,params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 1) {

	    new
	        targetid;

		if(sscanf(params, "u", targetid))
			return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/check [playerid]");

		if(playerVariables[targetid][pStatus] != 1)
			return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");

		if(playerVariables[playerid][pAdminLevel] < playerVariables[targetid][pAdminLevel])
			return SendClientMessage(playerid, COLOR_GREY, "You can't check a higher level administrator.");

		showStats(playerid, targetid);
	}
	return 1;
}

CMD:veh(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 3) {
        if(!isnull(params)) {

			new
				carid = strval(params),
				Float: carSpawnPos[4], // 3 for the usual dimensions, +1 for the rotation/angle.
				messageString[64];

			if(carid < 400 || carid > 611)
				return SendClientMessage(playerid, COLOR_WHITE, "Valid car IDs start at 400, and end at 611.");

			if(systemVariables[vehicleCounts][0] + systemVariables[vehicleCounts][1] + systemVariables[vehicleCounts][2] < MAX_VEHICLES) {
				GetPlayerPos(playerid, carSpawnPos[0], carSpawnPos[1], carSpawnPos[2]);
				GetPlayerFacingAngle(playerid, carSpawnPos[3]);

				AdminSpawnedVehicles[vehCount] = CreateVehicle(carid, carSpawnPos[0], carSpawnPos[1], carSpawnPos[2], carSpawnPos[3], -1, -1, -1);
				systemVariables[vehicleCounts][2]++;

				LinkVehicleToInterior(AdminSpawnedVehicles[vehCount], GetPlayerInterior(playerid));
				SetVehicleVirtualWorld(AdminSpawnedVehicles[vehCount], GetPlayerVirtualWorld(playerid));

				PutPlayerInVehicle(playerid, AdminSpawnedVehicles[vehCount], 0);

				switch(carid) {
					case 427, 428, 432, 601, 528: SetVehicleHealth(AdminSpawnedVehicles[vehCount], 5000.0);
				}

				format(messageString, sizeof(messageString), "You have spawned a %s (vehicle ID %d).", VehicleNames[carid - 400], AdminSpawnedVehicles[vehCount]);
				SendClientMessage(playerid, COLOR_WHITE, messageString);

				vehCount++;
			}
			else {
				SendClientMessage(playerid, COLOR_GREY, "(error) 01x08");
				printf("ERROR: Vehicle limit reached (MODEL %d, MAXIMUM %d, TYPE ADMIN) [01x08]", carid, MAX_VEHICLES);
			}
        }
        else {
            return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/veh [vehicleid]");
        }
    }
	return 1;
}

CMD:despawnavehicles(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 3) {
        new
			x;

        for(new i = 0; i < MAX_VEHICLES; i++) {
			if(AdminSpawnedVehicles[i] >= 1) {
			    DestroyVehicle(AdminSpawnedVehicles[i]);
			    AdminSpawnedVehicles[i] = 0;
			    x++;
				systemVariables[vehicleCounts][2]--;
			}
		}

		format(szMessage, sizeof(szMessage), "%d admin-spawned vehicles have been automatically destroyed.", x);
		SendClientMessage(playerid, COLOR_WHITE, szMessage);
    }
	return 1;
}

CMD:vdespawn(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 3) {

        for(new i = 0; i < MAX_VEHICLES; i++) {
			if(AdminSpawnedVehicles[i] == GetPlayerVehicleID(playerid)) {
				format(szMessage, sizeof(szMessage), "You have successfully despawned vehicle %d.", AdminSpawnedVehicles[i]);
				DestroyVehicle(AdminSpawnedVehicles[i]);
				AdminSpawnedVehicles[i] = 0;
				systemVariables[vehicleCounts][2]--;
				SendClientMessage(playerid, COLOR_WHITE, szMessage);
				return 1;
			}
		}
		SendClientMessage(playerid, COLOR_WHITE, "You are not in an admin spawned vehicle.");
    }
	return 1;
}

CMD:serverstats(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 1) {

	    new
			statString[128];

		SendClientMessage(playerid, COLOR_TEAL, "----------------------------------------------------------------------------");
        SendClientMessage(playerid, COLOR_WHITE, "System variables (current):");
        format(statString, sizeof(statString), "Objects: %d | Pickups: %d | 3D Text Labels: %d | Static vehicles: %d | Player vehicles: %d | Admin vehicles: %d", CountDynamicObjects(), CountDynamicPickups(), CountDynamic3DTextLabels(), systemVariables[vehicleCounts][0], systemVariables[vehicleCounts][1], systemVariables[vehicleCounts][2]);
		SendClientMessage(playerid, COLOR_WHITE, statString);
        format(statString, sizeof(statString), "Houses: %d | Businesses: %d | Total vehicle count: %d/%d | Weather: %d | Pending weather change: %d/%d", systemVariables[houseCount], systemVariables[businessCount], systemVariables[vehicleCounts][0] + systemVariables[vehicleCounts][1] + systemVariables[vehicleCounts][2], MAX_VEHICLES, weatherVariables[0], weatherVariables[1], MAX_WEATHER_POINTS);
		SendClientMessage(playerid, COLOR_WHITE, statString);
		SendClientMessage(playerid, COLOR_TEAL, "----------------------------------------------------------------------------");
	}
	return 1;
}

CMD:setplayervehicle(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 3) {
		new
			string[64],
		    carModelID,
		    targetID;

		if(sscanf(params, "ud", targetID, carModelID)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/setplayervehicle [playerid] [model id]");
		if(targetID == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");

		if((carModelID < 400 || carModelID > 611) && carModelID != 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid model ID (valid IDs are between 400 and 611). Specify model 0 to delete a player vehicle.");

		GetPlayerName(targetID, szPlayerName, MAX_PLAYER_NAME);

		if(carModelID == 0) { // Basically, specifying 0 in the command will delete the vehicle (which was pretty useful in the past).
			if(playerVariables[targetID][pCarModel] >= 1) {
				DestroyPlayerVehicle(targetID);
				format(string, sizeof(string), "You have deleted %s (ID: %d)'s vehicle.", szPlayerName, targetID);
				SendClientMessage(playerid, COLOR_WHITE, string);
			}
			else return SendClientMessage(playerid, COLOR_GREY, "This player does not own a vehicle.");
		}
		else {

			DestroyPlayerVehicle(targetID);

			GetPlayerPos(playerid, playerVariables[targetID][pCarPos][0], playerVariables[targetID][pCarPos][1], playerVariables[targetID][pCarPos][2]);
			GetPlayerFacingAngle(playerid, playerVariables[targetID][pCarPos][3]);

			playerVariables[targetID][pCarModel] = carModelID;

			SpawnPlayerVehicle(targetID);
			format(string, sizeof(string), "You have set %s (ID: %d)'s vehicle to a %s.", szPlayerName, targetID, VehicleNames[playerVariables[targetID][pCarModel] - 400]);
			SendClientMessage(playerid, COLOR_WHITE, string);
		}
	}
	return 1;
}