YCMD:asellbusiness(playerid, params[], help) {
    if(playerVariables[playerid][pAdminLevel] < 3) return true;
	new
		bizzID = strval(params);

	if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/asellbusiness [businessid]");
	if(bizzID < 1 || bizzID > MAX_BUSINESSES) return SendClientMessage(playerid, COLOR_GREY, "Invalid business ID.");

	format(businessVariables[bizzID][bOwner], MAX_PLAYER_NAME, "Nobody");
	businessVariables[bizzID][bLocked] = 1;

	updateBusiness(bizzID);

	va_SendClientMessage(playerid, COLOR_WHITE, "You have admin-sold business ID %d.", bizzID);

    return true;
}

YCMD:btype(playerid, params[], help) {
    if(playerVariables[playerid][pAdminLevel] < 4) return true;
	new
		bCID,
		bCType;

	if(sscanf(params, "dd", bCID, bCType)) {
		SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/btype [businessid] [type]");
		SendClientMessage(playerid, COLOR_GREY, "Types: 0 - None, 1 - 24/7, 2 - Clothing, 3 - Bar, 4 - Sex Shop, 5 - Vehicle Dealership, 6 - Gym");
		SendClientMessage(playerid, COLOR_GREY, "Types: 7 - Restaurant");
		return 1;
	}

	if(isnull(businessVariables[bCID][bOwner])) return SendClientMessage(playerid, COLOR_GREY, "Invalid business ID.");
 
	va_SendClientMessage(playerid, COLOR_WHITE, "You have changed business ID %d to type %d.", bCID, bCType);

	businessVariables[bCID][bType] = bCType;
	saveBusiness(bCID);
	
	foreach(new i : Player) { // iterator for playerInBizz ? maybe
		if(GetPlayerVirtualWorld(i)-BUSINESS_VIRTUAL_WORLD == bCID)
			businessTypeMessages(bCID, i);
	} 

	return true;
}

YCMD:createbusiness(playerid, params[], help) {
    if(playerVariables[playerid][pAdminLevel] < 4) return true;

	Dialog_Show(playerid, DIALOG_CREATE_BUSINESS, DIALOG_STYLE_LIST, "Create business", "Exterior\nInterior\nComplete", "Ok", "Cancel");
	
	return true;
} 
/*
i will make /aeditbusiness with all commands in one dialog for this
YCMD:bprice(playerid, params[], help) {
	if(playerVariables[playerid][pAdminLevel] < 3) return true;
	new
		businessID,
		businessPrice;

	if(sscanf(params, "dd", businessID, businessPrice)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/bprice [businessid] [price]");
	if(businessID < 1 || businessID > MAX_BUSINESSES) return SendClientMessage(playerid, COLOR_GREY, "Invalid business ID.");
	
	va_SendClientMessage(playerid, COLOR_WHITE, "You have set business %d's price to $%d.", businessID, businessPrice);

	businessVariables[businessID][bPrice] = businessPrice;

	updateBusiness(businessID); 

	return true;
}

YCMD:movebusiness(playerid, params[], help) {
	if(playerVariables[playerid][pAdminLevel] < 3) return true;
	new
		houseID,
		subject[32];

	if(sscanf(params, "ds[32]", houseID, subject)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/movebusiness [businessid] [exterior/interior]"); 
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

	return 1;
}
*/

YCMD:gotobusiness(playerid, params[], help) {
	if(playerVariables[playerid][pAdminLevel] < 3) return true;
	if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/gotobusiness [businessid]");
	new houseID = strval(params);

	if(!Iter_Contains(bizz, houseID)) return SendClientMessage(playerid, COLOR_GREY, "Invalid business ID.");

	SetPlayerPos(playerid, businessVariables[houseID][bExteriorPos][0],businessVariables[houseID][bExteriorPos][1], businessVariables[houseID][bExteriorPos][2]);
	SetPlayerInterior(playerid, 0);
	SetPlayerVirtualWorld(playerid, 0);

	return true;
}

Dialog:DIALOG_CREATE_BUSINESS(playerid, response, listitem, inputtext[]) {
	if(!response) return true; 

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	switch(listitem+1) {
		case 1: {
			SetPVarFloat(playerid, "pBeX", x);
            SetPVarFloat(playerid, "pBeY", y);
            SetPVarFloat(playerid, "pBeZ", z);

			SetPVarInt(playerid, "bExt", 1);

			SendClientMessage(playerid, COLOR_WHITE, "Business exterior position configured.");
		}
		case 2: {
			SetPVarFloat(playerid, "pBiX", x);
            SetPVarFloat(playerid, "pBiY", y);
            SetPVarFloat(playerid, "pBiZ", z);

			SetPVarInt(playerid, "pBiID", GetPlayerInterior(playerid)); 

            SendClientMessage(playerid, COLOR_WHITE, "Business interior position configured.");
		}
		case 3: {
			if(GetPVarInt(playerid, "bExt") != 1 || GetPVarInt(playerid, "pBiID") < 1) return SendClientMessage(playerid, COLOR_GREY, "You haven't configured either the business exterior or interior. Creation attempt failed.");
			if(Iter_Count(bizz)+1 > MAX_BUSINESSES) return SendClientMessage(playerid, COLOR_WHITE, "There are no available business slots left.");

			inline finishBizz() {
				new i = cache_insert_id();

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

				Iter_Add(bizz, i);
				updateBusiness(i);
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
			}
			
			MySQL_TQueryInline(gSQL, using inline finishBizz, "INSERT INTO businesses (businessOwner, businessName) VALUES('Nobody', 'Nothing')"); 
		}
	}

	return true;
}