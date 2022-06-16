CMD:savevehicle(playerid, params[]) {
    if(playerVariables[playerid][pAdminLevel] >= 4) {
		if(!IsPlayerInAnyVehicle(playerid))
			return SendClientMessage(playerid, COLOR_GREY, "You need to be in a vehicle to save it.");

		if(GetPVarInt(playerid, "sCc") == 1) {

		    new
		        i,
		        queryString[255],
		        Float: vPos[4]; // x, y, z + z angle

		    GetVehiclePos(GetPlayerVehicleID(playerid), vPos[0], vPos[1], vPos[2]);
		    GetVehicleZAngle(GetPlayerVehicleID(playerid), vPos[3]);

		    format(queryString, sizeof(queryString), "INSERT INTO vehicles (vehicleModelID, vehiclePosX, vehiclePosY, vehiclePosZ, vehiclePosRotation) VALUES('%d', '%f', '%f', '%f', '%f')", GetVehicleModel(GetPlayerVehicleID(playerid)), vPos[0], vPos[1], vPos[2], vPos[3]);
		    mysql_query(queryString);

		    i = mysql_insert_id();

		    SendClientMessage(playerid, COLOR_WHITE, "Vehicle saved!");

		    vehicleVariables[i][vVehicleModelID] = GetVehicleModel(GetPlayerVehicleID(playerid));
		    vehicleVariables[i][vVehiclePosition][0] = vPos[0];
		    vehicleVariables[i][vVehiclePosition][1] = vPos[1];
		    vehicleVariables[i][vVehiclePosition][2] = vPos[2];

		    vehicleVariables[i][vVehicleRotation] = vPos[3];
		    vehicleVariables[i][vVehicleGroup] = 0;

		    vehicleVariables[i][vVehicleScriptID] = GetPlayerVehicleID(playerid);

		    for(new x = 0; x < MAX_VEHICLES; x++) {
		    	if(AdminSpawnedVehicles[x] == GetPlayerVehicleID(playerid)) {
		    	    AdminSpawnedVehicles[x] = 0; // If the vehicle is admin-spawned, we can remove it from the array and move it to the vehicle script enum/arrays.
		    	}
		    }

			systemVariables[vehicleCounts][2]--;
			systemVariables[vehicleCounts][0]++;
			DeletePVar(playerid, "sCc");
		}
		else {
		    SetPVarInt(playerid, "sCc", 1);
		    return SendClientMessage(playerid, COLOR_GREY, "Are you sure you wish to save this vehicle? Re-type the command to verify your action is legitimate.");
		}
	}
	return 1;
} 

CMD:vgroup(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 4) {
	    if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You must be inside the vehicle that you wish to alter the group requirement of.");

		new
			string[96],
		    groupParam = strval(params);

		if(groupParam < 0 || groupParam > MAX_GROUPS) return SendClientMessage(playerid, COLOR_GREY, "Invalid group ID.");

        for(new x = 0; x < MAX_VEHICLES; x++) {
            if(vehicleVariables[x][vVehicleScriptID] == GetPlayerVehicleID(playerid)) {
                vehicleVariables[x][vVehicleGroup] = groupParam;

                saveVehicle(x);

                switch(groupParam) {

					case 0: format(string, sizeof(string), "You have removed group restrictions from this vehicle (%d).", x);
					default: format(string, sizeof(string), "You have changed this vehicle's group to %s (vehicle %d).", groupVariables[groupParam][gGroupName], x);
				}
				SendClientMessage(playerid, COLOR_WHITE, string);
				return 1;
			}
        }
	}
	return 1;
}

CMD:vcolour(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 4) {
	    if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You must be inside the vehicle that you wish to alter the colour of.");

		new
			string[80],
		    colour1,
			colour2;

		if(sscanf(params,"dd", colour1, colour2)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/vcolour [colour 1] [colour 2]");

        for(new x = 0; x < MAX_VEHICLES; x++) {
            if(vehicleVariables[x][vVehicleScriptID] == GetPlayerVehicleID(playerid)) {
                vehicleVariables[x][vVehicleColour][0] = colour1;
				vehicleVariables[x][vVehicleColour][1] = colour2;

				ChangeVehicleColor(vehicleVariables[x][vVehicleScriptID], vehicleVariables[x][vVehicleColour][0], vehicleVariables[x][vVehicleColour][1]);

                saveVehicle(x);

				format(string, sizeof(string), "You have changed this vehicle's colour combination to %d, %d (vehicle %d).", colour1, colour2, x);
				SendClientMessage(playerid, COLOR_WHITE, string);
				return 1;
			}
        }
	}
	return 1;
}

CMD:vrespawn(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 1 && IsPlayerInAnyVehicle(playerid)) SetVehicleToRespawnEx(GetPlayerVehicleID(playerid));
	return 1;
}

CMD:vmassrespawn(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 4) {
		SetAllVehiclesToRespawn();
		SendClientMessage(playerid, COLOR_WHITE, "All vehicles have been respawned.");
	}
	return 1;
}

CMD:vmodel(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 4) {
		if(isnull(params)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/vmodel [modelid]");
		else if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You must be inside the vehicle that you wish to change the model of.");
		else if(strval(params) < 400 || strval(params) > 611) return SendClientMessage(playerid, COLOR_WHITE, "Valid car IDs start at 400, and end at 611.");

		new
			string[64];

        for(new x = 0; x < MAX_VEHICLES; x++) {
            if(vehicleVariables[x][vVehicleScriptID] == GetPlayerVehicleID(playerid)) {

				vehicleVariables[x][vVehicleModelID] = strval(params);

				DestroyVehicle(vehicleVariables[x][vVehicleScriptID]);
				vehicleVariables[x][vVehicleScriptID] = CreateVehicle(vehicleVariables[x][vVehicleModelID], vehicleVariables[x][vVehiclePosition][0], vehicleVariables[x][vVehiclePosition][1], vehicleVariables[x][vVehiclePosition][2], vehicleVariables[x][vVehicleRotation], vehicleVariables[x][vVehicleColour][0], vehicleVariables[x][vVehicleColour][1], 60000);
				PutPlayerInVehicle(playerid, vehicleVariables[x][vVehicleScriptID], 0);

                saveVehicle(x);

				format(string, sizeof(string), "You have successfully changed vehicle %d to a %s.", x, VehicleNames[vehicleVariables[x][vVehicleModelID] - 400]);
				SendClientMessage(playerid, COLOR_WHITE, string);
				return 1;
			}
        }
	}
	return 1;
}

CMD:vmove(playerid, params[]) {
	if(playerVariables[playerid][pAdminLevel] >= 4) {
	    if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_GREY, "You must be inside the vehicle that you wish to move.");

		new
			string[42];

        for(new x = 0; x < MAX_VEHICLES; x++) {
            if(vehicleVariables[x][vVehicleScriptID] == GetPlayerVehicleID(playerid)) {

                GetVehiclePos(x, vehicleVariables[x][vVehiclePosition][0], vehicleVariables[x][vVehiclePosition][1], vehicleVariables[x][vVehiclePosition][2]);
				GetVehicleZAngle(x, vehicleVariables[x][vVehicleRotation]);

				DestroyVehicle(vehicleVariables[x][vVehicleScriptID]);
				vehicleVariables[x][vVehicleScriptID] = CreateVehicle(vehicleVariables[x][vVehicleModelID], vehicleVariables[x][vVehiclePosition][0], vehicleVariables[x][vVehiclePosition][1], vehicleVariables[x][vVehiclePosition][2], vehicleVariables[x][vVehicleRotation], vehicleVariables[x][vVehicleColour][0], vehicleVariables[x][vVehicleColour][1], 60000);
				PutPlayerInVehicle(playerid, vehicleVariables[x][vVehicleScriptID], 0);

                saveVehicle(x);

				format(string, sizeof(string), "You have successfully moved vehicle %d.", x);
				SendClientMessage(playerid, COLOR_WHITE, string);
				return 1;
			}
        }
	}
	return 1;
}