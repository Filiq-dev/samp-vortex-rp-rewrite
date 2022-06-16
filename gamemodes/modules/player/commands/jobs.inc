YCMD:fixcar(playerid, params[], help) {
	if(!(jobVariables[playerVariables[playerid][pJob]][jJobType] == 3 || playerVariables[playerid][pAdminDuty] >= 1)) return true;
	if(!IsPlayerInAnyVehicle(playerid)) return true;

	new 
		Float: soPos[3],
		vehicleID = GetPlayerVehicleID(playerid);

	if(playerVariables[playerid][pJobDelay] != 0) return va_SendClientMessage(playerid, COLOR_GREY, "You need to wait %d seconds until you can use a mechanic command again.",playerVariables[playerid][pJobDelay]);
	if(GetPlayerSpeed(playerid, 0) != 0) return SendClientMessage(playerid, COLOR_WHITE, "You must stop your vehicle first."); 

	GetVehiclePos(vehicleID, soPos[0], soPos[1], soPos[2]);
	PlayerPlaySoundEx(1133, soPos[0], soPos[1], soPos[2]);

	RepairVehicle(vehicleID);
	va_SendClientMessage(playerid, COLOR_WHITE, "You have repaired your %s.", VehicleNames[GetVehicleModel(vehicleID) - 400]);

	playerVariables[playerid][pJobDelay] = 60; 

	return true;
}

YCMD:colourcar(playerid, params[], help) {
	if(jobVariables[playerVariables[playerid][pJob]][jJobType] != 3) return true;

	new
		colors[2],
		Float: soPos[3],
		vehicleID = GetPlayerVehicleID(playerid);

	if(!vehicleID) return true;
	if(sscanf(params, "dd", colors[0], colors[1])) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/colourcar [colour 1] [colour 2]");
	if(playerVariables[playerid][pJobDelay] != 0) return SendClientMessage(playerid, COLOR_WHITE, "Please wait your job reload time.");
	if(GetPlayerSpeed(playerid, 0) != 0) return SendClientMessage(playerid, COLOR_WHITE, "You must stop your vehicle first.");
	if(!(colors[0] >= 0 && colors[0] < 256 && colors[1] >= 0 && colors[1] < 256)) return SendClientMessage(playerid, COLOR_WHITE, "Valid vehicle colours are 0 to 255."); 

	GetVehiclePos(vehicleID, soPos[0], soPos[1], soPos[2]);
	ChangeVehicleColor(vehicleID, colors[0], colors[1]);
	PlayerPlaySoundEx(1134, soPos[0], soPos[1], soPos[2]);

	foreach(new v : Player) {
		if(playerVariables[v][pCarID] == vehicleID) {
			playerVariables[v][pCarColour][0] = colors[0];
			playerVariables[v][pCarColour][1] = colors[1];
		}
	}
	SendClientMessage(playerid, COLOR_WHITE, "You have resprayed your vehicle.");
	playerVariables[playerid][pJobDelay] = 60; 

	return true;
}

YCMD:trackplates(playerid, params[], help) {
	if(jobVariables[playerVariables[playerid][pJob]][jJobType] != 2 && groupVariables[playerVariables[playerid][pGroup]][gGroupType] != 1) return SendClientMessage(playerid, COLOR_GREY, "You are not a Detective or a LEO.");
	if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/trackplates [plate]");
 	if(playerVariables[playerid][pJobSkill][1] < 500 && groupVariables[playerVariables[playerid][pGroup]][gGroupType] != 1) return SendClientMessage(playerid, COLOR_GREY, "You are not a Level 5 detective.");

	foreach(new x : Player) 
		if(strcmp(playerVariables[x][pCarLicensePlate], params, true) == 0)
			return va_SendClientMessage(playerid, COLOR_WHITE, "Plate: "EMBED_GREY"%s{FFFFFF} | Vehicle Owner: "EMBED_GREY"%s", playerVariables[x][pCarLicensePlate], getName(playerid));

	return true;
}

YCMD:trackhouse(playerid, params[], help) {
	new id;

	if(jobVariables[playerVariables[playerid][pJob]][jJobType] != 2) return true;
	if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/trackhouse [playerid]");
	if(playerVariables[playerid][pJobSkill][1] < 400) return SendClientMessage(playerid, COLOR_GREY, "Your detective skill must be at least level 8 to use this.");
	if(!IsPlayerAuthed(id)) return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
	if(id == playerid) return SendClientMessage(playerid, COLOR_GREY, "Use /home to set a checkpoint to your house.");
	if(playerVariables[playerid][pJobDelay] >= 1) va_SendClientMessage(playerid, COLOR_GREY, "You need to wait %d seconds until you can use a detective command again.",playerVariables[playerid][pJobDelay]);
	if(playerVariables[playerid][pCheckpoint] >= 2) return va_SendClientMessage(playerid, COLOR_GREY, "You already have an active checkpoint (%s), reach it first, or /killcheckpoint.", getPlayerCheckpointReason(playerid));
	if(playerVariables[id][pAdminDuty] >= 1) SendClientMessage(playerid, COLOR_GREY, "You can't track this person's house at the moment.");
	if(!(getPlayerHouseID(id) >= 1)) return SendClientMessage(playerid, COLOR_GREY, "This person does not own a house.");

	new house = getPlayerHouseID(id);

	SetPlayerCheckpoint(playerid, houseVariables[house][hHouseExteriorPos][0], houseVariables[house][hHouseExteriorPos][1], houseVariables[house][hHouseExteriorPos][2], 5.0);
	va_SendClientMessage(playerid, COLOR_WHITE, "A checkpoint has been set to %s's house.", getName(playerid));

	switch(playerVariables[playerid][pJobSkill][1]) {
		case 400 .. 449: playerVariables[playerid][pJobDelay] = 40;
		case 450 .. 499: playerVariables[playerid][pJobDelay] = 30;
		default: playerVariables[playerid][pJobDelay] = 20;
	}

	playerVariables[playerid][pJobSkill][1]++;
	playerVariables[playerid][pCheckpoint] = 1; 
	        
	return 1;
}

YCMD:trackbusiness(playerid, params[], help) {
	new id;

	if(jobVariables[playerVariables[playerid][pJob]][jJobType] != 2) return true;
	if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/trackbusiness [playerid]");
	if(playerVariables[playerid][pJobSkill][1] < 400) return SendClientMessage(playerid, COLOR_GREY, "Your detective skill must be at least level 8 to use this.");
	if(!IsPlayerAuthed(id)) return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
	if(id == playerid) return SendClientMessage(playerid, COLOR_GREY, "Use /business to set a checkpoint to your business.");
	if(playerVariables[playerid][pJobDelay] >= 1) return va_SendClientMessage(playerid, COLOR_GREY, "You need to wait %d seconds until you can use a detective command again.",playerVariables[playerid][pJobDelay]);
	if(playerVariables[playerid][pCheckpoint] >= 2) va_SendClientMessage(playerid, COLOR_GREY, "You already have an active checkpoint (%s), reach it first, or /killcheckpoint.", getPlayerCheckpointReason(playerid));
	if(playerVariables[id][pAdminDuty] >= 1) SendClientMessage(playerid, COLOR_GREY, "You can't track this person's business at the moment.");
	if(!(getPlayerBusinessID(id) >= 1)) return SendClientMessage(playerid, COLOR_GREY, "This person does not own a business.");
	
	new house = getPlayerBusinessID(id);

	SetPlayerCheckpoint(playerid, businessVariables[house][bExteriorPos][0], businessVariables[house][bExteriorPos][1], businessVariables[house][bExteriorPos][2], 5.0);
	va_SendClientMessage(playerid, COLOR_WHITE, "A checkpoint has been set to %s's business.", playerVariables[id][pNormalName]);

	switch(playerVariables[playerid][pJobSkill][1]) {
		case 400 .. 449: playerVariables[playerid][pJobDelay] = 40;
		case 450 .. 499: playerVariables[playerid][pJobDelay] = 30;
		default: playerVariables[playerid][pJobDelay] = 20;
	}

	playerVariables[playerid][pJobSkill][1]++;
	playerVariables[playerid][pCheckpoint] = 1; 
				
	return true;
}

YCMD:trackcar(playerid, params[], help) {
	new id;

	if(jobVariables[playerVariables[playerid][pJob]][jJobType] != 2) return true;
	if(playerVariables[playerid][pJobSkill][1] < 250) return SendClientMessage(playerid, COLOR_GREY, "Your detective skill must be at least level 5 to use this.");
	if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/trackcar [playerid]");
	if(!IsPlayerAuthed(id)) return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
	if(id == playerid) return SendClientMessage(playerid, COLOR_GREY, "Use /findcar to track your own vehicle.");
	if(playerVariables[playerid][pJobDelay] >= 1) return va_SendClientMessage(playerid, COLOR_GREY, "You need to wait %d seconds until you can use a detective command again.",playerVariables[playerid][pJobDelay]);
	if(playerVariables[playerid][pCheckpoint] >= 2) return va_SendClientMessage(playerid, COLOR_GREY,"You already have an active checkpoint (%s), reach it first, or /killcheckpoint.", getPlayerCheckpointReason(playerid));
	if(playerVariables[id][pAdminDuty] >= 1) return SendClientMessage(playerid, COLOR_GREY, "You can't track this person's vehicle at the moment."); 

	GetVehiclePos(playerVariables[id][pCarID], playerVariables[id][pCarPos][0], playerVariables[id][pCarPos][1], playerVariables[id][pCarPos][2]);
	SetPlayerCheckpoint(playerid, playerVariables[id][pCarPos][0], playerVariables[id][pCarPos][1], playerVariables[id][pCarPos][2], 10.0);
	va_SendClientMessage(playerid, COLOR_WHITE, "A checkpoint has been set, %s's %s was last seen at the marked area.", playerVariables[id][pNormalName], VehicleNames[playerVariables[id][pCarModel] - 400]);

	switch(playerVariables[playerid][pJobSkill][1]) {
		case 250 .. 299: playerVariables[playerid][pJobDelay] = 70;
		case 300 .. 349: playerVariables[playerid][pJobDelay] = 60;
		case 350 .. 399: playerVariables[playerid][pJobDelay] = 50;
		case 400 .. 449: playerVariables[playerid][pJobDelay] = 40;
		case 450 .. 499: playerVariables[playerid][pJobDelay] = 30;
		default: playerVariables[playerid][pJobDelay] = 20;
	}

	playerVariables[playerid][pJobSkill][1]++;
	playerVariables[playerid][pCheckpoint] = 1;
	
	return true;
}

YCMD:track(playerid, params[], help) { 
	if(playerVariables[playerid][pJobDelay] >= 1) return va_SendClientMessage(playerid, COLOR_GREY, "You need to wait %d seconds until you can use a detective command again.",playerVariables[playerid][pJobDelay]);
	if(playerVariables[playerid][pCheckpoint] >= 2) return va_SendClientMessage(playerid, COLOR_GREY,"You already have an active checkpoint (%s), reach it first, or /killcheckpoint.", getPlayerCheckpointReason(playerid));
	
	new id, Float:FindFloats[3];

	if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/track [playerid]");
	if(id == playerid) return SendClientMessage(playerid, COLOR_GREY, "You can't track yourself.");
	if(playerVariables[id][pStatus] != 1) return SendClientMessage(playerid, COLOR_GREY, "The specified player ID is either not connected or has not authenticated.");
	if(jobVariables[playerVariables[playerid][pJob]][jJobType] != 2) return true;
	if(GetPlayerInterior(id) >= 1 || GetPlayerVirtualWorld(id) >= 1 || playerVariables[id][pSpectating] != INVALID_PLAYER_ID) SendClientMessage(playerid, COLOR_GREY, "That player is an alternate interior or virtual world.");
	if(playerVariables[id][pAdminDuty] >= 1) SendClientMessage(playerid, COLOR_GREY, "You can't track this person at the moment.");
	 
	GetPlayerPos(id, FindFloats[0], FindFloats[1], FindFloats[2]);
	SetPlayerCheckpoint(playerid, FindFloats[0], FindFloats[1], FindFloats[2], 5.0); 

	va_SendClientMessage(playerid, COLOR_WHITE, "A checkpoint has been set, %s was last seen at the marked area.", playerVariables[id][pNormalName]);

	playerVariables[playerid][pCheckpoint] = 1;

	switch(playerVariables[playerid][pJobSkill][1]) {
		case 0 .. 49: playerVariables[playerid][pJobDelay] = 120;
		case 50 .. 99: playerVariables[playerid][pJobDelay] = 110;
		case 100 .. 149: playerVariables[playerid][pJobDelay] = 100;
		case 150 .. 199: playerVariables[playerid][pJobDelay] = 90;
		case 200 .. 249: playerVariables[playerid][pJobDelay] = 80;
		case 250 .. 299: playerVariables[playerid][pJobDelay] = 70;
		case 300 .. 349: playerVariables[playerid][pJobDelay] = 60;
		case 350 .. 399: playerVariables[playerid][pJobDelay] = 50;
		case 400 .. 449: playerVariables[playerid][pJobDelay] = 40;
		case 450 .. 499: playerVariables[playerid][pJobDelay] = 30;
		default: playerVariables[playerid][pJobDelay] = 20;
	}

	playerVariables[playerid][pJobSkill][1] ++;

	switch(playerVariables[playerid][pJobSkill][1]) 
	{
		case 50, 100, 150, 200, 250, 300, 350, 400, 450, 500: 
		{ 
			va_SendClientMessage(playerid,COLOR_WHITE,"Congratulations! Your detective skill level is now %d. You will now have a lower delay between each track attempt.",playerVariables[playerid][pJobSkill][1]/50);
		}
	}
	
	return true;
}

YCMD:noscar(playerid, params[], help) {
	if(jobVariables[playerVariables[playerid][pJob]][jJobType] != 3) return true; 
	if(GetPlayerVehicleID(playerid) == 0) return SendClientMessage(playerid, COLOR_GREY, "You're not in any vehicle.");
	new vehicleID = GetPlayerVehicleID(playerid);  
	new Float:soPos[3], vehicleModel = GetVehicleModel(vehicleID);
	if(IsInvalidNOSVehicle(vehicleModel)) return va_SendClientMessage(playerid, COLOR_GREY, "You can't modify this %s.", VehicleNames[vehicleModel - 400]);
	if(playerVariables[playerid][pJobDelay] != 0) return va_SendClientMessage(playerid, COLOR_GREY, "You need to wait %d seconds until you can use a mechanic command again.",playerVariables[playerid][pJobDelay]);

	GetVehiclePos(vehicleID, soPos[0], soPos[1], soPos[2]);
	PlayerPlaySoundEx(1133, soPos[0], soPos[1], soPos[2]);

	AddVehicleComponent(vehicleID, 1010); 
	va_SendClientMessage(playerid, COLOR_WHITE, "You have applied nitrous to your %s for $1,000.", VehicleNames[vehicleModel - 400]);

	playerVariables[playerid][pMoney] -= 1000;
	playerVariables[playerid][pJobDelay] = 60;
	
	return true;
}

YCMD:hydcar(playerid, params[], help) {
	if(jobVariables[playerVariables[playerid][pJob]][jJobType] != 3) return true;
	if(GetPlayerVehicleID(playerid) == 0) return SendClientMessage(playerid, COLOR_GREY, "You're not in any vehicle.");
	new vehicleID = GetPlayerVehicleID(playerid);  
	new Float:soPos[3], vehicleModel = GetVehicleModel(vehicleID);
	if(IsInvalidNOSVehicle(vehicleModel)) return va_SendClientMessage(playerid, COLOR_GREY, "You can't modify this %s.", VehicleNames[vehicleModel - 400]);
	if(playerVariables[playerid][pJobDelay] != 0) return va_SendClientMessage(playerid, COLOR_GREY, "You need to wait %d seconds until you can use a mechanic command again.",playerVariables[playerid][pJobDelay]);

	GetVehiclePos(vehicleID, soPos[0], soPos[1], soPos[2]);
	PlayerPlaySoundEx(1133, soPos[0], soPos[1], soPos[2]);

	AddVehicleComponent(vehicleID, 1087);
	va_SendClientMessage(playerid, COLOR_WHITE, "You have applied hydraulics to your %s for $1,000.", VehicleNames[vehicleModel - 400]);

	playerVariables[playerid][pMoney] -= 1000;
	playerVariables[playerid][pJobDelay] = 60;
	
	return true;
} 

YCMD:getmats(playerid, params[], help) {
    if(jobVariables[playerVariables[playerid][pJob]][jJobType] != 1) return true;
	if(!IsPlayerInRangeOfPoint(playerid, 5, 1423.9871, -1319.2954, 13.5547)) return true;
	if(playerVariables[playerid][pCheckpoint] != 0) return va_SendClientMessage(playerid, COLOR_WHITE, "You already have an active checkpoint (%s), reach it first, or /killcheckpoint.", getPlayerCheckpointReason(playerid));
	if(playerVariables[playerid][pMoney] < 1000) return SendClientMessage(playerid, COLOR_GREY, "You need to pay $1000 to collect materials.");
	
	SetPlayerCheckpoint(playerid, 2166.6870, -2272.5073, 13.3623, 10);
	SendClientMessage(playerid, COLOR_WHITE, "Reach the checkpoint to collect your materials.");

	playerVariables[playerid][pCheckpoint] = 2;
	playerVariables[playerid][pMoney] -= 1000;
	playerVariables[playerid][pMatrunTime] = 1; 

	return true;
}

YCMD:dropcar(playerid, params[], help) {
	if(playerVariables[playerid][pCheckpoint] != 0) return va_SendClientMessage(playerid, COLOR_WHITE, "You already have an active checkpoint (%s), reach it first, or /killcheckpoint.", getPlayerCheckpointReason(playerid));
	if(playerVariables[playerid][pDropCarTimeout] >= 1) return SendClientMessage(playerid, COLOR_GREY, "You can't drop a vehicle as you still have time to wait. Check /time.");

	playerVariables[playerid][pCheckpoint] = 3;
	SendClientMessage(playerid, COLOR_WHITE, "Reach the checkpoint to drop your vehicle off at the crane.");
	SetPlayerCheckpoint(playerid, 2699.2781, -2225.4299, 13.5501, 10);
	
	return true;
}

YCMD:creategun(playerid, params[], help) {
	/*if(jobVariables[playerVariables[playerid][pJob]][jJobType] == 1) {
		if(playerVariables[playerid][pFreezeType] == 0) {
			if(playerVariables[playerid][pJobDelay] == 0) {
				switch(playerVariables[playerid][pJobSkill][0]) {
					case 0 .. 49: ShowPlayerDialog(playerid, DIALOG_CREATEGUN, DIALOG_STYLE_LIST, "Weapon Selection", "Katana (30)\nCane (30)\nPool Cue (33)\nBaseball Bat (35)\nShovel (50)","Select", "Cancel");
					case 50 .. 99: ShowPlayerDialog(playerid, DIALOG_CREATEGUN, DIALOG_STYLE_LIST, "Weapon Selection", "Katana (30)\nCane (30)\nPool Cue (33)\nBaseball Bat (35)\nShovel (50)\n9mm pistol (250)","Select", "Cancel");
					case 100 .. 149: ShowPlayerDialog(playerid, DIALOG_CREATEGUN, DIALOG_STYLE_LIST, "Weapon Selection", "Katana (30)\nCane (30)\nPool Cue (33)\nBaseball Bat (35)\nShovel (50)\n9mm Pistol (250)\nSilenced Pistol (300)","Select", "Cancel");
					case 150 .. 199: ShowPlayerDialog(playerid, DIALOG_CREATEGUN, DIALOG_STYLE_LIST, "Weapon Selection", "Katana (30)\nCane (30)\nPool Cue (33)\nBaseball Bat (35)\nShovel (50)\n9mm Pistol (250)\nSilenced Pistol (300)\nShotgun (550)","Select", "Cancel");
					case 200 .. 249: ShowPlayerDialog(playerid, DIALOG_CREATEGUN, DIALOG_STYLE_LIST, "Weapon Selection", "Katana (30)\nCane (30)\nPool Cue (33)\nBaseball Bat (35)\nShovel (50)\n9mm Pistol (250)\nSilenced Pistol (300)\nShotgun (550)\nDesert Eagle (680)","Select", "Cancel");
					case 250 .. 299: ShowPlayerDialog(playerid, DIALOG_CREATEGUN, DIALOG_STYLE_LIST, "Weapon Selection", "Katana (30)\nCane (30)\nPool Cue (33)\nBaseball Bat (35)\nShovel (50)\n9mm Pistol (250)\nSilenced Pistol (300)\nShotgun (550)\nDesert Eagle (680)\nMP5 (850)","Select", "Cancel");
					case 300 .. 349: ShowPlayerDialog(playerid, DIALOG_CREATEGUN, DIALOG_STYLE_LIST, "Weapon Selection", "Katana (30)\nCane (30)\nPool Cue (33)\nBaseball Bat (35)\nShovel (50)\n9mm Pistol (250)\nSilenced Pistol (300)\nShotgun (550)\nDesert Eagle (680)\nMP5 (850)\nMicro Uzi (900)","Select", "Cancel");
					case 350 .. 399: ShowPlayerDialog(playerid, DIALOG_CREATEGUN, DIALOG_STYLE_LIST, "Weapon Selection", "Katana (30)\nCane (30)\nPool Cue (33)\nBaseball Bat (35)\nShovel (50)\n9mm Pistol (250)\nSilenced Pistol (300)\nShotgun (550)\nDesert Eagle (680)\nMP5 (850)\nMicro Uzi (900)\nAK-47 (1500)","Select", "Cancel");
					case 400 .. 449: ShowPlayerDialog(playerid, DIALOG_CREATEGUN, DIALOG_STYLE_LIST, "Weapon Selection", "Katana (30)\nCane (30)\nPool Cue (33)\nBaseball Bat (35)\nShovel (50)\n9mm Pistol (250)\nSilenced Pistol (300)\nShotgun (550)\nDesert Eagle (680)\nMP5 (850)\nMicro Uzi (900)\nAK-47 (1500)\nM4A1 (2000)","Select", "Cancel");
					case 450 .. 499: ShowPlayerDialog(playerid, DIALOG_CREATEGUN, DIALOG_STYLE_LIST, "Weapon Selection", "Katana (30)\nCane (30)\nPool Cue (33)\nBaseball Bat (35)\nShovel (50)\n9mm Pistol (250)\nSilenced Pistol (300)\nShotgun (550)\nDesert Eagle (680)\nMP5 (850)\nMicro Uzi (900)\nAK-47 (1500)\nM4A1 (2000)\nSniper (2450)","Select", "Cancel");
					default: ShowPlayerDialog(playerid, DIALOG_CREATEGUN, DIALOG_STYLE_LIST, "Weapon Selection", "Katana (30)\nCane (30)\nPool Cue (33)\nBaseball Bat (35)\nShovel (50)\n9mm Pistol (250)\nSilenced Pistol (300)\nShotgun (550)\nDesert Eagle (680)\nMP5 (850)\nMicro Uzi (900)\nAK-47 (1500)\nM4A1 (2000)\nSniper (2450)\nSPAS12 (2550)\nKevlar Vest (1750)","Select", "Cancel");
				}
			}
			else SendClientMessage(playerid, COLOR_GREY, "You must wait your reload time (30 seconds).");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You can't do this while cuffed, tazed, or frozen.");
	}*/
	return 1;
}

YCMD:getjob(playerid, params[], help) {
    if(playerVariables[playerid][pStatus] < 1) return true;
	if(playerVariables[playerid][pJob] != 0) return SendClientMessage(playerid, COLOR_WHITE, "You already have a job (type /quitjob first).");
	
	foreach(new h : jobs) {
		if(!IsPlayerInRangeOfPoint(playerid, 5, jobVariables[h][jJobPosition][0], jobVariables[h][jJobPosition][1], jobVariables[h][jJobPosition][2]))
			continue;
		 
		va_SendClientMessage(playerid, COLOR_WHITE, "Congratulations. You have now become a %s.", jobVariables[h][jJobName]);
		playerVariables[playerid][pJob] = h; 

		break;
	}
	
	return true;
}

YCMD:quitjob(playerid, params[], help) {
    if(playerVariables[playerid][pStatus] < 1) return true;
	if(playerVariables[playerid][pJob] == 0) return true;
	
	va_SendClientMessage(playerid, COLOR_WHITE, "You have quit your job (%s).", jobVariables[playerVariables[playerid][pJob]][jJobName]);
	
	playerVariables[playerid][pJob] = 0;
	playerVariables[playerid][pJobDelay] = 0;
	
	return true;
} 