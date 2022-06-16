CMD:asellhouse(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 3) {
        new
            houseID = strval(params);

		if(!isnull(params)) {
		    if(houseID < 1 || houseID > MAX_HOUSES) return SendClientMessage(playerid, COLOR_GREY, "Invalid house ID.");

	        new
	            labelString[96];

	        format(houseVariables[houseID][hHouseOwner], MAX_PLAYER_NAME, "Nobody");
	        format(labelString, sizeof(labelString), "House %d (un-owned - /buyhouse)\nPrice: $%d\n\n(locked)", houseID, houseVariables[houseID][hHousePrice]);

	        DestroyDynamicPickup(houseVariables[houseID][hPickupID]);
	        DestroyDynamic3DTextLabel(houseVariables[houseID][hLabelID]);

	        houseVariables[houseID][hLabelID] = CreateDynamic3DTextLabel(labelString, COLOR_YELLOW, houseVariables[houseID][hHouseExteriorPos][0], houseVariables[houseID][hHouseExteriorPos][1], houseVariables[houseID][hHouseExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
			houseVariables[houseID][hPickupID] = CreateDynamicPickup(1273, 23, houseVariables[houseID][hHouseExteriorPos][0], houseVariables[houseID][hHouseExteriorPos][1], houseVariables[houseID][hHouseExteriorPos][2], 0, houseVariables[houseID][hHouseExteriorID], -1, 250);

			houseVariables[houseID][hHouseLocked] = 1;

			format(labelString, sizeof(labelString), "You have admin-sold house ID %d.", houseID);
			SendClientMessage(playerid, COLOR_WHITE, labelString);

		    saveHouse(houseID);
		}
		else {
		    return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/asellhouse [houseid]");
		}
    }

    return 1;
} 

CMD:createhouse(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 4) {
		new
	   		Float: floatPos[3];
	   		
		if(!strcmp(params, "Exterior", true)) {
	        GetPlayerPos(playerid, floatPos[0], floatPos[1], floatPos[2]);

	        SetPVarFloat(playerid, "pHeX", floatPos[0]);
	        SetPVarFloat(playerid, "pHeY", floatPos[1]);
	        SetPVarFloat(playerid, "pHeZ", floatPos[2]);

	        SetPVarInt(playerid, "hExt", 1);

	        SendClientMessage(playerid, COLOR_WHITE, "House exterior position configured.");
		} else if(!strcmp(params, "Interior", true)) {
            GetPlayerPos(playerid, floatPos[0], floatPos[1], floatPos[2]);

            SetPVarFloat(playerid, "pHiX", floatPos[0]);
            SetPVarFloat(playerid, "pHiY", floatPos[1]);
            SetPVarFloat(playerid, "pHiZ", floatPos[2]);

			SetPVarInt(playerid, "pHiID", GetPlayerInterior(playerid));
            SetPVarInt(playerid, "hInt", 1);

            SendClientMessage(playerid, COLOR_WHITE, "House interior position configured.");
		} else if(!strcmp(params, "Complete", true)) {
        	if(GetPVarInt(playerid, "hExt") != 1 || GetPVarInt(playerid, "hInt") != 1)
				return SendClientMessage(playerid, COLOR_GREY, "You haven't configured either the house exterior or interior. Creation attempt failed.");

			new
			    i,
           		labelString[96];

			mysql_query("INSERT INTO houses (houseOwner, houseLocked) VALUES('Nobody', '1')");
			i = mysql_insert_id();

			if(isnull(houseVariables[i][hHouseOwner])) {
				houseVariables[i][hHouseExteriorPos][0] = GetPVarFloat(playerid, "pHeX");
				houseVariables[i][hHouseExteriorPos][1] = GetPVarFloat(playerid, "pHeY");
				houseVariables[i][hHouseExteriorPos][2] = GetPVarFloat(playerid, "pHeZ");

			    houseVariables[i][hHouseInteriorPos][0] = GetPVarFloat(playerid, "pHiX");
			    houseVariables[i][hHouseInteriorPos][1] = GetPVarFloat(playerid, "pHiY");
			    houseVariables[i][hHouseInteriorPos][2] = GetPVarFloat(playerid, "pHiZ");

			    houseVariables[i][hHouseExteriorID] = 0;
			    houseVariables[i][hHouseInteriorID] = GetPVarInt(playerid, "pHiID");

			    houseVariables[i][hHouseLocked] = 1;

 		        format(houseVariables[i][hHouseOwner], MAX_PLAYER_NAME, "Nobody");
		        format(labelString, sizeof(labelString), "House %d (un-owned - /buyhouse)\nPrice: $%d\n\n(locked)", i, houseVariables[i][hHousePrice]);

		        houseVariables[i][hLabelID] = CreateDynamic3DTextLabel(labelString, COLOR_YELLOW, houseVariables[i][hHouseExteriorPos][0], houseVariables[i][hHouseExteriorPos][1], houseVariables[i][hHouseExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
				houseVariables[i][hPickupID] = CreateDynamicPickup(1273, 23, houseVariables[i][hHouseExteriorPos][0], houseVariables[i][hHouseExteriorPos][1], houseVariables[i][hHouseExteriorPos][2], 0, 0, -1, 250);

				saveHouse(i);

			    DeletePVar(playerid, "pHeX");
			    DeletePVar(playerid, "pHeY");
			    DeletePVar(playerid, "pHeZ");
			    DeletePVar(playerid, "pHiX");
			    DeletePVar(playerid, "pHeY");
			    DeletePVar(playerid, "pHeZ");
			    DeletePVar(playerid, "pHiID");

			    SetPlayerInterior(playerid, 0);
			    SetPlayerPos(playerid, houseVariables[i][hHouseExteriorPos][0], houseVariables[i][hHouseExteriorPos][1], houseVariables[i][hHouseExteriorPos][2]);

				systemVariables[houseCount]++;
		        return SendClientMessage(playerid, COLOR_WHITE, "House created!");
			} else
				return SendClientMessage(playerid, COLOR_WHITE, "There are no available house slots left, sorry!");
		} else
		    return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/createhouse [exterior/interior/complete]");
	}
	return 1;
}

CMD:hprice(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 3) {
	    new
	        houseID,
	        housePrice;

		if(sscanf(params, "dd", houseID, housePrice))
			return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/hprice [houseid] [price]");

	    if(houseID < 1 || houseID > MAX_HOUSES)
			return SendClientMessage(playerid, COLOR_GREY, "Invalid house ID.");

		format(szMessage, sizeof(szMessage), "You have set house %d's price to $%d.", houseID, housePrice);
		SendClientMessage(playerid, COLOR_WHITE, szMessage);

		houseVariables[houseID][hHousePrice] = housePrice;

		if(!strcmp(houseVariables[houseID][hHouseOwner], "Nobody", true) && strlen(houseVariables[houseID][hHouseOwner]) >= 1) {
			switch(houseVariables[houseID][hHouseLocked]) {
				case 0: format(szMessage, sizeof(szMessage), "House %d (un-owned - /buyhouse)\nPrice: $%d\n\n(locked)", houseID, houseVariables[houseID][hHousePrice]);
				default: format(szMessage, sizeof(szMessage), "House %d (un-owned - /buyhouse)\nPrice: $%d\n\nPress ~k~~PED_DUCK~ to enter.", houseID, houseVariables[houseID][hHousePrice]);
			}

			UpdateDynamic3DTextLabelText(houseVariables[houseID][hLabelID], COLOR_YELLOW, szMessage);
		}
	}
	return 1;
}

CMD:movehouse(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 3) {
	    new
	        houseID,
	        subject[32];

		if(sscanf(params, "ds[32]", houseID, subject)) {
		    SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/movehouse [houseid] [exterior/interior]");
		}
		else {
		    if(houseID < 1 || houseID > MAX_HOUSES) return SendClientMessage(playerid, COLOR_GREY, "Invalid house ID.");

            if(strcmp(subject, "exterior", true) == 0) {
			    GetPlayerPos(playerid, houseVariables[houseID][hHouseExteriorPos][0], houseVariables[houseID][hHouseExteriorPos][1], houseVariables[houseID][hHouseExteriorPos][2]);

			    DestroyDynamic3DTextLabel(houseVariables[houseID][hLabelID]);
			    DestroyDynamicPickup(houseVariables[houseID][hPickupID]);

				if(!strcmp(houseVariables[houseID][hHouseOwner], "Nobody", true) && strlen(houseVariables[houseID][hHouseOwner]) >= 1) {
				    new
				        labelString[96];

				    if(houseVariables[houseID][hHouseLocked] == 1) {
				    	format(labelString, sizeof(labelString), "House %d (un-owned - /buyhouse)\nPrice: $%d\n\n(locked)", houseID, houseVariables[houseID][hHousePrice]);
				    }
				    else {
				        format(labelString, sizeof(labelString), "House %d (un-owned - /buyhouse)\nPrice: $%d\n\nPress ~k~~PED_DUCK~ to enter.", houseID, houseVariables[houseID][hHousePrice]);
				    }

				    houseVariables[houseID][hLabelID] = CreateDynamic3DTextLabel(labelString, COLOR_YELLOW, houseVariables[houseID][hHouseExteriorPos][0], houseVariables[houseID][hHouseExteriorPos][1], houseVariables[houseID][hHouseExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
					houseVariables[houseID][hPickupID] = CreateDynamicPickup(1273, 23, houseVariables[houseID][hHouseExteriorPos][0], houseVariables[houseID][hHouseExteriorPos][1], houseVariables[houseID][hHouseExteriorPos][2], 0, houseVariables[houseID][hHouseExteriorID], -1, 250);

				}
				else {
				    new
				        labelString[96];

				    if(houseVariables[houseID][hHouseLocked] == 1) {
				    	format(labelString, sizeof(labelString), "House %d (owned)\nOwner: %s\n\n(locked)", houseID, houseVariables[houseID][hHouseOwner]);
				    }
				    else {
				        format(labelString, sizeof(labelString), "House %d (owned)\nOwner: %s\n\nPress ~k~~PED_DUCK~ to enter.", houseID, houseVariables[houseID][hHouseOwner]);
				    }

				    houseVariables[houseID][hLabelID] = CreateDynamic3DTextLabel(labelString, COLOR_YELLOW, houseVariables[houseID][hHouseExteriorPos][0], houseVariables[houseID][hHouseExteriorPos][1], houseVariables[houseID][hHouseExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
				    houseVariables[houseID][hPickupID] = CreateDynamicPickup(1272, 23, houseVariables[houseID][hHouseExteriorPos][0], houseVariables[houseID][hHouseExteriorPos][1], houseVariables[houseID][hHouseExteriorPos][2], 0, houseVariables[houseID][hHouseExteriorID], -1, 50);
				}

				SendClientMessage(playerid, COLOR_WHITE, "The house exterior has successfully been changed.");
			}
			else if(strcmp(subject, "interior", true) == 0) {
			    GetPlayerPos(playerid, houseVariables[houseID][hHouseInteriorPos][0], houseVariables[houseID][hHouseInteriorPos][1], houseVariables[houseID][hHouseInteriorPos][2]);
			    houseVariables[houseID][hHouseInteriorID] = GetPlayerInterior(playerid);
			    SendClientMessage(playerid, COLOR_WHITE, "The house interior has successfully been changed.");
			}

			saveHouse(houseID);
		}
	}

	return 1;
}

CMD:gotohouse(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 3) {

		if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/gotohouse [houseid]");
	    new
	        houseID = strval(params);

		if(houseID < 1 || houseID > MAX_HOUSES) return SendClientMessage(playerid, COLOR_GREY, "Invalid house ID.");

		SetPlayerPos(playerid, houseVariables[houseID][hHouseExteriorPos][0], houseVariables[houseID][hHouseExteriorPos][1], houseVariables[houseID][hHouseExteriorPos][2]);
		SetPlayerInterior(playerid, 0);
		SetPlayerVirtualWorld(playerid, 0);

	}
	return 1;
}

