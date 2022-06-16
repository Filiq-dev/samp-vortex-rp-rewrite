#include <YSI_Coding/y_hooks>

#define MAX_BUSINESSES          (50)
#define MAX_BUSINESS_ITEMS      (MAX_BUSINESSES*6)
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

enum businessItemsE {
	bItemBusiness,
	bItemType,
	bItemName[32],
	bItemPrice,
}

new 
    businessVariables[MAX_BUSINESSES][businessE],
    businessItems[MAX_BUSINESS_ITEMS][businessItemsE],
    Iterator:bizz<MAX_BUSINESSES>,
    Iterator:bizzItems<MAX_BUSINESS_ITEMS>;

hook OnGameModeInit() {
    #if defined DEBUG
        print("hook OnGameModeInit()_businesses has been called");
    #endif

    mysql_tquery(gSQL, "SELECT * FROM businessitems", "loadItems", "");
    mysql_tquery(gSQL, "SELECT * FROM businesses", "loadBusinesses", "");

    return true;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    #if defined DEBUG
        printf("hook OnPlayerKeyStateChange(%d, %d, %d)_business has been called", playerid, newkeys, oldkeys);
    #endif  

    if(IsKeyJustDown(KEY_CROUCH, newkeys, oldkeys)) {
        foreach(new x : bizz) {
			if(IsPlayerInRangeOfPoint(playerid, 2.0, businessVariables[x][bExteriorPos][0], businessVariables[x][bExteriorPos][1], businessVariables[x][bExteriorPos][2])) {
				if(businessVariables[x][bLocked] == 1) {
					SendClientMessage(playerid, COLOR_GREY, "This business is locked.");

					if(playerVariables[playerid][pAdminLevel] >= 1 && playerVariables[playerid][pAdminDuty] >= 1) {
					    SetPVarInt(playerid, "bE", x); // I'd create a variable for this, but seeing as we'll only ever use this for one thing, this will be better for optimization.
					    // Dialog_Open(playerid, DIALOG_BUSINESS_ENTER, DIALOG_STYLE_MSGBOX, "SERVER: Businesses", "{FFFFFF}This business is locked.\r\nAs an "EMBED_GREY"administrator{FFFFFF}, you can breach this lock and enter. Would you like to do so?", "Yes", "No");
					}
					else if(groupVariables[playerVariables[playerid][pGroup]][gGroupType] == 1 && playerVariables[playerid][pGroup] != 0) {
					    SetPVarInt(playerid, "bE", x);
					    // Dialog_Open(playerid, DIALOG_BUSINESS_ENTER, DIALOG_STYLE_MSGBOX, "SERVER: Businesses", "{FFFFFF}This business is locked.\r\nAs a "EMBED_GREY"law enforcement officer{FFFFFF}, you can breach this lock and enter. Would you like to do so?", "Yes", "No");
					}
				}
				else {
					businessTypeMessages(x, playerid);

					SetPlayerPos(playerid, businessVariables[x][bInteriorPos][0], businessVariables[x][bInteriorPos][1], businessVariables[x][bInteriorPos][2]);
					SetPlayerInterior(playerid, businessVariables[x][bInterior]);
					SetPlayerVirtualWorld(playerid, BUSINESS_VIRTUAL_WORLD+x);
				}
				return 1;
			}
			if(IsPlayerInRangeOfPoint(playerid, 2.0, businessVariables[x][bInteriorPos][0], businessVariables[x][bInteriorPos][1], businessVariables[x][bInteriorPos][2]) && GetPlayerVirtualWorld(playerid) == BUSINESS_VIRTUAL_WORLD+x) {
				SetPlayerPos(playerid, businessVariables[x][bExteriorPos][0], businessVariables[x][bExteriorPos][1], businessVariables[x][bExteriorPos][2]);
				SetPlayerInterior(playerid, 0);
				SetPlayerVirtualWorld(playerid, 0);
				return 1;
			}
		}
    }

    return true;
}

function loadBusinesses() { 
    #if defined DEBUG
        print("function loadBusinesses()_groups has been called");
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
        switch(businessVariables[x][bLocked]) {
            case 1: {
                if(!strcmp(businessVariables[x][bOwner], "Nobody", true)) {
                    format(gString, sizeof(gString), "%s\n(Business %d - un-owned)\nPrice: $%d (/buybusiness)\n\n(locked)", businessVariables[x][bName], x, businessVariables[x][bPrice]);
                }
                else {
                    format(gString, sizeof(gString), "%s\n(Business %d - owned by %s)\n\n(locked)", businessVariables[x][bName], x, businessVariables[x][bOwner]);
                }
            }
            case 0: {
                if(!strcmp(businessVariables[x][bOwner], "Nobody", true)) {
                    format(gString, sizeof(gString), "%s\n(Business %d - un-owned)\nPrice: $%d (/buybusiness)\n\nPress ~k~~PED_DUCK~ to enter", businessVariables[x][bName], x, businessVariables[x][bPrice]);
                }
                else {
                    format(gString, sizeof(gString), "%s\n(Business %d - owned by %s)\n\nPress ~k~~PED_DUCK~ to enter", businessVariables[x][bName], x, businessVariables[x][bOwner]);
                }
            }
        } 

        // format(gString, sizeof(gString), "%s\n(Business %d - %s%s)\nPrice: $%d (/buybusiness)\n\n(locked)", businessVariables[x][bName], x, (strcmp(businessVariables[x][bOwner], "Nobody", true) != 1 ? "owned by" : "un-owned"), (strcmp(businessVariables[x][bOwner], "Nobody", true) != 1 ? businessVariables[x][bOwner] : ""))
        
        businessVariables[x][bLabelID] = CreateDynamic3DTextLabel(gString, COLOR_YELLOW, businessVariables[x][bExteriorPos][0], businessVariables[x][bExteriorPos][1], businessVariables[x][bExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
        businessVariables[x][bPickupID] = CreateDynamicPickup(1239, 23, businessVariables[x][bExteriorPos][0], businessVariables[x][bExteriorPos][1], businessVariables[x][bExteriorPos][2], 0, 0, -1, 250);

        Iter_Add(bizz, x); 
    } 

    printf("[SQL] Businesses loaded: %d", Iter_Count(bizz)); 
} 

function loadItems() { 
    #if defined DEBUG
        print("function loadItems()_groups has been called");
    #endif 

    new x = 0;
    for ( new i, j = cache_num_rows(); i != j; ++i ) {
	    cache_get_value_int(i, "itemBusinessId", businessItems[x][bItemBusiness]);
	    cache_get_value_int(i, "itemTypeId", businessItems[x][bItemType]);
	    cache_get_value_int(i, "itemPrice", businessItems[x][bItemPrice]);

	    cache_get_value_name(i, "itemName", businessItems[x][bItemName], 32);
    
        
        Iter_Add(bizzItems, x);
    } 
    printf("[SQL] Businesses items loaded: %d", Iter_Count(bizzItems)); 
}

function createRelevantItems(businessid) {
    #if defined DEBUG
        print("function createRelevantItems()_business has been called");
    #endif 

    gLargeQuery[0] = (EOS);
	switch(businessVariables[businessid][bType]) {
		case 1: {
			mysql_format(gSQL, gLargeQuery, sizeof(gLargeQuery), "INSERT INTO `businessitems` (`itemBusinessId`, `itemTypeId`, `itemPrice`, `itemName`) VALUES (%d, 1, 5, 'Rope'), (%d, 2, 15, 'Walkie Talkie'), (%d, 3, 10, 'Phone Book'),", businessid, businessid, businessid);
			mysql_format(gSQL, gLargeQuery, sizeof(gLargeQuery), "%s(%d, 4, 10, 'Phone Credit'), (%d, 5, 10, 'Mobile Phone');", gLargeQuery, businessid, businessid);
		}
		case 2: mysql_format(gSQL, gLargeQuery, sizeof(gLargeQuery), "INSERT INTO `businessitems` (`itemBusinessId`, `itemTypeId`, `itemPrice`, `itemName`) VALUES (%d, 18, 50, 'All Skins');", businessid);
		case 3: mysql_format(gSQL, gLargeQuery, sizeof(gLargeQuery), "INSERT INTO `businessitems` (`itemBusinessId`, `itemTypeId`, `itemPrice`, `itemName`) VALUES (%d, 14, 4, 'Cigar'), (%d, 15, 1, 'Sprunk'), (%d, 16, 10, 'Wine'), (%d, 17, 3, 'Beer');", businessid, businessid, businessid, businessid);
		case 4: {
 			mysql_format(gSQL, gLargeQuery, sizeof(gLargeQuery), "INSERT INTO `businessitems` (`itemBusinessId`, `itemTypeId`, `itemPrice`, `itemName`) VALUES (%d, 9, 10, 'Purple Dildo'), (%d, 10, 15, 'Small White Vibrator'), (%d, 11, 20, 'Large White Vibrator'),", businessid, businessid, businessid);
			mysql_format(gSQL, gLargeQuery, sizeof(gLargeQuery), "%s(%d, 12, 15, 'Silver Vibrator'), (%d, 13, 10, 'Flowers');", gLargeQuery, businessid, businessid);
		}
		case 7: mysql_format(gSQL, gLargeQuery, sizeof(gLargeQuery), "INSERT INTO `businessitems` (`itemBusinessId`, `itemTypeId`, `itemPrice`, `itemName`) VALUES (%d, 6, 5, 'Box Meal 1'), (%d, 7, 10, 'Box Meal 2'), (%d, 8, 20, 'Box Meal 3');", businessid, businessid, businessid);
	
	}
    mysql_tquery(gSQL, gLargeQuery, "", "");
	mysql_tquery(gSQL, "SELECT * FROM businessitems", "loadItems", "");

	return true;
}

function saveBusiness(id) {
	if(strlen(businessVariables[id][bOwner]) >= 1) {
		new queryString[1424];

		mysql_format(gSQL, queryString, sizeof(queryString), "UPDATE businesses SET businessExteriorX = '%f', businessExteriorY = '%f', businessExteriorZ = '%f', businessInteriorX = '%f', businessInteriorY = '%f', businessInteriorZ = '%f', businessInterior = '%d', businessType = '%d', businessName = '%s', businessOwner = '%s', businessPrice = '%d', businessVault = '%d', businessLock = '%d', businessMiscX = '%f', businessMiscY = '%f', businessMiscZ = '%f' WHERE businessID = '%d'", businessVariables[id][bExteriorPos][0],
		businessVariables[id][bExteriorPos][1],	businessVariables[id][bExteriorPos][2],	businessVariables[id][bInteriorPos][0], businessVariables[id][bInteriorPos][1],	businessVariables[id][bInteriorPos][2],	businessVariables[id][bInterior], businessVariables[id][bType], businessVariables[id][bName], businessVariables[id][bOwner], businessVariables[id][bPrice], businessVariables[id][bVault], businessVariables[id][bLocked], businessVariables[id][bMiscPos][0],
		businessVariables[id][bMiscPos][1], businessVariables[id][bMiscPos][2], id);
		mysql_tquery(gSQL, queryString, "", "");
	}
	else {
	    return false;
	}

	return true;
}

stock getPlayerBusinessID(playerid) { 

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
				switch(playerVariables[playerid][pFish]) {
				    case 0: {
				        // Dialog_Open(playerid, DIALOG_SELL_FISH, DIALOG_STYLE_MSGBOX, "SERVER: Fishing", "You are currently carrying $1000 worth of fish.\n\nWould you like to sell your fish to this store for $1000?", "Yes", "No");
				    }
				    case 1: {
				        // Dialog_Open(playerid, DIALOG_SELL_FISH, DIALOG_STYLE_MSGBOX, "SERVER: Fishing", "You are currently carrying $750 worth of fish.\n\nWould you like to sell your fish to this store for $750?", "Yes", "No");
				    }
				    case 2: {
            			// Dialog_Open(playerid, DIALOG_SELL_FISH, DIALOG_STYLE_MSGBOX, "SERVER: Fishing", "You are currently carrying $250 worth of fish.\n\nWould you like to sell your fish to this store for $250?", "Yes", "No");
				    }
				    case 3: {
				        // Dialog_Open(playerid, DIALOG_SELL_FISH, DIALOG_STYLE_MSGBOX, "SERVER: Fishing", "You are currently carrying $900 worth of fish.\n\nWould you like to sell your fish to this store for $900?", "Yes", "No");
				    }
				    case 4: {
				        // Dialog_Open(playerid, DIALOG_SELL_FISH, DIALOG_STYLE_MSGBOX, "SERVER: Fishing", "You are currently carrying $500 worth of fish.\n\nWould you like to sell your fish to this store for $500?", "Yes", "No");
				    }
				}
			}
		}
		case 2: {
			SendClientMessage(playerid, COLOR_WHITE, "Welcome! The commands of this business are as follows: /buyclothes");
		}
		case 3, 4, 7: {
			SendClientMessage(playerid, COLOR_WHITE, "Welcome! The commands of this business are as follows: /buy");
		}
		case 5: {
			SendClientMessage(playerid, COLOR_WHITE, "Welcome! The commands of this business are as follows: /buyvehicle");
		}
		case 6: {
			SendClientMessage(playerid, COLOR_WHITE, "Welcome! The commands of this business are as follows: /buyfightstyle");
		}
	}
	
	return 1;
}

Dialog:DIALOG_SELL_FISH(playerid, response, itemlist, inputtext[]) {
    if(!response) return true;

    switch(playerVariables[playerid][pFish]) {
        case 0: {
            playerVariables[playerid][pMoney] += 1000;
            SendClientMessage(playerid, COLOR_WHITE, "Congratulations, you have sold your collected fish for $1000.");
        }
        case 1: {
            playerVariables[playerid][pMoney] += 750;
            SendClientMessage(playerid, COLOR_WHITE, "Congratulations, you have sold your collected fish for $750.");
        }
        case 2: {
            playerVariables[playerid][pMoney] += 250;
            SendClientMessage(playerid, COLOR_WHITE, "Congratulations, you have sold your collected fish for $250.");
        }
        case 3: {
            playerVariables[playerid][pMoney] += 900;
            SendClientMessage(playerid, COLOR_WHITE, "Congratulations, you have sold your collected fish for $900.");
        }
        case 4: {
            playerVariables[playerid][pMoney] += 500;
            SendClientMessage(playerid, COLOR_WHITE, "Congratulations, you have sold your collected fish for $500.");
        }
    }

    playerVariables[playerid][pFish] = -1; 

    return true;
}

Dialog:DIALOG_BUSINESS_ENTER(playerid, response, itemlist, inputtext[]) {
    if(response == 1) {
        new x = GetPVarInt(playerid, "bE"); // So we don't have to access it each and every time.
        DeletePVar(playerid, "bE"); 

        nearByMessage(playerid, COLOR_PURPLE, 12.0,"* %s breaks down the door and enters the business.", getName(playerid));

        if(playerVariables[playerid][pAdminDuty] < 1 && businessVariables[x][bLocked] == 1) {
            businessVariables[x][bLocked] = 0;
            
            gString[0] = (EOS);
            if(!strcmp(businessVariables[x][bOwner], "Nobody"))
                format(gString, sizeof(gString), "%s\n(Business %d - un-owned)\nPrice: $%d (/buybusiness)\n\n(locked)", businessVariables[x][bName], x, businessVariables[x][bPrice]);
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