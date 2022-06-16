stock initiateVehicleSpawns() {
	return mysql_query("SELECT * FROM vehicles", THREAD_INITIATE_VEHICLES);
}   
stock PurchaseVehicleFromDealer(playerid, model, price) { // This is going to stop so much code-rape. :3
	if(playerVariables[playerid][pMoney] >= price) {
		if(playerVariables[playerid][pCarModel] < 1) {

			new
				string[64],
				businessID = GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD;

			playerVariables[playerid][pCarModel] = model; // Set the model.
			playerVariables[playerid][pCarPaintjob] = -1;

			playerVariables[playerid][pCarColour][0] = random(126);
			playerVariables[playerid][pCarColour][1] = random(126);

			playerVariables[playerid][pCarPos][0] = businessVariables[businessID][bMiscPos][0]; // Set the pos to the business misc pos.
			playerVariables[playerid][pCarPos][1] = businessVariables[businessID][bMiscPos][1];
			playerVariables[playerid][pCarPos][2] = businessVariables[businessID][bMiscPos][2];

			SpawnPlayerVehicle(playerid);

			playerVariables[playerid][pMoney] -= price;
			businessVariables[businessID][bVault] += price;

			format(string, sizeof(string), "Congratulations! You have purchased a %s for $%d.", VehicleNames[model - 400], price);
			SendClientMessage(playerid, COLOR_WHITE, string);

			ShowPlayerDialog(playerid, DIALOG_LICENSE_PLATE, DIALOG_STYLE_INPUT, "License plate registration", "Please enter a license plate for your vehicle. \n\nThere is only two conditions:\n- The license plate must be unique\n- The license plate can be alphanumerical, but it must consist of only 7 characters and include one space.", "Select", "");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You already have a vehicle; sell it first.");
	}
	else SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this vehicle.");
}

stock DestroyPlayerVehicle(playerid) { // This can be used for two things; resetting all vars, and completely destroying a player vehicle.

	playerVariables[playerid][pCarPos][0] = 0.0;
	playerVariables[playerid][pCarPos][1] = 0.0;
	playerVariables[playerid][pCarPos][2] = 0.0;
	playerVariables[playerid][pCarPos][3] = 0.0;
	playerVariables[playerid][pCarColour][0] = -1;
	playerVariables[playerid][pCarColour][1] = -1;
	playerVariables[playerid][pCarModel] = 0;
	playerVariables[playerid][pCarPaintjob] = -1; // 0 is a valid paintjob. D:
	playerVariables[playerid][pCarTrunk][0] = 0;
	playerVariables[playerid][pCarTrunk][1] = 0;

	new
		x;

	while(x < 13) {
		playerVariables[playerid][pCarMods][x] = 0;
		x++;
	}

	x = 0;

	while(x < 5) {
		playerVariables[playerid][pCarWeapons][x] = 0;
		x++;
	}

	if(doesVehicleExist(playerVariables[playerid][pCarID])) DestroyVehicle(playerVariables[playerid][pCarID]);

	playerVariables[playerid][pCarID] = -1;
	systemVariables[vehicleCounts][1]--;

	return 1;
}

stock SetAllVehiclesToRespawn() { // Doesn't bother looping through all cars/players, more efficient


	systemVariables[vehicleCounts][0] = 0;
	systemVariables[vehicleCounts][1] = 0;

	for(new x; x < MAX_VEHICLES; x++) {
		if(doesVehicleExist(vehicleVariables[x][vVehicleScriptID])) { // Saved
			DestroyVehicle(vehicleVariables[x][vVehicleScriptID]);
			vehicleVariables[x][vVehicleScriptID] = CreateVehicle(vehicleVariables[x][vVehicleModelID], vehicleVariables[x][vVehiclePosition][0], vehicleVariables[x][vVehiclePosition][1], vehicleVariables[x][vVehiclePosition][2], vehicleVariables[x][vVehicleRotation], vehicleVariables[x][vVehicleColour][0], vehicleVariables[x][vVehicleColour][1], 60000);
			systemVariables[vehicleCounts][0]++;
		}
		else if(doesVehicleExist(AdminSpawnedVehicles[x])) { // Admin
			SetVehicleToRespawn(AdminSpawnedVehicles[x]);
		}
	}
	foreach(Player, v) {  // Player.
		if(doesVehicleExist(playerVariables[v][pCarID]) && playerVariables[v][pCarModel] >= 1) {

			GetVehiclePos(playerVariables[v][pCarID], playerVariables[v][pCarPos][0], playerVariables[v][pCarPos][1], playerVariables[v][pCarPos][2]);
			GetVehicleZAngle(playerVariables[v][pCarID], playerVariables[v][pCarPos][3]);

			DestroyVehicle(playerVariables[v][pCarID]);
			playerVariables[v][pCarID] = CreateVehicle(playerVariables[v][pCarModel], playerVariables[v][pCarPos][0], playerVariables[v][pCarPos][1], playerVariables[v][pCarPos][2], playerVariables[v][pCarPos][3], playerVariables[v][pCarColour][0], playerVariables[v][pCarColour][1], -1);

			for(new i = 0; i < 13; i++) {
				if(playerVariables[v][pCarMods][i] >= 1) AddVehicleComponent(playerVariables[v][pCarID], playerVariables[v][pCarMods][i]);
			}
			if(playerVariables[v][pCarPaintjob] >= 0) ChangeVehiclePaintjob(playerVariables[v][pCarID], playerVariables[v][pCarPaintjob]);
			systemVariables[vehicleCounts][1]++;
		}
	}
	return 1;
}

stock SetVehicleToRespawnEx(vehicleid) { // Great for respawning any given type of vehicle (player/admin/saved).

	foreach(Player, v) {  // Player.
		if(vehicleid == playerVariables[v][pCarID] && playerVariables[v][pCarModel] >= 1) {

			GetVehiclePos(playerVariables[v][pCarID], playerVariables[v][pCarPos][0], playerVariables[v][pCarPos][1], playerVariables[v][pCarPos][2]);
			GetVehicleZAngle(playerVariables[v][pCarID], playerVariables[v][pCarPos][3]);

			DestroyVehicle(playerVariables[v][pCarID]);
			playerVariables[v][pCarID] = CreateVehicle(playerVariables[v][pCarModel], playerVariables[v][pCarPos][0], playerVariables[v][pCarPos][1], playerVariables[v][pCarPos][2], playerVariables[v][pCarPos][3], playerVariables[v][pCarColour][0], playerVariables[v][pCarColour][1], -1);

			for(new i = 0; i < 13; i++) {
				if(playerVariables[v][pCarMods][i] >= 1) AddVehicleComponent(playerVariables[v][pCarID], playerVariables[v][pCarMods][i]);
			}
			if(playerVariables[v][pCarPaintjob] >= 0) ChangeVehiclePaintjob(playerVariables[v][pCarID], playerVariables[v][pCarPaintjob]);
			return 1;
		}
	}

	for(new x; x < MAX_VEHICLES; x++) {
		if(vehicleVariables[x][vVehicleScriptID] == vehicleid) { // Saved
			DestroyVehicle(vehicleVariables[x][vVehicleScriptID]);
			vehicleVariables[x][vVehicleScriptID] = CreateVehicle(vehicleVariables[x][vVehicleModelID], vehicleVariables[x][vVehiclePosition][0], vehicleVariables[x][vVehiclePosition][1], vehicleVariables[x][vVehiclePosition][2], vehicleVariables[x][vVehicleRotation], vehicleVariables[x][vVehicleColour][0], vehicleVariables[x][vVehicleColour][1], 60000);
			return 1;
		}
		else if(AdminSpawnedVehicles[x] == vehicleid) { // Admin
			SetVehicleToRespawn(AdminSpawnedVehicles[x]);
			return 1;
		}
	}
	return 1;
}

stock SpawnPlayerVehicle(playerid) {
	if(playerVariables[playerid][pCarModel] >= 1) {
		if(systemVariables[vehicleCounts][0] + systemVariables[vehicleCounts][1] + systemVariables[vehicleCounts][2] < MAX_VEHICLES) {
			if(doesVehicleExist(playerVariables[playerid][pCarID])) DestroyVehicle(playerVariables[playerid][pCarID]); // In case the IDs decide to f*$^# up.
			playerVariables[playerid][pCarID] = CreateVehicle(playerVariables[playerid][pCarModel], playerVariables[playerid][pCarPos][0], playerVariables[playerid][pCarPos][1], playerVariables[playerid][pCarPos][2], playerVariables[playerid][pCarPos][3], playerVariables[playerid][pCarColour][0], playerVariables[playerid][pCarColour][1], -1);

			for(new i = 0; i < 13; i++) {
				if(playerVariables[playerid][pCarMods][i] >= 1) AddVehicleComponent(playerVariables[playerid][pCarID], playerVariables[playerid][pCarMods][i]);
			}

			systemVariables[vehicleCounts][1]++;
			if(playerVariables[playerid][pCarPaintjob] >= 0) ChangeVehiclePaintjob(playerVariables[playerid][pCarID], playerVariables[playerid][pCarPaintjob]);
	        SetVehicleNumberPlate(playerVariables[playerid][pCarID], playerVariables[playerid][pCarLicensePlate]);

	        // De-stream the vehicle
	        SetVehicleVirtualWorld(playerVariables[playerid][pCarID], GetVehicleVirtualWorld(playerVariables[playerid][pCarID])+1);
	        SetVehicleVirtualWorld(playerVariables[playerid][pCarID], GetVehicleVirtualWorld(playerVariables[playerid][pCarID])-1);
		}
		else printf("ERROR: Vehicle limit reached (MODEL %d, PLAYER %d, MAXIMUM %d, TYPE PLAYER) [01x08]", playerVariables[playerid][pCarModel], playerid, MAX_VEHICLES);
	}
	return 1;
}

stock saveVehicle(const id) {
	if(vehicleVariables[id][vVehicleModelID] >= 1) {
	    new
	        queryString[255];

	    GetVehiclePos(vehicleVariables[id][vVehicleScriptID], vehicleVariables[id][vVehiclePosition][0], vehicleVariables[id][vVehiclePosition][1], vehicleVariables[id][vVehiclePosition][2]);
	    GetVehicleZAngle(vehicleVariables[id][vVehicleScriptID], vehicleVariables[id][vVehicleRotation]);

	    format(queryString, sizeof(queryString), "UPDATE vehicles SET vehicleModelID = '%d', vehiclePosX = '%f', vehiclePosY = '%f', vehiclePosZ = '%f', vehiclePosRotation = '%f', vehicleGroup = '%d', vehicleCol1 = '%d', vehicleCol2 = '%d' WHERE vehicleID = '%d'", vehicleVariables[id][vVehicleModelID],	vehicleVariables[id][vVehiclePosition][0],
		vehicleVariables[id][vVehiclePosition][1], vehicleVariables[id][vVehiclePosition][2], vehicleVariables[id][vVehicleRotation], vehicleVariables[id][vVehicleGroup], vehicleVariables[id][vVehicleColour][0], vehicleVariables[id][vVehicleColour][1], id);
		mysql_query(queryString);
	}
	return 1;
} 