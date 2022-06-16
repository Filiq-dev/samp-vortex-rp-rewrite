CMD:drop(playerid, params[]) {
	new
		string[64];

	if(GetPlayerWeapon(playerid) == 0 || playerVariables[playerid][pEvent] != 0) format(string, sizeof(string),"Materials (%d)\nPhone\nWalkie Talkie", playerVariables[playerid][pMaterials]);
	else format(string, sizeof(string),"Materials (%d)\nPhone\nWalkie Talkie\nCurrent weapon (%s)", playerVariables[playerid][pMaterials], WeaponNames[GetPlayerWeapon(playerid)]);

    ShowPlayerDialog(playerid, DIALOG_DROPITEM, DIALOG_STYLE_LIST, "Inventory", string, "Select", "Cancel");
	return 1;
}

CMD:connections(playerid, params[]) {
	format(szQueryOutput, sizeof(szQueryOutput), "SELECT playeraccounts.playerName, playerconnections.conTS FROM playerconnections INNER JOIN playeraccounts ON playerconnections.conPlayerID = playeraccounts.playerID WHERE playeraccounts.playerID = '%d' LIMIT 5", playerVariables[playerid][pInternalID]);
	mysql_query(szQueryOutput, THREAD_LAST_CONNECTIONS, playerid);
	return 1;
} 

CMD:o(playerid, params[]) {
	return cmd_ooc(playerid, params);
}

CMD:ooc(playerid, params[]) {
    if(systemVariables[OOCStatus] == 1)
		return SendClientMessage(playerid, COLOR_GREY, "The OOC chat channel is currently disabled.");

	if(playerVariables[playerid][pOOCMuted] == 1)
		return SendClientMessage(playerid, COLOR_GREY, "You have been muted from the OOC chat channel.");

    if(!isnull(params)) {
    	new
			playerName2[MAX_PLAYER_NAME];

	    GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);

	    format(szMessage, sizeof(szMessage), "(( %s says: %s ))", szPlayerName, params);

		foreach(Player, x) {
			if(playerVariables[x][pSeeOOC] == 1) {
			    GetPlayerName(x, playerName2, MAX_PLAYER_NAME);
			    if(strfind(szMessage, playerName2, true) == -1) {
			        format(szMessage, sizeof(szMessage), "(( %s says: %s ))", szPlayerName, params);
	  				SendClientMessage(x, COLOR_LIGHT, szMessage);
  				} else {
  				    if(strfind(playerName2, szPlayerName, true) != -1) {
				        format(szMessage, sizeof(szMessage), "(( %s says: %s ))", szPlayerName, params);
		  				SendClientMessage(x, COLOR_LIGHT, szMessage);
  				    } else {
						format(szMessage, sizeof(szMessage), "(( %s says: "EMBED_LIGHTRED"%s "EMBED_OOC"))", szPlayerName, params);
		  				SendClientMessage(x, COLOR_LIGHT, szMessage);
		  				PlayerPlaySound(x, 1057, 0, 0, 0);
	  				}
  				}
			}
		}
		
		#if !defined NO_IRC
			format(szMessage, sizeof(szMessage), "%s says [in-game]: %s", szPlayerName, params);
			IRC_Say(scriptBots[0], IRC_CHANNEL_MAIN, szMessage);
		#endif
		return 1;
	}
	else
		return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/(o)oc [message]");
}

CMD:seeooc(playerid, params[]) 
{
    if(playerVariables[playerid][pStatus] == 1) 
	{
		if(playerVariables[playerid][pSeeOOC] == 1) {
		    playerVariables[playerid][pSeeOOC] = 0;
		    SendClientMessage(playerid, COLOR_WHITE, "You will no longer see any chat submitted to the public OOC channel.");
		}
		else {
		    playerVariables[playerid][pSeeOOC] = 1;
		    SendClientMessage(playerid, COLOR_WHITE, "You will now see any chat submitted to the public OOC channel.");
		}
	}
	return 1;
}  


CMD:number(playerid, params[]) {
	if(playerVariables[playerid][pPhoneBook] >= 1) {
	    new
	        userID;

		if(sscanf(params, "u", userID)) {
		    return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/number [playerid]");
		}
		else {
		    if(!IsPlayerConnected(userID)) {
		        return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
		    }
		    else {
				GetPlayerName(userID, szPlayerName, MAX_PLAYER_NAME);

				if(playerVariables[userID][pPhoneNumber] == -1) {
				    format(szMessage, sizeof(szMessage), "Name: "EMBED_GREY"%s{FFFFFF} | Number: "EMBED_GREY"None", szPlayerName);
				    SendClientMessage(playerid, COLOR_WHITE, szMessage);
				}
				else {
					format(szMessage, sizeof(szMessage), "Name: "EMBED_GREY"%s{FFFFFF} | Number: "EMBED_GREY"%d", szPlayerName, playerVariables[userID][pPhoneNumber]);
					SendClientMessage(playerid, COLOR_WHITE, szMessage);
				}
			}
		}
	}
	else SendClientMessage(playerid, COLOR_GREY, "You don't have a phonebook.");
	return 1;
} 

CMD:help(playerid, params[]) {
	return showHelp(playerid);
} 

CMD:statistics(playerid, params[]) {
	return showStats(playerid,playerid);
}   

CMD:stats(playerid, params[]) {
	return cmd_statistics(playerid, params);
} 

CMD:pay(playerid, params[]) {
	new
		id,
		cash,
		string[128],
		ip1[32],
		ip2[32],
		giveplayerName[MAX_PLAYER_NAME];

	if(sscanf(params, "ud", id, cash))
		return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/pay [playerid] [amount]");

	if(playerVariables[playerid][pMoney] >= cash) {
		if(id != playerid && IsPlayerAuthed(id)) {
			if(cash > 0 && ((playerVariables[playerid][pPlayingHours] < 10 && cash < 5000) || playerVariables[playerid][pPlayingHours] >= 10)) {
				if(playerVariables[playerid][pAdminDuty] != 0 && playerVariables[playerid][pAdminLevel] > 0 || (IsPlayerInRangeOfPlayer(playerid, id, 4.0) && playerVariables[id][pSpectating] == INVALID_PLAYER_ID)) {

					GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
					GetPlayerName(id, giveplayerName, MAX_PLAYER_NAME);

					playerVariables[playerid][pMoney] -= cash;
					playerVariables[id][pMoney] += cash;

					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					PlayerPlaySound(id, 1052, 0.0, 0.0, 0.0);

					format(string, sizeof(string), "You have paid $%d to %s.", cash, giveplayerName);
					SendClientMessage(playerid, COLOR_WHITE, string);

					format(string, sizeof(string), "* %s takes out $%d in cash, and hands it to %s.", szPlayerName, cash, giveplayerName);
					nearByMessage(playerid, COLOR_PURPLE, string);

					GetPlayerIp(playerid, ip1, 32);
					GetPlayerIp(id, ip2, 32);

					format(string, sizeof(string), "%s has paid you $%d.", szPlayerName, cash);
					SendClientMessage(id, COLOR_WHITE, string);

					if(playerVariables[playerid][pSpamCount] >= 3)
					{
						if(!strcmp(ip1, ip2,true))
						{
							format(string, sizeof(string), "Warning: {FFFFFF}%s has attempted to repeatedly pay $%d to %s (sharing IP address %s).", szPlayerName, cash, giveplayerName, ip1);
							submitToAdmins(string, COLOR_HOTORANGE);
						}
						else if(playerVariables[playerid][pPlayingHours] < 2) {
							format(string, sizeof(string), "Warning: {FFFFFF}%s has attempted to repeatedly pay $%d to %s (with less than two playing hours).", szPlayerName, cash, giveplayerName);
							submitToAdmins(string, COLOR_HOTORANGE);
						}
					}
				}
				else SendClientMessage(playerid, COLOR_GREY, "You're too far away from this person.");
			}
			else SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified (too high, or too low).");
		}
		else SendClientMessage(playerid, COLOR_GREY, "Invalid player specified (either yourself, or not connected).");
	}
	return 1;
} 

CMD:mobile(playerid, params[]) {
    if(playerVariables[playerid][pPhoneNumber] == -1)
        return SendClientMessage(playerid, COLOR_GREY, "You do not have a mobile phone.");

    if(playerVariables[playerid][pPhoneStatus] != 1)
        return SendClientMessage(playerid, COLOR_GREY, "Your phone is not switched on.");

	ShowPlayerDialog(playerid, DIALOG_PHONE_MENU, DIALOG_STYLE_LIST, "Mobile Phone: Menu", "History\nContacts\nWidgets\nOrganiser\nMessaging\nApplications\nSettings", "Select", "Cancel");
	return 1;
}

CMD:sms(playerid, params[]) {
	new
	    number,
	    szQuery[256],
	    szClearMsg[94],
	    count,
	    message[94];

    if(sscanf(params, "ds[94]", number, message))
		return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/sms [number] [message]");

    if(number == -1)
		return SendClientMessage(playerid, COLOR_GREY, "Invalid number.");

    if(playerVariables[playerid][pPhoneStatus] != 1)
        return SendClientMessage(playerid, COLOR_GREY, "Your phone is not switched on.");

	if(playerVariables[playerid][pPhoneCredit] < 1)
		return SendClientMessage(playerid, COLOR_GREY, "You have no remaining phone credit - visit a 24/7 to top it up.");

    foreach(Player, x) {
		if(playerVariables[x][pPhoneNumber] == number) {
		    if(playerVariables[x][pPhoneStatus] == 1 && playerVariables[x][pPrisonID] != 3) {
		        GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);

		        format(szMessage, sizeof(szMessage), "SMS from %s (%d): %s", szPlayerName, playerVariables[playerid][pPhoneNumber], message);
		        SendClientMessage(x, COLOR_SMS, szMessage);

		        GetPlayerName(x, szPlayerName, MAX_PLAYER_NAME);

		        format(szMessage, sizeof(szMessage), "SMS sent to %s (%d): %s", szPlayerName, playerVariables[x][pPhoneNumber], message);
		        SendClientMessage(playerid, COLOR_SMS, szMessage);
				playerVariables[playerid][pPhoneCredit] -= 3;

				mysql_real_escape_string(message, szClearMsg);
				format(szQuery, sizeof(szQuery), "INSERT INTO `phonelogs` (`phoneNumber`, `phoneAction`) VALUES('%d', 'SMS to %s: %s')", playerVariables[playerid][pPhoneNumber], szPlayerName, message);
				mysql_query(szQuery);
		        return 1;
		    }
		    else {
				return SendClientMessage(playerid, COLOR_GREY, "The cellphone that you're trying to SMS is currently unavailable.");
			}
		}
		count++;
	}

	if(count < 1) return SendClientMessage(playerid, COLOR_GREY, "Invalid number.");
	return 1;
}

CMD:t(playerid, params[]) {
	return cmd_sms(playerid, params);
}

CMD:call(playerid, params[]) {
	new
		number,

		string[128];

 	if(isnull(params))
		return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/call [number]");

 	number = strval(params);

	if(playerVariables[playerid][pPhoneNumber] == -1) {
		SendClientMessage(playerid, COLOR_GREY, "You don't have a phone.");
	}
	else if(playerVariables[playerid][pPhoneNumber] == number) {
		SendClientMessage(playerid, COLOR_GREY, "You're trying to call yourself.");
	}
	else {
		if(playerVariables[playerid][pPhoneStatus] == 1) {
			if(playerVariables[playerid][pPhoneCredit] >= 1) {
				if(playerVariables[playerid][pPhoneCall] == -1) {
					if(number == 911) {
						SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
						playerVariables[playerid][pPhoneCall] = 911;
						SendClientMessage(playerid, COLOR_WHITE, "You've called Emergency services, please select the department you desire (i.e: LSPD, LSFMD).");
					}
					else if(number != -1) {
						GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
						format(string, sizeof(string), "* %s takes out their cellphone, and dials in a number.", szPlayerName, number);
						SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
						nearByMessage(playerid, COLOR_PURPLE, string);

						foreach(Player, i) {
							if(playerVariables[i][pPhoneNumber] == number)  {
								if(playerVariables[i][pStatus] == 1 && playerVariables[i][pSpectating] == INVALID_PLAYER_ID && playerVariables[i][pPhoneStatus] == 1 && playerVariables[i][pPhoneCall] == -1 && playerVariables[i][pPrisonID] != 3) {

									GetPlayerName(i, szPlayerName, MAX_PLAYER_NAME);
									format(string, sizeof(string), "* %s's cellphone starts to ring...", szPlayerName);
									nearByMessage(i, COLOR_PURPLE, string);
									SendClientMessage(i, COLOR_WHITE, "Use /p(ickup) to answer your phone.");

									SendClientMessage(playerid, COLOR_WHITE, "You can use the 'T' chat to proceed to talk.");
									playerVariables[playerid][pPhoneCall] = i;
									return 1;
								}
								else {
									SendClientMessage(playerid, COLOR_GREY, "(cellphone) *busy tone*");
									return 1;
								}
							}
						}
						if(playerVariables[playerid][pPhoneCall] == -1) SendClientMessage(playerid, COLOR_GREY, "(cellphone) *busy tone*");
					}
					else SendClientMessage(playerid, COLOR_GREY, "Invalid number.");
				}
				else SendClientMessage(playerid, COLOR_GREY, "You are currently in a call.");
			}
			else SendClientMessage(playerid, COLOR_GREY, "You have no remaining phone credit - visit a 24/7 to top it up.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You must switch your phone on first (/togphone).");
	}
	return 1;
}

CMD:p(playerid) return cmd_pickup(playerid);
CMD:pickup(playerid) 
{
	foreach(Player, i) 
	{
		// Setting the current-call var to the ID of the person calling.
		if(playerVariables[i][pPhoneCall] == playerid) 
		{
			playerVariables[playerid][pPhoneCall] = i;
			SendClientMessage(playerid, COLOR_WHITE, "You have answered your phone.");
			SendClientMessage(playerVariables[playerid][pPhoneCall], COLOR_WHITE, "The other person has answered the call.");
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
		}
	}
	return 1;
}

CMD:togphone(playerid, params[]) 
{
	if(playerVariables[playerid][pPhoneNumber] == -1)
	{
		SendClientMessage(playerid, COLOR_GREY, "You don't have a phone.");
	}
	else if(playerVariables[playerid][pPhoneStatus] == 1)
	{
		playerVariables[playerid][pPhoneStatus] = 0;
		SendClientMessage(playerid, COLOR_WHITE, "Your phone is now switched off.");
	}
	else if(playerVariables[playerid][pPhoneStatus] == 0)
	{
		playerVariables[playerid][pPhoneStatus] = 1;
		SendClientMessage(playerid, COLOR_WHITE, "Your phone is now switched on.");
	}
	return 1;
}

CMD:h(playerid) return cmd_hangup(playerid);
CMD:hangup(playerid) 
{
	if(playerVariables[playerid][pPhoneCall] != -1)
		SendClientMessage(playerid, COLOR_WHITE, "You have terminated the current call.");

	if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USECELLPHONE)
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);

	temp = playerVariables[playerid][pPhoneCall];
	if(-1 < temp < MAX_PLAYERS) // Valid values: 0 - MAX_PLAYERS (911 and such are used for special calls)
	{
		SendClientMessage(temp, COLOR_WHITE, "Your call has been terminated by the other party.");
		
		if(GetPlayerSpecialAction(temp) == SPECIAL_ACTION_USECELLPHONE)
			SetPlayerSpecialAction(temp, SPECIAL_ACTION_STOPUSECELLPHONE);
			
		playerVariables[temp][pPhoneCall] = -1;
	}
	
	playerVariables[playerid][pPhoneCall] = -1;
	return 1;
}

CMD:shakehand(playerid, params[]) {
	new
	    style,
		id;

    if(GetPlayerState(playerid) != 1)
		return SendClientMessage(playerid, COLOR_GREY, "You can only shake hands while on foot.");
		
    if(playerVariables[playerid][pFreezeTime] != 0)
		return SendClientMessage(playerid, COLOR_GREY, "You can't shake hands while cuffed, tazed, or frozen.");
		
	if(sscanf(params, "ud", id, style))
		return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/shakehand [playerid] [1-9]");
		
 	if(id != INVALID_PLAYER_ID) {
   		if(IsPlayerInRangeOfPlayer(playerid, id, 1.5)) {
     		if(style > 0 && style < 9) {
              	GetPlayerName(id, szPlayerName, MAX_PLAYER_NAME);

	            SetPVarInt(id,"hs",style); // DYNAMICALLY ALLOCATED MEMORY!!11. Nah, this won't be accessed regularly
				SetPVarInt(id,"hsID",playerid); // and won't stay in memory for very long.

	            format(szMessage, sizeof(szMessage), "You have requested to shake hands with %s.", szPlayerName);
	            SendClientMessage(playerid, COLOR_WHITE, szMessage);

           		GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
	            format(szMessage, sizeof(szMessage), "%s is requesting to shake hands with you - type /accept handshake to do so.", szPlayerName);
	            SendClientMessage(id, COLOR_NICESKY, szMessage);
	        }
         	else {
           		SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/shakehand [playerid] [1-8]");
           	}
       	}
       	else {
       		SendClientMessage(playerid, COLOR_GREY, "Please stand closer to them.");
	    }
  	}
    else {
	    SendClientMessage(playerid, COLOR_GREY, "That player is not connected or is not logged in.");
    }
	return 1;
}

CMD:time(playerid, params[]) {
	new
	    time[3];

	gettime(time[0], time[1], time[2]);

	if(time[1] < 10) format(szMessage, sizeof(szMessage), "The current time is %d:0%d (%d seconds).", time[0], time[1], time[2]);
	else format(szMessage, sizeof(szMessage), "The current time is %d:%d (%d seconds).", time[0], time[1], time[2]);

	SendClientMessage(playerid, COLOR_WHITE, szMessage);

	format(szMessage, sizeof(szMessage), "Your next paycheck is due in %d minutes (%d seconds).", (3600-playerVariables[playerid][pSeconds])/60, (3600-playerVariables[playerid][pSeconds]));
	SendClientMessage(playerid, COLOR_WHITE, szMessage);

	if(playerVariables[playerid][pDropCarTimeout] >= 1) {
		format(szMessage, sizeof(szMessage), "You will be able to drop vehicles at the crane again in %d seconds (%d minutes).", playerVariables[playerid][pDropCarTimeout], playerVariables[playerid][pDropCarTimeout]/60);
		SendClientMessage(playerid, COLOR_WHITE, szMessage);
	}
	return 1;
}


CMD:wt(playerid, params[]) {
    if(isnull(params))
        return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/wt [message]");

	else if(playerVariables[playerid][pFreezeType] > 0) {
		return SendClientMessage(playerid, COLOR_GREY, "You can't use this command while cuffed, tazed, or frozen.");
	}
    else switch(playerVariables[playerid][pWalkieTalkie]) {
		case -1: SendClientMessage(playerid, COLOR_GREY, "You don't have a walkie talkie.");
		case 0: SendClientMessage(playerid, COLOR_GREY, "You need to set a broadcast frequency first (using /setfrequency).");
		default: {
			GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
			format(szMessage, sizeof(szMessage), "(Walkie Talkie) %s: %s", szPlayerName, params);
			SendToFrequency(playerVariables[playerid][pWalkieTalkie], COLOR_SMS, szMessage);
			format(szMessage ,sizeof(szMessage),"(radio) ''%s''", params);
			SetPlayerChatBubble(playerid, szMessage, COLOR_CHATBUBBLE, 10.0, 10000);
	    }
	}
	return 1;
}

CMD:setfrequency(playerid, params[]) {
	if(isnull(params))
		return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/setfrequency [frequency] (0 to switch off).");

	else if(playerVariables[playerid][pWalkieTalkie] == -1) {
		return SendClientMessage(playerid, COLOR_GREY, "You don't have a walkie talkie.");
	}

	new
		walkieFreq = strval(params);

	if(walkieFreq < 0)
		return SendClientMessage(playerid, COLOR_GREY, "Invalid frequency specified.");

	else switch(walkieFreq) {
		case 0: {
			SendClientMessage(playerid, COLOR_GREY, "You have switched off your walkie talkie.");
			playerVariables[playerid][pWalkieTalkie] = 0;
		}
		default: {
			format(szMessage, sizeof(szMessage), "You are now broadcasting at the frequency of #%d khz.", walkieFreq);
			SendClientMessage(playerid, COLOR_WHITE, szMessage);
			playerVariables[playerid][pWalkieTalkie] = walkieFreq;
		}
	}
	return 1;
} 

CMD:ad(playerid, params[]) {
	if(!isnull(params)) {
		if(playerVariables[playerid][pPhoneNumber] != -1) {
		    if(adTick == 0) {
				if(playerVariables[playerid][pMoney] >= 1000) {
					new
						adText[128],
						queryString[255];

					mysql_real_escape_string(params, adText);
					format(queryString, sizeof(queryString), "INSERT INTO playeradvertisements (playerID, advertisementText, Time2) VALUES('%d', '%s', '%d')", playerVariables[playerid][pInternalID], adText, gettime());
					mysql_query(queryString);

					GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
					format(queryString, sizeof(queryString), "Advertisement: %s (by %s).", params, szPlayerName);
					SendClientMessageToAll(COLOR_GREEN, queryString);
					playerVariables[playerid][pMoney] -= 1000;
					adTick = 60;
				}
				else SendClientMessage(playerid, COLOR_GREY, "You don't have enough money for this.");
		    }
		    else {
				return SendClientMessage(playerid, COLOR_GREY, "You must wait 60 seconds to post a global advertisement.");
			}
		}
		else {
			return SendClientMessage(playerid, COLOR_GREY, "You don't have a phone, so you're unable to submit an advertisement.");
		}
	}
	else {
		return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/ad [advertisement text]");
	}
	return 1;
}

CMD:killcheckpoint(playerid, params[]) {
	DisablePlayerCheckpoint(playerid);
	playerVariables[playerid][pCheckpoint] = 0;
	playerVariables[playerid][pBackup] = -1;
	SendClientMessage(playerid, COLOR_WHITE,"You have disabled your current checkpoint.");
	return 1;
}

CMD:eject(playerid, params[]) {
	new
		targetID;

	if(sscanf(params, "u", targetID))
		return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/eject [playerid]");

	if(GetPlayerState(playerid) == 2) {
		if(GetPlayerVehicleID(playerid) == GetPlayerVehicleID(targetID)) {

			new
				string[128],
				playerName[2][MAX_PLAYER_NAME];

			GetPlayerName(playerid, playerName[0], MAX_PLAYER_NAME);
			GetPlayerName(targetID, playerName[1], MAX_PLAYER_NAME);

			format(string, sizeof(string), "* %s has thrown %s out of their vehicle.", playerName[0], playerName[1]);
			nearByMessage(playerid, COLOR_PURPLE, string);

			RemovePlayerFromVehicle(targetID);
		}
		else SendClientMessage(playerid, COLOR_GREY, "That person is not in your vehicle.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You're not driving a vehicle.");
	return 1;
}

CMD:detain(playerid, params[]) {
	new
		seat,
		targetID;

	if(sscanf(params, "ud", targetID, seat))
		return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/detain [playerid] [seat (1-3)]");

	if(playerVariables[targetID][pFreezeType] == 2 || playerVariables[targetID][pFreezeType] == 4) {
		if(seat > 0 && seat < 4) {
			if(IsPlayerInRangeOfPlayer(playerid, targetID, 5.0) && IsPlayerInRangeOfVehicle(playerid, GetClosestVehicle(playerid), 5.0)) {

				new
					detaintarget = GetClosestVehicle(playerid);

				if(checkVehicleSeat(detaintarget, seat) != 0) SendClientMessage(playerid, COLOR_GREY, "That seat ID is occupied.");

				else {

					new
						playerName[2][MAX_PLAYER_NAME],
						string[92];

					GetPlayerName(playerid, playerName[0], MAX_PLAYER_NAME);
					GetPlayerName(targetID, playerName[1], MAX_PLAYER_NAME);

					format(string, sizeof(string), "* %s has been detained into the vehicle by %s.", playerName[1], playerName[0]);
					nearByMessage(playerid, COLOR_PURPLE, string);

					PutPlayerInVehicle(targetID, detaintarget, seat);
				}
			}
			else SendClientMessage(playerid, COLOR_GREY, "You must be closer to the player you wish to detain, and the vehicle you wish to detain into.");
	    }
	}
	return 1;
}

CMD:drag(playerid, params[]) {

	new
		targetID,
		string[99],
		playerName[2][MAX_PLAYER_NAME];

	foreach(Player, x) {
		if(playerVariables[x][pDrag] == playerid) {

			GetPlayerName(playerid, playerName[0], MAX_PLAYER_NAME);
			GetPlayerName(x, playerName[1], MAX_PLAYER_NAME);

			playerVariables[x][pDrag] = -1;

			format(string, sizeof(string), "You have stopped dragging %s.", playerName[1]);
			SendClientMessage(playerid, COLOR_WHITE, string);

			format(string, sizeof(string), "* %s has stopped dragging %s, releasing their grip.", playerName[0], playerName[1]);

			return nearByMessage(playerid, COLOR_PURPLE, string);
		}
	}

	if(sscanf(params, "u", targetID))
		SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/drag [playerid] (use again to stop).");

	else if(playerVariables[targetID][pFreezeType] == 2 || playerVariables[targetID][pFreezeType] == 4) {
		if(IsPlayerInRangeOfPlayer(playerid, targetID, 2.0)) {
			if(!IsPlayerInAnyVehicle(targetID) && !IsPlayerInAnyVehicle(playerid)) {

				GetPlayerName(playerid, playerName[0], MAX_PLAYER_NAME);
				GetPlayerName(targetID, playerName[1], MAX_PLAYER_NAME);

				playerVariables[targetID][pDrag] = playerid;
				format(string, sizeof(string), "You are now dragging %s.", playerName[1]);
				SendClientMessage(playerid, COLOR_WHITE, string);

				format(string, sizeof(string), "* %s grabs %s by the arm, and starts dragging them.", playerName[0], playerName[1]);
				nearByMessage(playerid, COLOR_PURPLE, string);
			}
			else SendClientMessage(playerid, COLOR_GREY, "Neither you, nor the person you wish to drag, can be in a vehicle.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You're too far away.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "The person you wish to drag must be restrained first (cuffed, or tied).");
	return 1;
}


CMD:deposit(playerid, params[]) {

	new
		cash,
		string[128];

	if(sscanf(params, "d", cash)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/deposit [amount]");
	else if(cash <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
	else {
		if(IsPlayerInRangeOfPoint(playerid, 15.0, 2306.8481,-16.0682,26.7496) && GetPlayerVirtualWorld(playerid) == 2) {
			if(playerVariables[playerid][pMoney] < cash) SendClientMessage(playerid, COLOR_GREY, "You don't have enough money for this transaction.");
			else if(cash >= 1) {
				playerVariables[playerid][pBankMoney] += cash;
				playerVariables[playerid][pMoney] -= cash;
				format(string, sizeof(string), "You have deposited $%d into your bank account. Your account balance is now $%d.", cash, playerVariables[playerid][pBankMoney]);
				SendClientMessage(playerid, COLOR_DCHAT, string);
			}
			else SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
		}
		else {
			SendClientMessage(playerid, COLOR_GREY, "You're not at the bank.");
		}
	}
	return 1;
}

CMD:wiretransfer(playerid, params[]) {

	new
		cash,
		targetID,
		string[128];

	if(sscanf(params, "ud", targetID, cash)) SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/wiretransfer [playerid] [amount]");
	else if(cash <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
	else if(IsPlayerInRangeOfPoint(playerid, 15.0, 2306.8481,-16.0682,26.7496) && GetPlayerVirtualWorld(playerid) == 2) {
	    if(suspensionCheck(playerid) == 1)
	        return 1;

		if(playerVariables[playerid][pPlayingHours] >= 10) {
			if(IsPlayerAuthed(targetID)) {
				if(playerVariables[playerid][pBankMoney] >= cash) {
					if(cash >= 1) {

						playerVariables[playerid][pBankMoney] -= cash;
						playerVariables[targetID][pBankMoney] += cash;

						GetPlayerName(targetID, szPlayerName, MAX_PLAYER_NAME);
						format(string, sizeof(string), "You have transferred $%d into %s's account. Your account balance is now $%d.", cash, szPlayerName, playerVariables[playerid][pBankMoney]);
						SendClientMessage(playerid, COLOR_DCHAT, string);

						GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
						format(string, sizeof(string), "%s has transferred $%d into your account. Your account balance is now $%d.", szPlayerName, cash, playerVariables[playerid][pBankMoney]);
						SendClientMessage(targetID, COLOR_DCHAT, string);
					}
					else SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
				}
				else SendClientMessage(playerid, COLOR_GREY, "Your account balance is insufficient for this transaction.");
			}
			else SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You must have at least ten playing hours to use this command.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You're not at the bank.");
	return 1;
}

CMD:withdraw(playerid, params[]) {

	new
		cash,
		string[128];

	if(sscanf(params, "d", cash)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/withdraw [amount]");
	else if(cash <= 0) return SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
	else {
		if(IsPlayerInRangeOfPoint(playerid, 15.0, 2306.8481,-16.0682,26.7496) && GetPlayerVirtualWorld(playerid) == 2) {
	    	if(suspensionCheck(playerid) == 1)
	        	return 1;

			if(playerVariables[playerid][pBankMoney] < cash) SendClientMessage(playerid, COLOR_GREY, "Your account balance is insufficient for this transaction.");
			else if(cash >= 1) {
				playerVariables[playerid][pMoney] += cash;
				playerVariables[playerid][pBankMoney] -= cash;
				format(string, sizeof(string), "You have withdrawn $%d from your bank account. Your account balance is now $%d.", cash, playerVariables[playerid][pBankMoney]);
				SendClientMessage(playerid, COLOR_DCHAT, string);
			}
			else SendClientMessage(playerid, COLOR_GREY, "Invalid amount specified.");
		}
		else {
			SendClientMessage(playerid, COLOR_GREY, "You're not at the bank.");
		}
	}
	return 1;
}

CMD:balance(playerid, params[]) {
    if(IsPlayerInRangeOfPoint(playerid, 15.0, 2306.8481,-16.0682,26.7496) && GetPlayerVirtualWorld(playerid) == 2) {
	    if(suspensionCheck(playerid) == 1)
	        return 1;

        format(szMessage, sizeof(szMessage), "Your current bank account balance is: $%d", playerVariables[playerid][pBankMoney]);
        SendClientMessage(playerid, COLOR_DCHAT, szMessage);
    } else SendClientMessage(playerid, COLOR_GREY, "You're not at the bank.");
	return 1;
}

CMD:kill(playerid, params[]) {
    if(playerVariables[playerid][pEvent] != 0) {
		return SendClientMessage(playerid, COLOR_GREY, "You can't use this command while in an event.");
	}
    else if(playerVariables[playerid][pFreezeType] != 0) {
		return SendClientMessage(playerid, COLOR_GREY, "You can't use this command while cuffed, tazed, or frozen.");
	}
    else {
		return SetPlayerHealth(playerid, -1);
	}
}

CMD:untie(playerid, params[]) {
	if(sscanf(params, "u", iTarget)) {
    	return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/untie [playerid]");
    }
    else {
		if(iTarget == INVALID_PLAYER_ID)
			return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");

		if(iTarget == playerid)
			return SendClientMessage(playerid, COLOR_GREY, "You can't untie yourself.");

		if(IsPlayerInRangeOfPlayer(playerid, iTarget, 2.0)) {
			new
				playerName[2][MAX_PLAYER_NAME];

			GetPlayerName(playerid, playerName[0], MAX_PLAYER_NAME);
			GetPlayerName(iTarget, playerName[1], MAX_PLAYER_NAME);

			if(random(6) < 3) {
				if(playerVariables[iTarget][pFreezeType] != 4) {
					return SendClientMessage(playerid, COLOR_GREY, "This player is not tied.");
				}
				else {
					format(szMessage, sizeof(szMessage), "* %s has attempted to untie %s and has succeeded.", playerName[0], playerName[1]);
					nearByMessage(playerid, COLOR_PURPLE, szMessage);

					playerVariables[iTarget][pFreezeType] = 0;
					playerVariables[iTarget][pFreezeTime] = 0;

					TogglePlayerControllable(iTarget, true);

					return SendClientMessage(playerid, COLOR_WHITE, "Attempt successful!");
				}
			}
			else {
				format(szMessage, sizeof(szMessage), "* %s has attempted to untie %s and has failed.", playerName[0], playerName[1]);
				nearByMessage(playerid, COLOR_PURPLE, szMessage);

				return SendClientMessage(playerid, COLOR_GREY, "Attempt failed!");
			}
		}
		else SendClientMessage(playerid, COLOR_GREY, "You're too far away.");
	}
	return 1;
}

CMD:tie(playerid, params[]) {
	new
	    targetID;

	if(sscanf(params, "u", targetID)) { // Using sscanf instead of isnull because we're handling a playerid/name.
    	return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/tie [playerid]");
    }
    else {
		if(targetID == INVALID_PLAYER_ID)
			return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
			
		if(targetID == playerid)
			return SendClientMessage(playerid, COLOR_GREY, "You can't tie yourself.");
			
		if(IsPlayerInRangeOfPlayer(playerid, targetID, 2.0)) {

			new
				playerName[2][MAX_PLAYER_NAME],
				msgSz[128];

			GetPlayerName(playerid, playerName[0], MAX_PLAYER_NAME);
			GetPlayerName(targetID, playerName[1], MAX_PLAYER_NAME);

			if(playerVariables[playerid][pRope] >= 1) {
				if(random(6) < 3) {
					if(playerVariables[targetID][pFreezeType] > 0 && playerVariables[targetID][pFreezeType] < 5) {
						return SendClientMessage(playerid, COLOR_GREY, "Attempt failed: player is already frozen.");
					}
					else {
						playerVariables[playerid][pRope]--;

						format(msgSz, sizeof(msgSz), "* %s has attempted to tie %s and has succeeded.", playerName[0], playerName[1]);
						nearByMessage(playerid, COLOR_PURPLE, msgSz);

						TogglePlayerControllable(targetID, false);

						playerVariables[targetID][pFreezeType] = 4;
						playerVariables[targetID][pFreezeTime] = 180;

						return SendClientMessage(playerid, COLOR_WHITE, "Attempt successful!");
					}
				}
				else {
					format(msgSz, sizeof(msgSz), "* %s has attempted to tie %s and has failed.", playerName[0], playerName[1]);
					nearByMessage(playerid, COLOR_PURPLE, msgSz);

					return SendClientMessage(playerid, COLOR_GREY, "Attempt failed!");
				}
			}
			else {
				return SendClientMessage(playerid, COLOR_GREY, "You don't have any rope.");
			}
		}
		else SendClientMessage(playerid, COLOR_GREY, "You're too far away.");
	}
	return 1;
}



CMD:admins(playerid, params[]) {
    SendClientMessage(playerid, COLOR_TEAL, "----------------------------------------------------------------------------");

	foreach(Player, x) {
		if(playerVariables[x][pAdminLevel] >= 1 && playerVariables[x][pAdminDuty] >= 1) {
			format(szMessage, sizeof(szMessage), "Administrator %s is on duty (level %d).", playerVariables[x][pAdminName], playerVariables[x][pAdminLevel]);
			SendClientMessage(playerid, COLOR_GREEN, szMessage);
		}
		if(playerVariables[x][pAdminLevel] >= 1 && playerVariables[playerid][pAdminLevel] >= 1 && playerVariables[x][pAdminDuty] < 1) {
			format(szMessage, sizeof(szMessage), "Administrator %s (%s) is off duty (level %d).", playerVariables[x][pAdminName], playerVariables[x][pNormalName], playerVariables[x][pAdminLevel]);
			SendClientMessage(playerid, COLOR_GREY, szMessage);
		}
	}

    SendClientMessage(playerid, COLOR_TEAL, "----------------------------------------------------------------------------");
	return 1;
}

CMD:give(playerid, params[]) {
	new
	    giveSz[12],
		amount,
		targetID;

	if(sscanf(params, "us[12]d", targetID, giveSz, amount)) {
	    SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/give [playerid] [item] [amount]");
	    return SendClientMessage(playerid, COLOR_GREY, "Items: Materials");
	}
	else {
	    if(targetID == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
	    if(!IsPlayerInRangeOfPlayer(playerid, targetID, 5.0)) return SendClientMessage(playerid, COLOR_GREY, "You're too far away.");

	    if(strcmp(giveSz, "materials", true) == 0) {
	        new
	            playerName[2][MAX_PLAYER_NAME];

	        GetPlayerName(playerid, playerName[0], MAX_PLAYER_NAME);
	        GetPlayerName(targetID, playerName[1], MAX_PLAYER_NAME);

			if(playerVariables[playerid][pMaterials] >= amount) {
			    if(amount < 1)
					return 1;

                playerVariables[playerid][pMaterials] -= amount;
                playerVariables[targetID][pMaterials] += amount;

                format(szMessage, sizeof(szMessage), "You have given %d materials to %s.", amount, playerName[1]);
                SendClientMessage(playerid, COLOR_WHITE, szMessage);

                format(szMessage, sizeof(szMessage), "%s has given you %d materials.", playerName[0], amount);
                SendClientMessage(targetID, COLOR_WHITE, szMessage);

                format(szMessage, sizeof(szMessage), "* %s has given %d materials to %s.", playerName[0], amount, playerName[1]);
                nearByMessage(playerid, COLOR_PURPLE, szMessage);
			}
			else {
				format(szMessage, sizeof(szMessage), "You don't have enough materials to complete this trade. You need %d more materials.", playerVariables[playerid][pMaterials]-amount);
				SendClientMessage(playerid, COLOR_WHITE, szMessage);
			}
		}
	}
	return 1;
}

CMD:giveweapon(playerid, params[]) {
	new
		id,
		weapon;

	if(sscanf(params, "u", id))
		return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/giveweapon [playerid]");
		
	else if(playerVariables[playerid][pFreezeType] == 0) {
		if(id != INVALID_PLAYER_ID) {
	   	    if(IsPlayerInRangeOfPlayer(playerid, id, 4.0) && !IsPlayerInAnyVehicle(playerid)) {

				weapon = GetPlayerWeapon(playerid);

				switch(weapon) {
					case 16, 18, 35, 36, 37, 38, 39, 40, 44, 45, 46, 0: SendClientMessage(playerid, COLOR_GREY, "Invalid weapon.");
					default: {

						GetPlayerName(id, szPlayerName, MAX_PLAYER_NAME);
						format(szMessage, sizeof(szMessage), "You have offered to give %s your %s.", szPlayerName, WeaponNames[weapon]);
						SendClientMessage(playerid, COLOR_WHITE, szMessage);

						GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
						format(szMessage, sizeof(szMessage), "%s has offered to give you their %s - type /accept weapon to receive it.", szPlayerName, WeaponNames[weapon]);
						SendClientMessage(id, COLOR_NICESKY, szMessage);

						SetPVarInt(id,"gunID",playerid);
						SetPVarInt(playerid,"gun",weapon);
						SetPVarInt(playerid,"slot",GetWeaponSlot(weapon));
					}
				}
	    	}
	    	else SendClientMessage(playerid, COLOR_GREY, "You're too far away or in a vehicle.");
	    }
		else SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You can't do this while cuffed, tazed, or frozen.");
	return 1;
}

CMD:givearmour(playerid, params[]) {
	new
		id;

	if(sscanf(params, "u", id))
		return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/givearmour [playerid]");

	else if(playerVariables[playerid][pFreezeType] == 0) {
		if(id != INVALID_PLAYER_ID) {
	   	    if(IsPlayerInRangeOfPlayer(playerid, id, 4.0)) {

				new
					Float:fArmour;

				GetPlayerArmour(playerid, fArmour);

				if(fArmour > 0) {
					GetPlayerName(id, szPlayerName, MAX_PLAYER_NAME);
					format(szMessage, sizeof(szMessage), "You have offered to give %s your kevlar vest (%.1f percent).", szPlayerName, fArmour);
					SendClientMessage(playerid, COLOR_WHITE, szMessage);

					GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
					format(szMessage, sizeof(szMessage), "%s has offered to give you their kevlar vest (%.1f percent) - type /accept armour to receive it.", szPlayerName, fArmour);
					SendClientMessage(id, COLOR_NICESKY, szMessage);

					SetPVarInt(id, "aID", playerid + 1);
				}
				else SendClientMessage(playerid, COLOR_GREY, "You have no armour to give.");
	    	}
	    	else SendClientMessage(playerid, COLOR_GREY, "You're too far away.");
	    }
		else SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You can't do this while cuffed, tazed, or frozen.");
	return 1;
} 


CMD:seenewbie(playerid, params[]) {
	if(playerVariables[playerid][pNewbieEnabled] == 1) {
	    playerVariables[playerid][pNewbieEnabled] = 0;
	    SendClientMessage(playerid, COLOR_WHITE, "You will no longer see newbie chat.");
	}
	else {
	    playerVariables[playerid][pNewbieEnabled] = 1;
	    SendClientMessage(playerid, COLOR_WHITE, "You will now see newbie chat.");
	}
	return 1;
}

CMD:newbie(playerid, params[]) {
	if(playerVariables[playerid][pNewbieTimeout] > 0 && playerVariables[playerid][pAdminLevel] < 1) {
		SendClientMessage(playerid,COLOR_GREY, "You must wait until you can speak again in the newbie chat channel.");
		return 1;
	}
	if(!isnull(params)) {
	    GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);

		if(playerVariables[playerid][pAdminLevel] > 0 && playerVariables[playerid][pAdminDuty] != 0) {
			format(szMessage, sizeof(szMessage), "** Admin %s: %s", szPlayerName, params);
		}
		else if(playerVariables[playerid][pHelper] >= 1 && playerVariables[playerid][pHelperDuty] >= 1) {
		    format(szMessage, sizeof(szMessage), "** Helper %s: %s", szPlayerName, params);
			playerVariables[playerid][pNewbieTimeout] = 5;
		}
		else if(playerVariables[playerid][pAdminLevel] > 0 && playerVariables[playerid][pAdminDuty] == 0) {
			format(szMessage, sizeof(szMessage), "** Player %s: %s", szPlayerName, params);
		}
		else if(playerVariables[playerid][pPlayingHours] >= 100) {
			format(szMessage, sizeof(szMessage), "** Player %s: %s", szPlayerName, params);
			playerVariables[playerid][pNewbieTimeout] = 30;
		}
		else {
			format(szMessage, sizeof(szMessage), "** Newbie %s: %s", szPlayerName, params);
			playerVariables[playerid][pNewbieTimeout] = 30;
		}
		foreach(Player, x) {
			if(playerVariables[x][pStatus] == 1 && playerVariables[x][pNewbieEnabled] == 1) {
				SendClientMessage(x, COLOR_NEWBIE, szMessage);
			}
		}
	}
	else {
	    return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/(n)ewbie [question]");
	}
	return 1;
}  


CMD:n(playerid, params[]) {
	return cmd_newbie(playerid, params);
}

CMD:upgradelevel(playerid, params[]) {
	/* 1 level costs Level x min_level_upgrade_cost */
	if(playerVariables[playerid][pBankMoney] >= playerVariables[playerid][pLevel] + 1 * assetVariables[3][aAssetValue] && playerVariables[playerid][pBankMoney] > 0) {
	    if(playerVariables[playerid][pLevel] >= 10)
	        return SendClientMessage(playerid, COLOR_GREY, "You're at the maximum level.");

		if(FetchLevelFromHours(playerVariables[playerid][pPlayingHours]) == playerVariables[playerid][pLevel])
		    return SendClientMessage(playerid, COLOR_GREY, "You can't upgrade your level yet.");

        playerVariables[playerid][pLevel] += 1;
        playerVariables[playerid][pBankMoney] -= playerVariables[playerid][pLevel] + 1 * assetVariables[3][aAssetValue];

        SetPlayerScore(playerid, playerVariables[playerid][pLevel]);
	}
	return 1;
} 

CMD:accept(playerid, params[]) {
	if(!isnull(params)) {
		if(strcmp(params, "ticket", true) == 0) {

			new
				ticketer = GetPVarInt(playerid, "tID") - 1,
				ticketPrice = GetPVarInt(playerid, "tP"),
				ticketString[128],
				ticketNames[2][MAX_PLAYER_NAME];

			if(ticketer != -1 && ticketPrice > 0) {
				if(IsPlayerAuthed(ticketer)) {
					if(IsPlayerInRangeOfPlayer(playerid, ticketer, 3.0)) {
						if(playerVariables[playerid][pMoney] >= ticketPrice) {

							GetPlayerName(playerid, ticketNames[0], MAX_PLAYER_NAME);
							GetPlayerName(ticketer, ticketNames[1], MAX_PLAYER_NAME);

							format(ticketString, sizeof(ticketString), "* %s takes out $%d in cash, and hands it to %s.", ticketNames[0], ticketPrice, ticketNames[1]);
							nearByMessage(playerid, COLOR_PURPLE, ticketString);

							format(ticketString, sizeof(ticketString), "%s has accepted the $%d ticket you issued them - you have received $%d.", ticketNames[0], ticketPrice, ticketPrice / 2);
							SendClientMessage(ticketer, COLOR_WHITE, ticketString);

							format(ticketString, sizeof(ticketString), "You have paid the $%d ticket %s issued you.", ticketPrice, ticketNames[1]);
							SendClientMessage(playerid, COLOR_WHITE, ticketString);

							playerVariables[playerid][pMoney] -= ticketPrice;
							playerVariables[ticketer][pMoney] += ticketPrice / 2;

							groupVariables[playerVariables[ticketer][pGroup]][gSafe][0] += ticketPrice / 2;

							DeletePVar(playerid, "tID");
							DeletePVar(playerid, "tP");

						}
						else {

							format(ticketString, sizeof(ticketString), "You can't afford to pay this ticket of $%d - you need another $%d to do so.", ticketPrice, ticketPrice - playerVariables[playerid][pMoney]);
							SendClientMessage(playerid, COLOR_GREY, ticketString);
						}
					}
					else SendClientMessage(playerid, COLOR_GREY, "You're too far away.");
				}
				else {
					SendClientMessage(playerid, COLOR_GREY, "The person issuing the ticket has disconnected.");
					DeletePVar(playerid, "tID");
					DeletePVar(playerid, "tP");
				}
			}
			else SendClientMessage(playerid, COLOR_GREY, "Nobody has issued you a ticket.");
		}
		else if(strcmp(params, "givecar", true) == 0) {

			new
				playerCarOffer = GetPVarInt(playerid, "gC") - 1, // <Divide by zero here>
				giveCarString[128],
				x,
				giveCarPlayerName[2][MAX_PLAYER_NAME];

		    if(playerCarOffer != -1) {
		        if(IsPlayerAuthed(playerCarOffer)) {
					if(playerVariables[playerid][pCarModel] < 1) {
						if(IsPlayerInRangeOfPlayer(playerid, playerCarOffer, 5.0)) {
							GetVehiclePos(playerVariables[playerCarOffer][pCarID], playerVariables[playerid][pCarPos][0], playerVariables[playerid][pCarPos][1], playerVariables[playerid][pCarPos][2]);
							GetVehicleZAngle(playerVariables[playerCarOffer][pCarID], playerVariables[playerid][pCarPos][3]); // Get pos and Z angle, save 'em to the accepting player

							playerVariables[playerid][pCarModel] = playerVariables[playerCarOffer][pCarModel]; // Transfer the car model

							playerVariables[playerid][pCarColour][0] = playerVariables[playerCarOffer][pCarColour][0]; // And the colours, and paint job
							playerVariables[playerid][pCarColour][1] = playerVariables[playerCarOffer][pCarColour][1];
							playerVariables[playerid][pCarPaintjob] = playerVariables[playerCarOffer][pCarPaintjob];

							playerVariables[playerid][pCarTrunk][0] = playerVariables[playerCarOffer][pCarTrunk][0];
							playerVariables[playerid][pCarTrunk][1] = playerVariables[playerCarOffer][pCarTrunk][1];

							while(x < 13) {
								playerVariables[playerid][pCarMods][x] = GetVehicleComponentInSlot(playerVariables[playerCarOffer][pCarID], x); // Mods, too.
								x++;
							}

							x = 0;

							while(x < 5) {
								playerVariables[playerid][pCarWeapons][x] = playerVariables[playerCarOffer][pCarWeapons][x];
								x++;
							}

							GetPlayerName(playerCarOffer, giveCarPlayerName[1], MAX_PLAYER_NAME);
							GetPlayerName(playerid, giveCarPlayerName[0], MAX_PLAYER_NAME);

							format(giveCarString, sizeof(giveCarString), "%s has accepted your offer, and is now the owner of this %s.", giveCarPlayerName[0], VehicleNames[playerVariables[playerid][pCarModel] - 400]);
							SendClientMessage(playerCarOffer, COLOR_WHITE, giveCarString);

							format(giveCarString, sizeof(giveCarString), "You have accepted %s's offer, and are now the owner of this %s.", giveCarPlayerName[1], VehicleNames[playerVariables[playerid][pCarModel] - 400]);
							SendClientMessage(playerid, COLOR_WHITE, giveCarString);

							format(giveCarString, sizeof(giveCarString), "* %s has given their car keys to %s.", giveCarPlayerName[1], giveCarPlayerName[0]);
							nearByMessage(playerid, COLOR_PURPLE, giveCarString);

							DestroyPlayerVehicle(playerCarOffer);
							SpawnPlayerVehicle(playerid);
							DeletePVar(playerid, "gC");
							
							ShowPlayerDialog(playerid, DIALOG_LICENSE_PLATE, DIALOG_STYLE_INPUT, "License plate registration", "Please enter a license plate for your vehicle. \n\nThere is only two conditions:\n- The license plate must be unique\n- The license plate can be alphanumerical, but it must consist of only 7 characters and include one space.", "Select", "");
						}
						else SendClientMessage(playerid, COLOR_GREY, "You're too far away.");
					}
					else SendClientMessage(playerid, COLOR_GREY, "You already own a vehicle.");
		        }
		        else { // Offering player disconnects.
		            DeletePVar(playerid, "gC");
		            SendClientMessage(playerid, COLOR_GREY, "The person offering the vehicle has disconnected.");
		        }
		    }
		    else SendClientMessage(playerid, COLOR_GREY, "Nobody has offered you a vehicle.");
		}
	    else if(strcmp(params, "invite", true) == 0) {
	        if(GetPVarInt(playerid, "invID") >= 1) {
	            new
	                messageString[64];

	            playerVariables[playerid][pGroup] = GetPVarInt(playerid, "invID");
				playerVariables[playerid][pGroupRank] = 1;

				DeletePVar(playerid, "invID");

				format(messageString, sizeof(messageString), "You are now a member of the %s.", groupVariables[playerVariables[playerid][pGroup]][gGroupName]);
				SendClientMessage(playerid, COLOR_NICESKY, messageString);

				GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
				format(messageString, sizeof(messageString), "%s has joined the group (invitation).", szPlayerName);
				SendToGroup(playerVariables[playerid][pGroup], COLOR_GENANNOUNCE, messageString);

	        }
	        else {
				return SendClientMessage(playerid, COLOR_GREY, "You don't have an active group invite request.");
			}
	    }

		else if(strcmp(params, "handshake", true) == 0) {
		    if(GetPVarInt(playerid,"hs") != 0) {
		        if(GetPlayerState(playerid) != 1) return SendClientMessage(playerid, COLOR_GREY, "You can only do this while on foot.");
		        if(IsPlayerInRangeOfPlayer(playerid, GetPVarInt(playerid,"hsID"), 1.5)) {

		            new
						Float: PosFloats[3],
						string[73],
						playerNames[2][MAX_PLAYER_NAME],
						shakeOffer = GetPVarInt(playerid,"hsID"),
						shakeStyle = GetPVarInt(playerid,"hs");

					if(!IsPlayerAuthed(shakeOffer)) return 1;

					PlayerFacePlayer(playerid, shakeOffer);
		            GetPlayerPos(shakeOffer, PosFloats[0], PosFloats[1], PosFloats[2]);
		            GetXYInFrontOfPlayer(shakeOffer, PosFloats[0], PosFloats[1], 0.5);
		            SetPlayerPos(playerid, PosFloats[0], PosFloats[1], PosFloats[2]); // Ensures that the players are in perfect position for it to happen.

					switch(shakeStyle) {
			            case 1: {
			                ApplyAnimation(playerid, "GANGS", "hndshkaa", 3.0, 1, 1, 1, 0, 1500, 1);
			                ApplyAnimation(shakeOffer, "GANGS", "hndshkaa", 3.0, 1, 1, 1, 0, 1500, 1);
			                ApplyAnimation(playerid, "GANGS", "hndshkaa", 3.0, 1, 1, 1, 0, 1500, 1);
			                ApplyAnimation(shakeOffer, "GANGS", "hndshkaa", 3.0, 1, 1, 1, 0, 1500, 1);
						}
			            case 2: {
			                ApplyAnimation(playerid, "GANGS", "hndshkba", 3.0, 1, 1, 1, 0, 1500, 1 );
			                ApplyAnimation(shakeOffer, "GANGS", "hndshkba", 3.0, 1, 1, 1, 0, 1500, 1);
			                ApplyAnimation(playerid, "GANGS", "hndshkba", 3.0, 1, 1, 1, 0, 1500, 1 );
			                ApplyAnimation(shakeOffer, "GANGS", "hndshkba", 3.0, 1, 1, 1, 0, 1500, 1);
			            }
			            case 3: {
			                ApplyAnimation(playerid, "GANGS", "hndshkca", 3.0, 1, 1, 1, 0, 1500, 1 );
			                ApplyAnimation(shakeOffer, "GANGS", "hndshkcb", 3.0, 1, 1, 1, 0, 1500, 1);
			                ApplyAnimation(playerid, "GANGS", "hndshkca", 3.0, 1, 1, 1, 0, 1500, 1 );
			                ApplyAnimation(shakeOffer, "GANGS", "hndshkcb", 3.0, 1, 1, 1, 0, 1500, 1);
			            }
			            case 4: {
			                ApplyAnimation(playerid, "GANGS", "hndshkda",3.0, 1, 1, 1, 0, 1500, 1);
			                ApplyAnimation(shakeOffer, "GANGS", "hndshkda", 3.0, 1, 1, 1, 0, 1500, 1);
			                ApplyAnimation(playerid, "GANGS", "hndshkda",3.0, 1, 1, 1, 0, 1500, 1);
			                ApplyAnimation(shakeOffer, "GANGS", "hndshkda", 3.0, 1, 1, 1, 0, 1500, 1);
			            }
			            case 5: {
			                ApplyAnimation(playerid, "GANGS", "hndshkea", 3.0, 1, 1, 1, 0, 1500, 1);
			                ApplyAnimation(shakeOffer, "GANGS", "hndshkea", 3.0, 1, 1, 1, 0, 1500, 1);
			                ApplyAnimation(playerid, "GANGS", "hndshkea", 3.0, 1, 1, 1, 0, 1500, 1);
			                ApplyAnimation(shakeOffer, "GANGS", "hndshkea", 3.0, 1, 1, 1, 0, 1500, 1);
			            }
			            case 6: {
			                ApplyAnimation(playerid, "GANGS", "hndshkfa", 3.0, 1, 1, 1, 0, 1500, 1 );
			                ApplyAnimation(shakeOffer, "GANGS", "hndshkfa", 3.0, 1, 1, 1, 0, 1500, 1);
			                ApplyAnimation(playerid, "GANGS", "hndshkfa", 3.0, 1, 1, 1, 0, 1500, 1 );
			                ApplyAnimation(shakeOffer, "GANGS", "hndshkfa", 3.0, 1, 1, 1, 0, 1500, 1);
			            }
			            case 7: {
			                ApplyAnimation(playerid, "GANGS", "prtial_hndshk_01", 3.0, 1, 1, 1, 0, 1500, 1);
			                ApplyAnimation(shakeOffer, "GANGS", "prtial_hndshk_01", 3.0, 1, 1, 1, 0, 1500, 1);
			                ApplyAnimation(playerid, "GANGS", "prtial_hndshk_01", 3.0, 1, 1, 1, 0, 1500, 1);
			                ApplyAnimation(shakeOffer, "GANGS", "prtial_hndshk_01", 3.0, 1, 1, 1, 0, 1500, 1);
			            }
			            case 8: {
							ApplyAnimation(playerid, "GANGS", "prtial_hndshk_biz_01", 3.7, 1, 1, 1, 0, 2200, 1);
							ApplyAnimation(shakeOffer, "GANGS", "prtial_hndshk_biz_01", 3.5, 1, 1, 1, 0, 2200, 1);
							ApplyAnimation(playerid, "GANGS", "prtial_hndshk_biz_01", 3.7, 1, 1, 1, 0, 2200, 1);
							ApplyAnimation(shakeOffer, "GANGS", "prtial_hndshk_biz_01", 3.5, 1, 1, 1, 0, 2200, 1);
			            }
					}
					GetPlayerName(playerid, playerNames[0], MAX_PLAYER_NAME);
					GetPlayerName(shakeOffer, playerNames[1], MAX_PLAYER_NAME);
					DeletePVar(playerid,"hs");
					DeletePVar(playerid,"hsID");
					format(string, sizeof(string), "* %s has shaken hands with %s.", playerNames[1], playerNames[0]);
					nearByMessage(playerid, COLOR_PURPLE, string);
				}
				else {
				    SendClientMessage( playerid, COLOR_GREY, "You're too far away.");
				}
		    }
		    else {
		        SendClientMessage(playerid, COLOR_GREY, "You don't have a pending handshake request.");
		    }
		}
		else if(strcmp(params, "weapon", true) == 0) {

			new
				playerOffering = GetPVarInt(playerid,"gunID"),
				weaponOffering = GetPVarInt(GetPVarInt(playerid,"gunID"),"gun"),
				slotOffering = GetPVarInt(GetPVarInt(playerid,"gunID"),"slot"),
				WplayerName[2][MAX_PLAYER_NAME],
				wstring[128];

	   		if(weaponOffering != 0 && slotOffering != 0) {
				if(IsPlayerInRangeOfPlayer(playerid, playerOffering, 5.0) && !IsPlayerInAnyVehicle(playerid) && !IsPlayerInAnyVehicle(playerOffering)) {

					if(playerVariables[playerOffering][pWeapons][slotOffering] != weaponOffering) {
						return SendClientMessage(playerid, COLOR_GREY, "The player offering you a weapon no longer has it.");
					}
					else if(playerVariables[playerOffering][pFreezeType] > 0) {
						return SendClientMessage(playerid, COLOR_GREY, "That person is cuffed, tazed, or frozen - they can't do this.");
					}
					else if(playerVariables[playerid][pFreezeType] > 0) {
						return SendClientMessage(playerid, COLOR_GREY, "You can't do this while cuffed, tazed, or frozen.");
					}
					else {

						givePlayerValidWeapon(playerid, weaponOffering);
						removePlayerWeapon(playerOffering, weaponOffering);

						GetPlayerName(playerOffering, WplayerName[0], MAX_PLAYER_NAME);
						GetPlayerName(playerid, WplayerName[1], MAX_PLAYER_NAME);

						format(wstring, sizeof(wstring), "You have accepted the %s from %s.", WeaponNames[weaponOffering], WplayerName[0]);
						SendClientMessage(playerid, COLOR_WHITE, wstring);

						format(wstring, sizeof(wstring), "%s has accepted the %s you offered them.", WplayerName[1], WeaponNames[weaponOffering]);
						SendClientMessage(playerOffering, COLOR_WHITE, wstring);

						format(wstring, sizeof(wstring), "* %s has given their %s to %s.", WplayerName[0], WeaponNames[weaponOffering], WplayerName[1]);
						nearByMessage(playerid, COLOR_PURPLE, wstring);

						DeletePVar(playerOffering,"gun");
						DeletePVar(playerid,"gunID");
						DeletePVar(playerOffering,"slot");
					}
		    	}
		    	else SendClientMessage(playerid, COLOR_GREY, "You're too far away from the person offering, or either of you are in a vehicle.");
		    }
	    	else SendClientMessage(playerid, COLOR_GREY, "Nobody offered you a weapon.");
		}
		else if(strcmp(params, "armour", true) == 0) {

			new
				aplayerOffering = GetPVarInt(playerid,"aID") - 1,
				AplayerName[2][MAX_PLAYER_NAME],
				astring[128];

	   		if(playerOffering != INVALID_PLAYER_ID) {
				if(IsPlayerInRangeOfPlayer(playerid, aplayerOffering, 5.0)) {

					if(playerVariables[aplayerOffering][pFreezeType] > 0) {
						return SendClientMessage(playerid, COLOR_GREY, "That person is cuffed, tazed, or frozen - they can't do this.");
					}
					else if(playerVariables[playerid][pFreezeType] > 0) {
						return SendClientMessage(playerid, COLOR_GREY, "You can't do this while cuffed, tazed, or frozen.");
					}
					else {

						new
							Float:ArmourFloats[2];

						GetPlayerArmour(aplayerOffering, ArmourFloats[0]);
						GetPlayerArmour(playerid, ArmourFloats[1]);

						if(ArmourFloats[1] + ArmourFloats[0] >= 100) SetPlayerArmour(playerid, 100);
						else SetPlayerArmour(playerid, ArmourFloats[1] + ArmourFloats[0]);

						SetPlayerArmour(aplayerOffering, 0.0);

						GetPlayerName(aplayerOffering, AplayerName[0], MAX_PLAYER_NAME);
						GetPlayerName(playerid, AplayerName[1], MAX_PLAYER_NAME);
						format(astring, sizeof(astring), "You have accepted the kevlar vest from %s.", AplayerName[0]);
						SendClientMessage(playerid, COLOR_WHITE, astring);

						format(astring, sizeof(astring), "%s has accepted the kevlar vest you offered them.", AplayerName[1]);
						SendClientMessage(aplayerOffering, COLOR_WHITE, astring);

						format(astring, sizeof(astring), "* %s has given their kevlar vest to %s.", AplayerName[0], AplayerName[1]);
						nearByMessage(playerid, COLOR_PURPLE, astring);

						DeletePVar(playerid,"aID");
					}
		    	}
		    	else SendClientMessage(playerid, COLOR_GREY, "You're too far away from the person offering.");
		    }
	    	else SendClientMessage(playerid, COLOR_GREY, "Nobody offered you armour.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "Invalid item specified.");
    }
    else {
		SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/accept [item]");
		SendClientMessage(playerid, COLOR_GREY, "Items: Invite, Handshake, Weapon, Givecar, Ticket, Armour");
	}
	return 1;
} 

CMD:seepms(playerid, params[]) {
	switch(playerVariables[playerid][pPMStatus]) {
		case 0: {
		    playerVariables[playerid][pPMStatus] = 1;
			return SendClientMessage(playerid, COLOR_WHITE, "You have disabled your PMs.");
		}
		case 1: {
		    playerVariables[playerid][pPMStatus] = 0;
			return SendClientMessage(playerid, COLOR_WHITE, "You have enabled your PMs.");
		}
	}
	return 1;
}

CMD:pm(playerid, params[])
{
	new
		message[128],
		id;

	if(sscanf(params, "us[128]", id, message))
		SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/pm [playerid] [message]");
	else if(playerVariables[id][pStatus] != 1)
		SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
	else if(playerVariables[id][pPMStatus] != 0)
		SendClientMessage(playerid, COLOR_GREY, "That player's PMs aren't enabled.");
	else
	{
		GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);

		format(szMessage, sizeof(szMessage), "(( PM from %s: %s ))", szPlayerName, message);
		SendClientMessage(id, COLOR_YELLOW, szMessage);

		GetPlayerName(id, szPlayerName, MAX_PLAYER_NAME);

		format(szMessage, sizeof(szMessage), "(( PM sent to %s: %s ))", szPlayerName, message);
		SendClientMessage(playerid, COLOR_GREY, szMessage);
    }
	return 1;
}

CMD:w(playerid, params[]) {
	return cmd_whisper(playerid, params);
}

CMD:whisper(playerid, params[]) {
	new
		message[128],
		id;

	if(sscanf(params, "us[128]", id, message)) {
		SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/whisper [playerid] [message]");
	}
	else if(playerVariables[id][pStatus] != 1)
	{
		SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
	}
	else if(playerVariables[id][pSeeWhisper] != 0) 
	{
		SendClientMessage(playerid, COLOR_GREY, "That player's whispers aren't enabled.");
	}
	else if(!IsPlayerInRangeOfPlayer(playerid, id, 2.0))
	{
		SendClientMessage(playerid, COLOR_GREY, "You're too far away.");
	}
	{
		new
			giveplayerName[MAX_PLAYER_NAME];
		GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
		format(szMessage, sizeof(szMessage), "%s whispers: %s", szPlayerName, message);
		SendClientMessage(id, COLOR_NICESKY, szMessage);

		GetPlayerName(id, giveplayerName, MAX_PLAYER_NAME);

		format(szMessage, sizeof(szMessage), "You whisper to %s: %s", giveplayerName, message);
		SendClientMessage(playerid, COLOR_NICESKY, szMessage);

		format(szMessage, sizeof(szMessage), "* %s whispers something to %s.", szPlayerName, giveplayerName);
		nearByMessage(playerid, COLOR_PURPLE, szMessage, 2.0);
    }
	return 1;
}

CMD:adminchat(playerid, params[]) {
	return cmd_a(playerid, params);
}

CMD:commands(playerid, params[]) {
	return showHelp(playerid);
}

stock showHelp(playerid) {
	return ShowPlayerDialog(playerid, DIALOG_HELP, DIALOG_STYLE_LIST, "SERVER: Commands", "General\nChat\nGroups\nAnimations\nHouses\nJobs\n\nBusinesses\nHelpers\nVehicles\nBank", "Select", "Exit");
}

CMD:do(playerid, params[]) {
    if(playerVariables[playerid][pStatus] >= 1) {
        if(!isnull(params)) {
			GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);

			format(szMessage, sizeof(szMessage), "* %s (( %s )) ", params, szPlayerName);
			nearByMessage(playerid, COLOR_PURPLE, szMessage);
		}
		else {
			return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/do [action]");
		}
	}
	return 1;
} 

CMD:charity(playerid, params[]) {
    if(playerVariables[playerid][pStatus] >= 1) {
        new
            value = strval(params);

        if(value < 1 || value >= 5000000)
			return SendClientMessage(playerid, COLOR_GREY, "The charity declined your donation.");

        if(playerVariables[playerid][pMoney] < 1)
			return SendClientMessage(playerid, COLOR_GREY, "The charity declined your donation.");

	    playerVariables[playerid][pMoney] -= value;

        if(playerVariables[playerid][pMoney] < 1)
			playerVariables[playerid][pMoney] = 0;

		format(szMessage, sizeof(szMessage), "The charity accepted your donation of $%d.", value);
        SendClientMessage(playerid, COLOR_YELLOW, szMessage);
    }

    return 1;
}

CMD:flipcoin(playerid, params[]) { // HAHAHAHAHAHAH OH WOW
    if(playerVariables[playerid][pStatus] >= 1) {
		GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);

		if(playerVariables[playerid][pMoney] < 1)
			return SendClientMessage(playerid, COLOR_GREY, "You have no coins to flip.");

		if(random(5) < 3) {
			format(szMessage, sizeof(szMessage), "* %s has flipped their coin. The coin lands on the 'heads' side.", szPlayerName);
			nearByMessage(playerid, COLOR_PURPLE, szMessage);
		}
		else {
			format(szMessage, sizeof(szMessage), "* %s has flipped their coin. The coin lands on the 'tails' side.", szPlayerName);
			nearByMessage(playerid, COLOR_PURPLE, szMessage);
		}
	}
	return 1;
}



CMD:me(playerid, params[]) {
    if(playerVariables[playerid][pStatus] >= 1) {
        if(!isnull(params)) {
			GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);

			format(szMessage, sizeof(szMessage), "* %s %s", szPlayerName, params);
			nearByMessage(playerid, COLOR_PURPLE, szMessage);
		}
		else {
			return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/me [action]");
		}
	}
	return 1;
}

CMD:low(playerid, params[]) {
	if(playerVariables[playerid][pStatus] >= 1) {

		if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/low [message]");

		new
			queryString[255],
		    textString[128];

		GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
		format(textString, sizeof(textString), "%s says quietly [%s Accent]: %s", szPlayerName, playerVariables[playerid][pAccent], params);
		nearByMessage(playerid, COLOR_WHITE, textString, 2.0);
		format(textString, sizeof(textString), "(quietly) \"%s\"", params);
		SetPlayerChatBubble(playerid, textString, COLOR_CHATBUBBLE, 3.0, 10000);
		mysql_real_escape_string(textString, textString);

		format(queryString, sizeof(queryString), "INSERT INTO chatlogs (value, playerinternalid) VALUES('%s', '%d')", textString, playerVariables[playerid][pInternalID]);
		mysql_query(queryString);
	}
	return 1;
}

CMD:l(playerid, params[]) {
	return cmd_low(playerid, params);
}

CMD:shout(playerid, params[]) {
	if(playerVariables[playerid][pStatus] >= 1) {

		if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/shout [message]");
		new

			queryString[255],
		    textString[128];

		GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
		format(textString, sizeof(textString), "(shouts) \"%s!\"", params);
		SetPlayerChatBubble(playerid, textString, COLOR_CHATBUBBLE, 30.0, 10000);
		format(textString, sizeof(textString), "%s shouts [%s Accent]: %s!", szPlayerName, playerVariables[playerid][pAccent], params);
		nearByMessage(playerid, COLOR_WHITE, textString, 20.0);
		mysql_real_escape_string(textString, textString);

		format(queryString, sizeof(queryString), "INSERT INTO chatlogs (value, playerinternalid) VALUES('%s', '%d')", textString, playerVariables[playerid][pInternalID]);
		mysql_query(queryString);
	}
	return 1;
}

CMD:s(playerid, params[]) {
	return cmd_shout(playerid, params);
}

CMD:b(playerid, params[]) {
	if(playerVariables[playerid][pStatus] >= 1) {

		if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/b [message]");
		new

			queryString[255],
		    textString[128];

		GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
		format(textString, sizeof(textString), "%s says: (( %s ))", szPlayerName, params);
		nearByMessage(playerid, COLOR_WHITE, textString, 5.0);
		mysql_real_escape_string(textString, textString);

		format(queryString, sizeof(queryString), "INSERT INTO chatlogs (value, playerinternalid) VALUES('%s', '%d')", textString, playerVariables[playerid][pInternalID]);
		mysql_query(queryString);
	}
	return 1;
}

CMD:report(playerid, params[]) {
	if(systemVariables[reportSystem] == 0) {
		if(isnull(params)) {
		    SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/report [message]");
		}
		else {
		    if(playerVariables[playerid][pReport] >= 1) {
		        SendClientMessage(playerid, COLOR_WHITE, "You already have an active report within our system, please wait for it to be answered.");
		    }
		    else {
		        if(strlen(params) >= 64) {
		            return SendClientMessage(playerid, COLOR_GREY, "Your report message was too long. Keep it under 64 characters.");
		        }
		        else {
				    SendClientMessage(playerid, COLOR_YELLOW, "Your report has been submitted and queued.");

        			strcpy(playerVariables[playerid][pReportMessage], params, 64);
				    playerVariables[playerid][pReport] = 1;

				    submitToAdmins("A new report has been submitted, check '/reports list'", COLOR_YELLOW);
			    }
		    }
		}
	}
	else {
	    SendClientMessage(playerid, COLOR_WHITE, "The report system is disabled right now. Please try again later.");
	}

	return 1;
}