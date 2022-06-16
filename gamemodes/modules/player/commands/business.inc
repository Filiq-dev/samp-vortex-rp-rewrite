#define noBizz(%0) SendClientMessage(%0, COLOR_GREY, "You don't own a business.");

YCMD:bbalance(playerid, params[], help) {
	if(!getPlayerBusinessID(playerid)) return noBizz(playerid);
    
    va_SendClientMessage(playerid, COLOR_WHITE, "Business Vault Balance: $%d.", businessVariables[getPlayerBusinessID(playerid)][bVault]);
    
	return true;
}

YCMD:business(playerid, params[], help) {

	new business = getPlayerBusinessID(playerid);

	if(business == 0) return noBizz(playerid);
    if(playerVariables[playerid][pCheckpoint] != 0) va_SendClientMessage(playerid, COLOR_GREY,"You already have an active checkpoint (%s), reach it first, or /killcheckpoint.", getPlayerCheckpointReason(playerid));
    
    SetPlayerCheckpoint(playerid, businessVariables[business][bExteriorPos][0], businessVariables[business][bExteriorPos][1], businessVariables[business][bExteriorPos][2], 5.0);
    SendClientMessage(playerid, COLOR_WHITE, "A checkpoint has been set to your business.");
    playerVariables[playerid][pCheckpoint] = 6;
    
	return true;
}

YCMD:sellbusiness(playerid, params[], help) {
	if(playerVariables[playerid][pStatus] == 0) return true;

    new businessID = getPlayerBusinessID(playerid);

    if(businessID < 1) return noBizz(playerid); 

    playerVariables[playerid][pMoney] += businessVariables[businessID][bPrice];

    gString[0] = (EOS);
    format(businessVariables[businessID][bOwner], MAX_PLAYER_NAME, "Nobody");
    format(gString, sizeof(gString), "%s\n(Business %d - un-owned)\nPrice: $%d (/buybusiness)\n\nPress ~k~~PED_DUCK~ to enter", businessVariables[businessID][bName], businessID, businessVariables[businessID][bPrice]);

    DestroyDynamicPickup(businessVariables[businessID][bPickupID]);
    DestroyDynamic3DTextLabel(businessVariables[businessID][bLabelID]);

    businessVariables[businessID][bLabelID] = CreateDynamic3DTextLabel(gString, COLOR_YELLOW, businessVariables[businessID][bExteriorPos][0], businessVariables[businessID][bExteriorPos][1], businessVariables[businessID][bExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
    businessVariables[businessID][bPickupID] = CreateDynamicPickup(1239, 23, businessVariables[businessID][bExteriorPos][0], businessVariables[businessID][bExteriorPos][1], businessVariables[businessID][bExteriorPos][2], 0, 0, -1, 250);

    businessVariables[businessID][bLocked] = 1; 

    va_SendClientMessage(playerid, COLOR_WHITE, "Business sold! You have been given back $%d for the business.", businessVariables[businessID][bPrice]);

    saveBusiness(businessID); 

	return true;
}

YCMD:bwithdraw(playerid, params[], help) {
	if(getPlayerBusinessID(playerid) == 0) return noBizz(playerid)
    if(!isnull(params)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/bwithdraw [amount]");

    new
        amount = strval(params),
        businessID = getPlayerBusinessID(playerid);

    if(amount < 1 || amount >= 5000000) return SendClientMessage(playerid, COLOR_GREY, "Withdrawal attempt failed.");

    if(businessVariables[businessID][bVault] >= amount) {
        va_SendClientMessage(playerid, COLOR_WHITE, "You have withdrawn $%d from your business.", amount);

        businessVariables[businessID][bVault] -= amount;
        playerVariables[playerid][pMoney] += amount;
    }
    
	return true;
}

YCMD:buyclothes(playerid, params[], help) {
	if(!(GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD >= 1)) return true;

    new
        skinID,
        slotID,
        iPrice,
        houseID = getPlayerHouseID(playerid),
        businessID = GetPlayerVirtualWorld(playerid) - BUSINESS_VIRTUAL_WORLD; 

    if(businessVariables[businessID][bType] != 2) return true;
    if(houseID >= 1) {
        if(sscanf(params, "dd", skinID, slotID)) return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/buyclothes [skinid] [house slot]");
        else if(slotID < 1 || slotID > 5) return SendClientMessage(playerid, COLOR_GREY, "Invalid slot specified.");
            
        houseVariables[houseID][hWardrobe][slotID - 1] = skinID;
    }
    else if(!isnull(params)) {
        skinID = strval(params);
    }
    else { 
        SendClientMessage(playerid, COLOR_GREY, "Skins here cost $500");
        SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/buyclothes [skinid] (Skins cost $500.)");
    }

    
    if(!IsValidSkin(skinID)) return SendClientMessage(playerid, COLOR_GREY, "Invalid skin ID.");
    else if(!IsPublicSkin(skinID) && playerVariables[playerid][pGroup] != 1) return SendClientMessage(playerid, COLOR_GREY, "You can't purchase this skin.");
    else if(playerVariables[playerid][pMoney] < 500) return SendClientMessage(playerid, COLOR_GREY, "You don't have $500 available.");

    playerVariables[playerid][pMoney] -= 500;
    businessVariables[businessID][bVault] += 500;
    playerVariables[playerid][pSkin] = skinID;

    SetPlayerSkin(playerid, skinID); 
        
	return true;
}

YCMD:buy(playerid, params[], help) {
	if(GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD >= 1) {
	    new businessID = GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD;

	    if(businessVariables[businessID][bType] == 0) return true; 

		switch(businessVariables[businessID][bType]) {
			case 1: Dialog_Show(playerid, DIALOG_TWENTYFOURSEVEN, DIALOG_STYLE_LIST, "SERVER: 24/7", "Rope ($5)\nWalkie Talkie ($750)\nPhone Book ($75)\nPhone Credit ($25)\nCellphone ($100)", "Select", "Exit");
			case 3: Dialog_Show(playerid, DIALOG_BAR, DIALOG_STYLE_LIST, "SERVER: Bar", "Stock Bar Cigars ($2)\nSprunk ($3)\nBeer ($5)\nWine ($10)", "Select", "Exit");
			case 4: Dialog_Show(playerid, DIALOG_SEX_SHOP, DIALOG_STYLE_LIST, "SERVER: Sex Shop", "Purple Dildo ($5)\nSmall White Vibrator ($8)\nSilver Vibrator ($10)\nLarge White Vibrator ($12)\nFlowers ($14), "Select", "Exit");
			case 7: {
                if(playerVariables[playerid][pMoney] < 250) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
                GetPlayerHealth(playerid, playerVariables[playerid][pHealth]);

                if(playerVariables[playerid][pHealth] > 70.0) return SendClientMessage(playerid, COLOR_GREY, "You are unable to consume this product.");

                playerVariables[playerid][pMoney] -= 250;
                businessVariables[businessID][bVault] += 250;

                SetPlayerHealth(playerid, playerVariables[playerid][pHealth]+30);

                SendClientMessage(playerid, COLOR_WHITE, "You've purchased and consumed the 250, which has increased your health by 30 percent.");
            }
		}
	}

	return true;
}  
/*
rewrite all vehicle systems
YCMD:buyvehicle(playerid, params[], help) {
	if(GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD >= 1) {
	    new
	        businessID = GetPlayerVirtualWorld(playerid) - BUSINESS_VIRTUAL_WORLD;

		if(businessVariables[businessID][bMiscPos][0] == 0.0 && businessVariables[businessID][bMiscPos][1] == 0.0 && businessVariables[businessID][bMiscPos][2] == 0.0) {
			return SendClientMessage(playerid, COLOR_GREY, "No spawn position has been set by the business owner - until one is set, the business will not operate.");
		}
	    switch(businessVariables[businessID][bType]) {
			case 5: {
				// ShowPlayerDialog(playerid, DIALOG_BUYCAR, DIALOG_STYLE_LIST, "SERVER: Vehicle Dealership", "Second Hand\nClassic Autos\nSedans\nSUVs/Trucks\nMotorcycles\nPerformance Vehicles", "Select", "Cancel");
			}
		}
	}
	return 1;
}
*/
YCMD:buyfightstyle(playerid, params[], help) {
	if(GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD >= 1) {
	    new businessID = GetPlayerVirtualWorld(playerid) - BUSINESS_VIRTUAL_WORLD;

	    if(businessVariables[businessID][bType] == 6) 
            Dialog_Open(playerid, DIALOG_FIGHTSTYLE, DIALOG_STYLE_LIST, "SERVER: Fighting Styles", "Boxing ($10,000)\nKung Fu ($25,000)\nKnee Head ($15,000)\nGrab & Kick ($12,000)\nElbow ($10,000)\nGhetto ($5,000)", "Select", "Cancel");
	}
	return 1;
}  

Dialog:DIALOG_FIGHTSTYLE(playerid, response, listitem, inputtext[]) {
    if(!response) return true;

    new business = GetPlayerVirtualWorld(playerid) - BUSINESS_VIRTUAL_WORLD;

    switch(listitem) {
        case 0: {
            if(playerVariables[playerid][pFightStyle] == FIGHT_STYLE_BOXING) return (playerid, COLOR_GREY, "You are already using this style.");
            if(playerVariables[playerid][pMoney] < 10000) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough money to purchase this.");

            playerVariables[playerid][pMoney] -= 10000;
            businessVariables[business][bVault] += 10000;
            playerVariables[playerid][pFightStyle] = FIGHT_STYLE_BOXING; 
        }
        case 1: {
            if(playerVariables[playerid][pFightStyle] == FIGHT_STYLE_KUNGFU) return (playerid, COLOR_GREY, "You are already using this style.");
            if(playerVariables[playerid][pMoney] < 25000) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough money to purchase this.");

            playerVariables[playerid][pMoney] -= 25000;
            businessVariables[business][bVault] += 25000;
            playerVariables[playerid][pFightStyle] = FIGHT_STYLE_KUNGFU;  
        }
        case 2: {
            if(playerVariables[playerid][pFightStyle] == FIGHT_STYLE_KNEEHEAD) return (playerid, COLOR_GREY, "You are already using this style.");
            if(playerVariables[playerid][pMoney] < 15000) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough money to purchase this.");

            playerVariables[playerid][pMoney] -= 15000;
            businessVariables[business][bVault] += 15000;
            playerVariables[playerid][pFightStyle] = FIGHT_STYLE_KNEEHEAD;  
        }
        case 3: {
            if(playerVariables[playerid][pFightStyle] == FIGHT_STYLE_GRABKICK) return (playerid, COLOR_GREY, "You are already using this style.");
            if(playerVariables[playerid][pMoney] < 15000) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough money to purchase this.");

            playerVariables[playerid][pMoney] -= 15000;
            businessVariables[business][bVault] += 15000;
            playerVariables[playerid][pFightStyle] = FIGHT_STYLE_GRABKICK;   
        }
        case 4: {
            if(playerVariables[playerid][pFightStyle] == FIGHT_STYLE_ELBOW) return (playerid, COLOR_GREY, "You are already using this style.");
            if(playerVariables[playerid][pMoney] < 10000) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough money to purchase this.");

            playerVariables[playerid][pMoney] -= 10000;
            businessVariables[business][bVault] += 10000;
            playerVariables[playerid][pFightStyle] = FIGHT_STYLE_ELBOW;    
        }
        case 5: {
            if(playerVariables[playerid][pFightStyle] == FIGHT_STYLE_NORMAL) return (playerid, COLOR_GREY, "You are already using this style.");
            if(playerVariables[playerid][pMoney] < 5000) return SendClientMessage(playerid, COLOR_GREY, "You do not have enough money to purchase this.");

            playerVariables[playerid][pMoney] -= 5000;
            businessVariables[business][bVault] += 5000;
            playerVariables[playerid][pFightStyle] = FIGHT_STYLE_NORMAL;     
        }

        SendClientMessage(playerid, COLOR_WHITE, "You have successfully purchased this style of fighting.");
        SetPlayerFightingStyle(playerid, playerVariables[playerid][pFightStyle]); 
    } 
}

YCMD:businessname(playerid, params[], help) {
	if(getPlayerBusinessID(playerid) >= 1) {
	    if(isnull(params))
			 return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/businessname [businessname]");

	    if(strlen(params) >= 43 || strlen(params) < 1)
			return SendClientMessage(playerid, COLOR_GREY, "Invalid name length (1-42).");

	    new
	        x = getPlayerBusinessID(playerid);

	    format(result, sizeof(result), "You have changed the name of your business to '%s'.", params);
	    SendClientMessage(playerid, COLOR_WHITE, result);

		mysql_real_escape_string(params, params);
		strcpy(businessVariables[x][bName], params, 20);

	    switch(businessVariables[x][bLocked]) {
			case 1: {
				format(result, sizeof(result), "%s\n(Business %d - owned by %s)\n\n(locked)", businessVariables[x][bName], x, businessVariables[x][bOwner]);
			}
			case 0: {
				format(result, sizeof(result), "%s\n(Business %d - owned by %s)\n\nPress ~k~~PED_DUCK~ to enter", businessVariables[x][bName], x, businessVariables[x][bOwner]);
			}
		}

		UpdateDynamic3DTextLabelText(businessVariables[x][bLabelID], COLOR_YELLOW, result);
	}
	return 1;
}

YCMD:bspawnpos(playerid, params[], help) {

	if(getPlayerBusinessID(playerid) >= 1) {

		new
			businessID = getPlayerBusinessID(playerid);

		if(businessVariables[businessID][bType] == 5) {
			if(IsPlayerInRangeOfPoint(playerid, 30.0, businessVariables[businessID][bExteriorPos][0], businessVariables[businessID][bExteriorPos][1], businessVariables[businessID][bExteriorPos][2])) {
				GetPlayerPos(playerid, businessVariables[businessID][bMiscPos][0], businessVariables[businessID][bMiscPos][1], businessVariables[businessID][bMiscPos][2]);
				SendClientMessage(playerid, COLOR_WHITE, "You have successfully altered the spawn position of your vehicle dealership business.");
			}
			else SendClientMessage(playerid, COLOR_GREY, "You must be within thirty metres of the exterior of your business.");
		}
		else SendClientMessage(playerid, COLOR_GREY, "You don't own a vehicle dealership.");
	}
	return 1;
}


YCMD:lockbusiness(playerid, params[], help) {
	if(getPlayerBusinessID(playerid) >= 1) {
	    new
	        x = getPlayerBusinessID(playerid);

	    switch(businessVariables[x][bLocked]) {
			case 0: {
				format(result, sizeof(result), "%s\n(Business %d - owned by %s)\n\n(locked)", businessVariables[x][bName], x, businessVariables[x][bOwner]);

				businessVariables[x][bLocked] = 1;
				SendClientMessage(playerid, COLOR_WHITE, "Business locked.");
			}
			case 1: {
				format(result, sizeof(result), "%s\n(Business %d - owned by %s)\n\nPress ~k~~PED_DUCK~ to enter", businessVariables[x][bName], x, businessVariables[x][bOwner]);

			    businessVariables[x][bLocked] = 0;
			    SendClientMessage(playerid, COLOR_WHITE, "Business unlocked.");
			}
		}
		UpdateDynamic3DTextLabelText(businessVariables[x][bLabelID], COLOR_YELLOW, result);
	}

	return 1;
}

YCMD:buybusiness(playerid, params[], help) {
    if(playerVariables[playerid][pStatus] >= 1) {
        for(new x = 0; x < MAX_BUSINESSES; x++) {
			if(IsPlayerInRangeOfPoint(playerid, 5, businessVariables[x][bExteriorPos][0], businessVariables[x][bExteriorPos][1], businessVariables[x][bExteriorPos][2])) {
				if(!strcmp(businessVariables[x][bOwner], "Nobody", true)) {
				    if(businessVariables[x][bPrice] == -1) return SendClientMessage(playerid, COLOR_GREY, "This business was blocked from being purchased by an administrator.");
					if(getPlayerBusinessID(playerid) >= 1) return SendClientMessage(playerid, COLOR_GREY, "You already own a business.");
					if(playerVariables[playerid][pMoney] >= businessVariables[x][bPrice]) {
						playerVariables[playerid][pMoney] -= businessVariables[x][bPrice];

						new
						    labelString[96];

						strcpy(businessVariables[x][bOwner], playerVariables[playerid][pNormalName], MAX_PLAYER_NAME);

						DestroyDynamicPickup(businessVariables[x][bPickupID]);

					    if(businessVariables[x][bLocked] == 1) {
					    	format(labelString, sizeof(labelString), "%s\n(Business %d - owned by %s)\n\n(locked)", businessVariables[x][bName], x, businessVariables[x][bOwner]);
					    }
					    else {
					        format(labelString, sizeof(labelString), "%s\n(Business %d - owned by %s)\n\nPress ~k~~PED_DUCK~ to enter", businessVariables[x][bName], x, businessVariables[x][bOwner]);
					    }
						UpdateDynamic3DTextLabelText(businessVariables[x][bLabelID], COLOR_YELLOW, labelString);
						businessVariables[x][bPickupID] = CreateDynamicPickup(1239, 23, businessVariables[x][bExteriorPos][0], businessVariables[x][bExteriorPos][1], businessVariables[x][bExteriorPos][2], 0, 0, -1, 250);

						SendClientMessage(playerid, COLOR_WHITE, "Congratulations on your purchase!");

						saveBusiness(x);
					}
					else SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this business.");
				}
				else {
					return SendClientMessage(playerid, COLOR_GREY, "You can't purchase an owned business.");
				}
			}
		}
    }
	return 1;
} 

Dialog:DIALOG_TWENTYFOURSEVEN(playerid, response, listitem, inputtext[]) {
    if(!response) return true;

    new businessID = GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD; 

    switch(listitem+1) {
        case 1: {
            if(playerVariables[playerid][pMoney] < 5) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
            if(playerVariables[playerid][pRope] >= 30) return SendClientMessage(playerid, COLOR_GREY, "You are unable to purchase any more rope.");

            playerVariables[playerid][pMoney] -= 5;
            playerVariables[playerid][pRope]++;
            businessVariables[businessID][bVault] += 5;

            SendClientMessage(playerid, COLOR_WHITE, "You have purchased 1 line of Rope!");  
        }
        case 2: {
            if(playerVariables[playerid][pMoney] < 750) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
            if(playerVariables[playerid][pWalkieTalkie] != -1) return SendClientMessage(playerid, COLOR_GREY, "You are unable to purchase another walkie talkie.");

            playerVariables[playerid][pMoney] -= 750;
            businessVariables[businessID][bVault] += 750;
            playerVariables[playerid][pWalkieTalkie] = 0;

            SendClientMessage(playerid, COLOR_WHITE, "You have purchased a walkie talkie - use /setfrequency to tune it, and /wt to speak."); 
        }
        case 3: {
            if(playerVariables[playerid][pMoney] < 75) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
            if(playerVariables[playerid][pPhoneBook] != 1) return SendClientMessage(playerid, COLOR_GREY, "You are unable to purchase another phonebook.");

            playerVariables[playerid][pMoney] -= 75;
            businessVariables[businessID][bVault] += 75;
            playerVariables[playerid][pPhoneBook] = 1;

            SendClientMessage(playerid, COLOR_WHITE, "You have purchased a phonebook. Use /number to trace a number down!");
        }
        case 4: {
            if(playerVariables[playerid][pPhoneNumber] == -1) return SendClientMessage(playerid, COLOR_GREY, "You do not have a phone.");
            if(playerVariables[playerid][pMoney] < 25) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
           
            playerVariables[playerid][pMoney] -= 25;
            businessVariables[businessID][bVault] += 25;
            playerVariables[playerid][pPhoneCredit] += 25*60; 

            SendClientMessage(playerid, COLOR_WHITE, "You have purchased a $25 credit voucher for your mobile phone which has been automatically applied."); 
        }
        case 5: {
            if(playerVariables[playerid][pMoney] < 100) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
            if(playerVariables[playerid][pPhoneNumber] != -1) SendClientMessage(playerid, COLOR_GREY, "You already had a phone, your phone will be replaced and your number will be changed.");
 
            playerVariables[playerid][pMoney] -= 100;
            businessVariables[businessID][bVault] += 100;
            playerVariables[playerid][pPhoneNumber] = random(89999999)+10000000; // Random eight digit phone number (which won't get crazy ones like 0, etc)
 
            va_SendClientMessage(playerid, COLOR_WHITE, "You have purchased a Phone! Your number is %d.", playerVariables[playerid][pPhoneNumber]);
        }
    }

    return true;
}

Dialog:DIALOG_SEX_SHOP(playerid, response, listitem, inputtext[]) {
    if(!response) return true; 

    new 
        businessID = GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD;

    switch(listitem) {
        case 0: { // Purple Dildo
            if(playerVariables[playerid][pMoney] < 5) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to pay for this product."); 

            playerVariables[playerid][pMoney] -= 5;
            businessVariables[businessID][bVault] += 5;
            givePlayerValidWeapon(playerid, 10);
        }
        case 1: { // Small White Vibrator
            if(playerVariables[playerid][pMoney] < 8) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to pay for this product.");

            playerVariables[playerid][pMoney] -= 8;
            businessVariables[businessID][bVault] += 8;
            givePlayerValidWeapon(playerid, 11);
        }
        case 2: { // Silver Vibrator
            if(playerVariables[playerid][pMoney] < 10) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to pay for this product.");

            playerVariables[playerid][pMoney] -= 10;
            businessVariables[businessID][bVault] += 10;
            givePlayerValidWeapon(playerid, 13);
        }
        case 3: { // Large White Vibrator
            if(playerVariables[playerid][pMoney] < 12) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to pay for this product.");

            playerVariables[playerid][pMoney] -= 12;
            businessVariables[businessID][bVault] += 12;
            givePlayerValidWeapon(playerid, 12);
        }
        case 4: { // Flowers
            if(playerVariables[playerid][pMoney] < 14) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to pay for this product.");

            playerVariables[playerid][pMoney] -= 14;
            businessVariables[businessID][bVault] += 14;
            givePlayerValidWeapon(playerid, 14);
        }
    }
}

Dialog:DIALOG_BAR(playerid, response, listitem, inputtext[]) { 
    if(!response) return true;

    new businessID = GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD;

    switch(listitem) {
        case 0: { // Stock Bar Cigars
            if(playerVariables[playerid][pMoney] < 2) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to pay for this product.");
            
            playerVariables[playerid][pMoney] -= 2;
            businessVariables[businessID][bVault] += 2;

            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_SMOKE_CIGGY);  
        }
        case 1: { // Sprunk
            if(playerVariables[playerid][pMoney] < 2) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to pay for this product.");
            
            playerVariables[playerid][pMoney] -= 3;
            businessVariables[businessID][bVault] += 3;
           
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK); 
        }
        case 3: { // Beer
            if(playerVariables[playerid][pMoney] < 2) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to pay for this product.");
           
            playerVariables[playerid][pMoney] -= 5;
            businessVariables[businessID][bVault] += 5;
            
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_BEER); 
        }
        case 4: { // Wine
            if(playerVariables[playerid][pMoney] < 2) return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to pay for this product.");
            
            playerVariables[playerid][pMoney] -= 10;
            businessVariables[businessID][bVault] += 10;

            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_WINE); 
        }
    }
} 