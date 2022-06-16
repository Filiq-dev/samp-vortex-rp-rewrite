/*
						Copyright 2010-2011 Frederick Wright

		   Licensed under the Apache License, Version 2.0 (the "License");
		   you may not use this file except in compliance with the License.
		   You may obtain a copy of the License at

		     		http://www.apache.org/licenses/LICENSE-2.0

		   Unless required by applicable law or agreed to in writing, software
		   distributed under the License is distributed on an "AS IS" BASIS,
		   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
		   See the License for the specific language governing permissions and
		   limitations under the License.

		INITIAL SCRIPT:
		    Vortex Roleplay 2 Script

        Rewrited by Filiqp_
            Vortex Roleplay 3 Script

		AUTHOR:
			Frederick Wright [mrfrederickwright@gmail.com]
			Stefan Rosic [streetfire68@hotmail.com]
			Filipqp_ [filipmacheama@gmail.com]

		ADDITIONAL CREDITS:
		    All other unmentioned mapping: JamesC [http://forum.sa-mp.com/member.php?u=97617]
			Gym Map: Marcel_Collins [http://forum.sa-mp.com/showthread.php?p=1537421]
			LS Mall: cessil [http://forum.sa-mp.com/member.php?u=50597]

		MISC INFO:
			gGroupType listing:
				0 - Gangs
				1 - Police
				2 - Government
				3 - Hitmen
				4 - LSFMD

				Reserved group slots
				1 - LSPD
				3 - Government
				4 - LSFMD

				Job Types
				1 - Arms Dealer
				2 - Detective
				3 - Mechanic
				4 - Fisherman
				
				Business Item Types:
				1 - Rope
				2 - Walkie Talkie
				3 - Phonebook
				4 - Mobile Phone Credit
				5 - Mobile Phone
				6 - 5% health increase (food)
				7 - 10% health increase (food)
				8 - 30% health increase (food)
				9 - Purple Dildo
				10 - Small White Vibrator
				11 - Large White Vibrator
				12 - Silver Vibrator
				13 - Flowers
				14 - Cigar(s)
				15 - Sprunk
				16 - Wine
				17 - Beer
				18 - All Skins

			Error Codes:
				01x01 - Attempted to deposit an invalid (negative) amount of money to a house safe.
				01x02 - Attempted to deposit an invalid (negative) amount of materials to a house safe.
				01x03 - Attempted to withdraw an invalid (negative) amount of money from a house safe.
				01x04 - Attempted to withdraw an invalid (negative) amount of materials from a house safe.
				01x05 - No checkpoint reason. The checkpoint handle hasn't had a string defined in getPlayerCheckpointReason()
				01x08 - Too many vehicles spawned (in danger of exceeding MAX_VEHICLES).

			Business Types:
			    0 - None
			    1 - 24/7
				2 - Clothing Store
				3 - Bar
				4 - Sex Shop
				5 - Car Dealership
				6 - Gym
				7 - Restaurant
*/  

#include "modules/core/init.inc"

main() {
    #if defined DEBUG
        print("main() has been called");
    #endif
}

public OnGameModeInit() {
    #if defined DEBUG
        print("OnGameModeInit() has been called");
    #endif

	mysql_tquery(gSQL, "UPDATE playeraccounts SET playerStatus = '0' WHERE playerStatus = '1'", "", "");

    SetGameModeText(SERVER_NAME" "SERVER_VERSION);
	GetConsoleVarAsString("weburl", szServerWebsite, sizeof(szServerWebsite));

    ShowPlayerMarkers(0);
	EnableStuntBonusForAll(false);
	DisableInteriorEnterExits();
	UsePlayerPedAnims();

    print("-----------------------------------------------------------------");
	print("Script: Vortex Roleplay 2 by Calgon and Brian.");
	print("Script: Rewrited by Filiqp_ for open.mp");
	print("Status: Loaded OnGameModeInit, running version "SERVER_VERSION);
	print("-----------------------------------------------------------------");
	
	if(strfind(SERVER_VERSION, "BETA", true) != -1) {
	    print("-----------------------------------------------------------------");
	    print("WARNING: You are running a BETA version of the script.");
	    print("WARNING: This script is not optimized (or specifically built) for public usage yet.");
	    print("-----------------------------------------------------------------");
	}

	bullshit();

    return true;
}

public OnGameModeExit() {
    #if defined DEBUG
        print("OnGameModeExit() has been called");
    #endif  

    return true;
} 

public OnPlayerConnect(playerid) {
    #if defined DEBUG
        printf("public OnPlayerConnect(%d) has been called", playerid);
    #endif

    /*
	    (a) Attempts must be made to protect players from access to explicit content. If your
	        server contains elements that may be considered only suitable for adults, your server
	        must state this fact to the player when they first join.
	*/
	SendClientMessage(playerid, COLOR_LIGHTRED, "WARNING: This server contains explicit content which requires you to be 18+ to play here.");

    SetPlayerMapIcon(playerid, 10, 595.5443, -1250.3405, 18.2836, 52, 0); // idk what are doing this here but i will leave here for now

    /* Mall object removal - 0.3d */
    // Remove the original mall mesh
	RemoveBuildingForPlayer(playerid, 6130, 1117.5859, -1490.0078, 32.7188, 10.0);

	// This is the mall mesh LOD
	RemoveBuildingForPlayer(playerid, 6255, 1117.5859, -1490.0078, 32.7188, 10.0);

	// There are some trees on the outside of the mall which poke through one of the interiors
	RemoveBuildingForPlayer(playerid, 762, 1175.3594, -1420.1875, 19.8828, 0.25);
	RemoveBuildingForPlayer(playerid, 615, 1166.3516, -1417.6953, 13.9531, 0.25); 

    return true;
}

public OnPlayerDisconnect(playerid, reason) {
    #if defined DEBUG
        printf("public OnPlayerDisconnect(%d) has been called", playerid);
    #endif

	if(!Iter_Contains(Admin, playerid)) {
		switch(reason) {
			case 1: nearByMessage(playerid, COLOR_GENANNOUNCE, 12.0, "%s has left the server.", playerVariables[playerid][pNormalName]);
			case 2:	nearByMessage(playerid, COLOR_GENANNOUNCE, 12.0, "%s has been kicked or banned from the server.", playerVariables[playerid][pNormalName]);
			default: nearByMessage(playerid, COLOR_GENANNOUNCE, 12.0, "%s has timed out from the server.", playerVariables[playerid][pNormalName]);
		}
	}

	if(playerVariables[playerid][pFreezeType] >= 1 && playerVariables[playerid][pFreezeType] <= 4) {
		playerVariables[playerid][pPrisonTime] = 900;
		playerVariables[playerid][pPrisonID] = 2;
	}
	if(playerVariables[playerid][pDrag] != -1) {
		SendClientMessage(playerVariables[playerid][pDrag], COLOR_WHITE, "The person you were dragging has disconnected.");
		playerVariables[playerVariables[playerid][pDrag]][pDrag] = -1; // Kills off any disconnections.
	}
	if(playerVariables[playerid][pPhoneCall] != -1 && playerVariables[playerid][pPhoneCall] < MAX_PLAYERS) {

		SendClientMessage(playerVariables[playerid][pPhoneCall], COLOR_WHITE, "Your call has been terminated by the other party.");

		if(GetPlayerSpecialAction(playerVariables[playerid][pPhoneCall]) == SPECIAL_ACTION_USECELLPHONE) {
			SetPlayerSpecialAction(playerVariables[playerid][pPhoneCall], SPECIAL_ACTION_STOPUSECELLPHONE);
		}

		playerVariables[playerVariables[playerid][pPhoneCall]][pPhoneCall] = -1;
	} 
	if(playerVariables[playerid][pCarModel] >= 1) {
		DestroyVehicle(playerVariables[playerid][pCarID]);
		// systemVariables[vehicleCounts][1]--; to do on iterators
		playerVariables[playerid][pCarID] = -1;
	}
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	#if defined DEBUG
        printf("public OnPlayerKeyStateChange(%d, %d, %d) has been called", playerid, newkeys, oldkeys);
    #endif  
	// Disregard any key state changes if the player is frozen and prevent any further code from being executed

	if(playerVariables[playerid][pFreezeType] != 0 && playerVariables[playerid][pFreezeTime] != 0)
	    return false;

	if(IsKeyJustDown(KEY_SUBMISSION, newkeys, oldkeys)) {
	    if(GetVehicleModel(GetPlayerVehicleID(playerid)) == 525) { // For impounding cars.

	        new
				playerTowTruck = GetPlayerVehicleID(playerid);

	        if(!IsTrailerAttachedToVehicle(playerTowTruck)) {
				new
					targetVehicle = GetClosestVehicle(playerid, playerTowTruck); // Exempt the player's own vehicle from the loop.

				if(!IsAPlane(targetVehicle) && IsPlayerInRangeOfVehicle(playerid, targetVehicle, 10.0)) {
					AttachTrailerToVehicle(targetVehicle, playerTowTruck);
				}
	        }
	        else DetachTrailerFromVehicle(playerTowTruck);
	    }
	}
	if(IsKeyJustDown(KEY_FIRE, newkeys, oldkeys)) {
		if(GetPlayerWeapon(playerid) == 17 && !IsPlayerInAnyVehicle(playerid) && playerVariables[playerid][pFreezeType] == 0) {
			foreach(new i : Player) {
				if(playerid != i && !IsPlayerInAnyVehicle(i) && playerVariables[i][pFreezeType] == 0 && GetPlayerSkin(i) != 285) {
					if(IsPlayerAimingAtPlayer(playerid, i)) {

						playerVariables[i][pFreezeType] = 5; // Using 5 on FreezeType makes more sense
						playerVariables[i][pFreezeTime] = 10;
						TogglePlayerControllable(i, false);
						SetPlayerDrunkLevel(i, 50000);
						ApplyAnimation(i, "FAT", "IDLE_TIRED", 4.1, true, true, true, true, false, true);
					}
				}
			}
		}
	}
    
	if(IsKeyJustDown(KEY_SECONDARY_ATTACK, newkeys, oldkeys)) { 
		if(IsPlayerInRangeOfPoint(playerid,1.0,237.9,115.6,1010.2)) {
			SetPlayerPos(playerid,237.9,115.6,1010.2);
			SetPlayerFacingAngle(playerid, 270);
			ApplyAnimation(playerid, "VENDING", "VEND_Use", 1.0, false, false, false, false, 4000);
			SetTimerEx("VendDrink", 2500, false, "d", playerid);
		}
	}
	if(IsKeyJustDown(KEY_CROUCH, newkeys, oldkeys)) {  
		if(IsPlayerInRangeOfPoint(playerid, 2.0, 595.5443,-1250.3405,18.2836)) {
			SetPlayerPos(playerid, 2306.8481,-16.0682,26.7496);
			SetPlayerVirtualWorld(playerid, 2);
		}
		else if(IsPlayerInRangeOfPoint(playerid, 2.0, 2306.8481,-16.0682,26.7496)) {
			SetPlayerPos(playerid, 595.5443,-1250.3405,18.2836);
			SetPlayerVirtualWorld(playerid, 0);
		}  
	}
	
	return true;
}

public OnPlayerStateChange(playerid, newstate, oldstate) {
	#if defined DEBUG
	    printf("[debug] OnPlayerStateChange(%d, %d, %d)", playerid, newstate, oldstate);
	#endif
	
	if(newstate == 3) {
		if(IsAPlane(GetPlayerVehicleID(playerid))) {
			givePlayerValidWeapon(playerid, 46);
		}
	}
	else if(newstate == 2) { // Removed the passenger check, as it caused weapons to bug.
		if(playerVariables[playerid][pEvent] == 0) {
			ResetPlayerWeapons(playerid);
			givePlayerWeapons(playerid);
		}
		if(IsAPlane(GetPlayerVehicleID(playerid))) {
			givePlayerValidWeapon(playerid, 46);
		}
/*
		for(new i = 0; i < MAX_VEHICLES; i++) {
		    if(vehicleVariables[i][vVehicleScriptID] == GetPlayerVehicleID(playerid) && vehicleVariables[i][vVehicleGroup] != 0 && vehicleVariables[i][vVehicleGroup] != playerVariables[playerid][pGroup]) {

				if(playerVariables[playerid][pAdminLevel] >= 1 && playerVariables[playerid][pAdminDuty] >= 1) {
					format(szMessage, sizeof(szMessage), "This %s (model %d, ID %d) is locked to group %s (%d).", VehicleNames[GetVehicleModel(i) - 400], GetVehicleModel(i), i, groupVariables[vehicleVariables[i][vVehicleGroup]][gGroupName], vehicleVariables[i][vVehicleGroup]);
					SendClientMessage(playerid, COLOR_GREY, szMessage);
					return 1;
				}
				else {
					SendClientMessage(playerid, COLOR_GREY, "This vehicle is locked.");
					RemovePlayerFromVehicle(playerid);
					return 1;
				}
			}
        }
		remake system
*/ 
		foreach(new x : Player) {
			if(playerVariables[x][pCarID] == GetPlayerVehicleID(playerid)) {
				if(playerVariables[playerid][pAdminLevel] >= 1 && playerVariables[playerid][pAdminDuty] >= 1) { 
					va_SendClientMessage(playerid, COLOR_GREY, "This %s (model %d, ID %d) is owned by %s.", VehicleNames[playerVariables[x][pCarModel] - 400], playerVariables[x][pCarModel], playerVariables[x][pCarID], getName(x));
					break;
				}
				else if(playerVariables[x][pCarLock] == 1) {
					RemovePlayerFromVehicle(playerid);
					SendClientMessage(playerid, COLOR_GREY, "This vehicle is locked.");
					break;
				}
			}
		}
		

		// Confirm the old state was on foot and if they should be frozen, then remove them
		if(oldstate == 1 && playerVariables[playerid][pFreezeType] != 0 && playerVariables[playerid][pFreezeTime] != 0)
  			RemovePlayerFromVehicle(playerid);
    }

	foreach(new x : Player) {
		if(playerVariables[x][pSpectating] != INVALID_PLAYER_ID && playerVariables[x][pSpectating] == playerid) {
			if(newstate == 2 && oldstate == 1 || newstate == 3 && oldstate == 1) {
				PlayerSpectateVehicle(x, GetPlayerVehicleID(playerid));
			}
			else {
				PlayerSpectatePlayer(x, playerid);
			}
		}
	}
	
	return 1;
}  

static charCount[MAX_PLAYERS][3]; // if we create in the function, everytime when a player text something in chat, we will create this var and its not optim for server
public e_COMMAND_ERRORS:OnPlayerCommandReceived(playerid, cmdtext[], e_COMMAND_ERRORS:success) {
	#if defined DEBUG
	    printf("OnPlayerCommandReceived(%d, %s) has been called", playerid, cmdtext);
		printf("[server] [cmd] %s (ID %d): %s", getName(playerid), playerid, cmdtext);
	#endif
	
	if(GetPVarInt(playerid, "pAdminFrozen") == 1) Kick(playerid); 
	if(playerVariables[playerid][pStatus] != 1) return COMMAND_DISABLED;  
	if(playerVariables[playerid][pMuted] > 0) {
		SendClientMessage(playerid, COLOR_GREY, "You cannot submit any commands or text at the moment, as you have been muted.");
		return COMMAND_ZERO_RET;
	}

	playerVariables[playerid][pSpamCount]++;

	for(new i; i < strlen(cmdtext); i++) switch(cmdtext[i]) {
		case '0' .. '9': charCount[playerid][0]++;
		case '.': charCount[playerid][1]++;
		case ':': charCount[playerid][2]++;
	}

	if(charCount[playerid][0] > 8 && charCount[playerid][1] >= 3 && charCount[playerid][2] >= 1 && playerVariables[playerid][pAdminLevel] < 1) { 
		submitToAdmins(COLOR_HOTORANGE, "Warning: {FFFFFF}%s may be server advertising: '%s'.", getName(playerid), cmdtext);

		return COMMAND_ZERO_RET;
	}
	return COMMAND_OK;
}

static iRetStr[MAX_PLAYERS];
public OnPlayerText(playerid, text[]) {
	#if defined DEBUG
	    printf("OnPlayerText(%d, %s) has been called", playerid, text);
	#endif

	if(playerVariables[playerid][pStatus] >= 1 && playerVariables[playerid][pMuted] == 0) {
		iRetStr[playerid] = strfind(text, "(", true, 0); 

		if(iRetStr[playerid] < 4 && iRetStr[playerid] != -1 && playerVariables[playerid][pAdminDuty] == 0) { 
			nearByMessage(playerid, COLOR_WHITE, 12.0, "%s says: (( %s ))", getName(playerid), text);

			return true;
		}

		if(playerVariables[playerid][pPhoneCall] != -1) {
			gString[0] = (EOS);
			format(gString, sizeof(gString), "(cellphone) \"%s\"", text);
			SetPlayerChatBubble(playerid, gString, COLOR_CHATBUBBLE, 10.0, 10000);

			nearByMessage(playerid, COLOR_WHITE, 12.0, "(cellphone) %s says: %s", getName(playerid), text);

			switch (playerVariables[playerid][pPhoneCall]) {
				case 911: {
					if(!strcmp(text, "LSPD", true) || !strcmp(text, "police", true)) {
						SendClientMessage(playerid, COLOR_WHITE, "(cellphone) 911: You have reached the Los Santos Police emergency hotline; can you describe the crime?");
						playerVariables[playerid][pPhoneCall] = 912;
					}
					else if(!strcmp(text, "LSFMD", true) || !strcmp(text, "medic", true) || !strcmp(text, "ambulance", true)) {
						SendClientMessage(playerid, COLOR_WHITE, "(cellphone) 911: This is the Los Santos Fire & Medic Department emergency hotline; describe the emergency, please.");
						playerVariables[playerid][pPhoneCall] = 914;
					}
					else SendClientMessage(playerid, COLOR_WHITE, "(cellphone) 911: Sorry, I didn't quite understand that... speak again?");
				}
				case 912: {
					if(strlen(text) > 1) {
						new location[MAX_ZONE_NAME];

						GetPlayer2DZone(playerid, location, MAX_ZONE_NAME); 
						SendToGroup(1, COLOR_RADIOCHAT, "Dispatch: %s has reported: '%s' (10-20 %s)", getName(playerid), text, location);
						SendClientMessage(playerid, COLOR_WHITE, "(cellphone) 911: Thank you for reporting this incident; a patrol unit is now on its way.");
						SendClientMessage(playerid, COLOR_WHITE, "Your call has been terminated by the other party.");

						if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USECELLPHONE) {
							SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);
						}
						playerVariables[playerid][pPhoneCall] = -1;
					}
				}
				case 914: {
					if(strlen(text) > 1) {
						new location[MAX_ZONE_NAME];

						GetPlayer2DZone(playerid, location, MAX_ZONE_NAME);
						SendToGroupType(4, COLOR_RED, "Dispatch: %s has reported '%s' (10-20 %s)", getName(playerid), text, location);
						SendClientMessage(playerid, COLOR_WHITE, "(cellphone) 911: Thank you for reporting this incident; we are on our way.");
						SendClientMessage(playerid, COLOR_WHITE, "Your call has been terminated by the other party.");

						if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USECELLPHONE)
							SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);

						playerVariables[playerid][pPhoneCall] = -1;
					}
				}
				default: { // If they're calling a player, this code is executed.
					va_SendClientMessage(playerVariables[playerid][pPhoneCall], COLOR_GREY, "(cellphone) %s says: %s", getName(playerid), text);
					// mysql_real_escape_string(szMessage, szMessage);
					// format(szLargeString, sizeof(szLargeString), "INSERT INTO chatlogs (value, playerinternalid) VALUES('%s', '%d')", szMessage, playerVariables[playerid][pInternalID]);
					// mysql_query(szLargeString);
				}
			}
		}

		else {
		    if(playerVariables[playerid][pAdminDuty] >= 1) nearByMessage(playerid, COLOR_GREY, 12.0, "%s says: (( %s ))", getName(playerid), text);
			else nearByMessage(playerid, COLOR_GREY, 12.0, "{FFFFFF}%s says: %s", getName(playerid), text);

			// mysql_real_escape_string(szMessage, szMessage);
			// format(szLargeString, sizeof(szLargeString), "INSERT INTO chatlogs (value, playerinternalid) VALUES('%s', '%d')", szMessage, playerVariables[playerid][pInternalID]);
			// mysql_query(szLargeString);

			gString[0] = (EOS);
			format(gString, sizeof(gString), "\"%s\"", text);
			SetPlayerChatBubble(playerid, gString, COLOR_CHATBUBBLE, 10.0, 10000);
		}

		playerVariables[playerid][pSpamCount]++;
	}
	return false;
}

public OnVehicleSpawn(vehicleid) {
	#if defined DEBUG
	    printf("OnVehicleSpawn(%d) has been called", vehicleid);
	#endif
	
	switch(GetVehicleModel(vehicleid)) {
		case 427, 428, 432, 601, 528: SetVehicleHealth(vehicleid, 5000.0); // Enforcer, Securicar, Rhino, SWAT Tank, FBI truck - this is the armour plating dream come true.
	}

	return true;
}

public OnPlayerRequestSpawn(playerid) {
	if(playerVariables[playerid][pFirstLogin] >= 1)
	    return false;

	return true;
}

public OnPlayerSpawn(playerid) {
	#if defined DEBUG
	    printf("OnPlayerSpawn(%d) has been called", playerid);
	#endif

	PreloadAnimLib(playerid,"BOMBER");
	PreloadAnimLib(playerid,"RAPPING");
	PreloadAnimLib(playerid,"SHOP");
	PreloadAnimLib(playerid,"BEACH");
	PreloadAnimLib(playerid,"SMOKING");
	PreloadAnimLib(playerid,"ON_LOOKERS");
	PreloadAnimLib(playerid,"DEALER");
	PreloadAnimLib(playerid,"CRACK");
	PreloadAnimLib(playerid,"CARRY");
	PreloadAnimLib(playerid,"COP_AMBIENT");
	PreloadAnimLib(playerid,"PARK");
	PreloadAnimLib(playerid,"INT_HOUSE");
	PreloadAnimLib(playerid,"FOOD");
	PreloadAnimLib(playerid,"GANGS");
	PreloadAnimLib(playerid,"PED");
	PreloadAnimLib(playerid,"FAT"); 

	SetPlayerColor(playerid, COLOR_WHITE);
	SetPlayerFightingStyle(playerid, playerVariables[playerid][pFightStyle]);

    SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, 998);
    SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, 998); // Skilled, but not dual-wield.

	if(playerVariables[playerid][pHospitalized] >= 1)
	    return initiateHospital(playerid);
	
	if(playerVariables[playerid][pPrisonTime] >= 1) {
	    switch(playerVariables[playerid][pPrisonID]) {
			case 1: {
			    SetPlayerPos(playerid, -26.8721, 2320.9290, 24.3034);
				SetPlayerInterior(playerid, 0);
				SetPlayerVirtualWorld(playerid, 0);
			}
			case 2: {
				SetPlayerPos(playerid, 264.58, 77.38, 1001.04);
				SetPlayerInterior(playerid, 6);
				SetPlayerVirtualWorld(playerid, 0);
			}
			case 3: {

				SetPlayerInterior(playerid, 10);
				SetPlayerVirtualWorld(playerid, GROUP_VIRTUAL_WORLD+1);

				new spawn = random(sizeof(JailSpawns));

				SetPlayerPos(playerid, JailSpawns[spawn][0], JailSpawns[spawn][1], JailSpawns[spawn][2]);
				SetPlayerFacingAngle(playerid, 0);
			}
		}
		return 1;
	}  

	SetPlayerSkin(playerid, playerVariables[playerid][pSkin]);
	SetPlayerPos(playerid, playerVariables[playerid][pPos][0], playerVariables[playerid][pPos][1], playerVariables[playerid][pPos][2]);
	SetPlayerInterior(playerid, playerVariables[playerid][pInterior]);
	SetPlayerVirtualWorld(playerid, playerVariables[playerid][pVirtualWorld]);
	SetCameraBehindPlayer(playerid);

	playerVariables[playerid][pSkinSet] = 1;

	ResetPlayerWeapons(playerid);
	givePlayerWeapons(playerid);

	if(playerVariables[playerid][pEvent] >= 1)
		playerVariables[playerid][pEvent] = 0;

	if(playerVariables[playerid][pAdminDuty] == 1) {
		SetPlayerHealth(playerid, 500000.0);
	}
	else {
		SetPlayerHealth(playerid, playerVariables[playerid][pHealth]);
		SetPlayerArmour(playerid, playerVariables[playerid][pArmour]);
	} 
	TogglePlayerControllable(playerid, true);

	return true;
}
/*
??
public OnPlayerShootPlayer(Shooter,Target,Float:HealthLost,Float:ArmourLost) {
	if(playerVariables[Shooter][pTazer] == 1 && groupVariables[playerVariables[Shooter][pGroup]][gGroupType] == 1 && playerVariables[Shooter][pGroup] != 0 && GetPlayerWeapon(Shooter) == 22) {
	    if(IsPlayerInAnyVehicle(Target) || IsPlayerInAnyVehicle(Shooter))
	        return 1;

		if(groupVariables[playerVariables[Target][pGroup]][gGroupType] == 1 && playerVariables[Target][pGroup] != 0)
		    return 1; 

		TogglePlayerControllable(Target, 0);
		GameTextForPlayer(Target, "~n~~r~ Tazed!",4000, 4);

		playerVariables[Target][pFreezeTime] = 15;
		playerVariables[Target][pFreezeType] = 1;
 
		nearByMessage(Shooter, COLOR_PURPLE, 12.0, "* %s fires their tazer at %s, stunning them.", getName(Shooter), getName(Target)); 
		va_SendClientMessage(Shooter, COLOR_NICESKY, "You have successfully stunned %s.", getName(Target));

		ApplyAnimation(Target,"CRACK","crckdeth2",4.1,0,1,1,1,1,1);
	}
	return true;
}
*/
public OnVehicleRespray(playerid, vehicleid, color1, color2) {
	#if defined DEBUG
	    printf("[debug] OnVehicleRespray(%d, %d, %d, %d)", playerid, vehicleid, color1, color2);
	#endif

	/* With modifications, we don't need to do this as there's already a GetVehicleComponentInSlot function.
	However, this will save paint if a player who doesn't own the car is driving. */
	SetPVarInt(playerid, "pC", 1);
	foreach(new v : Player) {
		if(GetPlayerVehicleID(playerid) == playerVariables[v][pCarID]) {
			playerVariables[v][pCarColour][0] = color1;
			playerVariables[v][pCarColour][1] = color2;
		}
	}
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid) { // No need to deduct money; thanks to SA:MP, OnVehicleRespray is called when a paint job has been applied.
	#if defined DEBUG
	    printf("[debug] OnVehiclePaintjob(%d, %d, %d)", playerid, vehicleid, paintjobid);
	#endif
	
	SetPVarInt(playerid, "pC", 1);
	foreach(new v : Player) {
		if(GetPlayerVehicleID(playerid) == playerVariables[v][pCarID]) {
			playerVariables[v][pCarPaintjob] = paintjobid;
		}
	}
}

public OnEnterExitModShop(playerid, enterexit, interiorid) {
	#if defined DEBUG
	    printf("[debug] OnEnterExitModShop(%d, %d, %d)", playerid, enterexit, interiorid);
	#endif

	if(enterexit == 0) {
		if(GetPVarInt(playerid, "pC") == 1) {
			playerVariables[playerid][pMoney] -= 500;
			DeletePVar(playerid, "pC");
		}
		foreach(new v : Player) {
			if(GetPlayerVehicleID(playerid) == playerVariables[v][pCarID]) {
				for(new i = 0; i < 13; i++) {
					playerVariables[v][pCarMods][i] = GetVehicleComponentInSlot(playerVariables[v][pCarID], i);
				}
			}
		}
	}
}

public OnVehicleMod(playerid, vehicleid, componentid) {
	#if defined DEBUG
	    printf("[debug] OnVehicleMod(%d, %d, %d)", playerid, vehicleid, componentid);
	#endif
	
	if(GetPlayerInterior(playerid) < 1 && GetPlayerInterior(playerid) > 3 && playerVariables[playerid][pAdminLevel] < 3) { 
		submitToAdmins(COLOR_HOTORANGE, "AdmWarn: {FFFFFF}%s may possibly be hacking vehicle mods (added component %d to their %s).", getName(playerid), componentid, VehicleNames[GetVehicleModel(vehicleid) - 400]);
	}

	else if(GetPlayerInterior(playerid) >= 1 && GetPlayerInterior(playerid) <= 3) {

		switch(componentid) { // Get the price for the vehicle component, only if they're in a mod garage.

			case 1024:												playerVariables[playerid][pMoney] -= 50;
			case 1006:  											playerVariables[playerid][pMoney] -= 80;
			case 1004, 1145, 1013, 1091, 1086:						playerVariables[playerid][pMoney] -= 100;
			case 1005, 1143, 1022, 1035, 1088:						playerVariables[playerid][pMoney] -= 150;
			case 1021, 1009, 1002, 1016, 1068, 1153:				playerVariables[playerid][pMoney] -= 200;
			case 1011:												playerVariables[playerid][pMoney] -= 220;
			case 1012, 1020, 1003, 1067:							playerVariables[playerid][pMoney] -= 250;
			case 1019:												playerVariables[playerid][pMoney] -= 300;
			case 1018, 1023, 1093:									playerVariables[playerid][pMoney] -= 350;
			case 1014, 1000:										playerVariables[playerid][pMoney] -= 400;
			case 1163, 1090, 1070:									playerVariables[playerid][pMoney] -= 450;
			case 1008, 1007, 1017, 1015, 1044, 1043, 1036:		   	playerVariables[playerid][pMoney] -= 500;
			case 1045:												playerVariables[playerid][pMoney] -= 510;
			case 1001, 1158, 1069, 1164:							playerVariables[playerid][pMoney] -= 550;
			case 1050, 1058, 1097:									playerVariables[playerid][pMoney] -= 620;
			case 1162, 1089:										playerVariables[playerid][pMoney] -= 650;
			case 1028, 1085:										playerVariables[playerid][pMoney] -= 770;
			case 1122, 1106, 1108, 1118:							playerVariables[playerid][pMoney] -= 780;
			case 1134:												playerVariables[playerid][pMoney] -= 800;
			case 1082:												playerVariables[playerid][pMoney] -= 820;
			case 1064, 1133:										playerVariables[playerid][pMoney] -= 830;
			case 1165, 1167, 1065:									playerVariables[playerid][pMoney] -= 850;
			case 1175, 1177, 1172, 1080:							playerVariables[playerid][pMoney] -= 900;
			case 1100, 1119, 1192:									playerVariables[playerid][pMoney] -= 940;
			case 1173, 1161, 1166, 1168:							playerVariables[playerid][pMoney] -= 950;
			case 1010, 1149, 1176, 1042, 1136, 1025, 1096, 1174:   	playerVariables[playerid][pMoney] -= 1000;
			case 1155, 1154:										playerVariables[playerid][pMoney] -= 1030;
			case 1160, 1159:										playerVariables[playerid][pMoney] -= 1050;
			case 1150:												playerVariables[playerid][pMoney] -= 1090;
			case 1193, 1073:										playerVariables[playerid][pMoney] -= 1100;
			case 1190, 1078:										playerVariables[playerid][pMoney] -= 1200;
			case 1135, 1087:										playerVariables[playerid][pMoney] -= 1500;
			case 1083, 1076:										playerVariables[playerid][pMoney] -= 1560;
			case 1179, 1184:										playerVariables[playerid][pMoney] -= 2150;
			case 1046:												playerVariables[playerid][pMoney] -= 710;
			case 1152:												playerVariables[playerid][pMoney] -= 910;
			case 1151:												playerVariables[playerid][pMoney] -= 840;
			case 1054:												playerVariables[playerid][pMoney] -= 210;
			case 1053:												playerVariables[playerid][pMoney] -= 130;
			case 1049:												playerVariables[playerid][pMoney] -= 810;
			case 1047:												playerVariables[playerid][pMoney] -= 670;
			case 1048:												playerVariables[playerid][pMoney] -= 530;
			case 1066:												playerVariables[playerid][pMoney] -= 750;
			case 1034:												playerVariables[playerid][pMoney] -= 790;
			case 1037:												playerVariables[playerid][pMoney] -= 690;
			case 1171:												playerVariables[playerid][pMoney] -= 990;
			case 1148:												playerVariables[playerid][pMoney] -= 890;
			case 1038:												playerVariables[playerid][pMoney] -= 190;
			case 1146:												playerVariables[playerid][pMoney] -= 490;
			case 1039:												playerVariables[playerid][pMoney] -= 390;
			case 1059:												playerVariables[playerid][pMoney] -= 720;
			case 1157:												playerVariables[playerid][pMoney] -= 930;
			case 1156:												playerVariables[playerid][pMoney] -= 920;
			case 1055:												playerVariables[playerid][pMoney] -= 230;
			case 1061:												playerVariables[playerid][pMoney] -= 180;
			case 1060:												playerVariables[playerid][pMoney] -= 530;
			case 1056:												playerVariables[playerid][pMoney] -= 520;
			case 1057:												playerVariables[playerid][pMoney] -= 430;
			case 1029:												playerVariables[playerid][pMoney] -= 680;
			case 1169:												playerVariables[playerid][pMoney] -= 970;
			case 1170:												playerVariables[playerid][pMoney] -= 880;
			case 1141:												playerVariables[playerid][pMoney] -= 980;
			case 1140:												playerVariables[playerid][pMoney] -= 870;
			case 1032:												playerVariables[playerid][pMoney] -= 170;
			case 1033:												playerVariables[playerid][pMoney] -= 120;
			case 1138:												playerVariables[playerid][pMoney] -= 580;
			case 1139:												playerVariables[playerid][pMoney] -= 470;
			case 1026:												playerVariables[playerid][pMoney] -= 480;
			case 1031:												playerVariables[playerid][pMoney] -= 370;
			case 1092:												playerVariables[playerid][pMoney] -= 750;
			case 1128:												playerVariables[playerid][pMoney] -= 3340;
			case 1103:												playerVariables[playerid][pMoney] -= 3250;
			case 1183:												playerVariables[playerid][pMoney] -= 2040;
			case 1182:												playerVariables[playerid][pMoney] -= 2130;
			case 1181:												playerVariables[playerid][pMoney] -= 2050;
			case 1104:												playerVariables[playerid][pMoney] -= 1610;
			case 1105:												playerVariables[playerid][pMoney] -= 1540;
			case 1126:												playerVariables[playerid][pMoney] -= 3340;
			case 1127:												playerVariables[playerid][pMoney] -= 3250;
			case 1185:												playerVariables[playerid][pMoney] -= 2040;
			case 1180:												playerVariables[playerid][pMoney] -= 2130;
			case 1178:												playerVariables[playerid][pMoney] -= 2050;
			case 1123:												playerVariables[playerid][pMoney] -= 860;
			case 1125:												playerVariables[playerid][pMoney] -= 1120;
			case 1130:												playerVariables[playerid][pMoney] -= 3380;
			case 1131:												playerVariables[playerid][pMoney] -= 3290;
			case 1189:												playerVariables[playerid][pMoney] -= 2200;
			case 1188:												playerVariables[playerid][pMoney] -= 2080;
			case 1187:												playerVariables[playerid][pMoney] -= 2175;
			case 1186:												playerVariables[playerid][pMoney] -= 2095;
			case 1129:												playerVariables[playerid][pMoney] -= 1650;
			case 1132:												playerVariables[playerid][pMoney] -= 1590;
			case 1113:												playerVariables[playerid][pMoney] -= 3340;
			case 1114:												playerVariables[playerid][pMoney] -= 3250;
			case 1117:												playerVariables[playerid][pMoney] -= 2040;
			case 1115:												playerVariables[playerid][pMoney] -= 2130;
			case 1116:												playerVariables[playerid][pMoney] -= 2050;
			case 1109:												playerVariables[playerid][pMoney] -= 1610;
			case 1110:												playerVariables[playerid][pMoney] -= 1540;
			case 1191:												playerVariables[playerid][pMoney] -= 1040;
			case 1079:												playerVariables[playerid][pMoney] -= 1030;
			case 1075:												playerVariables[playerid][pMoney] -= 980;
			case 1077:												playerVariables[playerid][pMoney] -= 1620;
			case 1074:												playerVariables[playerid][pMoney] -= 1030;
			case 1081:												playerVariables[playerid][pMoney] -= 1230;
			case 1084:												playerVariables[playerid][pMoney] -= 1350;
			case 1098:												playerVariables[playerid][pMoney] -= 1140;
		}
	}
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid) {
	#if defined DEBUG
	    printf("[debug] OnPlayerExitVehicle(%d, %d)", playerid, vehicleid);
	#endif
	
	return SetPlayerArmedWeapon(playerid, 0);
}

public OnVehicleStreamIn(vehicleid, forplayerid) {
	#if defined DEBUG
	    printf("[debug] OnVehicleStreamIn(%d, %d)", vehicleid, forplayerid);
	#endif
	
	foreach(new x : Player) {
	    if(playerVariables[x][pCarID] == vehicleid && playerVariables[x][pCarLock] == 1) {
			SetVehicleParamsForPlayer(vehicleid, forplayerid, 0, 1);
	    }
	}
	return 1;
}  

public OnPlayerDeath(playerid, killerid, reason) {
	#if defined DEBUG
	    printf("[debug] OnPlayerDeath(%d, %d, %d)", playerid, killerid, reason);
	#endif 
  
	if(playerVariables[playerid][pAdminDuty] == 1) {
		GetPlayerPos(playerid, playerVariables[playerid][pPos][0], playerVariables[playerid][pPos][1], playerVariables[playerid][pPos][2]);
	}
	else playerVariables[playerid][pHospitalized] = 1;
	
	return true;
} 

public OnPlayerEnterCheckpoint(playerid) {
	#if defined DEBUG
	    printf("[debug] OnPlayerEnterCheckpoint(%d)", playerid);
	#endif
	
	switch(playerVariables[playerid][pCheckpoint]) {
	    case 1: {
	        SendClientMessage(playerid, COLOR_WHITE, "You have reached your destination.");
	        DisablePlayerCheckpoint(playerid);

	        playerVariables[playerid][pCheckpoint] = 0;
	    }
		case 2: {
		    if(playerVariables[playerid][pMatrunTime] < 30) { // why is this check just here?
				submitToAdmins(COLOR_HOTORANGE, "AdmWarn: {FFFFFF}%s may possibly be teleport matrunning (reached checkpoint in %d seconds).", getName(playerid), playerVariables[playerid][pMatrunTime]);
			}
		    else {
		        SendClientMessage(playerid, COLOR_WHITE, "You have collected 100 materials!");
		        DisablePlayerCheckpoint(playerid);

		        playerVariables[playerid][pCheckpoint] = 0;
		        playerVariables[playerid][pMaterials] += 100;
		        playerVariables[playerid][pMatrunTime] = 0;
	        }
	    }
	    case 3: {
			if(!IsPlayerInAnyVehicle(playerid))
				return SendClientMessage(playerid, COLOR_GREY, "You aren't in a vehicle; please get a vehicle to drop off at the crane.");

			else if(playerVariables[playerid][pCarID] == GetPlayerVehicleID(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You can't sell your own vehicle here.");

			foreach(new v : Player) {
				if(playerVariables[v][pCarID] == GetPlayerVehicleID(playerid)) {
					DestroyVehicle(GetPlayerVehicleID(playerid)); // If an owned car is destroyed... it'll be manually despawned...

					playerVariables[v][pCarPos][0] = 2157.5559; // ...moved to the LS junk yard...
					playerVariables[v][pCarPos][1] = -1977.6494;
					playerVariables[v][pCarPos][2] = 13.3835;
					playerVariables[v][pCarPos][3] = 177.3687; // have its Z angle set

					// SpawnPlayerVehicle(v); // And spawned. vehicles.inc 

					SetVehicleHealth(playerVariables[v][pCarID], 400.0); // A wrecked car is a wrecked car.
				}
				else SetVehicleToRespawn(GetPlayerVehicleID(playerid));
			}

			new 
				rand;

			switch(GetVehicleModel(GetPlayerVehicleID(playerid))) { // Thanks to Danny for these, lol
				case 405: rand = random(2000) + 2500;
				case 561: rand = random(2000) + 2750;
				case 535: rand = random(2000) + 2250;
				case 463: rand = random(2000) + 2000;
				case 461: rand = random(2000) + 2500;
				case 429: rand = random(2000) + 4500;
				case 451: rand = random(2000) + 4750;
				case 491: rand = random(2000) + 1800;
				case 492: rand = random(2000) + 1500;
				case 603: rand = random(2000) + 3800;
				case 502: rand = random(2000) + 5000;
				case 558: rand = random(2000) + 2500;
				case 554: rand = random(2000) + 1900;
				case 588: rand = random(2000) + 2300;
				case 518: rand = random(2000) + 2250;
				case 475: rand = random(2000) + 2300;
				case 542: rand = random(2000) + 2000;
				case 466: rand = random(2000) + 2400;
				case 462: rand = random(2000) + 500;
				case 596: rand = random(2000) + 3000;
				case 427: rand = random(2000) + 4500;
				case 528: rand = random(2000) + 4250;
				case 601: rand = random(2000) + 5000;
				case 523: rand = random(2000) + 3000;
				case 600: rand = random(2000) + 2250;
				case 468: rand = random(2000) + 2000;
				case 418: rand = random(2000) + 2100;
				case 482: rand = random(2000) + 2750;
				case 440: rand = random(2000) + 2250;
				case 587: rand = random(2000) + 3800;
				case 412: rand = random(2000) + 2500;
				case 534: rand = random(2000) + 2700;
				case 536: rand = random(2000) + 2600;
				case 567: rand = random(2000) + 2650;
				case 448: rand = random(2000) + 550;
				case 602: rand = random(2000) + 3100;
				case 586: rand = random(2000) + 2200;
				case 421: rand = random(2000) + 2900;
				case 581: rand = random(2000) + 2250;
				case 521: rand = random(2000) + 2750;
				case 598: rand = random(2000) + 3250;
				case 574: rand = random(2000) + 750;
				case 500: rand = random(2000) + 2700;
				case 579: rand = random(2000) + 3100;
				case 467: rand = random(2000) + 2000;
				case 426: rand = random(2000) + 2600;
				case 555: rand = random(2000) + 3250;
				case 437: rand = random(2000) + 4800;
				case 428: rand = random(2000) + 4750;
				case 442: rand = random(2000) + 2200;
				case 458: rand = random(2000) + 2000;
				case 527: rand = random(2000) + 1950;
				case 496: rand = random(2000) + 2100;
				case 400: rand = random(2000) + 3000;
				case 605: rand = random(2000) + 900;
				case 604: rand = random(2000) + 900;
				case 522: rand = random(2000) + 5000;
				case 438: rand = random(2000) + 2750;
				case 420: rand = random(2000) + 2600;
				case 404: rand = random(2000) + 1250;
				case 585: rand = random(2000) + 2400;
				case 543: rand = random(2000) + 2000;
				case 515: rand = random(2000) + 4500;
				case 560: rand = random(2000) + 3900;
				case 409: rand = random(2000) + 2950;
				case 402: rand = random(2000) + 3250;
				default: rand = random(2000) + 1000;
			}

			playerVariables[playerid][pDropCarTimeout] = 1800;
            playerVariables[playerid][pCheckpoint] = 0;
            DisablePlayerCheckpoint(playerid);
			playerVariables[playerid][pMoney] += rand;
 
            va_SendClientMessage(playerid, COLOR_WHITE, "You have dropped your vehicle at the crane and earned $%d!", rand);
		}
		case 4: {

	        SendClientMessage(playerid, COLOR_WHITE, "You have reached your vehicle.");
	        DisablePlayerCheckpoint(playerid);

	        playerVariables[playerid][pCheckpoint] = 0;

		}
		case 5: {
			if(playerVariables[playerid][pBackup] != -1) {

				SendClientMessage(playerid, COLOR_WHITE, "You have reached the backup checkpoint.");
				DisablePlayerCheckpoint(playerid);

				playerVariables[playerid][pCheckpoint] = 0;
				playerVariables[playerid][pBackup] = -1;
			}
		}
    	default: {
			DisablePlayerCheckpoint(playerid);
			playerVariables[playerid][pCheckpoint] = 0;
		}
    }
	return 1;
}

public OnPlayerUpdate(playerid)
{

	if(playerVariables[playerid][pOnRequest] != INVALID_PLAYER_ID) {
	    new
			Keys,
			ud,
			lr;

	    GetPlayerKeys(playerid, Keys, ud, lr);

	    if(lr > 0) {
	        GetPlayerPos(playerVariables[playerid][pOnRequest], playerVariables[playerVariables[playerid][pOnRequest]][pPos][0], playerVariables[playerVariables[playerid][pOnRequest]][pPos][1], playerVariables[playerVariables[playerid][pOnRequest]][pPos][2]);

	        SetPlayerPos(playerid, playerVariables[playerVariables[playerid][pOnRequest]][pPos][0], playerVariables[playerVariables[playerid][pOnRequest]][pPos][1], playerVariables[playerVariables[playerid][pOnRequest]][pPos][2]);
			TextDrawHideForPlayer(playerid, textdrawVariables[1]);

			SendClientMessage(playerid, COLOR_WHITE, "You have teleported to the player who has requested help.");

			playerVariables[playerid][pOnRequest] = INVALID_PLAYER_ID;
		}
	    else if(lr < 0) {
			TextDrawHideForPlayer(playerid, textdrawVariables[1]);
			playerVariables[playerid][pOnRequest] = INVALID_PLAYER_ID;
		}
	}

	if(playerVariables[playerid][pTabbed] == 1) {
		playerVariables[playerid][pTabbed] = 0;
		DestroyDynamic3DTextLabel(playerVariables[playerid][pAFKLabel]);
		if(playerVariables[playerid][pOutstandingWeaponRemovalSlot] >= 1) {
		    if(playerVariables[playerid][pOutstandingWeaponRemovalSlot] == 40) {
		        ResetPlayerWeapons(playerid);
		    }
		    else {
			    ResetPlayerWeapons(playerid);
				playerVariables[playerid][pWeapons][playerVariables[playerid][pOutstandingWeaponRemovalSlot]] = 0;
				givePlayerWeapons(playerid);
			}
			playerVariables[playerid][pAnticheatExemption] = 6;
		}
	} 
	playerVariables[playerid][pConnectedSeconds] = gettime();
	return 1;
} 

function VendDrink(playerid) {
    new
		Float:health;

	ApplyAnimation(playerid, "VENDING", "VEND_Drink_P", 1.0, false, false, false, false, 1750);
	GetPlayerHealth(playerid,health);
	if(health > 65.0) SetPlayerHealth(playerid,100.0); // This limits player health to 100 (as values over 100.0 could otherwise be achieved, which isn't good).
	else SetPlayerHealth(playerid,health+35.0); // A Sprunk machine gives exactly 35.0 HP per hit.
	return 1;
} 

function bullshit() {
	CreateDynamic3DTextLabel("Materials Pickup!\n\nType /getmats as an Arms Dealer \nto collect materials!", COLOR_YELLOW, 1423.9871, -1319.2954, 13.5547, 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
	CreateDynamicPickup(1239, 23, 1423.9871, -1319.2954, 13.5547, 0, -1, -1, 50);

	/* -------------------------------------- Mapping (objects, static 3D texts, static pickups) -------------------------------------- */

	// ls mall
	CreateDynamicObject(19322,1117.58000000,-1490.01000000,32.72000000,0.00000000,0.00000000,0.00000000); //
	CreateDynamicObject(19323,1117.58000000,-1490.01000000,32.72000000,0.00000000,0.00000000,0.00000000); //
	CreateDynamicObject(19325,1155.40000000,-1434.89000000,16.49000000,0.00000000,0.00000000,0.30000000); //
	CreateDynamicObject(19325,1155.37000000,-1445.41000000,16.31000000,0.00000000,0.00000000,0.00000000); //
	CreateDynamicObject(19325,1155.29000000,-1452.38000000,16.31000000,0.00000000,0.00000000,0.00000000); //
	CreateDynamicObject(19325,1157.36000000,-1468.35000000,16.31000000,0.00000000,0.00000000,18.66000000); //
	CreateDynamicObject(19325,1160.64000000,-1478.37000000,16.31000000,0.00000000,0.00000000,17.76000000); //
	CreateDynamicObject(19325,1159.84000000,-1502.06000000,16.31000000,0.00000000,0.00000000,-19.92000000); //
	CreateDynamicObject(19325,1139.28000000,-1523.71000000,16.31000000,0.00000000,0.00000000,-69.36000000); //
	CreateDynamicObject(19325,1117.06000000,-1523.43000000,16.51000000,0.00000000,0.00000000,-109.44000000); //
	CreateDynamicObject(19325,1097.18000000,-1502.43000000,16.51000000,0.00000000,0.00000000,-158.58000000); //
	CreateDynamicObject(19325,1096.47000000,-1478.29000000,16.51000000,0.00000000,0.00000000,-197.94000000); //
	CreateDynamicObject(19325,1099.70000000,-1468.27000000,16.51000000,0.00000000,0.00000000,-197.94000000); //
	CreateDynamicObject(19325,1101.81000000,-1445.45000000,16.22000000,0.00000000,0.00000000,-180.24000000); //
	CreateDynamicObject(19325,1101.76000000,-1452.47000000,16.22000000,0.00000000,0.00000000,-181.62000000); //
	CreateDynamicObject(19325,1101.77000000,-1434.88000000,16.22000000,0.00000000,0.00000000,-180.24000000); //
	CreateDynamicObject(19325,1094.31000000,-1444.92000000,23.47000000,0.00000000,0.00000000,-180.24000000); //
	CreateDynamicObject(19325,1094.37000000,-1458.37000000,23.47000000,0.00000000,0.00000000,-179.46000000); //
	CreateDynamicObject(19325,1093.01000000,-1517.44000000,23.44000000,0.00000000,0.00000000,-138.72000000); //
	CreateDynamicObject(19325,1101.08000000,-1526.64000000,23.42000000,0.00000000,0.00000000,-137.34000000); //
	CreateDynamicObject(19325,1155.12000000,-1526.38000000,23.46000000,0.00000000,0.00000000,-42.12000000); //
	CreateDynamicObject(19325,1163.09000000,-1517.25000000,23.46000000,0.00000000,0.00000000,-40.74000000); //
	CreateDynamicObject(19325,1163.04000000,-1442.06000000,23.40000000,0.00000000,0.00000000,-0.12000000); //
	CreateDynamicObject(19325,1163.09000000,-1428.47000000,23.50000000,0.00000000,0.00000000,0.54000000); //
	CreateDynamicObject(19326,1155.34000000,-1446.73000000,16.38000000,0.00000000,0.00000000,-89.82000000); //
	CreateDynamicObject(19326,1155.25000000,-1443.85000000,16.36000000,0.00000000,0.00000000,-89.82000000); //
	CreateDynamicObject(19326,1155.37000000,-1436.32000000,16.36000000,0.00000000,0.00000000,-89.82000000); //
	CreateDynamicObject(19326,1155.35000000,-1433.51000000,16.36000000,0.00000000,0.00000000,-89.70000000); //
	CreateDynamicObject(19329,1155.18000000,-1440.22000000,18.70000000,0.00000000,0.00000000,89.04000000); //
	CreateDynamicObject(19329,1161.59000000,-1431.50000000,17.93000000,0.00000000,0.00000000,0.00000000); //
	CreateDynamicObject(19329,1160.40000000,-1448.79000000,17.96000000,0.00000000,0.00000000,0.00000000); //
	CreateDynamicObject(2543,1168.18000000,-1436.39000000,14.79000000,0.00000000,0.00000000,0.30000000); //
	CreateDynamicObject(2535,1182.74000000,-1448.30000000,14.70000000,0.00000000,0.00000000,-90.96000000); //
	CreateDynamicObject(2543,1167.10000000,-1436.40000000,14.79000000,0.00000000,0.00000000,0.31000000); //
	CreateDynamicObject(2538,1172.31000000,-1435.32000000,14.79000000,0.00000000,0.00000000,180.34000000); //
	CreateDynamicObject(2539,1171.38000000,-1435.31000000,14.79000000,0.00000000,0.00000000,180.19000000); //
	CreateDynamicObject(2540,1169.56000000,-1435.36000000,14.79000000,0.00000000,0.00000000,180.17000000); //
	CreateDynamicObject(1984,1157.37000000,-1442.59000000,14.79000000,0.00000000,0.00000000,-450.06000000); //
	CreateDynamicObject(2012,1163.25000000,-1448.31000000,14.75000000,0.00000000,0.00000000,-179.16000000); //
	CreateDynamicObject(2012,1169.29000000,-1431.92000000,14.75000000,0.00000000,0.00000000,359.80000000); //
	CreateDynamicObject(1987,1163.13000000,-1436.34000000,14.79000000,0.00000000,0.00000000,361.06000000); //
	CreateDynamicObject(1988,1164.13000000,-1436.33000000,14.79000000,0.00000000,0.00000000,360.80000000); //
	CreateDynamicObject(2871,1164.79000000,-1443.96000000,14.79000000,0.00000000,0.00000000,177.73000000); //
	CreateDynamicObject(2871,1164.70000000,-1444.98000000,14.79000000,0.00000000,0.00000000,358.07000000); //
	CreateDynamicObject(2942,1155.52000000,-1464.68000000,15.43000000,0.00000000,0.00000000,-71.22000000); //
	CreateDynamicObject(1987,1164.12000000,-1435.32000000,14.77000000,0.00000000,0.00000000,180.96000000); //
	CreateDynamicObject(2530,1171.13000000,-1443.79000000,14.79000000,0.00000000,0.00000000,-182.16000000); //
	CreateDynamicObject(1991,1173.75000000,-1439.56000000,14.79000000,0.00000000,0.00000000,179.47000000); //
	CreateDynamicObject(1996,1169.82000000,-1439.50000000,14.79000000,0.00000000,0.00000000,179.10000000); //
	CreateDynamicObject(1996,1174.24000000,-1435.38000000,14.79000000,0.00000000,0.00000000,179.24000000); //
	CreateDynamicObject(1991,1175.23000000,-1435.39000000,14.79000000,0.00000000,0.00000000,179.57000000); //
	CreateDynamicObject(1995,1182.65000000,-1435.10000000,14.79000000,0.00000000,0.00000000,90.00000000); //
	CreateDynamicObject(1994,1182.66000000,-1438.07000000,14.79000000,0.00000000,0.00000000,90.00000000); //
	CreateDynamicObject(1993,1182.66000000,-1437.08000000,14.79000000,0.00000000,0.00000000,90.00000000); //
	CreateDynamicObject(2542,1163.78000000,-1443.92000000,14.76000000,0.00000000,0.00000000,178.77000000); //
	CreateDynamicObject(2536,1166.88000000,-1445.07000000,14.70000000,0.00000000,0.00000000,-0.42000000); //
	CreateDynamicObject(2542,1163.70000000,-1444.93000000,14.78000000,0.00000000,0.00000000,-1.74000000); //
	CreateDynamicObject(1984,1157.34000000,-1435.71000000,14.79000000,0.00000000,0.00000000,-450.06000000); //
	CreateDynamicObject(2012,1166.31000000,-1448.28000000,14.75000000,0.00000000,0.00000000,-180.12000000); //
	CreateDynamicObject(2530,1172.14000000,-1443.83000000,14.79000000,0.00000000,0.00000000,-181.38000000); //
	CreateDynamicObject(2530,1173.14000000,-1443.85000000,14.79000000,0.00000000,0.00000000,-180.96000000); //
	CreateDynamicObject(2530,1174.13000000,-1443.88000000,14.79000000,0.00000000,0.00000000,-181.50000000); //
	CreateDynamicObject(1981,1170.76000000,-1439.52000000,14.79000000,0.00000000,0.00000000,-181.74000000); //
	CreateDynamicObject(1981,1171.76000000,-1439.54000000,14.79000000,0.00000000,0.00000000,-180.80000000); //
	CreateDynamicObject(1981,1172.75000000,-1439.55000000,14.79000000,0.00000000,0.00000000,-180.84000000); //
	CreateDynamicObject(2535,1182.75000000,-1447.28000000,14.70000000,0.00000000,0.00000000,-90.78000000); //
	CreateDynamicObject(2535,1182.74000000,-1446.28000000,14.70000000,0.00000000,0.00000000,-90.78000000); //
	CreateDynamicObject(2535,1182.74000000,-1445.26000000,14.70000000,0.00000000,0.00000000,-90.00000000); //
	CreateDynamicObject(2541,1182.75000000,-1444.22000000,14.79000000,0.00000000,0.00000000,-90.06000000); //
	CreateDynamicObject(2541,1182.75000000,-1443.20000000,14.79000000,0.00000000,0.00000000,-90.06000000); //
	CreateDynamicObject(2541,1182.74000000,-1442.16000000,14.79000000,0.00000000,0.00000000,-90.06000000); //
	CreateDynamicObject(2543,1182.76000000,-1441.18000000,14.79000000,0.00000000,0.00000000,-90.84000000); //
	CreateDynamicObject(2541,1182.79000000,-1440.17000000,14.79000000,0.00000000,0.00000000,-90.06000000); //
	CreateDynamicObject(2543,1182.72000000,-1439.15000000,14.79000000,0.00000000,0.00000000,-90.84000000); //
	CreateDynamicObject(1990,1182.66000000,-1431.67000000,14.79000000,0.00000000,0.00000000,3.30000000); //
	CreateDynamicObject(1990,1181.63000000,-1431.73000000,14.79000000,0.00000000,0.00000000,3.30000000); //
	CreateDynamicObject(1990,1180.61000000,-1431.81000000,14.79000000,0.00000000,0.00000000,3.30000000); //
	CreateDynamicObject(1990,1179.61000000,-1431.83000000,14.79000000,0.00000000,0.00000000,3.30000000); //
	CreateDynamicObject(1990,1178.61000000,-1431.89000000,14.79000000,0.00000000,0.00000000,3.30000000); //
	CreateDynamicObject(1990,1177.59000000,-1431.86000000,14.79000000,0.00000000,0.00000000,3.30000000); //
	CreateDynamicObject(1993,1182.66000000,-1436.09000000,14.79000000,0.00000000,0.00000000,90.00000000); //
	CreateDynamicObject(2012,1175.50000000,-1431.82000000,14.75000000,0.00000000,0.00000000,361.17000000); //
	CreateDynamicObject(2012,1172.42000000,-1431.87000000,14.75000000,0.00000000,0.00000000,359.93000000); //
	CreateDynamicObject(2012,1160.10000000,-1448.35000000,14.75000000,0.00000000,0.00000000,-179.94000000); //
	CreateDynamicObject(2539,1170.45000000,-1435.33000000,14.79000000,0.00000000,0.00000000,181.26000000); //
	CreateDynamicObject(2545,1161.82000000,-1431.84000000,14.91000000,0.00000000,0.00000000,-90.54000000); //
	CreateDynamicObject(2545,1160.82000000,-1431.83000000,14.91000000,0.00000000,0.00000000,-90.54000000); //
	CreateDynamicObject(2545,1159.81000000,-1431.86000000,14.91000000,0.00000000,0.00000000,-90.54000000); //
	CreateDynamicObject(2545,1162.82000000,-1431.87000000,14.91000000,0.00000000,0.00000000,-90.54000000); //
	CreateDynamicObject(1988,1163.13000000,-1435.34000000,14.79000000,0.00000000,0.00000000,541.46000000); //
	CreateDynamicObject(1988,1166.07000000,-1436.32000000,14.79000000,0.00000000,0.00000000,360.80000000); //
	CreateDynamicObject(1987,1165.07000000,-1436.33000000,14.79000000,0.00000000,0.00000000,361.06000000); //
	CreateDynamicObject(1987,1166.11000000,-1435.30000000,14.77000000,0.00000000,0.00000000,180.96000000); //
	CreateDynamicObject(1988,1165.07000000,-1435.31000000,14.79000000,0.00000000,0.00000000,540.44000000); //
	CreateDynamicObject(2536,1165.79000000,-1445.07000000,14.70000000,0.00000000,0.00000000,-1.20000000); //
	CreateDynamicObject(2536,1167.83000000,-1445.07000000,14.70000000,0.00000000,0.00000000,-0.06000000); //
	CreateDynamicObject(2871,1165.79000000,-1444.00000000,14.79000000,0.00000000,0.00000000,178.27000000); //
	CreateDynamicObject(2871,1166.81000000,-1444.03000000,14.79000000,0.00000000,0.00000000,179.35000000); //
	CreateDynamicObject(2871,1167.79000000,-1444.04000000,14.79000000,0.00000000,0.00000000,179.89000000); //
	CreateDynamicObject(2543,1168.13000000,-1435.36000000,14.79000000,0.00000000,0.00000000,180.05000000); //
	CreateDynamicObject(2543,1167.10000000,-1435.37000000,14.79000000,0.00000000,0.00000000,180.35000000); //
	CreateDynamicObject(2012,1170.63000000,-1440.67000000,14.75000000,0.00000000,0.00000000,359.50000000); //
	CreateDynamicObject(2012,1173.77000000,-1440.72000000,14.75000000,0.00000000,0.00000000,359.82000000); //
	CreateDynamicObject(2012,1177.30000000,-1445.31000000,14.75000000,0.00000000,0.00000000,359.93000000); //
	CreateDynamicObject(1996,1173.36000000,-1448.30000000,14.79000000,0.00000000,0.00000000,179.10000000); //
	CreateDynamicObject(1981,1174.33000000,-1448.32000000,14.79000000,0.00000000,0.00000000,-181.74000000); //
	CreateDynamicObject(1981,1175.32000000,-1448.35000000,14.79000000,0.00000000,0.00000000,-180.84000000); //
	CreateDynamicObject(1981,1176.30000000,-1448.37000000,14.79000000,0.00000000,0.00000000,-180.84000000); //
	CreateDynamicObject(1991,1177.28000000,-1448.37000000,14.79000000,0.00000000,0.00000000,179.47000000); //
	CreateDynamicObject(1996,1178.33000000,-1448.36000000,14.79000000,0.00000000,0.00000000,179.24000000); //
	CreateDynamicObject(1991,1179.33000000,-1448.37000000,14.79000000,0.00000000,0.00000000,179.57000000); //
	CreateDynamicObject(1994,1176.82000000,-1444.16000000,14.79000000,0.00000000,0.00000000,-0.84000000); //
	CreateDynamicObject(1995,1178.81000000,-1444.20000000,14.79000000,0.00000000,0.00000000,-1.26000000); //
	CreateDynamicObject(2543,1168.89000000,-1444.06000000,14.79000000,0.00000000,0.00000000,178.97000000); //
	CreateDynamicObject(2543,1169.91000000,-1444.07000000,14.79000000,0.00000000,0.00000000,179.69000000); //
	CreateDynamicObject(2543,1169.87000000,-1445.12000000,14.79000000,0.00000000,0.00000000,-0.06000000); //
	CreateDynamicObject(2543,1168.86000000,-1445.11000000,14.79000000,0.00000000,0.00000000,0.31000000); //
	CreateDynamicObject(2538,1167.02000000,-1431.87000000,14.79000000,0.00000000,0.00000000,0.42000000); //
	CreateDynamicObject(2539,1166.03000000,-1431.89000000,14.79000000,0.00000000,0.00000000,0.70000000); //
	CreateDynamicObject(2540,1164.04000000,-1431.91000000,14.79000000,0.00000000,0.00000000,0.60000000); //
	CreateDynamicObject(2539,1165.03000000,-1431.91000000,14.79000000,0.00000000,0.00000000,1.02000000); //
	CreateDynamicObject(2538,1176.17000000,-1436.38000000,14.79000000,0.00000000,0.00000000,0.24000000); //
	CreateDynamicObject(2539,1174.22000000,-1436.37000000,14.79000000,0.00000000,0.00000000,-0.06000000); //
	CreateDynamicObject(2540,1173.22000000,-1436.36000000,14.79000000,0.00000000,0.00000000,0.18000000); //
	CreateDynamicObject(2539,1175.20000000,-1436.38000000,14.79000000,0.00000000,0.00000000,-2.06000000); //
	CreateDynamicObject(2540,1173.26000000,-1435.31000000,14.79000000,0.00000000,0.00000000,180.17000000); //
	CreateDynamicObject(1991,1175.74000000,-1439.58000000,14.79000000,0.00000000,0.00000000,179.57000000); //
	CreateDynamicObject(1996,1174.74000000,-1439.57000000,14.79000000,0.00000000,0.00000000,179.24000000); //
	CreateDynamicObject(1996,1176.17000000,-1435.37000000,14.79000000,0.00000000,0.00000000,179.24000000); //
	CreateDynamicObject(1991,1177.16000000,-1435.38000000,14.79000000,0.00000000,0.00000000,179.57000000); //
	CreateDynamicObject(2540,1169.44000000,-1436.35000000,14.79000000,0.00000000,0.00000000,0.18000000); //
	CreateDynamicObject(2539,1170.43000000,-1436.35000000,14.79000000,0.00000000,0.00000000,0.90000000); //
	CreateDynamicObject(2539,1171.34000000,-1436.33000000,14.79000000,0.00000000,0.00000000,0.58000000); //
	CreateDynamicObject(2538,1172.22000000,-1436.32000000,14.79000000,0.00000000,0.00000000,0.30000000); //
	CreateDynamicObject(2871,1163.40000000,-1440.68000000,14.79000000,0.00000000,0.00000000,360.41000000); //
	CreateDynamicObject(2536,1164.49000000,-1440.73000000,14.70000000,0.00000000,0.00000000,-1.20000000); //
	CreateDynamicObject(2536,1165.49000000,-1440.75000000,14.70000000,0.00000000,0.00000000,-0.42000000); //
	CreateDynamicObject(2536,1166.50000000,-1440.75000000,14.70000000,0.00000000,0.00000000,-0.06000000); //
	CreateDynamicObject(2543,1167.61000000,-1440.64000000,14.79000000,0.00000000,0.00000000,0.31000000); //
	CreateDynamicObject(2543,1168.62000000,-1440.64000000,14.79000000,0.00000000,0.00000000,0.30000000); //
	CreateDynamicObject(2543,1168.64000000,-1439.60000000,14.79000000,0.00000000,0.00000000,180.05000000); //
	CreateDynamicObject(2543,1167.67000000,-1439.61000000,14.79000000,0.00000000,0.00000000,180.35000000); //
	CreateDynamicObject(2871,1163.65000000,-1439.67000000,14.79000000,0.00000000,0.00000000,180.61000000); //
	CreateDynamicObject(2871,1164.68000000,-1439.67000000,14.79000000,0.00000000,0.00000000,179.77000000); //
	CreateDynamicObject(2871,1165.68000000,-1439.68000000,14.79000000,0.00000000,0.00000000,180.61000000); //
	CreateDynamicObject(2871,1166.68000000,-1439.66000000,14.79000000,0.00000000,0.00000000,180.61000000); //
	CreateDynamicObject(1990,1175.09000000,-1444.97000000,14.79000000,0.00000000,0.00000000,-2.46000000); //
	CreateDynamicObject(1990,1181.63000000,-1431.73000000,14.79000000,0.00000000,0.00000000,3.30000000); //
	CreateDynamicObject(1990,1174.07000000,-1444.94000000,14.79000000,0.00000000,0.00000000,0.48000000); //
	CreateDynamicObject(1990,1173.09000000,-1444.94000000,14.79000000,0.00000000,0.00000000,-1.20000000); //
	CreateDynamicObject(1990,1172.11000000,-1444.92000000,14.79000000,0.00000000,0.00000000,-1.14000000); //
	CreateDynamicObject(1990,1171.12000000,-1444.91000000,14.79000000,0.00000000,0.00000000,-0.72000000); //
	CreateDynamicObject(2530,1168.54000000,-1448.31000000,14.79000000,0.00000000,0.00000000,-178.98000000); //
	CreateDynamicObject(2530,1169.60000000,-1448.29000000,14.79000000,0.00000000,0.00000000,-178.98000000); //
	CreateDynamicObject(2530,1170.67000000,-1448.30000000,14.79000000,0.00000000,0.00000000,-178.98000000); //
	CreateDynamicObject(2530,1171.72000000,-1448.32000000,14.79000000,0.00000000,0.00000000,-181.50000000); //
	CreateDynamicObject(2530,1175.13000000,-1443.91000000,14.79000000,0.00000000,0.00000000,-181.50000000); //
	CreateDynamicObject(2012,1176.82000000,-1440.75000000,14.75000000,0.00000000,0.00000000,359.93000000); //
	CreateDynamicObject(1995,1177.71000000,-1439.63000000,14.79000000,0.00000000,0.00000000,0.00000000); //
	CreateDynamicObject(1994,1176.73000000,-1439.63000000,14.79000000,0.00000000,0.00000000,0.06000000); //
	CreateDynamicObject(1993,1177.83000000,-1444.15000000,14.79000000,0.00000000,0.00000000,179.46000000); //
	
	/*
	    --- CUSTOM MAP ---
	    
		Credits to: Marcel_Collins
		Release thread: http://forum.sa-mp.com/showthread.php?p=1537421
	*/
	
	CreateDynamicObject(1257,2242.38281250,-1725.93640137,13.82606697,0.00000000,0.00000000,90.00000000); //object(bustopm)(1)
	CreateDynamicObject(1229,2240.03955078,-1727.28039551,14.10655499,0.00000000,0.00000000,88.00000000); //object(bussign1)(1)
	CreateDynamicObject(1215,2224.59545898,-1712.75476074,13.11704731,0.00000000,0.00000000,0.00000000); //object(bollardlight)(1)
	CreateDynamicObject(1215,2236.68701172,-1725.17114258,13.11119843,0.00000000,0.00000000,0.00000000); //object(bollardlight)(3)
	CreateDynamicObject(1215,2221.71606445,-1723.97021484,13.12682343,0.00000000,0.00000000,0.00000000); //object(bollardlight)(4)
	CreateDynamicObject(1215,2225.08544922,-1726.94616699,13.12256432,0.00000000,0.00000000,0.00000000); //object(bollardlight)(5) (5)
	CreateDynamicObject(996,2230.76025391,-1727.23754883,13.29563046,0.00000000,0.00000000,0.00000000); //object(lhouse_barrier1)(1)
	CreateDynamicObject(997,2238.22485352,-1727.02954102,12.54687500,0.00000000,0.00000000,88.00000000); //object(lhouse_barrier3)(2)
	CreateDynamicObject(997,2225.60278320,-1727.18811035,12.65393353,0.00000000,0.00000000,0.00000000); //object(lhouse_barrier3)(3)
	CreateDynamicObject(997,2222.02197266,-1724.68554688,12.56250000,0.00000000,0.00000000,318.00000000); //object(lhouse_barrier3)(4)
	CreateDynamicObject(997,2221.68579102,-1719.86242676,12.53577995,0.00000000,0.00000000,266.00000000); //object(lhouse_barrier3)(5)
	CreateDynamicObject(996,2221.84472656,-1718.27014160,13.26626015,0.00000000,0.00000000,84.00000000); //object(lhouse_barrier1)(2)
	CreateDynamicObject(997,2223.02758789,-1710.96203613,12.58030415,0.00000000,0.00000000,0.00000000); //object(lhouse_barrier3)(7)

	/* Bank */
	CreateDynamicPickup(1239, 23, 595.5443,-1250.3405,18.2836, 0, -1, -1, 50);
	CreateDynamic3DTextLabel("Bank of Los Santos\nPress ~k~~PED_DUCK~ to enter", COLOR_YELLOW, 595.5443,-1250.3405,18.2836, 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
	/* /arrest */
	CreateDynamic3DTextLabel("Los Santos Police Department\nProcessing Entrance\n\n(/arrest)", COLOR_COOLBLUE, 1528.5240,-1678.2472,5.8906, 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 15.0);

	/* Exterior LSPD gates */
	LSPDGates[0][0] = CreateDynamicObject(968, 1544.681640625, -1630.8924560547, 13.15, 0.0, 90.0, 90.0, 0, 0, _, 200.0);
	LSPDGates[1][0] = CreateDynamicObject(10184,1589.19995117,-1637.98498535,14.69999981,0.00000000,0.00000000,270.00000000, 0, 0, _, 200.0);

	/* LSPD doors */
	LSPDObjs[0][0] = CreateDynamicObject(1569,232.89999390,107.57499695,1009.21179199,0.00000000,0.00000000,90.00000000, _, 10, _, 200.0); //commander south
	LSPDObjs[0][1] = CreateDynamicObject(1569,232.89941406,110.57499695,1009.21179199,0.00000000,0.00000000,270.00000000, _, 10, _, 200.0); //commander north
	LSPDObjs[1][0] = CreateDynamicObject(1569,275.75000000,118.89941406,1003.61718750,0.00000000,0.00000000,270.00000000, _, 10, _, 200.0); // interrogation north
	LSPDObjs[1][1] = CreateDynamicObject(1569,275.75000000,115.89941406,1003.61718750,0.00000000,0.00000000,90.00000000, _, 10, _, 200.0); // interrogation south
	LSPDObjs[2][0] = CreateDynamicObject(1569,253.20410156,107.59960938,1002.22070312,0.00000000,0.00000000,90.00000000, _,10, _, 200.0); // north west lobby door
	LSPDObjs[2][1] = CreateDynamicObject(1569,253.19921875,110.59960938,1002.22070312,0.00000000,0.00000000,270.00000000, _,10, _, 200.0); // north east lobby door
	LSPDObjs[3][0] = CreateDynamicObject(1569,239.56933594,116.09960938,1002.22070312,0.00000000,0.00000000,90.00000000, _,10, _, 200.0); // south west lobby door
	LSPDObjs[3][1] = CreateDynamicObject(1569,239.56445312,119.09960938,1002.22070312,0.00000000,0.00000000,269.98901367, _,10, _, 200.0); // south east lobby door
	LSPDObjs[4][0] = CreateDynamicObject(1569,264.45019531,115.82421875,1003.62286377,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(gen_doorext15) (3)
	LSPDObjs[4][1] = CreateDynamicObject(1569,267.45214844,115.82910156,1003.62286377,0.00000000,0.00000000,179.99450684, _,10, _, 200.0); //object(gen_doorext15) (8)
	LSPDObjs[5][0] = CreateDynamicObject(1569,267.32000732,112.53222656,1003.62286377,0.00000000,0.00000000,179.99450684, _,10, _, 200.0); //object(gen_doorext15) (4)
	LSPDObjs[5][1] = CreateDynamicObject(1569,264.32000732,112.52929688,1003.62286377,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(gen_doorext15) (5)
	LSPDObjs[6][0] = CreateDynamicObject(1569,229.59960938,119.52929688,1009.22442627,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(gen_doorext15) (9)
	LSPDObjs[6][1] = CreateDynamicObject(1569,232.59960938,119.53515625,1009.22442627,0.00000000,0.00000000,179.99450684, _,10, _, 200.0); //object(gen_doorext15) (10)
	LSPDObjs[7][0] = CreateDynamicObject(1569,219.30000305,116.52999878,998.01562500,0.00000000,0.00000000,180.00000000, _,10, _, 200.0); //cell east door
	LSPDObjs[7][1] = CreateDynamicObject(1569,216.30000305,116.52929688,998.01562500,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //cell west door

	/* LSPD interior objects (1st version) */
	CreateDynamicObject(1886,240.39999390,107.69999695,1010.70001221,35.00000000,0.00000000,135.00000000, _,10, _, 200.0); //object(nt_securecam1_01) (1)
	CreateDynamicObject(2058,262.23831177,107.09999847,1006.12506104,270.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(cj_gun_docs) (1)
	CreateDynamicObject(1491,222.17500305,119.45999908,1009.21502686,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(gen_doorint01) (1)
	CreateDynamicObject(1491,258.54980469,117.67968750,1007.82000732,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(gen_doorint01) (3)
	CreateDynamicObject(1491,260.73925781,117.67968750,1007.82000732,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(gen_doorint01) (4)
	CreateDynamicObject(2612,263.50000000,112.34960938,1005.50000000,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(police_nb2) (1)
	CreateDynamicObject(3857,233.04499817,124.00000000,1013.00000000,0.00000000,0.00000000,315.00000000, _,10, _, 200.0); //object(ottosmash3) (1)
	CreateDynamicObject(3857,232.73730469,124.00000000,1013.00000000,0.00000000,0.00000000,135.00012207, _,10, _, 200.0); //object(ottosmash3) (2)
	CreateDynamicObject(1491,225.05999756,115.94999695,1002.22998047,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(gen_doorint01) (2)
	CreateDynamicObject(1491,233.11000061,119.25000000,1002.22998047,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(gen_doorint01) (5)
	CreateDynamicObject(1491,236.80957031,119.25000000,1002.22998047,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(gen_doorint01) (6)
	CreateDynamicObject(3051,275.77499390,122.65599823,1004.97937012,0.00000000,0.00000000,46.00000000, _,10, _, 200.0); //object(lift_dr) (1)
	CreateDynamicObject(3051,275.75000000,121.50000000,1004.97937012,0.00000000,0.00000000,45.00000000, _,10, _, 200.0); //object(lift_dr) (2)
	CreateDynamicObject(1485,227.89999390,125.30000305,1010.21002197,50.00000000,10.00000000,2.00000000, _,10, _, 200.0); //object(cj_ciggy) (1)
	CreateDynamicObject(1510,228.07321167,125.27845001,1010.15997314,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(dyn_ashtry) (1)
	CreateDynamicObject(2196,228.40014648,125.53178406,1010.13958740,0.00000000,0.00000000,29.77478027, _,10, _, 200.0); //object(work_lamp1) (1)
	CreateDynamicObject(2063,262.95996094,107.40136719,1004.53997803,0.00000000,0.00000000,179.99450684, _,10, _, 200.0); //object(cj_greenshelves) (1)
	CreateDynamicObject(2043,262.29138184,107.46166229,1004.09997559,0.00000000,0.00000000,294.36035156, _,10, _, 200.0); //object(ammo_box_m4) (1)
	CreateDynamicObject(353,262.79998779297,107.68000030518,1004.9,91.9,89,240, _,10, _, 200.0); //object(cj_mp5k) (2)
	CreateDynamicObject(1672,262.62597656,107.59999847,1005.37500000,0.00000000,90.00000000,0.00000000, _,10, _, 200.0); //object(gasgrenade) (1)
	CreateDynamicObject(1672,262.81585693,107.48020935,1005.41998291,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(gasgrenade) (2)
	CreateDynamicObject(14782,267.76998901,109.30000305,1004.63323975,0.00000000,0.00000000,270.00000000, _,10, _, 200.0); //object(int3int_boxing30) (2)
	CreateDynamicObject(14782,260.79980469,108.75000000,1004.63323975,0.00000000,0.00000000,90.00000000, _,10, _, 200.0); //object(int3int_boxing30) (3)
	CreateDynamicObject(2359,263.54296875,107.39648438,1005.53002930,0.00000000,0.00000000,183.89465332, _,10, _, 200.0); //object(ammo_box_c5) (1)
	CreateDynamicObject(2038,263.47906494,107.32552338,1004.51000977,270.00000000,0.00000000,29.91000366, _,10, _, 200.0); //object(ammo_box_s2) (1)
	CreateDynamicObject(356,262.60000610352,107.30000305176,1004.4799804688,96, 90, 290, _,10, _, 200.0); //object(cj_m16) (1)
	CreateDynamicObject(2690,267.92782593,108.53081512,1003.97998047,0.00000000,0.00000000,312.13256836, _,10, _, 200.0); //object(cj_fire_ext) (1)
	CreateDynamicObject(2058,262.98568726,107.09528351,1005.36926270,90.00000000,180.00549316,359.98352051, _,10, _, 200.0); //object(cj_gun_docs) (1)
	CreateDynamicObject(11631,269.81250000,118.18945312,1004.86309814,0.00000000,0.00000000,270.00000000, _,10, _, 200.0); //object(ranch_desk) (1)
	CreateDynamicObject(2356,269.14312744,117.66873169,1003.61718750,0.00000000,0.00000000,294.49548340, _,10, _, 200.0); //object(police_off_chair) (1)
	CreateDynamicObject(2094,262.86523438,110.89941406,1003.60998535,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(swank_cabinet_4) (1)
	CreateDynamicObject(1886,267.73999023,107.50000000,1007.40002441,20.00000000,0.00000000,235.00000000, _,10, _, 200.0); //object(shop_sec_cam) (1)
	CreateDynamicObject(2606,267.36914062,120.50683594,1004.59997559,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(cj_police_counter2) (1)
	CreateDynamicObject(2606,267.36914062,120.50683594,1005.04998779,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(cj_police_counter2) (2)
	CreateDynamicObject(1738,270.29000854,120.00000000,1004.27178955,0.00000000,0.00000000,269.27026367, _,10, _, 200.0); //object(cj_radiator_old) (1)
	CreateDynamicObject(2180,265.50552368,120.27999878,1003.61718750,0.00000000,0.00000000,180.54052734, _,10, _, 200.0); //object(med_office5_desk_3) (1)
	CreateDynamicObject(1788,265.60000610,120.50000000,1004.48681641,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(swank_video_1) (1)
	CreateDynamicObject(1782,265.59960938,120.50000000,1004.65002441,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(med_video_2) (1)
	CreateDynamicObject(2595,264.21002197,120.37789154,1004.77404785,0.00000000,0.00000000,314.65002441, _,10, _, 200.0); //object(cj_shop_tv_video) (1)
	CreateDynamicObject(1785,265.59960938,120.50976562,1004.84997559,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(low_video_1) (1)
	CreateDynamicObject(1840,264.81204224,120.58029938,1004.41882324,0.00000000,0.00000000,105.60998535, _,10, _, 200.0); //object(speaker_2) (1)
	CreateDynamicObject(1840,265.70001221,120.55999756,1004.96264648,0.00000000,0.00000000,75.00000000, _,10, _, 200.0); //object(speaker_2) (2)
	CreateDynamicObject(2356,265.15481567,119.43829346,1003.61718750,0.00000000,0.00000000,34.19393921, _,10, _, 200.0); //object(police_off_chair) (2)
	CreateDynamicObject(1775,238.87988281,115.59960938,1010.32000732,0.00000000,0.00000000,270.26916504, _,10, _, 200.0); //object(vendmach) (1)
	CreateDynamicObject(4100,246.51953125,119.39941406,1005.40002441,0.00000000,179.99450684,219.99023438, _,10, _, 200.0); //object(meshfence1_lan) (1)
	CreateDynamicObject(4100,253.19999695,117.80000305,1010.50000000,320.00000000,90.00000000,90.00000000, _,10, _, 200.0); //object(pol_comp_gate) (1)
	CreateDynamicObject(2101,266.74893188,120.49598694,1005.28363037,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(med_hi_fi_3) (1)
	CreateDynamicObject(1886,264.25000000,116.55000305,1007.29998779,30.00000000,0.00000000,140.00000000, _,10, _, 200.0); //object(shop_sec_cam) (2)
	CreateDynamicObject(2611,268.47473145,116.05200195,1005.25000000,0.00000000,0.00000000,180.00000000, _,10, _, 200.0); //object(police_nb1) (1)
	CreateDynamicObject(4100,232.84960938,128.50000000,1011.91998291,0.00000000,0.00000000,49.99877930, _,10, _, 200.0); //object(meshfence1_lan) (4)
	CreateDynamicObject(2595,226.24514771,120.27544403,1011.28753662,0.00000000,0.00000000,77.72994995, _,10, _, 200.0); //object(cj_shop_tv_video) (2)
	CreateDynamicObject(3934,1563.90014648,-1700.00000000,27.40211487,0.00000000,0.00000000,0.00000000, 0, 0, _, 200.0); //object(helipad01) (2)
	CreateDynamicObject(1496,1564.14257812,-1667.36914062,27.39560699,0.00000000,0.00000000,0.00000000, 0, 0, _, 200.0); //object(gen_doorshop02) (1)
	CreateDynamicObject(2953,228.27796936,125.20470428,1010.14331055,0.00000000,0.00000000,143.45983887, _,10, _, 200.0); //object(kmb_paper_code) (1)
	CreateDynamicObject(4100,239.60000610,113.19999695,1010.50000000,319.99877930,90.00000000,90.00000000, _,10, _, 200.0); //object(pol_comp_gate) (1)
	CreateDynamicObject(2054,263.76342773,112.13343811,1004.64001465,0.00000000,0.00000000,36.00000000, _,10, _, 200.0); //object(cj_capt_hat) (1)
	CreateDynamicObject(2053,264.10845947,112.14072418,1004.66998291,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(cj_jerry_hat) (1)
	CreateDynamicObject(351,262.85000610352,111.90000152588,1004.6599731445,275,90,106, _,10, _, 200.0); //object(cj_m16) (2)
	CreateDynamicObject(2040,262.57006836,112.05036163,1004.72113037,0.00000000,0.00000000,342.13513184, _,10, _, 200.0); //object(ammo_box_m1) (1)
	CreateDynamicObject(2068,264.29998779,109.19999695,1007.00000000,0.00000000,0.00000000,90.00000000, _,10, _, 200.0); //object(cj_cammo_net) (1)
	CreateDynamicObject(1516,272.90374756,118.44168854,1003.79998779,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(dyn_table_03) (1)
	CreateDynamicObject(1810,272.74725342,117.44008636,1003.61718750,0.00000000,0.00000000,183.70996094, _,10, _, 200.0); //object(cj_foldchair) (1)
	CreateDynamicObject(1810,273.19308472,119.28445435,1003.61718750,0.00000000,0.00000000,2.00000000, _,10, _, 200.0); //object(cj_foldchair) (2)
	CreateDynamicObject(2953,272.84149170,118.41313934,1004.34997559,0.00000000,0.00000000,89.00000000, _,10, _, 200.0); //object(kmb_paper_code) (2)
	CreateDynamicObject(2953,272.89001465,118.30000305,1004.34997559,0.00000000,0.00000000,13.00000000, _,10, _, 200.0); //object(kmb_paper_code) (3)
	CreateDynamicObject(2196,273.04998779,118.69999695,1004.32000732,0.00000000,0.00000000,335.00000000, _,10, _, 200.0); //object(work_lamp1) (2)
	CreateDynamicObject(1886,228.80000305,116.00000000,1002.20001221,10.00000000,0.00000000,290.00000000, _,10, _, 200.0); //object(shop_sec_cam) (3)
	CreateDynamicObject(1491,265.17999268,112.68000031,1007.82000732,0.00000000,0.00000000,270.00000000, _,10, _, 200.0); //object(gen_doorint01) (4)
	CreateDynamicObject(2954,224.00000000,107.40000153,998.70062256,0.00000000,90.00000000,89.99993896, _,10, _, 200.0); //object(kmb_ot) (1)
	CreateDynamicObject(2954,228.19999695,107.39941406,998.70062256,0.00000000,90.00000000,90.00000000, _,10, _, 200.0); //object(kmb_ot) (2)
	CreateDynamicObject(2954,220.09960938,107.39941406,998.70062256,0.00000000,90.00000000,89.99996948, _,10, _, 200.0); //object(kmb_ot) (3)
	CreateDynamicObject(2954,216.10000610,107.39941406,998.70062256,0.00000000,90.00000000,90.00000000, _,10, _, 200.0); //object(kmb_ot) (4)
	CreateDynamicObject(1235,225.47909546,121.89310455,1009.72180176,0.00000000,0.00000000,0.00000000, _,10, _, 200.0); //object(wastebin) (1)
	CreateDynamicObject(2602,226.00000000,108.50000000,998.53906250,0.00000000,0.00000000,90.00000000, _,10, _, 200.0); //object(police_cell_toilet) (1)
	CreateDynamicObject(2602,214.00000000,108.50000000,998.53906250,0.00000000,0.00000000,90.00000000, _,10, _, 200.0); //object(police_cell_toilet) (2)
	CreateDynamicObject(2602,222.09960938,108.50000000,998.53906250,0.00000000,0.00000000,90.00000000, _,10, _, 200.0); //object(police_cell_toilet) (3)
	CreateDynamicObject(2602,218.10000610,108.50000000,998.53906250,0.00000000,0.00000000,90.00000000, _,10, _, 200.0); //object(police_cell_toilet) (4)
	CreateDynamicObject(8167,218.50000000,112.50000000,999.20001221,0.00000000,0.00000000,90.00000000, _,10, _, 200.0); //object(apgate1_vegs01) (1)
	CreateDynamicObject(8167,226.34960938,112.50000000,999.20001221,0.00000000,0.00000000,90.00000000, _,10, _, 200.0); //object(apgate1_vegs01) (2)
	CreateDynamicObject(3785,215.50000000,109.90000153,1001.40997314,0.00000000,90.00000000,0.00000000, _,10, _, 200.0); //object(bulkheadlight) (1)
	CreateDynamicObject(3785,219.50000000,109.89941406,1001.40997314,0.00000000,90.00000000,0.00000000, _,10, _, 200.0); //object(bulkheadlight) (2)
	CreateDynamicObject(3785,223.50000000,109.89941406,1001.40997314,0.00000000,90.00000000,0.00000000, _,10, _, 200.0); //object(bulkheadlight) (3)
	CreateDynamicObject(3785,227.50000000,109.89941406,1001.40997314,0.00000000,90.00000000,0.00000000, _,10, _, 200.0); //object(bulkheadlight) (4)

	/* Exterior LSPD objects */
	CreateDynamicObject(3934,1563.89941406,-1650.34277344,27.40211487,0.00000000,0.00000000,0.00000000, 0, 0, _, 200.0); //object(helipad01) (2)
	CreateDynamicObject(1496,1563.84997559,-1671.13000488,51.45027542,0.00000000,0.00000000,0.00000000, 0, 0, _, 200.0); //object(gen_doorshop02) (2)
	CreateDynamicObject(982,1577.75000000,-1701.50000000,28.07836533,0.00000000,0.00000000,0.00000000, 0, 0, _, 200.0); //object(fence) (1)
	CreateDynamicObject(982,1577.75000000,-1650.30004883,28.07836533,0.00000000,0.00000000,0.00000000, 0, 0, _, 200.0); //object(fence) (3)
	CreateDynamicObject(982,1565.00000000,-1637.50000000,28.07836533,0.00000000,0.00000000,90.00000000, 0, 0, _, 200.0); //object(fence) (4)
	CreateDynamicObject(984,1549.02502441,-1637.50000000,28.03879547,0.00000000,0.00000000,90.00000000, 0, 0, _, 200.0); //object(fence2) (1)
	CreateDynamicObject(982,1565.00000000,-1714.30004883,28.07836533,0.00000000,0.00000000,90.00000000, 0, 0, _, 200.0); //object(fencet) (5)
	CreateDynamicObject(982,1577.75000000,-1675.89941406,28.07836533,0.00000000,0.00000000,0.00000000, 0, 0, _, 200.0); //object(fencest) (6)
	CreateDynamicObject(984,1549.02441406,-1714.29980469,28.03879547,0.00000000,0.00000000,90.00000000, 0, 0, _, 200.0); //object(fenceshit2) (3)
	CreateDynamicObject(983,1550.59997559,-1701.50000000,28.07836533,0.00000000,0.00000000,90.00000000, 0, 0, _, 200.0); //object(fenceshit3) (2)
	CreateDynamicObject(984,1542.59960938,-1643.89941406,28.03879547,0.00000000,0.00000000,0.00000000, 0, 0, _, 200.0); //object(fenceshit2) (6)
	CreateDynamicObject(983,1545.79980469,-1701.50000000,28.07836533,0.00000000,0.00000000,90.00000000, 0, 0, _, 200.0); //object(fenceshit3) (3)
	CreateDynamicObject(983,1550.59997559,-1650.30004883,28.07836533,0.00000000,0.00000000,90.00000000, 0, 0, _, 200.0); //object(fenceshit3) (4)
	CreateDynamicObject(983,1545.79980469,-1650.30004883,28.07836533,0.00000000,0.00000000,90.00000000, 0, 0, _, 200.0); //object(fenceshit3) (5)
	CreateDynamicObject(984,1542.59960938,-1707.89941406,28.03879547,0.00000000,0.00000000,0.00000000, 0, 0, _, 200.0); //object(fenceshit2) (7)
	CreateDynamicObject(984,1553.80004883,-1695.09997559,28.03000069,0.00000000,0.00000000,0.00000000, 0, 0, _, 200.0); //object(fenceshit2) (8)
	CreateDynamicObject(984,1553.79980469,-1656.69995117,28.03000069,0.00000000,0.00000000,0.00000000, 0, 0, _, 200.0); //object(fenceshit2) (9)
	CreateDynamicObject(983,1544.69995117,-1620.58996582,13.02000046,0.00000000,0.00000000,0.00000000); //object(fenceshit3) (1)
	CreateDynamicObject(1331,1544.54602051,-1616.99133301,13.10000038,0.00000000,0.00000000,0.00000000); //object(binnt01_la) (1)
	CreateDynamicObject(2952,1582.00000000,-1637.88598633,12.39045906,0.00000000,0.00000000,90.00000000); //object(kmb_gimpdoor) (1)
	CreateDynamicObject(983,1544.69921875,-1636.00000000,13.02000046,0.00000000,0.00000000,0.00000000); //object(fenceshit3) (6)
	CreateDynamicObject(2952,1582.00000000,-1638.30004883,12.39045906,0.00000000,0.00000000,90.00000000); //object(kmb_gimpdoor) (2)

	/* Moar crap */
	CreateDynamicObject(2842,2320.79003906,-1021.39941406,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (2)
	CreateDynamicObject(2842,2320.79003906,-1023.19921875,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (3)
	CreateDynamicObject(2842,2320.79003906,-1025.00000000,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (4)
	CreateDynamicObject(2842,2319.87500000,-1019.59997559,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (5)
	CreateDynamicObject(2842,2319.87500000,-1017.79998779,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (6)
	CreateDynamicObject(2842,2319.87500000,-1016.00000000,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (7)
	CreateDynamicObject(2842,2319.87500000,-1014.20001221,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (8)
	CreateDynamicObject(2842,2319.87500000,-1012.40002441,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (9)
	CreateDynamicObject(2842,2319.87500000,-1010.59997559,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (10)
	CreateDynamicObject(2842,2320.79003906,-1010.59960938,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (11)
	CreateDynamicObject(2842,2320.79003906,-1012.39941406,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (12)
	CreateDynamicObject(2842,2320.79003906,-1014.19921875,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (13)
	CreateDynamicObject(2842,2320.79003906,-1016.00000000,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (14)
	CreateDynamicObject(2842,2320.79003906,-1017.79980469,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (15)
	CreateDynamicObject(2842,2320.79003906,-1019.59960938,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (16)
	CreateDynamicObject(2842,2319.87500000,-1021.39941406,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (17)
	CreateDynamicObject(2842,2319.87500000,-1023.19921875,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (18)
	CreateDynamicObject(2842,2319.87500000,-1025.00000000,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_bedrug04) (19)
	CreateDynamicObject(2069,2322.39306641,-1007.62664795,1049.30004883,0.00000000,0.00000000,0.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(cj_mlight7) (1)
	CreateDynamicObject(2297,2322.41992188,-1018.77001953,1049.21997070,0.00000000,0.00000000,356.03002930, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(tv_unit_2) (1)
	CreateDynamicObject(2069,2322.28906250,-1021.15917969,1049.26501465,0.00000000,0.00000000,0.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(cj_mlight7) (2)
	CreateDynamicObject(2073,2319.97973633,-1013.20001221,1052.93005371,0.00000000,0.00000000,0.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(cj_mlight1) (1)
	CreateDynamicObject(2332,2328.48388672,-1016.84997559,1054.50000000,0.00000000,0.00000000,180.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(kev_safe) (1)
	CreateDynamicObject(2833,2325.89990234,-1010.70001221,1053.71875000,0.00000000,0.00000000,0.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_livingrug02) (1)
	CreateDynamicObject(1210,2322.50390625,-1009.73980713,1054.77001953,90.00000000,0.00000000,23.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(briefcase) (1)
	CreateDynamicObject(1742,2323.39990234,-1006.62500000,1053.70996094,0.00000000,0.00000000,0.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(med_bookshelf) (1)
	CreateDynamicObject(2894,2322.46752930,-1009.14672852,1054.67187500,0.00000000,0.00000000,89.73001099, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(kmb_rhymesbook) (1)
	CreateDynamicObject(1502,2321.91992188,-1023.88201904,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gen_doorint04) (1)
	CreateDynamicObject(1502,2317.95996094,-1013.89001465,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gen_doorint04) (2)
	CreateDynamicObject(1502,2321.91992188,-1013.88964844,1049.21093750,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gen_doorint04) (3)
	CreateDynamicObject(2069,2316.20019531,-1026.69848633,1049.25000000,0.00000000,0.00000000,0.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(cj_mlight7) (2)
	CreateDynamicObject(2267,2322.00000000,-1010.00000000,1051.36096191,0.00000000,0.00000000,90.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(frame_wood_3) (1)
	CreateDynamicObject(2813,2326.06225586,-1016.13732910,1050.25781250,0.00000000,0.00000000,308.25524902, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(gb_novels01) (1)
	CreateDynamicObject(1667,2324.96020508,-1011.50372314,1049.79870605,0.00000000,0.00000000,0.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(propwineglass1) (1)
	CreateDynamicObject(1667,2324.88867188,-1011.38964844,1049.79870605,0.00000000,0.00000000,0.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(propwineglass1) (2)
	CreateDynamicObject(1665,2324.96142578,-1011.71868896,1049.72058105,0.00000000,0.00000000,0.00000000, HOUSE_VIRTUAL_WORLD + 6, 9, _, 200.0); //object(propashtray1) (1)

	/* LSPD interior additions */
	CreateDynamicObject(1742,239.44921875,109.50000000,1009.21179199,0.00000000,0.00000000,270.26916504, _, 10, _, 200.0); //object(med_bookshelf) (1)
	CreateDynamicObject(2259,233.53700256,111.30000305,1010.52191162,0.00000000,0.00000000,90.00000000, _, 10, _, 200.0); //object(frame_clip_6) (1)
	CreateDynamicObject(1510,237.27488708,110.47866058,1010.05999756,0.00000000,0.00000000,0.00000000, _, 10, _, 200.0); //object(dyn_ashtry) (1)
	CreateDynamicObject(3044,237.19999695,110.61499786,1010.16998291,25.00000000,0.00000000,0.00000000, _, 10, _, 200.0); //object(cigar) (2)
	CreateDynamicObject(2894,237.23359680,109.39933777,1010.05700684,0.00000000,0.00000000,105.56491089, _, 10, _, 200.0); //object(kmb_rhymesbook) (1)
	CreateDynamicObject(16780,236.00000000,110.00000000,1012.85998535,0.00000000,0.00000000,0.00000000, _, 10, _, 200.0); //object(ufo_light03) (2)
	CreateDynamicObject(1744,237.30000305,113.25000000,1010.70001221,0.00000000,0.00000000,0.00000000, _, 10, _, 200.0); //object(med_shelf) (1)
	CreateDynamicObject(1235,238.86370850,112.72632599,1009.72180176,0.00000000,0.00000000,0.00000000, _, 10, _, 200.0); //object(wastebin) (1)
	CreateDynamicObject(1520,237.29576111,110.73871613,1010.05700684,0.00000000,0.00000000,0.00000000, _, 10, _, 200.0); //object(dyn_wine_bounce) (1)
	CreateDynamicObject(1742,239.44921875,108.06933594,1009.21179199,0.00000000,0.00000000,270.26916504, _, 10, _, 200.0); //object(med_bookshelf) (1)
	CreateDynamicObject(2833,238.00000000,109.40000153,1009.22998047,0.00000000,0.00000000,90.00000000, _, 10, _, 200.0); //object(gb_livingrug02) (1)
	CreateDynamicObject(2813,237.22207642,112.88127136,1011.04052734,0.00000000,0.00000000,0.00000000, _, 10, _, 200.0); //object(gb_novels01) (1)
	CreateDynamicObject(2332,239.60000610,111.50000000,1011.04998779,0.00000000,0.00000000,270.00000000, _, 10, _, 200.0); //object(kev_safe) (1)
	CreateDynamicObject(2558,238.82000732,112.00000000,1010.50000000,0.00000000,0.00000000,270.00000000, _, 10, _, 200.0); //object(curtain_1_closed) (1)
	CreateDynamicObject(2289,237.42500305,107.12000275,1011.24859619,0.00000000,0.00000000,179.99450684, _, 10, _, 200.0); //object(frame_2) (1)
	CreateDynamicObject(2267,231.40335083,128.39999390,1011.29760742,0.00000000,0.00000000,0.00000000, _, 10, _, 200.0); //object(frame_wood_3) (1)
	CreateDynamicObject(2894,229.15087891,125.28470612,1010.13958740,0.00000000,0.00000000,0.00000000, _, 10, _, 200.0); //object(kmb_rhymesbook) (2)

	/* LSPD 3D Text Labels */
	CreateDynamic3DTextLabel("Department building elevator\n(/elevator)", COLOR_YELLOW, 276.0980, 122.1232, 1004.6172, 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
	CreateDynamic3DTextLabel("Upper roof elevator\n(/elevator)", COLOR_YELLOW, 1564.6584,-1670.2607,52.4503, 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
	CreateDynamic3DTextLabel("Lower roof elevator\n(/elevator)", COLOR_YELLOW, 1564.8, -1666.2, 28.3, 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
	CreateDynamic3DTextLabel("Police garage elevator\n(/elevator)", COLOR_YELLOW, 1568.6676, -1689.9708, 6.2188, 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);

	/* -------------------------------------- END OF RAEP. -------------------------------------- */
}