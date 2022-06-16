#include <YSI_Coding/y_hooks>

#define MAX_HOUSES 100

enum houseE {
	Float: hHouseExteriorPos[3],
	Float: hHouseInteriorPos[3],
	hHouseInteriorID,
	hHouseLocked,
	hHouseExteriorID,
	hHousePrice,
	hPickupID,
	Text3D:hLabelID,
	hHouseOwner[MAX_PLAYER_NAME],
	hMoney,
	hWeapons[5],
	hWardrobe[5],
	hMaterials,
}

new
    houseVariables[MAX_HOUSES][houseE],
    Iterator:house<MAX_HOUSES>;

hook OnGameModeInit() {
    #if defined DEBUG
        print("hook OnGameModeInit()_house has been called");
    #endif

    mysql_tquery(gSQL, "SELECT * FROM houses", "loadHouses", "");

    return true;
}
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    #if defined DEBUG
        printf("hook OnPlayerKeyStateChange(%d, %d, %d)_business has been called", playerid, newkeys, oldkeys);
    #endif  

    if(IsKeyJustDown(KEY_CROUCH, newkeys, oldkeys)) {
        foreach(new x : bizz) { 
            if(IsPlayerInRangeOfPoint(playerid, 2.0, houseVariables[x][hHouseExteriorPos][0], houseVariables[x][hHouseExteriorPos][1], houseVariables[x][hHouseExteriorPos][2])) {
                if(houseVariables[x][hHouseLocked] == 1) {
                    SendClientMessage(playerid, COLOR_GREY, "This house is locked.");
                    if(playerVariables[playerid][pAdminLevel] >= 1 && playerVariables[playerid][pAdminDuty] >= 1) {
                        SetPVarInt(playerid, "hE", x); // I'd create a variable for this, but seeing as we'll only ever use this for one thing, this will be better for optimization.
                        // ShowPlayerDialog(playerid, DIALOG_HOUSE_ENTER, DIALOG_STYLE_MSGBOX, "SERVER: Housing", "This house is locked.\r\nAs an administrator, you can breach this lock and enter. Would you like to do so?", "Yes", "No");
                    }
                    else if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1 && playerVariables[playerid][pGroup] != 0) {
                        SetPVarInt(playerid, "hE", x);
                        // ShowPlayerDialog(playerid, DIALOG_HOUSE_ENTER, DIALOG_STYLE_MSGBOX, "SERVER: Housing", "This house is locked.\r\nAs a law enforcement officer, you can breach this lock and enter. Would you like to do so?", "Yes", "No");
                    }
                }
                else {
                    SetPlayerPos(playerid, houseVariables[x][hHouseInteriorPos][0], houseVariables[x][hHouseInteriorPos][1], houseVariables[x][hHouseInteriorPos][2]);
                    SetPlayerInterior(playerid, houseVariables[x][hHouseInteriorID]);
                    SetPlayerVirtualWorld(playerid, HOUSE_VIRTUAL_WORLD+x);
                }
                return 1;
            }
            if(IsPlayerInRangeOfPoint(playerid, 2.0, houseVariables[x][hHouseInteriorPos][0], houseVariables[x][hHouseInteriorPos][1], houseVariables[x][hHouseInteriorPos][2]) && GetPlayerVirtualWorld(playerid) == HOUSE_VIRTUAL_WORLD+x) {
                SetPlayerPos(playerid, houseVariables[x][hHouseExteriorPos][0], houseVariables[x][hHouseExteriorPos][1], houseVariables[x][hHouseExteriorPos][2]);
                SetPlayerInterior(playerid, houseVariables[x][hHouseExteriorID]);
                SetPlayerVirtualWorld(playerid, 0);
                return 1;
            }
        }
    }
    return true;
}

function loadHouses() { 
    #if defined DEBUG
        print("function loadHouses()_houses has been called");
    #endif 

    new x = 0;
    for ( new i, j = cache_num_rows(); i != j; ++i ) {
	    cache_get_value_int(i, "houseID", x); 
	    cache_get_value_int(i, "houseInteriorID", houseVariables[x][hHouseInteriorID]); 
	    cache_get_value_int(i, "housePrice", houseVariables[x][hHousePrice]); 
	    cache_get_value_int(i, "houseLocked", houseVariables[x][hHouseLocked]); 
	    cache_get_value_int(i, "houseMoney", houseVariables[x][hMoney]); 
	    cache_get_value_int(i, "houseMaterials", houseVariables[x][hMaterials]); 

	    cache_get_value_int(i, "houseWeapon1", houseVariables[x][hWeapons][0]);  
	    cache_get_value_int(i, "houseWeapon2", houseVariables[x][hWeapons][1]);  
	    cache_get_value_int(i, "houseWeapon3", houseVariables[x][hWeapons][2]);  
	    cache_get_value_int(i, "houseWeapon4", houseVariables[x][hWeapons][3]);  
	    cache_get_value_int(i, "houseWeapon5", houseVariables[x][hWeapons][4]);  

	    cache_get_value_int(i, "houseWardrobe1", houseVariables[x][hWardrobe][0]);  
	    cache_get_value_int(i, "houseWardrobe2", houseVariables[x][hWardrobe][1]);  
	    cache_get_value_int(i, "houseWardrobe3", houseVariables[x][hWardrobe][2]);  
	    cache_get_value_int(i, "houseWardrobe4", houseVariables[x][hWardrobe][3]);  
	    cache_get_value_int(i, "houseWardrobe5", houseVariables[x][hWardrobe][4]);  

	    cache_get_value_name(i, "houseOwner", houseVariables[x][hHouseOwner], MAX_PLAYER_NAME);
    
        cache_get_value_float(i, "houseExteriorPosX", houseVariables[x][hHouseExteriorPos][0]);
        cache_get_value_float(i, "houseExteriorPosY", houseVariables[x][hHouseExteriorPos][1]);
        cache_get_value_float(i, "houseExteriorPosZ", houseVariables[x][hHouseExteriorPos][2]);
        cache_get_value_float(i, "houseInteriorPosX", houseVariables[x][hHouseInteriorPos][0]);
        cache_get_value_float(i, "houseInteriorPosY", houseVariables[x][hHouseInteriorPos][1]);
        cache_get_value_float(i, "houseInteriorPosZ", houseVariables[x][hHouseInteriorPos][2]);

        if(!strcmp(houseVariables[x][hHouseOwner], "Nobody", true) && strlen(houseVariables[x][hHouseOwner]) >= 1) {
            new
                labelString[96];

            if(houseVariables[x][hHouseLocked] == 1) {
                format(labelString, sizeof(labelString), "House %d (un-owned - /buyhouse)\nPrice: $%d\n\n(locked)", x, houseVariables[x][hHousePrice]);
            }
            else {
                format(labelString, sizeof(labelString), "House %d (un-owned - /buyhouse)\nPrice: $%d\n\nPress ~k~~PED_DUCK~ to enter.", x, houseVariables[x][hHousePrice]);
            }

            houseVariables[x][hLabelID] = CreateDynamic3DTextLabel(labelString, COLOR_YELLOW, houseVariables[x][hHouseExteriorPos][0], houseVariables[x][hHouseExteriorPos][1], houseVariables[x][hHouseExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
            houseVariables[x][hPickupID] = CreateDynamicPickup(1273, 23, houseVariables[x][hHouseExteriorPos][0], houseVariables[x][hHouseExteriorPos][1], houseVariables[x][hHouseExteriorPos][2], 0, houseVariables[x][hHouseExteriorID], -1, 250);
        }
        else {
            new
                labelString[96];

            if(houseVariables[x][hHouseLocked] == 1) {
                format(labelString, sizeof(labelString), "House %d (owned)\nOwner: %s\n\n(locked)", x, houseVariables[x][hHouseOwner]);
            }
            else {
                format(labelString, sizeof(labelString), "House %d (owned)\nOwner: %s\n\nPress ~k~~PED_DUCK~ to enter.", x, houseVariables[x][hHouseOwner]);
            }

            houseVariables[x][hLabelID] = CreateDynamic3DTextLabel(labelString, COLOR_YELLOW, houseVariables[x][hHouseExteriorPos][0], houseVariables[x][hHouseExteriorPos][1], houseVariables[x][hHouseExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
            houseVariables[x][hPickupID] = CreateDynamicPickup(1272, 23, houseVariables[x][hHouseExteriorPos][0], houseVariables[x][hHouseExteriorPos][1], houseVariables[x][hHouseExteriorPos][2], 0, houseVariables[x][hHouseExteriorID], -1, 50);
        }
        
        Iter_Add(house, x);
    } 
    printf("[SQL] Houses loaded: %d", Iter_Count(house)); 
} 
stock saveHouse(const id) {
	if(strlen(houseVariables[id][hHouseOwner]) >= 1) {
		format(szLargeString, sizeof(szLargeString), "UPDATE houses SET houseExteriorPosX = '%f', houseExteriorPosY = '%f', houseExteriorPosZ = '%f', houseInteriorPosX = '%f', houseInteriorPosY = '%f', houseInteriorPosZ = '%f'", houseVariables[id][hHouseExteriorPos][0], houseVariables[id][hHouseExteriorPos][1], houseVariables[id][hHouseExteriorPos][2], houseVariables[id][hHouseInteriorPos][0], houseVariables[id][hHouseInteriorPos][1], houseVariables[id][hHouseInteriorPos][2]);
		format(szLargeString, sizeof(szLargeString), "%s, housePrice = '%d', houseOwner = '%s', houseExteriorID = '%d', houseInteriorID = '%d', houseLocked = '%d', houseMoney = '%d', houseMaterials = '%d', houseWeapon1 = '%d', houseWeapon2 = '%d', houseWeapon3 = '%d', houseWeapon4 = '%d', houseWeapon5 = '%d'", szLargeString, houseVariables[id][hHousePrice], houseVariables[id][hHouseOwner], houseVariables[id][hHouseExteriorID],
		houseVariables[id][hHouseInteriorID], houseVariables[id][hHouseLocked],	houseVariables[id][hMoney], houseVariables[id][hMaterials], houseVariables[id][hWeapons][0], houseVariables[id][hWeapons][1], houseVariables[id][hWeapons][2], houseVariables[id][hWeapons][3], houseVariables[id][hWeapons][4]);
		format(szLargeString, sizeof(szLargeString), "%s, houseWardrobe1 = '%d', houseWardrobe2 = '%d', houseWardrobe3 = '%d', houseWardrobe4 = '%d', houseWardrobe5 = '%d' WHERE houseID = '%d'", szLargeString, houseVariables[id][hWardrobe][0], houseVariables[id][hWardrobe][1], houseVariables[id][hWardrobe][2], houseVariables[id][hWardrobe][3], houseVariables[id][hWardrobe][4], id);
		mysql_tquery(gSQL, szLargeString, "", "");
	}
	else {
	    return false;
	}

	return 1;
}   

getPlayerHouseID(playerid) {
	new
	    x;

    while(x < MAX_HOUSES) {
		if(strlen(houseVariables[x][hHouseOwner]) >= 1) {
	        if(!strcmp(houseVariables[x][hHouseOwner], playerVariables[playerid][pNormalName], true)) {
				return x;
			}
		}
		x++;
	}

    return 0;
}
/*
case DIA1LOG_HOUSE_ENTER: {
    if(response == 1) {
        new
            x = GetPVarInt(playerid, "hE"); // So we don't have to access it each and every time.

        DeletePVar(playerid, "hE");

        GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);
        format(szMessage,sizeof(szMessage),"* %s breaks down the door and enters the house.", szPlayerName);
        nearByMessage(playerid, COLOR_PURPLE, szMessage);

        if(playerVariables[playerid][pAdminDuty] < 1 && houseVariables[x][hHouseLocked] == 1) { // Might seem redundant, but if many people break in at once this'll stop unnecessary code from being executed.
            houseVariables[x][hHouseLocked] = 0;
            if(!strcmp(houseVariables[x][hHouseOwner], "Nobody", true) && strlen(houseVariables[x][hHouseOwner]) >= 1)
                format(szMessage, sizeof(szMessage), "House %d (un-owned - /buyhouse)\nPrice: $%d\n\n(locked)", x, houseVariables[x][hHousePrice]);
            else
                format(szMessage, sizeof(szMessage), "House %d (owned)\nOwner: %s\n\nPress ~k~~PED_DUCK~ to enter.", x, houseVariables[x][hHouseOwner]);
                
            UpdateDynamic3DTextLabelText(houseVariables[x][hLabelID], COLOR_YELLOW, szMessage);
        }

        SetPlayerPos(playerid, houseVariables[x][hHouseInteriorPos][0], houseVariables[x][hHouseInteriorPos][1], houseVariables[x][hHouseInteriorPos][2]);
        SetPlayerInterior(playerid, houseVariables[x][hHouseInteriorID]);
        SetPlayerVirtualWorld(playerid, HOUSE_VIRTUAL_WORLD+x);
    }
} 
*/