CMD:asellbusiness(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 3) {
        new
            houseID = strval(params);

		if(!isnull(params)) {
		    if(houseID < 1 || houseID > MAX_BUSINESSES) return SendClientMessage(playerid, COLOR_GREY, "Invalid business ID.");

	        new
	            labelString[96];

	        format(businessVariables[houseID][bOwner], MAX_PLAYER_NAME, "Nobody");

	        DestroyDynamicPickup(businessVariables[houseID][bPickupID]);
	        DestroyDynamic3DTextLabel(businessVariables[houseID][bLabelID]);

			format(labelString, sizeof(labelString), "%s\n(Business %d - un-owned)\nPrice: $%d (/buybusiness)\n\n(locked)", businessVariables[houseID][bName], houseID, businessVariables[houseID][bPrice]);

			businessVariables[houseID][bLabelID] = CreateDynamic3DTextLabel(labelString, COLOR_YELLOW, businessVariables[houseID][bExteriorPos][0], businessVariables[houseID][bExteriorPos][1], businessVariables[houseID][bExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
			businessVariables[houseID][bPickupID] = CreateDynamicPickup(1239, 23, businessVariables[houseID][bExteriorPos][0], businessVariables[houseID][bExteriorPos][1], businessVariables[houseID][bExteriorPos][2], 0, 0, -1, 250);

			businessVariables[houseID][bLocked] = 1;

			format(labelString, sizeof(labelString), "You have admin-sold business ID %d.", houseID);
			SendClientMessage(playerid, COLOR_WHITE, labelString);

		    saveHouse(houseID);
		}
		else {
		    return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/asellbusiness [businessid]");
		}
    }

    return 1;
}

CMD:btype(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 4) {
        new
            bCID,
            bCType;

		if(sscanf(params, "dd", bCID, bCType)) {
		    SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/btype [businessid] [type]");
            SendClientMessage(playerid, COLOR_GREY, "Types: 0 - None, 1 - 24/7, 2 - Clothing, 3 - Bar, 4 - Sex Shop, 5 - Vehicle Dealership, 6 - Gym");
			SendClientMessage(playerid, COLOR_GREY, "Types: 7 - Restaurant");
			return 1;
		}

		if(!isnull(businessVariables[bCID][bOwner])) {
		    format(szMessage, sizeof(szMessage), "You have changed business ID %d to type %d.", bCID, bCType);
		    SendClientMessage(playerid, COLOR_WHITE, szMessage);
		    
		    if(businessVariables[bCID][bType] != bCType) {
		    	format(szQueryOutput, sizeof(szQueryOutput), "DELETE FROM `businessitems` WHERE `itemBusinessId` = %d;", bCID);
		    	mysql_query(szQueryOutput, THREAD_CHANGE_BUSINESS_TYPE_ITEMS, bCID);
		    }

		    businessVariables[bCID][bType] = bCType;
		    saveBusiness(bCID);
		    
		    foreach(Player, x) {
		        if(GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD == bCID)
					businessTypeMessages(bCID, x);
		    }
		} else return SendClientMessage(playerid, COLOR_GREY, "Invalid business ID.");
	}
	return 1;
}

CMD:createbusiness(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 4) {
    	new
          	Float: floatPos[3];

		if(!strcmp(params, "exterior", true)) {
	        GetPlayerPos(playerid, floatPos[0], floatPos[1], floatPos[2]);

            SetPVarFloat(playerid, "pBeX", floatPos[0]);
            SetPVarFloat(playerid, "pBeY", floatPos[1]);
            SetPVarFloat(playerid, "pBeZ", floatPos[2]);

            SetPVarInt(playerid, "bExt", 1);

            SendClientMessage(playerid, COLOR_WHITE, "Business exterior position configured.");
		}
        else if(!strcmp(params, "interior", true)) {
        	GetPlayerPos(playerid, floatPos[0], floatPos[1], floatPos[2]);

            SetPVarFloat(playerid, "pBiX", floatPos[0]);
            SetPVarFloat(playerid, "pBiY", floatPos[1]);
            SetPVarFloat(playerid, "pBiZ", floatPos[2]);

			SetPVarInt(playerid, "pBiID", GetPlayerInterior(playerid));
            SetPVarInt(playerid, "bInt", 1);

            SendClientMessage(playerid, COLOR_WHITE, "Business interior position configured.");
		}
        else if(!strcmp(params, "Complete", true)) {
        	if(GetPVarInt(playerid, "bExt") != 1 || GetPVarInt(playerid, "bInt") != 1)
            	return SendClientMessage(playerid, COLOR_GREY, "You haven't configured either the business exterior or interior. Creation attempt failed.");

			new
			    i,
	       		labelString[128];

			mysql_query("INSERT INTO businesses (businessOwner, businessName) VALUES('Nobody', 'New Business')");
			i = mysql_insert_id();

			if(isnull(businessVariables[i][bOwner])) {
				businessVariables[i][bExteriorPos][0] = GetPVarFloat(playerid, "pBeX");
			    businessVariables[i][bExteriorPos][1] = GetPVarFloat(playerid, "pBeY");
			    businessVariables[i][bExteriorPos][2] = GetPVarFloat(playerid, "pBeZ");

			    businessVariables[i][bInteriorPos][0] = GetPVarFloat(playerid, "pBiX");
			    businessVariables[i][bInteriorPos][1] = GetPVarFloat(playerid, "pBiY");
			    businessVariables[i][bInteriorPos][2] = GetPVarFloat(playerid, "pBiZ");

			    businessVariables[i][bInterior] = GetPVarInt(playerid, "pBiID");

 		        format(businessVariables[i][bOwner], MAX_PLAYER_NAME, "Nobody");
 		        format(businessVariables[i][bName], 32, "Nothing");

 		        businessVariables[i][bLocked] = 1;

		        format(labelString, sizeof(labelString), "%s\n(Business %d - un-owned)\nPrice: $%d (/buybusiness)\n\n(locked)", businessVariables[i][bName], i, businessVariables[i][bPrice]);

		        businessVariables[i][bLabelID] = CreateDynamic3DTextLabel(labelString, COLOR_YELLOW, businessVariables[i][bExteriorPos][0], businessVariables[i][bExteriorPos][1], businessVariables[i][bExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
				businessVariables[i][bPickupID] = CreateDynamicPickup(1239, 23, businessVariables[i][bExteriorPos][0], businessVariables[i][bExteriorPos][1], businessVariables[i][bExteriorPos][2], 0, 0, -1, 250);

				saveBusiness(i);

			    DeletePVar(playerid, "pBeX");
			    DeletePVar(playerid, "pBeY");
			    DeletePVar(playerid, "pBeZ");
			    DeletePVar(playerid, "pBiX");
			    DeletePVar(playerid, "pBeY");
			    DeletePVar(playerid, "pBeZ");
			    DeletePVar(playerid, "pBiID");

			    SetPlayerInterior(playerid, 0);
			    SetPlayerPos(playerid, businessVariables[i][bExteriorPos][0], businessVariables[i][bExteriorPos][1], businessVariables[i][bExteriorPos][2]);
				systemVariables[businessCount]++;
				
				createRelevantItems(i);
		        return SendClientMessage(playerid, COLOR_WHITE, "Business created!");
			} else
				return SendClientMessage(playerid, COLOR_WHITE, "There are no available business slots left.");

		} else
        	return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/createbusiness [interior/exit/complete]");
	}
	return 1;
}

CMD:bprice(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 3) {
	    new
	        businessID,
	        businessPrice;

		if(sscanf(params, "dd", businessID, businessPrice)) {
			return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/bprice [businessid] [price]");
		}
		else {
		    if(businessID < 1 || businessID > MAX_BUSINESSES) {
				return SendClientMessage(playerid, COLOR_GREY, "Invalid business ID.");
			}
		    else {
				format(szMessage, sizeof(szMessage), "You have set business %d's price to $%d.", businessID, businessPrice);
				SendClientMessage(playerid, COLOR_WHITE, szMessage);

				businessVariables[businessID][bPrice] = businessPrice;

				if(!strcmp(businessVariables[businessID][bOwner], "Nobody", true) && strlen(businessVariables[businessID][bOwner]) >= 1) {
					switch(businessVariables[businessID][bLocked]) {
						case 1: format(szMessage, sizeof(szMessage), "%s\n(Business %d - un-owned)\nPrice: $%d (/buybusiness)\n\n(locked)", businessVariables[businessID][bName], businessID, businessVariables[businessID][bPrice]);
						default: format(szMessage, sizeof(szMessage), "%s\n(Business %d - un-owned)\nPrice: $%d (/buybusiness)\n\nPress ~k~~PED_DUCK~ to enter.", businessVariables[businessID][bName], businessID, businessVariables[businessID][bPrice]);
					}

					UpdateDynamic3DTextLabelText(businessVariables[businessID][bLabelID], COLOR_YELLOW, szMessage);
				}
			}
		}
	}
	return 1;
}

CMD:movebusiness(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 3) {
	    new
	        houseID,
	        subject[32];

		if(sscanf(params, "ds[32]", houseID, subject)) {
		    SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/movebusiness [businessid] [exterior/interior]");
		}
		else {
		    if(houseID < 1 || houseID > MAX_BUSINESSES) return SendClientMessage(playerid, COLOR_GREY, "Invalid business ID.");

            if(strcmp(subject, "exterior", true) == 0) {
			    GetPlayerPos(playerid, businessVariables[houseID][bExteriorPos][0], businessVariables[houseID][bExteriorPos][1], businessVariables[houseID][bExteriorPos][2]);

			    DestroyDynamic3DTextLabel(businessVariables[houseID][bLabelID]);
			    DestroyDynamicPickup(businessVariables[houseID][bPickupID]);

				if(!strcmp(businessVariables[houseID][bOwner], "Nobody", true) && strlen(businessVariables[houseID][bOwner]) >= 1) {
				    new
				        labelString[96];

				    if(businessVariables[houseID][bLocked] == 1) {
				    	format(labelString, sizeof(labelString), "%s\n(Business %d - un-owned)\nPrice: $%d (/buybusiness)\n\n(locked)", businessVariables[houseID][bName], houseID, businessVariables[houseID][bPrice]);
				    }
				    else {
				        format(labelString, sizeof(labelString), "%s\n(Business %d - un-owned)\nPrice: $%d (/buybusiness)\n\nPress ~k~~PED_DUCK~ to enter.", businessVariables[houseID][bName], houseID, businessVariables[houseID][bPrice]);
				    }

				    businessVariables[houseID][bLabelID] = CreateDynamic3DTextLabel(labelString, COLOR_YELLOW, businessVariables[houseID][bExteriorPos][0], businessVariables[houseID][bExteriorPos][1], businessVariables[houseID][bExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
					businessVariables[houseID][bPickupID] = CreateDynamicPickup(1239, 23, businessVariables[houseID][bExteriorPos][0], businessVariables[houseID][bExteriorPos][1], businessVariables[houseID][bExteriorPos][2], 0, 0, -1, 250);

				}
				else {
				    new
				        labelString[96];

				    if(businessVariables[houseID][bLocked] == 1) {
				    	format(labelString, sizeof(labelString), "%s\n(Business %d - owned by %s)\n\n(locked)", businessVariables[houseID][bName], houseID, businessVariables[houseID][bOwner]);
				    }
				    else {
				        format(labelString, sizeof(labelString), "%s\n(Business %d - owned by %s)\n\nPress ~k~~PED_DUCK~ to enter", businessVariables[houseID][bName], houseID, businessVariables[houseID][bOwner]);
				    }

				    businessVariables[houseID][bLabelID] = CreateDynamic3DTextLabel(labelString, COLOR_YELLOW, businessVariables[houseID][bExteriorPos][0], businessVariables[houseID][bExteriorPos][1], businessVariables[houseID][bExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
					businessVariables[houseID][bPickupID] = CreateDynamicPickup(1239, 23, businessVariables[houseID][bExteriorPos][0], businessVariables[houseID][bExteriorPos][1], businessVariables[houseID][bExteriorPos][2], 0, 0, -1, 250);
				}

				SendClientMessage(playerid, COLOR_WHITE, "The business exterior has successfully been changed.");
			}
			else if(strcmp(subject, "interior", true) == 0) {
			    GetPlayerPos(playerid, businessVariables[houseID][bInteriorPos][0], businessVariables[houseID][bInteriorPos][1], businessVariables[houseID][bInteriorPos][2]);
			    businessVariables[houseID][bInterior] = GetPlayerInterior(playerid);
			    SendClientMessage(playerid, COLOR_WHITE, "The business interior has successfully been changed.");
			}
			saveBusiness(houseID);
		}
	}

	return 1;
}

CMD:gotobusiness(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 3) {

		if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/gotobusiness [businessid]");
	    new
	        houseID = strval(params);

		if(houseID < 1 || houseID > MAX_BUSINESSES) return SendClientMessage(playerid, COLOR_GREY, "Invalid business ID.");

		SetPlayerPos(playerid, businessVariables[houseID][bExteriorPos][0],businessVariables[houseID][bExteriorPos][1], businessVariables[houseID][bExteriorPos][2]);
		SetPlayerInterior(playerid, 0);
		SetPlayerVirtualWorld(playerid, 0);

	}
	return 1;
}

