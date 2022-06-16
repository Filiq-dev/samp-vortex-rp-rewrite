/*
    if somebody wants to have roleplay names can enable this and use it in login.inc
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

function clearScreen(playerid) {
    for(new i = 0; i < 40; i++)
        SendClientMessage(playerid, COLOR_WHITE, " ");
} 

stock nearByMessage(playerid, color, Float: Distance, const string[], {Float, _}:...) {
	new Float: nbCoords[3];
	GetPlayerPos(playerid, nbCoords[0], nbCoords[1], nbCoords[2]);

	new str[146];
    va_format(str, sizeof (str), string, va_start<4>);

	foreach(new i : Player)
	    if(playerVariables[i][pStatus] >= 1)
	        if(IsPlayerInRangeOfPoint(i, Distance, nbCoords[0], nbCoords[1], nbCoords[2]) && (GetPlayerVirtualWorld(i) == GetPlayerVirtualWorld(playerid)))
				return SendClientMessage(i, color, str);

	return true;
}

function PlayerPlaySoundEx(soundid, Float:x, Float:y, Float:z) { // Realistic sound playback
	foreach(new i : Player) {
		if(IsPlayerInRangeOfPoint(i, 30.0, x, y, z))
			PlayerPlaySound(i, soundid, x, y, z);
	} 
}

stock getName(playerid) {
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	return name;
}

function initiateHospital(playerid) {
	TogglePlayerControllable(playerid, false);
	SetPlayerVirtualWorld(playerid, 0);
	SetPlayerInterior(playerid, 0);

	if(random(2) == 0) {
		SetPlayerPos(playerid, 1188.4574,-1309.2242,10.5625);
		SetPlayerCameraPos(playerid,1188.4574,-1309.2242,13.5625+6.0);
		SetPlayerCameraLookAt(playerid,1175.5581,-1324.7922,18.1610);

		SetPVarInt(playerid, "hosp", 1);
	} else {
		SetPlayerPos(playerid, 1999.5308,-1449.3281,10.5594);
		SetPlayerCameraPos(playerid,1999.5308,-1449.3281,13.5594+6.0);
		SetPlayerCameraLookAt(playerid,2036.2179,-1410.3223,17.1641);

	    SetPVarInt(playerid, "hosp", 2);
	}

	SendClientMessage(playerid, COLOR_LIGHTRED, "You must spend some time in the Hospital to recover from the injuries you recently sustained.");
	SendClientMessage(playerid, COLOR_LIGHTRED, "Before you are discharged, hospital staff will confiscate your weapons and you will be billed for the health care you received.");
	playerVariables[playerid][pHospitalized] = 2;
	SetPlayerHealth(playerid, 10);

	return true;
}

function givePlayerWeapons(playerid) {
	new x;

	while(x < 13) {
		GivePlayerWeapon(playerid, playerVariables[playerid][pWeapons][x], 99999);
		x++;
	}

	return true;
}

function validResetPlayerWeapons(playerid) {
	playerVariables[playerid][pAnticheatExemption] = 6;

	new xLoop;

	ResetPlayerWeapons(playerid);

	while(xLoop < 13) {
		playerVariables[playerid][pWeapons][xLoop] = 0;
		xLoop++;
	}

	if(playerVariables[playerid][pTabbed] >= 1) {
		playerVariables[playerid][pOutstandingWeaponRemovalSlot] = 40;
	}

	return true;
}

function givePlayerValidWeapon(playerid, weapon) {
	switch(weapon) {
		case 0, 1: playerVariables[playerid][pWeapons][0] = weapon;
	    case 2, 3, 4, 5, 6, 7, 8, 9: playerVariables[playerid][pWeapons][1] = weapon;
	    case 22, 23, 24: playerVariables[playerid][pWeapons][2] = weapon;
	    case 25, 26, 27: playerVariables[playerid][pWeapons][3] = weapon;
	    case 28, 29, 32: playerVariables[playerid][pWeapons][4] = weapon;
	    case 30, 31: playerVariables[playerid][pWeapons][5] = weapon;
	    case 33, 34: playerVariables[playerid][pWeapons][6] = weapon;
	    case 35, 36, 37, 38: playerVariables[playerid][pWeapons][7] = weapon;
	    case 16, 17, 18, 39: playerVariables[playerid][pWeapons][8] = weapon;
	    case 41, 42, 43: playerVariables[playerid][pWeapons][9] = weapon;
	    case 10, 11, 12, 13, 14, 15: playerVariables[playerid][pWeapons][10] = weapon;
	    case 44, 45, 46: playerVariables[playerid][pWeapons][11] = weapon;
	    case 40: playerVariables[playerid][pWeapons][12] = weapon; 
	}
	GivePlayerWeapon(playerid, weapon, 99999);

	return true;
}

stock getPlayerCheckpointReason(const playerid) {
	new name[20];
	
    switch(playerVariables[playerid][pCheckpoint]) {
		case 1: name = "Detective"; 
		case 2: name = "Matrun";
		case 3: name = "Dropcar";
		case 4: name = "Findcar";
		case 5: name = "Backup";
		case 6: name = "Home/Business";
		default: name = "01x05";
	}
 
	return name;
}
 
stock GetPlayerSpeed(playerid, get3d) {
	new
		Float:Floats[3];

	if(IsPlayerInAnyVehicle(playerid))
	    GetVehicleVelocity(GetPlayerVehicleID(playerid), Floats[0], Floats[1], Floats[2]);
	else
	    GetPlayerVelocity(playerid, Floats[0], Floats[1], Floats[2]);

	return floatround(floatsqroot(get3d?(Floats[0]*Floats[0]+Floats[1]*Floats[1]+Floats[2]*Floats[2]):(Floats[0]*Floats[0]+Floats[1]*Floats[1]) ) *100.0*1.6);
} 

function IsInvalidNOSVehicle(modelid) {
	switch(modelid) {
		case 581, 523, 462, 521, 463, 522, 461, 448, 468, 586, 509, 481, 510, 472, 473, 493, 595, 484, 430, 453, 452, 446, 454, 590, 569, 537, 538, 570, 449: return true;
	}
	return false;
}

function hackerTrigger(playerid) {
	if(playerVariables[playerid][pTabbed] == 0) {
	    playerVariables[playerid][pHackWarnings]++;
	    playerVariables[playerid][pHackWarnTime] = 1;

	    printf("Hack Warning! Weapon %d (playerid: %d)", GetPlayerWeapon(playerid), playerid);

	    if(playerVariables[playerid][pHackWarnings] >= 3) {

			new
				wep = GetPlayerWeapon(playerid),
		        reason[94]; 

			submitToAdmins(COLOR_HOTORANGE, "Warning: {FFFFFF}%s may possibly be weapon hacking (%s).", getName(playerid), WeaponNames[wep]);

		    if(playerVariables[playerid][pHackWarnings] >= MAX_WEAPON_HACK_WARNINGS) {
		        format(reason, sizeof(reason), "Weapon Hacking (%s).", WeaponNames[wep]);
		        scriptBan(playerid, reason);
		    }
		}
    }
	return true;
} 

function PlayerFacePlayer(playerid, targetplayerid) { // Yeah, this'll fix the handshake headaches we had last time around (shaking air).
	new Float: Angle;

	GetPlayerFacingAngle(playerid, Angle);
	SetPlayerFacingAngle(targetplayerid, Angle+180);

	return true;
}

function GetXYInFrontOfPlayer(playerid, &Float:x, &Float:y, Float:distance) { // And this'll keep the players close.

	new Float: a;

	GetPlayerPos(playerid, x, y, a);
	GetPlayerFacingAngle(playerid, a);

	if (GetPlayerVehicleID(playerid)) {
 		GetVehicleZAngle(GetPlayerVehicleID(playerid), a);
	}

	x += (distance * floatsin(-a, degrees));
	y += (distance * floatcos(-a, degrees));
}

function IsPlayerAimingAtPlayer(playerid, aimid) {

	new
		Float:Floats[7];

	GetPlayerPos(playerid, Floats[0], Floats[1], Floats[2]);
	GetPlayerPos(aimid, Floats[3], Floats[4], Floats[5]);
	new Float:Distance = floatsqroot(floatpower(floatabs(Floats[0]-Floats[3]), 2) + floatpower(floatabs(Floats[1]-Floats[4]), 2));
	if(Distance < 10.0) {
		GetPlayerFacingAngle(playerid, Floats[6]);
		Floats[0] += (Distance * floatsin(-Floats[6], degrees));
		Floats[1] += (Distance * floatcos(-Floats[6], degrees));
	    Distance = floatsqroot(floatpower(floatabs(Floats[0]-Floats[3]), 2) + floatpower(floatabs(Floats[1]-Floats[4]), 2));

  		if(Distance < 2.0) {
    		return true;
  		}
	}
	return false;
} 

function ShutUp(slot) { // One function for eight doors. A WINRAR IS YOU!
	if(LSPDObjs[slot][2] == 1) switch(slot) {
		case 0: {
			MoveDynamicObject(LSPDObjs[0][0],232.89999390,107.57499695,1009.21179199,3.5); //commander south
			MoveDynamicObject(LSPDObjs[0][1],232.89941406,110.57499695,1009.21179199,3.5); //commander north
			LSPDObjs[0][2] = 0;
		}
		case 1: {
			MoveDynamicObject(LSPDObjs[1][0],275.75000000,118.89941406,1003.61718750,3.5); // interrogation north
			MoveDynamicObject(LSPDObjs[1][1],275.75000000,115.89941406,1003.61718750,3.5); // interrogation south
			LSPDObjs[1][2] = 0;
		}
		case 2: {
			MoveDynamicObject(LSPDObjs[2][0],253.20410156,107.59960938,1002.22070312,3.5); // north west lobby door
			MoveDynamicObject(LSPDObjs[2][1],253.19921875,110.59960938,1002.22070312,3.5); // north east lobby door
			LSPDObjs[2][2] = 0;
		}
		case 3: {
			MoveDynamicObject(LSPDObjs[3][0],239.56933594,116.09960938,1002.22070312,3.5); // south west lobby door
			MoveDynamicObject(LSPDObjs[3][1],239.56445312,119.09960938,1002.22070312,3.5); // south east lobby door
			LSPDObjs[3][2] = 0;
		}
		case 4: {
			MoveDynamicObject(LSPDObjs[4][0],264.45019531,115.82421875,1003.62286377,3.5); //object(gen_doorext15) (3)
			MoveDynamicObject(LSPDObjs[4][1],267.45214844,115.82910156,1003.62286377,3.5); //object(gen_doorext15) (8)
			LSPDObjs[4][2] = 0;
		}
		case 5: {
			MoveDynamicObject(LSPDObjs[5][0],267.32000732,112.53222656,1003.62286377,3.5); //object(gen_doorext15) (4)
			MoveDynamicObject(LSPDObjs[5][1],264.32000732,112.52929688,1003.62286377,3.5); //object(gen_doorext15) (5)
			LSPDObjs[5][2] = 0;
		}
		case 6: {
			MoveDynamicObject(LSPDObjs[6][0],229.59960938,119.52929688,1009.22442627,3.5); //object(gen_doorext15) (9)
			MoveDynamicObject(LSPDObjs[6][1],232.59960938,119.53515625,1009.22442627,3.5); //object(gen_doorext15) (10)
			LSPDObjs[6][2] = 0;
		}
		case 7: {
			MoveDynamicObject(LSPDObjs[7][0],219.30000305,116.52999878,998.01562500,3.5); //cell east door
			MoveDynamicObject(LSPDObjs[7][1],216.30000305,116.52929688,998.01562500,3.5); //cell west door
			LSPDObjs[7][2] = 0;
		}
	}
	return 1;
}
/*
stock GetWeaponSlot(weaponid) {
	switch(weaponid) {
		case 0, 1: return 0;
		case 2 .. 9: return 1;
		case 22 .. 24: return 2;
		case 25 .. 27: return 3;
		case 28, 29, 32: return 4;
		case 30, 31: return 5;
		case 33, 34: return 6;
		case 35 .. 38: return 7;
		case 16, 17, 18, 39, 40: return 8;
		case 41 .. 43: return 9;
		case 10 .. 15: return 10;
		case 44 .. 46: return 11;
	}
	return -1;
} 
*/
stock GetClosestVehicle(playerid, exception = INVALID_VEHICLE_ID) {
    new
		Float:Distance,
		target = -1;

    for(new v; v < MAX_VEHICLES; v++) if(doesVehicleExist(v)) {
        if(v != exception && (target < 0 || Distance > GetDistancePlayerVeh(playerid, v))) {
            target = v;
            Distance = GetDistancePlayerVeh(playerid, v);
        }
    }
    return target;
}

stock GetDistancePlayerVeh(playerid, veh) {

	new
	    Float:Floats[7];

	GetPlayerPos(playerid, Floats[0], Floats[1], Floats[2]);
	GetVehiclePos(veh, Floats[3], Floats[4], Floats[5]);
	Floats[6] = floatsqroot((Floats[3]-Floats[0])*(Floats[3]-Floats[0])+(Floats[4]-Floats[1])*(Floats[4]-Floats[1])+(Floats[5]-Floats[2])*(Floats[5]-Floats[2]));

	return floatround(Floats[6]);
}

function checkVehicleSeat(vehicleid, seatid) {
	foreach(new x : Player) {
	    if(GetPlayerVehicleID(x) == vehicleid && GetPlayerVehicleSeat(x) == seatid) return 1;
	}
	return 0;
}

function IsValidSkin(skinid) {
	if(skinid < 0 || skinid > 299)
		return false;

	switch(skinid) {
		case 3, 4, 5, 6, 8, 42, 65, 74, 86, 119, 149, 208, 268, 273, 289: return false;
	}
	return true;
}

function IsPublicSkin(skinid) {
	if(!IsValidSkin(skinid)) return false;

	switch(skinid) {
		case 274 .. 288, 265 .. 267, 71: return false;
	}
	return true;
} 

stock FetchLevelFromHours(const iHours) {
	switch(iHours) {
	    case 0..24: return 1;
	    case 25..48: return 2;
	    case 49..72: return 3;
	    case 73..100: return 4;
	    case 101..175: return 5;
	    case 176..200: return 6;
	    case 201..208: return 8;
	    case 209..336: return 9;
	    case 337..480: return 10;
	}
	return 0;
}
stock SendToFrequency(const frequency, const colour, const string[]) {
	if(frequency > 0) {
		foreach(new i : Player) {
			if(playerVariables[i][pStatus] == 1 && playerVariables[i][pWalkieTalkie] == frequency) {
				SendClientMessage(i, colour, string);
			}
		}
	}
	return 1;
} 

stock IsPlayerInRangeOfVehicle(playerid, vehicleid, Float: radius) {

	new
		Float:Floats[3];

	GetVehiclePos(vehicleid, Floats[0], Floats[1], Floats[2]);
	return IsPlayerInRangeOfPoint(playerid, radius, Floats[0], Floats[1], Floats[2]);
}

stock IsPlayerInRangeOfPlayer(playerid, playerid2, Float: radius) {

	new
		Float:Floats[3];

	GetPlayerPos(playerid2, Floats[0], Floats[1], Floats[2]);
	return IsPlayerInRangeOfPoint(playerid, radius, Floats[0], Floats[1], Floats[2]);
}

stock IsVehicleInRangeOfPoint(vehicleid, Float: radius, Float:x, Float:y, Float:z) {

	new
		Float:Floats[6];

	GetVehiclePos(vehicleid, Floats[0], Floats[1], Floats[2]);
	Floats[3] = (Floats[0] -x);
	Floats[4] = (Floats[1] -y);
	Floats[5] = (Floats[2] -z);
	if (((Floats[3] < radius) && (Floats[3] > -radius)) && ((Floats[4] < radius) && (Floats[4] > -radius)) && ((Floats[5] < radius) && (Floats[5] > -radius)))
		return 1;
	return 0;
}

stock removePlayerWeapon(playerid, weapon) {
	playerVariables[playerid][pAnticheatExemption] = 6;

	switch(weapon) {
		case 0, 1: {
		    if(playerVariables[playerid][pTabbed] >= 1) {
		        playerVariables[playerid][pOutstandingWeaponRemovalSlot] = 0;
		    }
		    else {
			    ResetPlayerWeapons(playerid);
		        playerVariables[playerid][pWeapons][0] = 0;
				givePlayerWeapons(playerid);
			}
	    }
	    case 2, 3, 4, 5, 6, 7, 8, 9: {
		    if(playerVariables[playerid][pTabbed] >= 1) {
		        playerVariables[playerid][pOutstandingWeaponRemovalSlot] = 1;
		    }
		    else {
			    ResetPlayerWeapons(playerid);
		        playerVariables[playerid][pWeapons][1] = 0;
				givePlayerWeapons(playerid);
			}
	    }
	    case 22, 23, 24: {
		    if(playerVariables[playerid][pTabbed] >= 1) {
		        playerVariables[playerid][pOutstandingWeaponRemovalSlot] = 2;
		    }
		    else {
			    ResetPlayerWeapons(playerid);
		        playerVariables[playerid][pWeapons][2] = 0;
				givePlayerWeapons(playerid);
			}
	    }
	    case 25, 26, 27: {
		    if(playerVariables[playerid][pTabbed] >= 1) {
		        playerVariables[playerid][pOutstandingWeaponRemovalSlot] = 3;
		    }
		    else {
			    ResetPlayerWeapons(playerid);
		        playerVariables[playerid][pWeapons][3] = 0;
				givePlayerWeapons(playerid);
			}
	    }
	    case 28, 29, 32: {
		    if(playerVariables[playerid][pTabbed] >= 1) {
		        playerVariables[playerid][pOutstandingWeaponRemovalSlot] = 4;
		    }
		    else {
			    ResetPlayerWeapons(playerid);
		        playerVariables[playerid][pWeapons][4] = 0;
				givePlayerWeapons(playerid);
			}
	    }
	    case 30, 31: {
		    if(playerVariables[playerid][pTabbed] >= 1) {
		        playerVariables[playerid][pOutstandingWeaponRemovalSlot] = 5;
		    }
		    else {
			    ResetPlayerWeapons(playerid);
		        playerVariables[playerid][pWeapons][5] = 0;
				givePlayerWeapons(playerid);
			}
	    }
	    case 33, 34: {
		    if(playerVariables[playerid][pTabbed] >= 1) {
		        playerVariables[playerid][pOutstandingWeaponRemovalSlot] = 6;
		    }
		    else {
			    ResetPlayerWeapons(playerid);
		        playerVariables[playerid][pWeapons][6] = 0;
				givePlayerWeapons(playerid);
			}
	    }
	    case 35, 36, 37, 38: {
		    if(playerVariables[playerid][pTabbed] >= 1) {
		        playerVariables[playerid][pOutstandingWeaponRemovalSlot] = 7;
		    }
		    else {
			    ResetPlayerWeapons(playerid);
		        playerVariables[playerid][pWeapons][7] = 0;
				givePlayerWeapons(playerid);
			}
	    }
	    case 16, 17, 18, 39: {
		    if(playerVariables[playerid][pTabbed] >= 1) {
		        playerVariables[playerid][pOutstandingWeaponRemovalSlot] = 8;
		    }
		    else {
			    ResetPlayerWeapons(playerid);
		        playerVariables[playerid][pWeapons][8] = 0;
				givePlayerWeapons(playerid);
			}
	    }
	    case 41, 42, 43: {
		    if(playerVariables[playerid][pTabbed] >= 1) {
		        playerVariables[playerid][pOutstandingWeaponRemovalSlot] = 9;
		    }
		    else {
			    ResetPlayerWeapons(playerid);
		        playerVariables[playerid][pWeapons][9] = 0;
				givePlayerWeapons(playerid);
			}
	    }
	    case 10, 11, 12, 13, 14, 15: {
		    if(playerVariables[playerid][pTabbed] >= 1) {
		        playerVariables[playerid][pOutstandingWeaponRemovalSlot] = 10;
		    }
		    else {
			    ResetPlayerWeapons(playerid);
		        playerVariables[playerid][pWeapons][10] = 0;
				givePlayerWeapons(playerid);
			}
	    }
	    case 44, 45, 46: {
		    if(playerVariables[playerid][pTabbed] >= 1) {
		        playerVariables[playerid][pOutstandingWeaponRemovalSlot] = 11;
		    }
		    else {
			    ResetPlayerWeapons(playerid);
		        playerVariables[playerid][pWeapons][11] = 0;
				givePlayerWeapons(playerid);
			}
	    }
	    case 40: {
		    if(playerVariables[playerid][pTabbed] >= 1) {
		        playerVariables[playerid][pOutstandingWeaponRemovalSlot] = 12;
		    }
		    else {
			    ResetPlayerWeapons(playerid);
		        playerVariables[playerid][pWeapons][12] = 0;
				givePlayerWeapons(playerid);
			}
	    }
	}
	return 1;
}   

function IsAPlane(vehicleid) {
	switch(GetVehicleModel(vehicleid)) {
		case 592, 577, 511, 512, 593, 520, 553, 476, 519, 460, 513, 548, 425, 417, 487, 488, 497, 563, 447, 469: return 1;
	}
	return 0;
}

function IsABoat(vehicleid) {
	switch(GetVehicleModel(vehicleid)) {
		case 472, 473, 493, 595, 484, 430, 453, 452, 446, 454: return 1;
	}
	return 0;
} 

function IsACopCar(vehicleid) {
	switch(GetVehicleModel(vehicleid)) {
		case 596 .. 599: return 1;
	}
	return 0;
}

stock PreloadAnimLib(playerid, const animlib[]) {
	return ApplyAnimation(playerid, animlib, "null", 0.0, false, false, false, false, false) ;
} 

function doesVehicleExist(const vehicleid) {
    if(GetVehicleModel(vehicleid) >= 400) {
		return 1;
	}
	return 0;
}

function adminLog(const string[], {Float, _}:...) {
	gString[0] = (EOS);
	va_format(gString, sizeof (gString), string, va_start<1>);

	gQuery[0] = (EOS);  
	mysql_format(gSQL, gQuery, sizeof(gQuery), "INSERT INTO adminlog (value, tickcount) VALUES('%s', '%d')", gString, GetTickCount());
	mysql_tquery(gSQL, gQuery, "", ""); // de facut finishQuery
} 