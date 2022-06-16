CMD:vdeposit(playerid, params[]) {
	if(playerVariables[playerid][pCarModel] >= 1) {
	    if(IsPlayerInRangeOfVehicle(playerid, playerVariables[playerid][pCarID], 6.0)) {
			new
			    amount,

			    houseOperation[64];

			if(sscanf(params, "s[32]d", houseOperation, amount)) {
			    return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/vdeposit [money/materials] [amount]");
			}
			else if(!strcmp(houseOperation, "money", true)) {
				if(playerVariables[playerid][pMoney] >= amount) {
					if(amount >= 1 && amount < 60000000) {
						playerVariables[playerid][pCarTrunk][0] += amount;
						playerVariables[playerid][pMoney] -= amount;

						if(playerVariables[playerid][pCarTrunk][0] < 1)
							playerVariables[playerid][pCarTrunk][0] = 0;

						if(playerVariables[playerid][pMoney] < 1)
							playerVariables[playerid][pMoney] = 0;

						format(houseOperation, sizeof(houseOperation), "You have deposited $%d in your vehicle.", amount);
						SendClientMessage(playerid, COLOR_WHITE, houseOperation);

						GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
						format(houseOperation, sizeof(houseOperation), "* %s deposits $%d in their vehicle.", szPlayerName, amount);
						nearByMessage(playerid, COLOR_PURPLE, houseOperation);
					}
					else {
						SendClientMessage(playerid, COLOR_GREY, "You can't deposit a negative amount into a house safe. (01x01)");
						printf("[error] 01x01, %d", playerid);
					}
				}
			}
			else if(!strcmp(houseOperation, "materials", true)) {
				if(playerVariables[playerid][pMaterials] >= amount) {
					if(amount >= 1 && amount < 60000000) {
						playerVariables[playerid][pCarTrunk][1] += amount;
						playerVariables[playerid][pMaterials] -= amount;

						if(playerVariables[playerid][pCarTrunk][1] < 1) playerVariables[playerid][pCarTrunk][1] = 0;
						if(playerVariables[playerid][pMaterials] < 1) playerVariables[playerid][pMaterials] = 0;

						format(houseOperation, sizeof(houseOperation), "You have deposited %d materials in your vehicle.", amount);
						SendClientMessage(playerid, COLOR_WHITE, houseOperation);

						GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
						format(houseOperation, sizeof(houseOperation), "* %s deposits %d materials in their vehicle.", szPlayerName, amount);
						nearByMessage(playerid, COLOR_PURPLE, houseOperation);
					}
					else {
						SendClientMessage(playerid, COLOR_GREY, "You can't deposit a negative amount into a house safe. (01x01)");
						printf("[error] 01x01, %d", playerid);
					}
				}
			}
			else SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/vdeposit [money/materials] [amount]");
	    }
	    else SendClientMessage(playerid, COLOR_GREY, "You're too far away from your vehicle.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You don't own a vehicle.");
	return 1;
}

CMD:vwithdraw(playerid, params[]) {
	if(playerVariables[playerid][pCarModel] >= 1) {
	    if(IsPlayerInRangeOfVehicle(playerid, playerVariables[playerid][pCarID], 6.0)) {
			new
			    amount,

			    houseOperation[72]; // For formatting afterwards.

			if(sscanf(params, "s[32]d", houseOperation, amount))
			    return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/vwithdraw [money/materials] [amount]");

			if(!strcmp(houseOperation, "money", true)) {
				if(playerVariables[playerid][pCarTrunk][0] >= amount) {
					if(amount >= 1 && amount < 60000000) {
						playerVariables[playerid][pCarTrunk][0] -= amount;
						playerVariables[playerid][pMoney] += amount;

						if(playerVariables[playerid][pCarTrunk][0] < 1) playerVariables[playerid][pCarTrunk][0] = 0;
						if(playerVariables[playerid][pMoney] < 1) playerVariables[playerid][pMoney] = 0;

						format(houseOperation, sizeof(houseOperation), "You have withdrawn $%d from your vehicle.", amount);
						SendClientMessage(playerid, COLOR_WHITE, houseOperation);

						GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
						format(houseOperation, sizeof(houseOperation), "* %s withdraws $%d from their vehicle.", szPlayerName, amount);
						nearByMessage(playerid, COLOR_PURPLE, houseOperation);
					}
					else SendClientMessage(playerid, COLOR_GREY, "(error) 01x04");
				}
			}
			else if(!strcmp(houseOperation, "materials", true)) {
				if(playerVariables[playerid][pCarTrunk][1] >= amount) {
					if(amount >= 1 && amount < 60000000) {
						playerVariables[playerid][pCarTrunk][1] -= amount;
						playerVariables[playerid][pMaterials] += amount;

						if(playerVariables[playerid][pCarTrunk][1] < 1) playerVariables[playerid][pCarTrunk][1] = 0;
						if(playerVariables[playerid][pMaterials] < 1) playerVariables[playerid][pMaterials] = 0;

						format(houseOperation, sizeof(houseOperation), "You have withdrawn %d materials from your vehicle.", amount);
						SendClientMessage(playerid, COLOR_WHITE, houseOperation);

						GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
						format(houseOperation, sizeof(houseOperation), "* %s withdraws %d materials from their vehicle.", szPlayerName, amount);
						nearByMessage(playerid, COLOR_PURPLE, houseOperation);
					}
					else SendClientMessage(playerid, COLOR_GREY, "(error) 01x04");
				}
			}
			else SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/vwithdraw [money/materials] [amount]");
	    }
	    else SendClientMessage(playerid, COLOR_GREY, "You're too far away from your vehicle.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You don't own a vehicle.");
	return 1;
}

CMD:vbalance(playerid, params[]) {
	new
		x;

	if(playerVariables[playerid][pCarModel] >= 1) {
		if(IsPlayerInRangeOfVehicle(playerid, playerVariables[playerid][pCarID], 6.0)) {

			new
				messageString[128];

			format(messageString, sizeof(messageString), "Money: $%d | Materials: %d", playerVariables[playerid][pCarTrunk][0], playerVariables[playerid][pCarTrunk][1]);

			for(new i; i < 5; i++) {
				if(playerVariables[playerid][pCarWeapons][i] > 0) {
					if(x == 0) format(messageString, sizeof(messageString),"%s | Weapons: %s (slot %d)", messageString, WeaponNames[playerVariables[playerid][pCarWeapons][i]], i);
					else format(messageString, sizeof(messageString),"%s, %s (slot %d)", messageString, WeaponNames[playerVariables[playerid][pCarWeapons][i]], i);
					x++;
				}
			}
			SendClientMessage(playerid, COLOR_WHITE, messageString);
		}
		else SendClientMessage(playerid, COLOR_GREY, "You're too far away from your vehicle.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You don't own a vehicle.");
	return 1;
}

CMD:vstoreweapon(playerid, params[]) {

	new
		slot = strval(params);

	if(isnull(params))
		return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/vstoreweapon [slot 1-5]");

	else if(playerVariables[playerid][pCarModel] >= 1) {
	    if(IsPlayerInRangeOfVehicle(playerid, playerVariables[playerid][pCarID], 6.0)) {
			if(slot >= 1 && slot <= 5) {
				if(playerVariables[playerid][pCarWeapons][slot - 1] == 0) {

					new
						string[86],
						weapon;

					GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
					weapon = GetPlayerWeapon(playerid);

					switch(weapon) {
						case 16, 18, 35, 36, 37, 38, 39, 40, 44, 45, 46, 0: SendClientMessage(playerid, COLOR_GREY, "Invalid weapon.");
						default: {
							playerVariables[playerid][pCarWeapons][slot - 1] = weapon;
							removePlayerWeapon(playerid, weapon);

							format(string, sizeof(string), "* %s places their %s in their vehicle.", szPlayerName, WeaponNames[weapon]);
							nearByMessage(playerid, COLOR_PURPLE, string);

							format(string, sizeof(string), "You have stored your %s in slot %d.", WeaponNames[weapon], slot);
							SendClientMessage(playerid, COLOR_WHITE, string);

						}
					}
				}
				else SendClientMessage(playerid, COLOR_GREY, "That slot is already occupied.");
			}
			else SendClientMessage(playerid, COLOR_GREY, "Invalid slot specified.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You're too far away from your vehicle.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You don't own a vehicle.");
	return 1;
}

CMD:vgetweapon(playerid, params[]) {

	new
		slot = strval(params);

	if(isnull(params))
		return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/vgetweapon [slot 1-5]");

	else if(playerVariables[playerid][pCarModel] >= 1) {
	    if(IsPlayerInRangeOfVehicle(playerid, playerVariables[playerid][pCarID], 6.0)) {
			if(slot >= 1 && slot <= 5) {
				if(playerVariables[playerid][pCarWeapons][slot - 1] != 0) {
					if(playerVariables[playerid][pWeapons][GetWeaponSlot(playerVariables[playerid][pCarWeapons][slot - 1])] == 0) {

						new
							string[86];

						GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
						givePlayerValidWeapon(playerid, playerVariables[playerid][pCarWeapons][slot - 1]);

						format(string, sizeof(string), "* %s retrieves their %s from their vehicle.", szPlayerName, WeaponNames[playerVariables[playerid][pCarWeapons][slot - 1]]);
						nearByMessage(playerid, COLOR_PURPLE, string);

						format(string, sizeof(string), "You have withdrawn your %s from slot %d.", WeaponNames[playerVariables[playerid][pCarWeapons][slot - 1]], slot);
						SendClientMessage(playerid, COLOR_WHITE, string);
						playerVariables[playerid][pCarWeapons][slot - 1] = 0;
					}
					else SendClientMessage(playerid, COLOR_GREY, "You already have a weapon of this type on you - drop it first.");
				}
				else SendClientMessage(playerid, COLOR_GREY, "There is no weapon stored in that slot.");
			}
			else SendClientMessage(playerid, COLOR_GREY, "Invalid slot specified.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You're too far away from your vehicle.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You don't own a vehicle.");
	return 1;
} 

CMD:abandoncar(playerid, params[]) {
	if(playerVariables[playerid][pCarModel] >= 1) {
		if(IsPlayerInRangeOfVehicle(playerid, playerVariables[playerid][pCarID], 5.0)) {
			DestroyPlayerVehicle(playerid);
			SendClientMessage(playerid, COLOR_GREY, "You have abandoned your vehicle.");

			if(playerVariables[playerid][pCheckpoint] == 4) {
				DisablePlayerCheckpoint(playerid);
				playerVariables[playerid][pCheckpoint] = 0;
			}
		}
		else SendClientMessage(playerid, COLOR_GREY, "You're too far away from your vehicle.");
	}
	return 1;
}

CMD:givecar(playerid, params[]) {
	if(sscanf(params, "u", iTarget))
		SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/givecar [playerid]");
	else if(!IsPlayerAuthed(iTarget))
		SendClientMessage(playerid, COLOR_GREY, "The specified player is not connected, or has not authenticated.");
	else
	{
		if(playerVariables[playerid][pCarModel] >= 1) 
		{
			if(IsPlayerInRangeOfPlayer(playerid, iTarget, 5.0)) 
			{
				SetPVarInt(iTarget, "gC", playerid + 1);
				// The usual culprit - barely accessed, barely used. As PVars return 0 if they don't exist, adding +1 ensures they return a valid playerid.

				GetPlayerName(iTarget, szPlayerName, MAX_PLAYER_NAME);

				format(szMessage, sizeof(szMessage), "You have offered %s the keys to your %s.", szPlayerName, VehicleNames[playerVariables[playerid][pCarModel] - 400]);
				SendClientMessage(playerid, COLOR_WHITE, szMessage);

				GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);

				format(szMessage, sizeof(szMessage), "%s is offering you the keys to their %s (type /accept givecar).", szPlayerName, VehicleNames[playerVariables[playerid][pCarModel] - 400]);
				SendClientMessage(iTarget, COLOR_NICESKY, szMessage);
			}
			else SendClientMessage(playerid, COLOR_GREY, "You're too far away from that person.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You don't own a vehicle.");
	}
	return 1;
}

CMD:lockcar(playerid, params[]) {
	if(doesVehicleExist(playerVariables[playerid][pCarID]) && playerVariables[playerid][pCarModel] >= 1) {
		if(IsPlayerInRangeOfVehicle(playerid, playerVariables[playerid][pCarID], 10.0)) {

			GetVehiclePos(playerVariables[playerid][pCarID], playerVariables[playerid][pCarPos][0], playerVariables[playerid][pCarPos][1], playerVariables[playerid][pCarPos][2]);
			PlayerPlaySoundEx(1145, playerVariables[playerid][pCarPos][0], playerVariables[playerid][pCarPos][1], playerVariables[playerid][pCarPos][2]);

			switch(playerVariables[playerid][pCarLock]) {
				case 0: {
					playerVariables[playerid][pCarLock] = 1;
					SendClientMessage(playerid, COLOR_WHITE, "You have locked your vehicle.");

					foreach(Player, x) {
						SetVehicleParamsForPlayer(playerVariables[playerid][pCarID], x, 0, 1);
					}
				}
				default: {
					playerVariables[playerid][pCarLock] = 0;
					SendClientMessage(playerid, COLOR_WHITE, "You have unlocked your vehicle.");

					foreach(Player, x) {
						SetVehicleParamsForPlayer(playerVariables[playerid][pCarID], x, 0, 0);
					}
				}
			}
		}
		else SendClientMessage(playerid, COLOR_GREY, "You're too far away from your vehicle.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You don't own a vehicle.");
	return 1;
}

CMD:findcar(playerid, params[]) {
	if(playerVariables[playerid][pCarModel] >= 1) {
		if(playerVariables[playerid][pCheckpoint] == 0 && playerVariables[playerid][pCheckpoint] != 4) {
			GetVehiclePos(playerVariables[playerid][pCarID], playerVariables[playerid][pCarPos][0], playerVariables[playerid][pCarPos][1], playerVariables[playerid][pCarPos][2]);
			SetPlayerCheckpoint(playerid, playerVariables[playerid][pCarPos][0], playerVariables[playerid][pCarPos][1], playerVariables[playerid][pCarPos][2], 10.0);
			playerVariables[playerid][pCheckpoint] = 4;

			format(szMessage, sizeof(szMessage), "A checkpoint has been set to your %s.", VehicleNames[playerVariables[playerid][pCarModel] - 400]);
			SendClientMessage(playerid, COLOR_WHITE, szMessage);
		}
		else {
			format(szMessage, sizeof(szMessage), "You already have an active checkpoint (%s), reach it first, or /killcheckpoint.", getPlayerCheckpointReason(playerid));
			SendClientMessage(playerid, COLOR_GREY, szMessage);
		}
	}
	else SendClientMessage(playerid, COLOR_GREY, "You don't own a vehicle.");
	return 1;
}

CMD:unmodcar(playerid, params[]) {
	if(playerVariables[playerid][pCarModel] >= 1) {
		if(IsPlayerInRangeOfVehicle(playerid, playerVariables[playerid][pCarID], 5.0)) {

			new
				Float: vHealth,
				Damage[4];

			GetVehicleDamageStatus(playerVariables[playerid][pCarID], Damage[0], Damage[1], Damage[2], Damage[3]);
			GetVehiclePos(playerVariables[playerid][pCarID], playerVariables[playerid][pCarPos][0], playerVariables[playerid][pCarPos][1], playerVariables[playerid][pCarPos][2]);
			GetVehicleZAngle(playerVariables[playerid][pCarID], playerVariables[playerid][pCarPos][3]);
			GetVehicleHealth(playerVariables[playerid][pCarID], vHealth);

			for(new i = 0; i < 13; i++) {
				playerVariables[playerid][pCarMods][i] = 0;
			}

			playerVariables[playerid][pCarPaintjob] = -1;

			if(IsPlayerInVehicle(playerid, playerVariables[playerid][pCarID]) && GetPlayerState(playerid) == 2) {
				DestroyVehicle(playerVariables[playerid][pCarID]);
				systemVariables[vehicleCounts][1]--;
				playerVariables[playerid][pCarID] = -1;
				SpawnPlayerVehicle(playerid);
				PutPlayerInVehicle(playerid, playerVariables[playerid][pCarID], 0);
			}
			else {
				DestroyVehicle(playerVariables[playerid][pCarID]);
				playerVariables[playerid][pCarID] = -1;
				systemVariables[vehicleCounts][1]--;
				SpawnPlayerVehicle(playerid);
			}
			SetVehicleHealth(playerVariables[playerid][pCarID], vHealth);
			UpdateVehicleDamageStatus(playerVariables[playerid][pCarID], Damage[0], Damage[1], Damage[2], Damage[3]);
		}
		else SendClientMessage(playerid, COLOR_GREY, "You're too far away from your vehicle.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You don't own a vehicle.");
	return 1;
}