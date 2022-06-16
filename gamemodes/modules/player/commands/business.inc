CMD:bbalance(playerid, params[]) {
	if(getPlayerBusinessID(playerid) >= 1) {
	    new
	        businessID = getPlayerBusinessID(playerid);

	    format(szMessage, sizeof(szMessage), "Business Vault Balance: $%d.", businessVariables[businessID][bVault]);
	    SendClientMessage(playerid, COLOR_WHITE, szMessage);
	}
	return 1;
}

CMD:business(playerid, params[]) {

	new
		business = getPlayerBusinessID(playerid);

	if(business >= 1) {
		if(playerVariables[playerid][pCheckpoint] >= 1) {
			new string[96];
			format(string, sizeof(string), "You already have an active checkpoint (%s), reach it first, or /killcheckpoint.", getPlayerCheckpointReason(playerid));
			return SendClientMessage(playerid, COLOR_GREY,string);
		}
		SetPlayerCheckpoint(playerid, businessVariables[business][bExteriorPos][0], businessVariables[business][bExteriorPos][1], businessVariables[business][bExteriorPos][2], 5.0);
		SendClientMessage(playerid, COLOR_WHITE, "A checkpoint has been set to your business.");
		playerVariables[playerid][pCheckpoint] = 6;
	}
	else SendClientMessage(playerid, COLOR_GREY, "You don't own a business.");
	return 1;
}

CMD:sellbusiness(playerid, params[]) {
	if(playerVariables[playerid][pStatus] >= 1) {
	    new
	        businessID = getPlayerBusinessID(playerid);

	    if(businessID < 1)
	        return 1;

	    new
	    	labelString[96];

		playerVariables[playerid][pMoney] += businessVariables[businessID][bPrice];

        format(businessVariables[businessID][bOwner], MAX_PLAYER_NAME, "Nobody");
        format(labelString, sizeof(labelString), "%s\n(Business %d - un-owned)\nPrice: $%d (/buybusiness)\n\nPress ~k~~PED_DUCK~ to enter", businessVariables[businessID][bName], businessID, businessVariables[businessID][bPrice]);

        DestroyDynamicPickup(businessVariables[businessID][bPickupID]);
        DestroyDynamic3DTextLabel(businessVariables[businessID][bLabelID]);

		businessVariables[businessID][bLabelID] = CreateDynamic3DTextLabel(labelString, COLOR_YELLOW, businessVariables[businessID][bExteriorPos][0], businessVariables[businessID][bExteriorPos][1], businessVariables[businessID][bExteriorPos][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 10.0);
		businessVariables[businessID][bPickupID] = CreateDynamicPickup(1239, 23, businessVariables[businessID][bExteriorPos][0], businessVariables[businessID][bExteriorPos][1], businessVariables[businessID][bExteriorPos][2], 0, 0, -1, 250);

		businessVariables[businessID][bLocked] = 1;

		format(labelString, sizeof(labelString), "Business sold! You have been given back $%d for the business.", businessVariables[businessID][bPrice]);
		SendClientMessage(playerid, COLOR_WHITE, labelString);

		saveBusiness(businessID);
	}
	return 1;
}

CMD:bwithdraw(playerid, params[]) {
	if(getPlayerBusinessID(playerid) >= 1) {
	    if(!isnull(params)) {
			new
			    amount = strval(params),
			    businessID = getPlayerBusinessID(playerid);

        	if(amount < 1 || amount >= 5000000)
				return SendClientMessage(playerid, COLOR_GREY, "Withdrawal attempt failed.");

			if(businessVariables[businessID][bVault] >= amount) {
				format(szMessage, sizeof(szMessage), "You have withdrawn $%d from your business.", amount);
				SendClientMessage(playerid, COLOR_WHITE, szMessage);

				businessVariables[businessID][bVault] -= amount;
				playerVariables[playerid][pMoney] += amount;
			}
		}
		else {
			return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/bwithdraw [amount]");
		}
	}
	return 1;
}

CMD:buyclothes(playerid, params[]) {
	if(GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD >= 1) {

		new
			skinID,
			slotID,
			iPrice,
			houseID = getPlayerHouseID(playerid),
			businessID = GetPlayerVirtualWorld(playerid) - BUSINESS_VIRTUAL_WORLD;
			
		if(businessID > 0) {
			for(new i = 0; i < MAX_BUSINESS_ITEMS; i++) {
				if(businessItems[i][bItemType] == 18 && businessItems[i][bItemBusiness] == businessID)
				    iPrice = businessItems[i][bItemPrice];
			}
		}

		if(businessVariables[businessID][bType] == 2) {
			if(houseID >= 1) {
				if(sscanf(params, "dd", skinID, slotID)) {
					return SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/buyclothes [skinid] [house slot]");
				}
				else if(!IsValidSkin(skinID)) {
					return SendClientMessage(playerid, COLOR_GREY, "Invalid skin ID.");
				}
				else if(!IsPublicSkin(skinID) && playerVariables[playerid][pGroup] != 1) {
					return SendClientMessage(playerid, COLOR_GREY, "You can't purchase this skin.");
				}
				else if(slotID < 1 || slotID > 5) {
					return SendClientMessage(playerid, COLOR_GREY, "Invalid slot specified.");
				}
				else if(playerVariables[playerid][pMoney] >= 500) {
					playerVariables[playerid][pMoney] -= 500;
					businessVariables[businessID][bVault] += 500;
					playerVariables[playerid][pSkin] = skinID;
					houseVariables[houseID][hWardrobe][slotID - 1] = skinID;
					return SetPlayerSkin(playerid, skinID);
				}
				else {
				    format(szMessage, sizeof(szMessage), "You don't have $%d available.", iPrice);
					SendClientMessage(playerid, COLOR_GREY, szMessage);
				}
			}
			else if(!isnull(params)) {
				skinID = strval(params);

				if(!IsValidSkin(skinID)) {
					return SendClientMessage(playerid, COLOR_GREY, "Invalid skin ID.");
				}
				else if(!IsPublicSkin(skinID) && playerVariables[playerid][pGroup] != 1) {
					return SendClientMessage(playerid, COLOR_GREY, "You can't purchase this skin.");
				}
				else {
				    if(playerVariables[playerid][pMoney] >= iPrice) {
						playerVariables[playerid][pMoney] -= iPrice;
						businessVariables[businessID][bVault] += iPrice;
						playerVariables[playerid][pSkin] = skinID;
						return SetPlayerSkin(playerid, skinID);
					} else {
					    format(szMessage, sizeof(szMessage), "You don't have $%d available.", iPrice);
						SendClientMessage(playerid, COLOR_GREY, szMessage);
					}
				}
			}
			else {
   				format(szMessage, sizeof(szMessage), "Skins here cost $%d", iPrice);
				SendClientMessage(playerid, COLOR_GREY, szMessage);
				SendClientMessage(playerid, COLOR_GREY, SYNTAX_MESSAGE"/buyclothes [skinid] (Skins cost $500.)");
			}
		}
	}
	return 1;
}

CMD:buy(playerid, params[]) {
	if(GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD >= 1) {
	    new
	        businessID = GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD;

	    if(businessVariables[businessID][bType] == 0)
	        return 1;
	        
		format(result, sizeof(result), "");
		
		new
		    iCount;

	    for(new i = 0; i < MAX_BUSINESS_ITEMS; i++) {
	    	if(businessItems[i][bItemBusiness] == businessID) {
	    	    format(szSmallString, sizeof(szSmallString), "menuItem%d", iCount);
	    	    SetPVarInt(playerid, szSmallString, i);
	    	    
	    	    if(businessItems[i][bItemType] == 4)
	        		format(result, sizeof(result), "%s\n$%d phone credit voucher", result, businessItems[i][bItemPrice]);
	        	else
	        		format(result, sizeof(result), "%s\n%s ($%d)", result, businessItems[i][bItemName], businessItems[i][bItemPrice]);

				iCount++;
      		}
	    }

		switch(businessVariables[businessID][bType]) {
			case 1: Dialog_Open(playerid, DIALOG_TWENTYFOURSEVEN, DIALOG_STYLE_LIST, "SERVER: 24/7", result, "Select", "Exit");
			case 3: Dialog_Open(playerid, DIALOG_BAR, DIALOG_STYLE_LIST, "SERVER: Bar", result, "Select", "Exit");
			case 4: Dialog_Open(playerid, DIALOG_SEX_SHOP, DIALOG_STYLE_LIST, "SERVER: Sex Shop", result, "Select", "Exit");
			case 7: Dialog_Open(playerid, DIALOG_FOOD, DIALOG_STYLE_LIST, "SERVER: Restaurant", result, "Select", "Exit");
		}
	}
	return 1;
}

Dialog:DIALOG_SEX_SHOP(playerid, response, listitem, inputtext[]) {
    if(!response) return true;

    listitem += 1;

    new
        i,
        b,
        businessID = GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD;

    foreach(new x : bizzItems) {
        b++;
        gString[0] = (EOS)
        format(gString, 20, "menuItem%d", x);
        if(GetPVarType(playerid, gString) != 0)
            i = GetPVarInt(playerid, gString);

        if(b == listitem) {
            foreach(new f : bizzItems) {
                gString[0] = (EOS)
                format(gString, 20, "menuItem%d", xf);
                if(GetPVarType(playerid, gString) != 0)
                    DeletePVar(playerid, gString);
                else
                    break;
            }

            break;
        }
    }

    switch(businessItems[i][bItemType]) {
        case 9: {
            if(playerVariables[playerid][pMoney] >= businessItems[i][bItemPrice]) {
                playerVariables[playerid][pMoney] -= businessItems[i][bItemPrice];
                businessVariables[businessID][bVault] += businessItems[i][bItemPrice]; 
                 
            } else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
        }
        case 10: {
            if(playerVariables[playerid][pMoney] >= businessItems[i][bItemPrice]) {
                playerVariables[playerid][pMoney] -= businessItems[i][bItemPrice];
                businessVariables[businessID][bVault] += businessItems[i][bItemPrice]; 
                
            } else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
        }
        case 11: {
            if(playerVariables[playerid][pMoney] >= businessItems[i][bItemPrice]) {
                playerVariables[playerid][pMoney] -= businessItems[i][bItemPrice];
                businessVariables[businessID][bVault] += businessItems[i][bItemPrice]; 
                
            } else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
        }
        case 12: {
            if(playerVariables[playerid][pMoney] >= businessItems[i][bItemPrice]) {
                playerVariables[playerid][pMoney] -= businessItems[i][bItemPrice];
                businessVariables[businessID][bVault] += businessItems[i][bItemPrice]; 
                
            } else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
        }
        case 13: {
            if(playerVariables[playerid][pMoney] >= businessItems[i][bItemPrice]) {
                playerVariables[playerid][pMoney] -= businessItems[i][bItemPrice];
                businessVariables[businessID][bVault] += businessItems[i][bItemPrice];
                
            } else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
        }
    }

    givePlayerValidWeapon(playerid, businessItems[i][bItemType]+1); 

    switch(random(4)) {
        case 0: va_SendClientMessage(playerid, COLOR_WHITE, "You've purchased the %s. Don't get too wild.", businessItems[i][bItemName]);
        case 1: va_SendClientMessage(playerid, COLOR_WHITE, "You've purchased the %s. Don't blame us if you get it stuck up there!", businessItems[i][bItemName]);
        case 2: va_SendClientMessage(playerid, COLOR_WHITE, "You've purchased the %s. There's no warranty for this product!", businessItems[i][bItemName]);
        case 3: va_SendClientMessage(playerid, COLOR_WHITE, "You've purchased the %s. Justin Bieber approves of this.", businessItems[i][bItemName]);
    }
}

Dialog:DIALOG_BAR(playerid, response, listitem, inputtext[]) { 
    if(!response)
        return 1;

    listitem += 1;

    new
        i,
        b,
        businessID = GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD;

    for(new x = 0; x < MAX_BUSINESS_ITEMS; x++) {
        b++;
        format(szSmallString, sizeof(szSmallString), "menuItem%d", x);
        if(GetPVarType(playerid, szSmallString) != 0)
            i = GetPVarInt(playerid, szSmallString);

        if(b == listitem) {
            for(new xf = 0; xf < MAX_BUSINESS_ITEMS; xf++) {
                format(szSmallString, sizeof(szSmallString), "menuItem%d", xf);
                if(GetPVarType(playerid, szSmallString) != 0)
                    DeletePVar(playerid, szSmallString);
                else
                    break;
            }

            break;
        }
    }

    switch(businessItems[i][bItemType]) {
        case 14: {
            if(playerVariables[playerid][pMoney] >= businessItems[i][bItemPrice]) {
                playerVariables[playerid][pMoney] -= businessItems[i][bItemPrice];
                businessVariables[businessID][bVault] += businessItems[i][bItemPrice];

                SetPlayerSpecialAction(playerid, SPECIAL_ACTION_SMOKE_CIGGY);

                format(szMessage, sizeof(szMessage), "You've purchased %s.", businessItems[i][bItemName]);
                SendClientMessage(playerid, COLOR_WHITE, szMessage);
                return 1;
            } else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
        }
        case 15: {
            if(playerVariables[playerid][pMoney] >= businessItems[i][bItemPrice]) {
                playerVariables[playerid][pMoney] -= businessItems[i][bItemPrice];
                businessVariables[businessID][bVault] += businessItems[i][bItemPrice];

                SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);

                format(szMessage, sizeof(szMessage), "You've purchased %s.", businessItems[i][bItemName]);
                SendClientMessage(playerid, COLOR_WHITE, szMessage);
                return 1;
            } else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
        }
        case 16: {
            if(playerVariables[playerid][pMoney] >= businessItems[i][bItemPrice]) {
                playerVariables[playerid][pMoney] -= businessItems[i][bItemPrice];
                businessVariables[businessID][bVault] += businessItems[i][bItemPrice];

                SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_BEER);

                format(szMessage, sizeof(szMessage), "You've purchased %s.", businessItems[i][bItemName]);
                SendClientMessage(playerid, COLOR_WHITE, szMessage);
                return 1;
            } else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
        }
        case 17: {
            if(playerVariables[playerid][pMoney] >= businessItems[i][bItemPrice]) {
                playerVariables[playerid][pMoney] -= businessItems[i][bItemPrice];
                businessVariables[businessID][bVault] += businessItems[i][bItemPrice];

                SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_WINE);

                format(szMessage, sizeof(szMessage), "You've purchased %s.", businessItems[i][bItemName]);
                SendClientMessage(playerid, COLOR_WHITE, szMessage);
                return 1;
            } else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
        }
    }
}

CMD:buyvehicle(playerid, params[]) {
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

CMD:buyfightstyle(playerid, params[]) {
	if(GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD >= 1) {
	    new businessID = GetPlayerVirtualWorld(playerid) - BUSINESS_VIRTUAL_WORLD;

	    if(businessVariables[businessID][bType] == 6) 
            Dialog_Open(playerid, DIALOG_FIGHTSTYLE, DIALOG_STYLE_LIST, "SERVER: Fighting Styles", "Boxing ($10,000)\nKung Fu ($25,000)\nKnee Head ($15,000)\nGrab & Kick ($12,000)\nElbow ($10,000)\nGhetto ($5,000)", "Select", "Cancel");
	}
	return 1;
}

Dialog:DIALOG_FOOD(playerid, response, listitem, inputtext[]) {
    if(!response)
        return 1;

    listitem += 1;

    new
        i,
        b,
        businessID = GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD;

    for(new x = 0; x < MAX_BUSINESS_ITEMS; x++) {
        b++;
        format(szSmallString, sizeof(szSmallString), "menuItem%d", x);
        if(GetPVarType(playerid, szSmallString) != 0)
            i = GetPVarInt(playerid, szSmallString);

        if(b == listitem) {
            for(new xf = 0; xf < MAX_BUSINESS_ITEMS; xf++) {
                format(szSmallString, sizeof(szSmallString), "menuItem%d", xf);
                if(GetPVarType(playerid, szSmallString) != 0)
                    DeletePVar(playerid, szSmallString);
                else
                    break;
            }

            break;
        }
    }

    switch(businessItems[i][bItemType]) {
        case 6: {
            if(playerVariables[playerid][pMoney] >= businessItems[i][bItemPrice]) {
                GetPlayerHealth(playerid, playerVariables[playerid][pHealth]);
                
                if(playerVariables[playerid][pHealth] > 95.0)
                    return SendClientMessage(playerid, COLOR_GREY, "You are unable to consume this product.");

                playerVariables[playerid][pMoney] -= businessItems[i][bItemPrice];
                businessVariables[businessID][bVault] += businessItems[i][bItemPrice];

                SetPlayerHealth(playerid, playerVariables[playerid][pHealth]+5);

                format(szMessage, sizeof(szMessage), "You've purchased and consumed the %s, which has increased your health by 5 percent.", businessItems[i][bItemName]);
                SendClientMessage(playerid, COLOR_WHITE, szMessage);
                return 1;
            } else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
        }
        case 7: {
            if(playerVariables[playerid][pMoney] >= businessItems[i][bItemPrice]) {
                GetPlayerHealth(playerid, playerVariables[playerid][pHealth]);

                if(playerVariables[playerid][pHealth] > 90.0)
                    return SendClientMessage(playerid, COLOR_GREY, "You are unable to consume this product.");

                playerVariables[playerid][pMoney] -= businessItems[i][bItemPrice];
                businessVariables[businessID][bVault] += businessItems[i][bItemPrice];

                SetPlayerHealth(playerid, playerVariables[playerid][pHealth]+10);

                format(szMessage, sizeof(szMessage), "You've purchased and consumed the %s, which has increased your health by 10 percent.", businessItems[i][bItemName]);
                SendClientMessage(playerid, COLOR_WHITE, szMessage);
                return 1;
            } else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
        }
        case 8: {
            if(playerVariables[playerid][pMoney] >= businessItems[i][bItemPrice]) {
                GetPlayerHealth(playerid, playerVariables[playerid][pHealth]);

                if(playerVariables[playerid][pHealth] > 70.0)
                    return SendClientMessage(playerid, COLOR_GREY, "You are unable to consume this product.");

                playerVariables[playerid][pMoney] -= businessItems[i][bItemPrice];
                businessVariables[businessID][bVault] += businessItems[i][bItemPrice];

                SetPlayerHealth(playerid, playerVariables[playerid][pHealth]+30);

                format(szMessage, sizeof(szMessage), "You've purchased and consumed the %s, which has increased your health by 30 percent.", businessItems[i][bItemName]);
                SendClientMessage(playerid, COLOR_WHITE, szMessage);
                return 1;
            } else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
        }
    }
}
Dialog:DIALOG_TWENTYFOURSEVEN(playerid, response, listitem, inputtext[]) {
    if(!response)
        return 1;
        
    listitem += 1;

    new
        i,
        b,
        businessID = GetPlayerVirtualWorld(playerid)-BUSINESS_VIRTUAL_WORLD;
        
    for(new x = 0; x < MAX_BUSINESS_ITEMS; x++) {
        b++;
        format(szSmallString, sizeof(szSmallString), "menuItem%d", x);
        if(GetPVarType(playerid, szSmallString) != 0)
            i = GetPVarInt(playerid, szSmallString);

        if(b == listitem) {
            for(new xf = 0; xf < MAX_BUSINESS_ITEMS; xf++) {
                format(szSmallString, sizeof(szSmallString), "menuItem%d", xf);
                if(GetPVarType(playerid, szSmallString) != 0)
                    DeletePVar(playerid, szSmallString);
                else
                    break;
            }
            
            break;
        }
    }
    
    switch(businessItems[i][bItemType]) {
        case 1: {
            if(playerVariables[playerid][pMoney] >= businessItems[i][bItemPrice]) {
                if(playerVariables[playerid][pRope] >= 30)
                    return SendClientMessage(playerid, COLOR_GREY, "You are unable to purchase any more rope.");

                playerVariables[playerid][pMoney] -= businessItems[i][bItemPrice];
                            
                businessVariables[businessID][bVault] += businessItems[i][bItemPrice];
                playerVariables[playerid][pRope]++;

                SendClientMessage(playerid, COLOR_WHITE, "You have purchased 1 line of Rope!");
                return 1;
            } else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
        }
        case 2: {
            if(playerVariables[playerid][pMoney] >= businessItems[i][bItemPrice]) {
                if(playerVariables[playerid][pWalkieTalkie] != -1)
                    return SendClientMessage(playerid, COLOR_GREY, "You are unable to purchase another walkie talkie.");

                playerVariables[playerid][pMoney] -= businessItems[i][bItemPrice];
                businessVariables[businessID][bVault] += businessItems[i][bItemPrice];
                playerVariables[playerid][pWalkieTalkie] = 0;

                SendClientMessage(playerid, COLOR_WHITE, "You have purchased a walkie talkie - use /setfrequency to tune it, and /wt to speak.");
                return 1;
            } else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
        }
        case 3: {
            if(playerVariables[playerid][pMoney] >= businessItems[i][bItemPrice]) {
                if(playerVariables[playerid][pPhoneBook] != 1)
                    return SendClientMessage(playerid, COLOR_GREY, "You are unable to purchase another phonebook.");

                playerVariables[playerid][pMoney] -= businessItems[i][bItemPrice];
                businessVariables[businessID][bVault] += businessItems[i][bItemPrice];
                playerVariables[playerid][pPhoneBook] = 1;

                SendClientMessage(playerid, COLOR_WHITE, "You have purchased a phonebook. Use /number to trace a number down!");
                return 1;
            } else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
        }
        case 4: {
            if(playerVariables[playerid][pPhoneNumber] == -1)
                return SendClientMessage(playerid, COLOR_GREY, "You do not have a phone.");

            if(playerVariables[playerid][pMoney] >= businessItems[i][bItemPrice]) {
                playerVariables[playerid][pMoney] -= businessItems[i][bItemPrice];
                businessVariables[businessID][bVault] += businessItems[i][bItemPrice];
                playerVariables[playerid][pPhoneCredit] += businessItems[i][bItemPrice]*60;

                format(szMessage, sizeof(szMessage), "You have purchased a $%d credit voucher for your mobile phone which has been automatically applied.", businessItems[i][bItemPrice]);
                SendClientMessage(playerid, COLOR_WHITE, szMessage);
                return 1;
            } else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
        }
        case 5: {
            if(playerVariables[playerid][pPhoneNumber] != -1)
                SendClientMessage(playerid, COLOR_GREY, "You already had a phone, your phone will be replaced and your number will be changed.");

            if(playerVariables[playerid][pMoney] >= businessItems[i][bItemPrice]) {
                playerVariables[playerid][pMoney] -= businessItems[i][bItemPrice];
                businessVariables[businessID][bVault] += businessItems[i][bItemPrice];
                playerVariables[playerid][pPhoneNumber] = random(89999999)+10000000; // Random eight digit phone number (which won't get crazy ones like 0, etc)

                format(szMessage, sizeof(szMessage), "You have purchased a %s! Your number is %d.", businessItems[i][bItemName], playerVariables[playerid][pPhoneNumber]);
                SendClientMessage(playerid, COLOR_WHITE, szMessage);
                return 1;
            } else return SendClientMessage(playerid, COLOR_GREY, "You don't have enough money to purchase this product.");
        }
    }
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

CMD:businessname(playerid, params[]) {
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

CMD:bspawnpos(playerid, params[]) {

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


CMD:lockbusiness(playerid, params[]) {
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

CMD:buybusiness(playerid, params[]) {
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