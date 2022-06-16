CMD:accepthelp(playerid, params[]) {
    if(playerVariables[playerid][pHelper] >= 1) {
        if(sscanf(params, "u", iTarget))
            return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/accepthelp [playerid]");

        else {
            if(iTarget == INVALID_PLAYER_ID)
				return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");

            if(GetPVarType(iTarget, "hR") == 0)
				return SendClientMessage(playerid, COLOR_GREY, "The specified playerid/name does not have an active help request.");

            new
                helpString[64];

            GetPVarString(iTarget, "hR", helpString, sizeof(helpString));

            GetPlayerName(iTarget, szPlayerName, MAX_PLAYER_NAME);

            format(szMessage, sizeof(szMessage), "You have accepted %s's help request (%s).", szPlayerName, helpString);
            SendClientMessage(playerid, COLOR_WHITE, szMessage);

            playerVariables[playerid][pOnRequest] = iTarget; // PVar lookup time is slower, better to use a variable for this.

            TextDrawShowForPlayer(playerid, textdrawVariables[1]);

            DeletePVar(iTarget, "hR");

            GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
            format(szMessage, sizeof(szMessage), "%s has accepted your help request.", szPlayerName);
            SendClientMessage(iTarget, COLOR_NEWBIE, szMessage);
        }
    }
	return 1;
}

CMD:viewhelp(playerid, params[]) {
    if(playerVariables[playerid][pHelper] >= 1) {
        SendClientMessage(playerid, COLOR_TEAL, "---------------------------------------------------------------------------------------------------------------------------------");
        foreach(Player, x) {
			if(GetPVarType(x, "hR") != 0) {
			    GetPVarString(x, "hR", szMediumString, sizeof(szMediumString));
				format(szMessage, sizeof(szMessage), "Requested by: %s | Problem: %s", playerVariables[x][pNormalName], szMediumString);
				SendClientMessage(playerid, COLOR_WHITE, szMessage);
			}
		}
        SendClientMessage(playerid, COLOR_TEAL, "---------------------------------------------------------------------------------------------------------------------------------");
	}
	return 1;
}

CMD:helpme(playerid, params[]) {
	if(playerVariables[playerid][pPlayingHours] < 20) {
	    if(isnull(params))
			return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/helpme [subject]");
			
	    if(strlen(params) >= 63) {
			return SendClientMessage(playerid, COLOR_GREY, "Your message was too long. 62 characters or lower, only.");
		} else {
			SetPVarString(playerid, "hR", params);
			SendClientMessage(playerid, COLOR_WHITE, "Your request has been sent. Please wait a few minutes, our helpers have a lot to deal with!");

			new
			    string[128];

			GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);

			format(string, sizeof(string), "A new help request from %s (ID: %d) has been submitted.", szPlayerName);

			foreach(Player, x) {
				if(playerVariables[x][pHelperDuty] >= 1 && playerVariables[x][pHelper] >= 1) {
					SendClientMessage(x, COLOR_NEWBIE, string);
				}
			}
		}
	}
	else {
		return SendClientMessage(playerid, COLOR_GREY, "You already have 20+ playing hours. You are unable to get help from a helper, please use /n for your questions.");
	}

	return 1;
}

CMD:helperduty(playerid, params[]) {
	if(playerVariables[playerid][pHelper] >= 1) {
	    switch(playerVariables[playerid][pHelperDuty]) {
			case 0: {
				playerVariables[playerid][pHelperDuty] = 1;
				SendClientMessage(playerid, COLOR_WHITE, "You are now on duty as a Helper.");
			}
			case 1: {
				playerVariables[playerid][pHelperDuty] = 0;
				SendClientMessage(playerid, COLOR_WHITE, "You are now off duty as a Helper.");
			}
		}
	}
	return 1; //
}  