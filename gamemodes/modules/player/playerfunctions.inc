stock showStats(playerid, targetid) {
	new
		param1[32],
		date[3],
		param2[32],
		param4[32],
		param3[32]; // I'll add one or two more of these later, they'll be used to show things like sex (if sex = whatever, param1 = "Male";). And we can use them over and over again.

    
	if(playerVariables[targetid][pJob] != 0) strcpy(param1, jobVariables[playerVariables[targetid][pJob]][jJobName], sizeof(param1));
	else param1 = "Unemployed";

	if(playerVariables[targetid][pPhoneNumber] != -1) strcpy(param3, playerVariables[targetid][pPhoneNumber], sizeof(param3));
	else param3 = "None";

	getdate(date[0], date[1], date[2]);
    SendClientMessage(playerid, COLOR_TEAL, "--------------------------------------------------------------------------------------------------------------------------------");

	va_SendClientMessage(playerid, COLOR_WHITE, "%s | Age: %d (born %d) | Gender: %s | Playing hours: %d | Phone number: %s | Job: %s", playerVariables[targetid][pNormalName], date[0]-playerVariables[targetid][pAge], playerVariables[targetid][pAge], (playerVariables[targetid][pGender] == 1 ? "Male" : "Female"), playerVariables[targetid][pPlayingHours], param3, param1, param4);

	if(playerVariables[targetid][pGroup] < 1) {
		param1 = "None";
		param2 = "None";
	} else {
		strcpy(param1, groupVariables[playerVariables[targetid][pGroup]][gGroupName], sizeof(param1));

		switch(playerVariables[targetid][pGroupRank]) {
			case 1: strcpy(param2, groupVariables[playerVariables[targetid][pGroup]][gGroupRankName1]sizeof(param2));
			case 2: strcpy(param2, groupVariables[playerVariables[targetid][pGroup]][gGroupRankName2]sizeof(param2));
			case 3: strcpy(param2, groupVariables[playerVariables[targetid][pGroup]][gGroupRankName3]sizeof(param2));
			case 4: strcpy(param2, groupVariables[playerVariables[targetid][pGroup]][gGroupRankName4]sizeof(param2));
			case 5: strcpy(param2, groupVariables[playerVariables[targetid][pGroup]][gGroupRankName5]sizeof(param2));
			case 6: strcpy(param2, groupVariables[playerVariables[targetid][pGroup]][gGroupRankName6]sizeof(param2));
		}
	}

	if(playerVariables[targetid][pWalkieTalkie] == -1) param3 = "None";
	else if(playerVariables[targetid][pWalkieTalkie] == 0) param3 = "Disabled";
	else format(param3, sizeof(param3), "#%d khz", playerVariables[targetid][pWalkieTalkie]);

	va_SendClientMessage(playerid, COLOR_WHITE, "Group: %s | Rank: %s (%d) | Bank: $%d | Cash: $%d | Materials: %d | Radio: %s", param1, param2, playerVariables[targetid][pGroupRank], playerVariables[targetid][pBankMoney], playerVariables[targetid][pMoney], playerVariables[targetid][pMaterials], param3);
	va_SendClientMessage(playerid, COLOR_WHITE, "Rope: %d | Weapon skill: %d (%d weapons) | Tracking skill: %d (%d searches) | Arrests: %d | Crimes: %d | Credit: $%d", playerVariables[targetid][pRope], playerVariables[targetid][pJobSkill][0]/50, playerVariables[targetid][pJobSkill][0], playerVariables[targetid][pJobSkill][1]/50, playerVariables[targetid][pJobSkill][1], playerVariables[targetid][pArrests], playerVariables[targetid][pCrimes], playerVariables[targetid][pPhoneCredit] / 60);

	if(playerVariables[playerid][pAdminLevel] >= 1) {
		new
			Float:HAFloats[2],
			country[MAX_COUNTRY_NAME];

		GetPlayerHealth(targetid, HAFloats[0]);
		GetPlayerArmour(targetid, HAFloats[1]);
		GetCountryName(playerVariables[targetid][pConnectionIP], country, sizeof(country));

		if(playerVariables[targetid][pCarModel] >= 400)
			format(param4, sizeof(param4), "%s (ID %d)", VehicleNames[playerVariables[targetid][pCarModel] - 400], playerVariables[targetid][pCarID]);
		else
			param4 = "None";

		param1 = (playerVariables[targetid][pStatus] != 1) ? ("Unauthenticated") : ("Authenticated");
			
		format(szMessage, sizeof(szMessage), "Status: %s | Admin Level: %d | Interior: %d | VW: %d | House: %d | Business: %d | Vehicle: %s", param1, playerVariables[targetid][pAdminLevel], playerVariables[targetid][pInterior], playerVariables[targetid][pVirtualWorld], getPlayerHouseID(targetid), getPlayerBusinessID(targetid), param4);
		SendClientMessage(playerid, COLOR_WHITE, szMessage);

		switch(playerVariables[targetid][pPrisonID]) {
			case 0: format(szMessage, sizeof(szMessage), "IP: %s | Country: %s | Admin Name: %s | Health: %.1f | Armour: %.1f", playerVariables[targetid][pConnectionIP], country, playerVariables[targetid][pAdminName], HAFloats[0], HAFloats[1]);
			case 1: format(szMessage, sizeof(szMessage), "IP: %s | Country: %s | Admin Name: %s | Health: %.1f | Armour: %.1f | Admin Prison Time: %d", playerVariables[targetid][pConnectionIP], country, playerVariables[targetid][pAdminName], HAFloats[0], HAFloats[1], playerVariables[targetid][pPrisonTime]);
			case 2: format(szMessage, sizeof(szMessage), "IP: %s | Country: %s | Admin Name: %s | Health: %.1f | Armour: %.1f | Admin Jail Time: %d", playerVariables[targetid][pConnectionIP], country, playerVariables[targetid][pAdminName], HAFloats[0], HAFloats[1], playerVariables[targetid][pPrisonTime]);
			case 3: format(szMessage, sizeof(szMessage), "IP: %s | Country: %s | Admin Name: %s | Health: %.1f | Armour: %.1f | Jail Time: %d", playerVariables[targetid][pConnectionIP], country, playerVariables[targetid][pAdminName], HAFloats[0], HAFloats[1], playerVariables[targetid][pPrisonTime]);
		}

		SendClientMessage(playerid, COLOR_WHITE, szMessage);
	}

	SendClientMessage(playerid, COLOR_TEAL, "--------------------------------------------------------------------------------------------------------------------------------");
	return 1;
}