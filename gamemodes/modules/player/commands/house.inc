CMD:sellhouse(playerid, params[]) {
	if(playerVariables[playerid][pStatus] >= 1) {
	    new
	        houseID = getPlayerHouseID(playerid);

	    if(houseID < 1)
	        return 1;

		new
	    	labelString[96];

        playerVariables[playerid][pMoney] += houseVariables[houseID][hHousePrice];

        format(houseVariables[houseID][hHouseOwner], MAX_PLAYER_NAME, "Nobody");
        format(labelString, sizeof(labelString), "House %d (un-owned - /buyhouse)\nPrice: $%d\n\n(locked)", houseID, houseVariables[houseID][hHousePrice]);

        DestroyDynamicPickup(houseVariables[houseID][hPickupID]);
        DestroyDynamic3DTextLabel(houseVariables[houseID][hLabelID]);

        houseVariables[houseID][hLabelID] = CreateDynamic3DTextLabel(labelString, COLOR_YELLOW, houseVariables[houseID][hHouseExteriorPos][0], houseVariables[houseID][hHouseExteriorPos][1], houseVariables[houseID][hHouseExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
		houseVariables[houseID][hPickupID] = CreateDynamicPickup(1273, 23, houseVariables[houseID][hHouseExteriorPos][0], houseVariables[houseID][hHouseExteriorPos][1], houseVariables[houseID][hHouseExteriorPos][2], 0, houseVariables[houseID][hHouseExteriorID], -1, 250);

		houseVariables[houseID][hHouseLocked] = 1;

		format(labelString, sizeof(labelString), "House sold! You have been given back $%d for the house.", houseVariables[houseID][hHousePrice]);
		SendClientMessage(playerid, COLOR_WHITE, labelString);

		saveHouse(houseID);
	}
	return 1;
}

CMD:ringbell(playerid, params[]) {
	if(GetPlayerState(playerid) == 1) {
		for(new x = 0; x < MAX_HOUSES; x++) {
			if(IsPlayerInRangeOfPoint(playerid, 2.0, houseVariables[x][hHouseExteriorPos][0], houseVariables[x][hHouseExteriorPos][1], houseVariables[x][hHouseExteriorPos][2])) {

				new
					string[80];

				GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
				format(string, sizeof(string), "* %s presses a button, ringing the doorbell of the house.", szPlayerName);
				nearByMessage(playerid, COLOR_PURPLE, string);

				foreach(Player, i) {
					if(GetPlayerVirtualWorld(i) == x + HOUSE_VIRTUAL_WORLD) {
						SendClientMessage(i, COLOR_PURPLE, "* The doorbell rings.");
					}
				}
			}
		}
	}
	return 1;
}

CMD:lockhouse(playerid, params[]) {
	if(playerVariables[playerid][pStatus] >= 1) {
	    new
	        houseID = getPlayerHouseID(playerid);

	    if(houseID >= 1) {
			if(IsPlayerInRangeOfPoint(playerid, 2.0, houseVariables[houseID][hHouseExteriorPos][0], houseVariables[houseID][hHouseExteriorPos][1], houseVariables[houseID][hHouseExteriorPos][2]) || IsPlayerInRangeOfPoint(playerid, 2.0, houseVariables[houseID][hHouseInteriorPos][0], houseVariables[houseID][hHouseInteriorPos][1], houseVariables[houseID][hHouseInteriorPos][2])) {

				new
					labelString[96];

				switch(houseVariables[houseID][hHouseLocked]) {
					case 1: {
						houseVariables[houseID][hHouseLocked] = 0;
						SendClientMessage(playerid, COLOR_WHITE, "House unlocked.");
						format(labelString, sizeof(labelString), "House %d (owned)\nOwner: %s\n\nPress ~k~~PED_DUCK~ to enter.", houseID, houseVariables[houseID][hHouseOwner]);
					}
					case 0: {
						houseVariables[houseID][hHouseLocked] = 1;
						SendClientMessage(playerid, COLOR_WHITE, "House locked.");
						format(labelString, sizeof(labelString), "House %d (owned)\nOwner: %s\n\n(locked)", houseID, houseVariables[houseID][hHouseOwner]);
					}
				}

				UpdateDynamic3DTextLabelText(houseVariables[houseID][hLabelID], COLOR_YELLOW, labelString);
				PlayerPlaySoundEx(1145, houseVariables[houseID][hHouseExteriorPos][0], houseVariables[houseID][hHouseExteriorPos][1], houseVariables[houseID][hHouseExteriorPos][2]);
				PlayerPlaySoundEx(1145, houseVariables[houseID][hHouseInteriorPos][0], houseVariables[houseID][hHouseInteriorPos][1], houseVariables[houseID][hHouseInteriorPos][2]);
			}
			else SendClientMessage(playerid, COLOR_GREY, "You're not at your house.");
	    }
	    else  SendClientMessage(playerid, COLOR_GREY, "You don't own a house.");
	}

	return 1;
}

CMD:changeclothes(playerid, params[]) {

	new
		slot = strval(params),
		houseID = getPlayerHouseID(playerid),
		string[64];

	if(isnull(params))
		return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/changeclothes [slot 1-5]");

	if(getPlayerHouseID(playerid) >= 1) {
	    if(GetPlayerVirtualWorld(playerid) == HOUSE_VIRTUAL_WORLD + houseID) {
			if(slot >= 1 && slot <= 5) {
				if(houseVariables[houseID][hWardrobe][slot - 1] != 0) {

					SetPlayerSkin(playerid, houseVariables[houseID][hWardrobe][slot - 1]);
					playerVariables[playerid][pSkin] = houseVariables[houseID][hWardrobe][slot - 1];

					format(string, sizeof(string), "You have changed your clothing (skin %d, slot %d).", houseVariables[houseID][hWardrobe][slot - 1], slot);
					SendClientMessage(playerid, COLOR_WHITE, string);

					GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
					format(string, sizeof(string), "* %s dresses in their new clothing.", szPlayerName);
					nearByMessage(playerid, COLOR_PURPLE, string);
				}
				else SendClientMessage(playerid, COLOR_GREY, "You don't have any clothing in that slot.");
		    }
		    else SendClientMessage(playerid, COLOR_GREY, "Invalid slot specified.");
	    }
	    else SendClientMessage(playerid, COLOR_GREY, "You're not inside your house.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You don't have a house.");
	return 1;
}

CMD:hgetweapon(playerid, params[]) {

	new
		slot = strval(params),
		houseID = getPlayerHouseID(playerid);

	if(isnull(params))
		return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/hgetweapon [slot 1-5]");

	else if(getPlayerHouseID(playerid) >= 1) {
	    if(GetPlayerVirtualWorld(playerid) == HOUSE_VIRTUAL_WORLD + houseID) {
			if(slot >= 1 && slot <= 5) {
				if(houseVariables[houseID][hWeapons][slot - 1] != 0) {
					if(playerVariables[playerid][pWeapons][GetWeaponSlot(houseVariables[houseID][hWeapons][slot - 1])] == 0) {

						new
							string[86];

						GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
						givePlayerValidWeapon(playerid, houseVariables[houseID][hWeapons][slot - 1]);

						format(string, sizeof(string), "* %s retrieves their %s from their safe.", szPlayerName, WeaponNames[houseVariables[houseID][hWeapons][slot - 1]]);
						nearByMessage(playerid, COLOR_PURPLE, string);

						format(string, sizeof(string), "You have withdrawn your %s from slot %d.", WeaponNames[houseVariables[houseID][hWeapons][slot - 1]], slot);
						SendClientMessage(playerid, COLOR_WHITE, string);
						houseVariables[houseID][hWeapons][slot - 1] = 0;
						saveHouse(houseID);
					}
					else SendClientMessage(playerid, COLOR_GREY, "You already have a weapon of this type on you - drop it first.");
				}
				else SendClientMessage(playerid, COLOR_GREY, "There is no weapon stored in that slot.");
			}
			else SendClientMessage(playerid, COLOR_GREY, "Invalid slot specified.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You're not inside your house.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You don't have a house.");
	return 1;
}

CMD:hstoreweapon(playerid, params[]) {

	new
		slot = strval(params),
		houseID = getPlayerHouseID(playerid);

	if(isnull(params))
		return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/hstoreweapon [slot 1-5]");

	else if(getPlayerHouseID(playerid) >= 1) {
	    if(GetPlayerVirtualWorld(playerid) == HOUSE_VIRTUAL_WORLD + houseID) {
			if(slot >= 1 && slot <= 5) {
				if(houseVariables[houseID][hWeapons][slot - 1] == 0) {

					new
						string[86],
						weapon;

					GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
					weapon = GetPlayerWeapon(playerid);

					switch(weapon) {
						case 16, 18, 35, 36, 37, 38, 39, 40, 44, 45, 46, 0: SendClientMessage(playerid, COLOR_GREY, "Invalid weapon.");
						default: {
							houseVariables[houseID][hWeapons][slot - 1] = weapon;
							removePlayerWeapon(playerid, weapon);

							format(string, sizeof(string), "* %s places their %s in their safe.", szPlayerName, WeaponNames[weapon]);
							nearByMessage(playerid, COLOR_PURPLE, string);

							format(string, sizeof(string), "You have stored your %s in slot %d.", WeaponNames[weapon], slot);
							SendClientMessage(playerid, COLOR_WHITE, string);

							saveHouse(houseID);
						}
					}
				}
				else SendClientMessage(playerid, COLOR_GREY, "That slot is already occupied.");
			}
			else SendClientMessage(playerid, COLOR_GREY, "Invalid slot specified.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You're not inside your house.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You don't have a house.");
	return 1;
}

CMD:hbalance(playerid, params[]) {
	new
	    houseID = getPlayerHouseID(playerid), // So we don't have to loop every single time... It's worth the 4 bytes!
		x;

	if(getPlayerHouseID(playerid) >= 1) {
	    new
	        messageString[128];

		format(messageString, sizeof(messageString), "Money: $%d | Materials: %d", houseVariables[houseID][hMoney], houseVariables[houseID][hMaterials]);

		for(new i; i < 5; i++) {
			if(houseVariables[houseID][hWeapons][i] > 0) {
				if(x == 0) format(messageString, sizeof(messageString),"%s | Weapons: %s (slot %d)", messageString, WeaponNames[houseVariables[houseID][hWeapons][i]], i);
				else format(messageString, sizeof(messageString),"%s, %s (slot %d)", messageString, WeaponNames[houseVariables[houseID][hWeapons][i]], i);
				x++;
			}
		}
		SendClientMessage(playerid, COLOR_WHITE, messageString);
	}
	else {
		return SendClientMessage(playerid, COLOR_GREY, "You don't have a house.");
	}
	return 1;
}

CMD:hwithdraw(playerid, params[]) {
	new
	    houseID = getPlayerHouseID(playerid); // So we don't have to loop every single time... It's worth the 4 bytes!

	if(getPlayerHouseID(playerid) >= 1) {
	    if(GetPlayerVirtualWorld(playerid) == HOUSE_VIRTUAL_WORLD+houseID) {
			new
			    amount,

			    houseOperation[72]; // For formatting afterwards.

			if(sscanf(params, "s[32]d", houseOperation, amount))
			    return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/hwithdraw [money/materials] [amount]");

		    if(!strcmp(houseOperation, "money", true)) {
		        if(houseVariables[houseID][hMoney] >= amount) {
		            if(amount >= 1 && amount < 60000000) {
		                houseVariables[houseID][hMoney] -= amount;
		                playerVariables[playerid][pMoney] += amount;

		                if(houseVariables[houseID][hMoney] < 1)
							houseVariables[houseID][hMoney] = 0;

		                if(playerVariables[playerid][pMoney] < 1)
							playerVariables[playerid][pMoney] = 0;

		                format(houseOperation, sizeof(houseOperation), "You have withdrawn $%d from your safe.", amount);
		                SendClientMessage(playerid, COLOR_WHITE, houseOperation);

						GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
						format(houseOperation, sizeof(houseOperation), "* %s withdraws $%d from their safe.", szPlayerName, amount);
						nearByMessage(playerid, COLOR_PURPLE, houseOperation);
		            }
					else {
						SendClientMessage(playerid, COLOR_GREY, "You can't withdraw a negative amount from a house safe. (01x03)");
						printf("[error] 01x03, %d", playerid);
					}
		        }
		    }
		    else if(!strcmp(houseOperation, "materials", true)) {
		        if(houseVariables[houseID][hMaterials] >= amount) {
		            if(amount >= 1 && amount < 60000000) {
		                houseVariables[houseID][hMaterials] -= amount;
		                playerVariables[playerid][pMaterials] += amount;

		                if(houseVariables[houseID][hMaterials] < 1)
							houseVariables[houseID][hMaterials] = 0;

						if(playerVariables[playerid][pMaterials] < 1)
							playerVariables[playerid][pMaterials] = 0;

		                format(houseOperation, sizeof(houseOperation), "You have withdrawn %d materials from your safe.", amount);
		                SendClientMessage(playerid, COLOR_WHITE, houseOperation);

						GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
						format(houseOperation, sizeof(houseOperation), "* %s withdraws %d materials from their safe.", szPlayerName, amount);
						nearByMessage(playerid, COLOR_PURPLE, houseOperation);
		            }
					else {
						SendClientMessage(playerid, COLOR_GREY, "You can't withdraw a negative amount from a house safe. (01x03)");
						printf("[error] 01x03, %d", playerid);
					}
		        }
		    }
		    else {
				return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/hwithdraw [money/materials] [amount]");
			}
		}
    }
    else {
		return SendClientMessage(playerid, COLOR_GREY, "You're not inside your house.");
	}
	return 1;
}

CMD:hdeposit(playerid, params[]) {
	new
	    houseID = getPlayerHouseID(playerid); // So we don't have to loop every single time... It's worth the 4 bytes!

	if(getPlayerHouseID(playerid) >= 1) {
	    if(GetPlayerVirtualWorld(playerid) == HOUSE_VIRTUAL_WORLD+houseID) {
			new
			    amount,
			    houseOperation[72]; // For formatting afterwards.

			if(sscanf(params, "s[32]d", houseOperation, amount)) {
			    return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/hdeposit [money/materials] [amount]");
			}
			else {
			    if(!strcmp(houseOperation, "money", true)) {
			        if(playerVariables[playerid][pMoney] >= amount) {
			            if(amount >= 1 && amount < 60000000) {
			                houseVariables[houseID][hMoney] += amount;
			                playerVariables[playerid][pMoney] -= amount;

			                if(houseVariables[houseID][hMoney] < 1) houseVariables[houseID][hMoney] = 0;
			                if(playerVariables[playerid][pMoney] < 1) playerVariables[playerid][pMoney] = 0;

			                format(houseOperation, sizeof(houseOperation), "You have deposited $%d in your safe.", amount);
			                SendClientMessage(playerid, COLOR_WHITE, houseOperation);

							GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
							format(houseOperation, sizeof(houseOperation), "* %s deposits $%d in their safe.", szPlayerName, amount);
							nearByMessage(playerid, COLOR_PURPLE, houseOperation);

			            }
					}
					else {
						SendClientMessage(playerid, COLOR_GREY, "You can't deposit a negative amountfrom a house safe. (01x01)");
						printf("[error] 01x01, %d", playerid);
			        }
			    }
			    else if(!strcmp(houseOperation, "materials", true)) {
			        if(playerVariables[playerid][pMaterials] >= amount) {
			            if(amount >= 1 && amount < 60000000) {
			                houseVariables[houseID][hMaterials] += amount;
			                playerVariables[playerid][pMaterials] -= amount;

			                if(houseVariables[houseID][hMaterials] < 1) houseVariables[houseID][hMaterials] = 0;
							if(playerVariables[playerid][pMaterials] < 1) playerVariables[playerid][pMaterials] = 0;

			                format(houseOperation, sizeof(houseOperation), "You have deposited %d materials in your safe.", amount);
			                SendClientMessage(playerid, COLOR_WHITE, houseOperation);

							GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
							format(houseOperation, sizeof(houseOperation), "* %s deposits %d materials in their safe.", szPlayerName, amount);
							nearByMessage(playerid, COLOR_PURPLE, houseOperation);
			            }
						else {
							SendClientMessage(playerid, COLOR_GREY, "You can't deposit a negative amountfrom a house safe. (01x01)");
							printf("[error] 01x01, %d", playerid);
				        }
			        }
			    }
			    else {
					return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/hdeposit [money/materials] [amount]");
				}
			}
	    }
	    else {
			return SendClientMessage(playerid, COLOR_GREY, "You're not inside your house.");
		}
	}
	return 1;
}

CMD:buyhouse(playerid, params[]) {
    if(playerVariables[playerid][pStatus] >= 1) {
        for(new x = 0; x < MAX_HOUSES; x++) {
			if(IsPlayerInRangeOfPoint(playerid, 5, houseVariables[x][hHouseExteriorPos][0], houseVariables[x][hHouseExteriorPos][1], houseVariables[x][hHouseExteriorPos][2])) {
				if(!strcmp(houseVariables[x][hHouseOwner], "Nobody", true)) {
				    if(houseVariables[x][hHousePrice] == -1) return SendClientMessage(playerid, COLOR_GREY, "This house was blocked from being purchased by an administrator.");
					if(getPlayerHouseID(playerid) >= 1) return SendClientMessage(playerid, COLOR_GREY, "You can't own 2 houses.");
					if(playerVariables[playerid][pMoney] >= houseVariables[x][hHousePrice]) {
						playerVariables[playerid][pMoney] -= houseVariables[x][hHousePrice];

						new
						    labelString[96];

						strcpy(houseVariables[x][hHouseOwner], playerVariables[playerid][pNormalName], MAX_PLAYER_NAME);

						DestroyDynamicPickup(houseVariables[x][hPickupID]);
						DestroyDynamic3DTextLabel(houseVariables[x][hLabelID]);

					    if(houseVariables[x][hHouseLocked] == 1) {
					    	format(labelString, sizeof(labelString), "House %d (owned)\nOwner: %s\n\n(locked)", x, houseVariables[x][hHouseOwner]);
					    }
					    else {
					        format(labelString, sizeof(labelString), "House %d (owned)\nOwner: %s\n\nPress ~k~~PED_DUCK~ to enter.", x, houseVariables[x][hHouseOwner]);
					    }

					    houseVariables[x][hLabelID] = CreateDynamic3DTextLabel(labelString, COLOR_YELLOW, houseVariables[x][hHouseExteriorPos][0], houseVariables[x][hHouseExteriorPos][1], houseVariables[x][hHouseExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
					    houseVariables[x][hPickupID] = CreateDynamicPickup(1272, 23, houseVariables[x][hHouseExteriorPos][0], houseVariables[x][hHouseExteriorPos][1], houseVariables[x][hHouseExteriorPos][2], 0, houseVariables[x][hHouseExteriorID], -1, 50);

						SendClientMessage(playerid, COLOR_WHITE, "Congratulations on your purchase - you are now the proud owner of this house!");

						saveHouse(x);
					}
					else SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this house.");
				}
				else {
					return SendClientMessage(playerid, COLOR_GREY, "You can't purchase an owned house.");
				}
			}
		}
    }
	return 1;
}

CMD:home(playerid, params[]) {

	new
		house = getPlayerHouseID(playerid);

	if(house >= 1) {
		if(playerVariables[playerid][pCheckpoint] >= 1) {
			new string[96];
			format(string, sizeof(string), "You already have an active checkpoint (%s), reach it first, or /killcheckpoint.", getPlayerCheckpointReason(playerid));
			return SendClientMessage(playerid, COLOR_GREY,string);
		}
		SetPlayerCheckpoint(playerid, houseVariables[house][hHouseExteriorPos][0], houseVariables[house][hHouseExteriorPos][1], houseVariables[house][hHouseExteriorPos][2], 5.0);
		SendClientMessage(playerid, COLOR_WHITE, "A checkpoint has been set to your house.");
		playerVariables[playerid][pCheckpoint] = 6;
	}
	else SendClientMessage(playerid, COLOR_GREY, "You don't own a house.");
	return 1;
} 

