#include <YSI_Coding/y_hooks>

#define MAX_BUSINESSES          (50)
#define BUSINESS_VIRTUAL_WORLD  (30000)

enum businessE {
	bType,
	bOwner[MAX_PLAYER_NAME],
	bName[32],
	Float: bExteriorPos[3],
	Float: bInteriorPos[3],
	bInterior,
	bLocked,
	Float: bMiscPos[3],
	bVault,
	Text3D: bLabelID,
	bPickupID,
	bPrice,
} 

new 
    businessVariables[MAX_BUSINESSES][businessE], 
    Iterator:bizz<MAX_BUSINESSES>;

hook OnGameModeInit() {
    #if defined DEBUG
        print("hook OnGameModeInit()_business has been called");
    #endif

    mysql_tquery(gSQL, "SELECT * FROM businesses", "loadBusinesses", "");

    return true;
} 

// find a more optimization code for enter in bizz
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    #if defined DEBUG
        printf("hook OnPlayerKeyStateChange(%d, %d, %d)_business has been called", playerid, newkeys, oldkeys);
    #endif  

    if(IsKeyJustDown(KEY_CROUCH, newkeys, oldkeys)) {
        foreach(new x : bizz) {
			if(IsPlayerInRangeOfPoint(playerid, 2.0, businessVariables[x][bExteriorPos][0], businessVariables[x][bExteriorPos][1], businessVariables[x][bExteriorPos][2])) {
				if(businessVariables[x][bLocked] == 1) {
					SendClientMessage(playerid, COLOR_GREY, "This business is locked.");

					if((playerVariables[playerid][pAdminLevel] >= 1 && playerVariables[playerid][pAdminDuty] >= 1) || (groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1 && playerVariables[playerid][pGroup] != 0)) {
						SetPVarInt(playerid, "bE", x); // I'd create a variable for this, but seeing as we'll only ever use this for one thing, this will be better for optimization.
						Dialog_Show(playerid, DIALOG_SHOW_ENTER, DIALOG_STYLE_MSGBOX, "SERVER: Businesses", "{FFFFFF}This business is locked.\r\nYou can breach this lock and enter. Would you like to do so?", "Yes", "No");
					}  
					return true;
				} 

				businessTypeMessages(x, playerid); 
				SetPlayerPos(playerid, businessVariables[x][bInteriorPos][0], businessVariables[x][bInteriorPos][1], businessVariables[x][bInteriorPos][2]);
				SetPlayerInterior(playerid, businessVariables[x][bInterior]);
				SetPlayerVirtualWorld(playerid, BUSINESS_VIRTUAL_WORLD+x);
				
				return true;
			}
			if(IsPlayerInRangeOfPoint(playerid, 2.0, businessVariables[x][bInteriorPos][0], businessVariables[x][bInteriorPos][1], businessVariables[x][bInteriorPos][2]) && GetPlayerVirtualWorld(playerid) == BUSINESS_VIRTUAL_WORLD+x) {
				SetPlayerPos(playerid, businessVariables[x][bExteriorPos][0], businessVariables[x][bExteriorPos][1], businessVariables[x][bExteriorPos][2]);
				SetPlayerInterior(playerid, 0);
				SetPlayerVirtualWorld(playerid, 0);

				return true;
			} 
		}
    }

    return true;
}

function loadBusinesses() { 
    #if defined DEBUG
        print("function loadBusinesses()_business has been called");
    #endif 

    new x = 0;
    for ( new i, j = cache_num_rows(); i != j; ++i ) {
	    cache_get_value_int(i, "businessID", x);

	    cache_get_value_name(i, "businessName", businessVariables[x][bName], 32); 
	    cache_get_value_name(i, "businessOwner", businessVariables[x][bOwner], MAX_PLAYER_NAME);  
        
	    cache_get_value_int(i, "businessType", businessVariables[x][bType]);	   
	    cache_get_value_int(i, "businessInterior", businessVariables[x][bInterior]);	   
	    cache_get_value_int(i, "businessLock", businessVariables[x][bLocked]);	   
	    cache_get_value_int(i, "businessPrice", businessVariables[x][bPrice]);	   
	    cache_get_value_int(i, "businessVault", businessVariables[x][bVault]);	   

	    cache_get_value_float(i, "businessExteriorX", businessVariables[x][bExteriorPos][0]);	 
	    cache_get_value_float(i, "businessExteriorY", businessVariables[x][bExteriorPos][1]);	 
	    cache_get_value_float(i, "businessExteriorZ", businessVariables[x][bExteriorPos][2]);	 
	    cache_get_value_float(i, "businessInteriorX", businessVariables[x][bInteriorPos][0]);	 
	    cache_get_value_float(i, "businessInteriorY", businessVariables[x][bInteriorPos][2]);	 
	    cache_get_value_float(i, "businessInteriorZ", businessVariables[x][bInteriorPos][2]);	 
	    cache_get_value_float(i, "businessMiscX", businessVariables[x][bMiscPos][0]);	 
	    cache_get_value_float(i, "businessMiscY", businessVariables[x][bMiscPos][1]);	 
	    cache_get_value_float(i, "businessMiscZ", businessVariables[x][bMiscPos][2]);	 

        gString[0] = (EOS);
		if(!strcmp(businessVariables[x][bOwner], "Nobody", true)) format(gString, sizeof(gString), "%s\n(Business %d - un-owned)\nPrice: $%d (/buybusiness)\n\n%s", businessVariables[x][bName], x, businessVariables[x][bPrice], (businessVariables[x][bLocked] == 1 ? "(locked)" : "Press ~k~~PED_DUCK~ to enter"));
		else format(gString, sizeof(gString), "%s\n(Business %d - owned by %s)\n\n%s", businessVariables[x][bName], x, businessVariables[x][bOwner], (businessVariables[x][bLocked] == 1 ? "(locked)" : "Press ~k~~PED_DUCK~ to enter"));

        businessVariables[x][bLabelID] = CreateDynamic3DTextLabel(gString, COLOR_YELLOW, businessVariables[x][bExteriorPos][0], businessVariables[x][bExteriorPos][1], businessVariables[x][bExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
        businessVariables[x][bPickupID] = CreateDynamicPickup(1239, 23, businessVariables[x][bExteriorPos][0], businessVariables[x][bExteriorPos][1], businessVariables[x][bExteriorPos][2], 0, 0, -1, 250);

        Iter_Add(bizz, x); 
    } 

    printf("[SQL] Businesses loaded: %d", Iter_Count(bizz)); 
}

function saveBusiness(id) {
    #if defined DEBUG
        printf("function saveBusiness(%d)_business has been called", id);
    #endif 
	
	if(strlen(businessVariables[id][bOwner]) < 1) return false;

	new count = GetTickCount();

	gLargeQuery[0] = (EOS);
	mysql_format(gSQL, gLargeQuery, sizeof(gLargeQuery), "UPDATE businesses SET businessExteriorX = '%f', businessExteriorY = '%f', businessExteriorZ = '%f', businessInteriorX = '%f', businessInteriorY = '%f', businessInteriorZ = '%f', businessInterior = '%d', businessType = '%d', businessName = '%s', businessOwner = '%s', businessPrice = '%d', businessVault = '%d', businessLock = '%d', businessMiscX = '%f', businessMiscY = '%f', businessMiscZ = '%f' WHERE businessID = '%d'", businessVariables[id][bExteriorPos][0], businessVariables[id][bExteriorPos][1],	businessVariables[id][bExteriorPos][2],	businessVariables[id][bInteriorPos][0], businessVariables[id][bInteriorPos][1],	businessVariables[id][bInteriorPos][2],	businessVariables[id][bInterior], businessVariables[id][bType], businessVariables[id][bName], businessVariables[id][bOwner], businessVariables[id][bPrice], businessVariables[id][bVault], businessVariables[id][bLocked], businessVariables[id][bMiscPos][0], businessVariables[id][bMiscPos][1], businessVariables[id][bMiscPos][2], id);
	mysql_tquery(gSQL, gLargeQuery, "finishQuery", "ddd", id, FINISH_SAVE_BUSINESS, count);

	return true;
}

function updateBusiness(id) {
    #if defined DEBUG
        printf("function updateBusiness(%d)_business has been called", id);
    #endif 

    gString[0] = (EOS); 
    if(!strcmp(businessVariables[id][bOwner], "Nobody", true)) format(gString, sizeof(gString), "%s\n(Business %d - un-owned)\nPrice: $%d (/buybusiness)\n\n%s", businessVariables[id][bName], id, businessVariables[id][bPrice], (businessVariables[id][bLocked] == 1 ? "(locked)" : "Press ~k~~PED_DUCK~ to enter"));
    else format(gString, sizeof(gString), "%s\n(Business %d - owned by %s)\n\n%s", businessVariables[id][bName], id, businessVariables[id][bOwner], (businessVariables[id][bLocked] == 1 ? "(locked)" : "Press ~k~~PED_DUCK~ to enter"));
    
    UpdateDynamic3DTextLabelText(businessVariables[id][bLabelID], COLOR_YELLOW, gString);
    saveBusiness(id);
}

stock getPlayerBusinessID(playerid) { 
    #if defined DEBUG
        printf("function getPlayerBusinessID(%d)_business has been called", playerid);
    #endif 

    foreach(new x : bizz) {
		if(strlen(businessVariables[x][bOwner]) < 1)
            continue;

        if(strcmp(businessVariables[x][bOwner], playerVariables[playerid][pNormalName], true)) 
           continue; 
        
        return x; 
	}

    return false;
}

stock businessTypeMessages(businessid, playerid) {
	switch(businessVariables[businessid][bType]) {
		case 1: {
			SendClientMessage(playerid, COLOR_WHITE, "Welcome! The commands of this business are as follows: /buy");
			if(playerVariables[playerid][pFish] != -1) {
				gString[0] = (EOS);
				format(gString, 125, "You are currently carrying $%d worth of fish.\n\nWould you like to sell your fish to this store for $%d?", getFishMoney(playerVariables[playerid][pFish]), getFishMoney(playerVariables[playerid][pFish]));
				Dialog_Show(playerid, DIALOG_SELL_FISH, DIALOG_STYLE_MSGBOX, "SERVER: Fishing", gString, "Yes", "No");
			}
		}
		case 2: SendClientMessage(playerid, COLOR_WHITE, "Welcome! The commands of this business are as follows: /buyclothes");
		case 3, 4, 7: SendClientMessage(playerid, COLOR_WHITE, "Welcome! The commands of this business are as follows: /buy");
		case 5: SendClientMessage(playerid, COLOR_WHITE, "Welcome! The commands of this business are as follows: /buyvehicle");
		case 6: SendClientMessage(playerid, COLOR_WHITE, "Welcome! The commands of this business are as follows: /buyfightstyle");
	}
	
	return 1;
}
 
Dialog:DIALOG_SELL_FISH(playerid, response, listitem, inputtext[]) {
    if(!response) return true;

	new fish = playerVariables[playerid][pFish];

	va_SendClientMessage(playerid, COLOR_WHITE, "Congratulations, you have sold your collected fish for $%d.", getFishMoney(fish));
	playerVariables[playerid][pMoney] += getFishMoney(fish);
    playerVariables[playerid][pFish] = -1; 

    return true;
}

Dialog:DIALOG_SHOW_ENTER(playerid, response, itemlist, inputtext[]) {
    if(response == 1) {
        new x = GetPVarInt(playerid, "bE"); 
        DeletePVar(playerid, "bE"); 

        nearByMessage(playerid, COLOR_PURPLE, 12.0,"* %s breaks down the door and enters the business.", getName(playerid));

        if(playerVariables[playerid][pAdminDuty] < 1 && businessVariables[x][bLocked] == 1) {
            businessVariables[x][bLocked] = 0;
            
            gString[0] = (EOS);
            if(!strcmp(businessVariables[x][bOwner], "Nobody"))
                format(gString, sizeof(gString), "%s\n(Business %d - un-owned)\nPrice: $%d (/buybusiness)\n\nPress ~k~~PED_DUCK~ to enter", businessVariables[x][bName], x, businessVariables[x][bPrice]);
            else
                format(gString, sizeof(gString), "%s\n(Business %d - owned by %s)\n\nPress ~k~~PED_DUCK~ to enter", businessVariables[x][bName], x, businessVariables[x][bOwner]);
            
            UpdateDynamic3DTextLabelText(businessVariables[x][bLabelID], COLOR_YELLOW, gString);
        }

        SetPlayerPos(playerid, businessVariables[x][bInteriorPos][0], businessVariables[x][bInteriorPos][1], businessVariables[x][bInteriorPos][2]);
        SetPlayerInterior(playerid, businessVariables[x][bInterior]);
        SetPlayerVirtualWorld(playerid, BUSINESS_VIRTUAL_WORLD+x);
    }

    return true; 
}