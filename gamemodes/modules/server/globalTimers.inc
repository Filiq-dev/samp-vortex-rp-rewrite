ptask oneSecound[1000](playerid) {
    if(adTick >= 1)
		adTick--;
        
    updateWorldTime(playerid);

    playerVariables[playerid][pConnectedSeconds] += 1;  

    if(playerVariables[playerid][pTabbed] == 0 && IsValidDynamic3DTextLabel(playerVariables[playerid][pAFKLabel]))
        DestroyDynamic3DTextLabel(playerVariables[playerid][pAFKLabel]); 

    if(playerVariables[playerid][pConnectedSeconds] < gettime()-1 && playerVariables[playerid][pTabbed] != 1 && playerVariables[playerid][pConnectedSeconds] >= 5 && GetPlayerState(playerid) != 9 && GetPlayerState(playerid) != 0 && GetPlayerState(playerid) != 7) {
        playerVariables[playerid][pTabbed] = 1;
        playerVariables[playerid][pAFKLabel] = CreateDynamic3DTextLabel("Paused.", COLOR_RED, 0, 0, 0, 7.5, playerid, _, 1, _, _, _, 7.5);
    }

    if(playerVariables[playerid][pStatus] == 1) {
        playerVariables[playerid][pSeconds]++; 

        if(!Iter_Contains(Admin, playerid)) {
            if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK && playerVariables[playerid][pJetpack] == 0 && playerVariables[playerid][pAdminLevel] == 0)
			{
				scriptBan(playerid, "Hacking (jetpack)");
			}
			else if(playerVariables[playerid][pEvent] == 0 && playerVariables[playerid][pAnticheatExemption] == 0)
			{
				new temp = GetPlayerWeapon(playerid);
				if(temp > 0 && GetPlayerState(playerid) == 1)
				{
					if(playerVariables[playerid][pWeapons][GetWeaponSlot(temp)] != temp)
						hackerTrigger(playerid);
				} 
			}
        }

        if(playerVariables[playerid][pAnticheatExemption] >= 1) {
            playerVariables[playerid][pAnticheatExemption]--;
        }

        if(playerVariables[playerid][pMuted] >= 1) {
            playerVariables[playerid][pMuted]--; // We don't need two variables for muting - just use -1 to permamute (admin mute) and a positive var for a temp mute.

            if(playerVariables[playerid][pMuted] == 0) {
                SendClientMessage(playerid, COLOR_GREY, "You have now been automatically unmuted.");
            }
        }

        if(playerVariables[playerid][pSpamCount] >= 1)
            playerVariables[playerid][pSpamCount]--;

        if(playerVariables[playerid][pSpamCount] >= 5 && playerVariables[playerid][pAdminLevel] == 0) {
            playerVariables[playerid][pMuted] = 10;
            playerVariables[playerid][pSpamCount] = 0;
            SendClientMessage(playerid, 0xCECECEFF, "You have been auto-muted for spamming. You will be unmuted in 10 seconds.");
        }

        if(playerVariables[playerid][pPhoneCall] != -1) {

            playerVariables[playerid][pPhoneCredit]--;

            if(playerVariables[playerid][pPhoneCredit] == 60) 
                SendClientMessage(playerid, COLOR_HOTORANGE, "You're almost out of credit, you have 60 seconds left.");
            else if(playerVariables[playerid][pPhoneCredit] < 1) {
                SendClientMessage(playerid, COLOR_WHITE, "Your phone has ran out of credit, visit a 24/7 to buy a top up voucher.");

                if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USECELLPHONE) 
                    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);

                if(playerVariables[playerid][pPhoneCall] != -1 && playerVariables[playerid][pPhoneCall] < MAX_PLAYERS) {
                    if(GetPlayerSpecialAction(playerVariables[playerid][pPhoneCall]) == SPECIAL_ACTION_USECELLPHONE)
                        SetPlayerSpecialAction(playerVariables[playerid][pPhoneCall], SPECIAL_ACTION_STOPUSECELLPHONE);

                    SendClientMessage(playerVariables[playerid][pPhoneCall], COLOR_WHITE, "Your call has been terminated by the other party (ran out of credit).");
                    
                    playerVariables[playerVariables[playerid][pPhoneCall]][pPhoneCall] = -1;
                }
                playerVariables[playerid][pPhoneCall] = -1;
            }
        } 

        if(playerVariables[playerid][pSpectating] != INVALID_PLAYER_ID) { // OnPlayerInteriorChange doesn't work properly when spectating.
            if(GetPlayerInterior(playerid) != GetPlayerInterior(playerVariables[playerid][pSpectating]))
                SetPlayerInterior(playerid, GetPlayerInterior(playerVariables[playerid][pSpectating]));

            if(GetPlayerVirtualWorld(playerid) != GetPlayerVirtualWorld(playerVariables[playerid][pSpectating]))
                SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(playerVariables[playerid][pSpectating]));
        }

        if(playerVariables[playerid][pBackup] != -1) {
            if(IsPlayerAuthed(playerVariables[playerid][pBackup])) {
                GetPlayerPos(playerVariables[playerid][pBackup], playerVariables[playerVariables[playerid][pBackup]][pPos][0], playerVariables[playerVariables[playerid][pBackup]][pPos][1], playerVariables[playerVariables[playerid][pBackup]][pPos][2]);
                SetPlayerCheckpoint(playerid, playerVariables[playerVariables[playerid][pBackup]][pPos][0], playerVariables[playerVariables[playerid][pBackup]][pPos][1], playerVariables[playerVariables[playerid][pBackup]][pPos][2], 10.0);
            }
            else {
                playerVariables[playerid][pBackup] = -1;
                playerVariables[playerid][pCheckpoint] = 0;

                SendClientMessage(playerid, COLOR_GREY, "The player requesting for backup has disconnected.");
                DisablePlayerCheckpoint(playerid);
            }
        }

        if(playerVariables[playerid][pDrag] != -1) { // Considering how slow SetPlayerPos works in practice, using a 1000ms timer in lieu of OnPlayerUpdate (the old script) is a better idea.
            if(IsPlayerAuthed(playerVariables[playerid][pDrag])) {
                switch(GetPlayerState(playerVariables[playerid][pDrag])) { // If they're not on foot, they're not gonna be dragging anything...
                    case 1: { // on foot
                        GetPlayerPos(playerVariables[playerid][pDrag], playerVariables[playerid][pPos][0], playerVariables[playerid][pPos][1], playerVariables[playerid][pPos][2]);
                        SetPlayerPos(playerid, playerVariables[playerid][pPos][0], playerVariables[playerid][pPos][1], playerVariables[playerid][pPos][2]);

                        SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(playerVariables[playerid][pDrag]));
                        SetPlayerInterior(playerid, GetPlayerInterior(playerVariables[playerid][pDrag]));
                    }
                    case 2, 3: {
                        SendClientMessage(playerVariables[playerid][pDrag], COLOR_GREY, "You can't enter a vehicle while dragging someone (use /detain).");
                        RemovePlayerFromVehicle(playerVariables[playerid][pDrag]);
                    }
                    case 7: { // Death
                        SendClientMessage(playerid, COLOR_WHITE, "The person who was dragging you has been wasted.");
                        playerVariables[playerid][pDrag] = -1;
                    }
                }
            }
            else {
                SendClientMessage(playerid, COLOR_WHITE, "The person who was dragging you has disconnected.");
                playerVariables[playerid][pDrag] = -1; // Kills off any disconnections.
            }
        }

        if(playerVariables[playerid][pMatrunTime] >= 1)
            playerVariables[playerid][pMatrunTime]++;

        if(playerVariables[playerid][pJobDelay] >= 1) {
            playerVariables[playerid][pJobDelay]--;
            if(playerVariables[playerid][pJobDelay] == 0) SendClientMessage(playerid, COLOR_WHITE, "Your job reload time is over.");
        }

        if(playerVariables[playerid][pNewbieTimeout] > 0) {
            playerVariables[playerid][pNewbieTimeout]--;
            if(playerVariables[playerid][pNewbieTimeout] == 0) SendClientMessage(playerid, COLOR_WHITE, "You may now speak in the newbie chat channel again.");
        }

        if(playerVariables[playerid][pHackWarnings] >= 1) {
            playerVariables[playerid][pHackWarnTime]++;

            if(playerVariables[playerid][pHackWarnTime] >= 10) {
                playerVariables[playerid][pHackWarnings] = 0;
                playerVariables[playerid][pHackWarnTime] = 0;
            }
        }

        if(playerVariables[playerid][pDropCarTimeout] >= 1) {
            playerVariables[playerid][pDropCarTimeout]--;
            if(playerVariables[playerid][pDropCarTimeout] == 1) {
                playerVariables[playerid][pDropCarTimeout] = 0;
                SendClientMessage(playerid, COLOR_WHITE, "You can now drop vehicles again at the crane.");
            }
        }

        if(playerVariables[playerid][pHospitalized] >= 2) {
            playerVariables[playerid][pHospitalized]++;
            GetPlayerHealth(playerid, playerVariables[playerid][pHealth]);
            SetPlayerHealth(playerid, playerVariables[playerid][pHealth]+7.5);

            if(playerVariables[playerid][pHealth]+10 >= 100) {
                SetPlayerHealth(playerid, 100);
                validResetPlayerWeapons(playerid);
                DeletePVar(playerid, "hosp");

                playerVariables[playerid][pHospitalized] = 0;
                playerVariables[playerid][pHealth] = 100;
                playerVariables[playerid][pArmour] = 0;
                playerVariables[playerid][pVirtualWorld] = 0;
                playerVariables[playerid][pInterior] = 0;
                playerVariables[playerid][pMoney] -= 1000;

                SendClientMessage(playerid, COLOR_LIGHTRED, "You have been released from Hospital.");
                SendClientMessage(playerid, COLOR_LIGHTRED, "You have been charged $1,000 for your stay, and any weapons you had have been confiscated.");
                
                switch(GetPVarInt(playerid, "hosp")) {
                    case 1: {
                        playerVariables[playerid][pPos][0] = 1172.359985;
                        playerVariables[playerid][pPos][1] = -1323.313110;
                        playerVariables[playerid][pPos][2] = 15.402919;
                    }
                    case 2: {
                        playerVariables[playerid][pPos][0] = 2034.196166;
                        playerVariables[playerid][pPos][1] = -1402.591430;
                        playerVariables[playerid][pPos][2] = 17.295030;
                    }
                }
                SetSpawnInfo(playerid, 0, playerVariables[playerid][pSkin], playerVariables[playerid][pPos][0], playerVariables[playerid][pPos][1], playerVariables[playerid][pPos][2], 0, 0, 0, 0, 0, 0, 0);
                SpawnPlayer(playerid);
            }
        }

        if(playerVariables[playerid][pSkinSet] >= 1) {
            playerVariables[playerid][pSkinSet]++;
            if(playerVariables[playerid][pSkinSet] == 3 && GetPlayerSkin(playerid) != playerVariables[playerid][pSkin]) {
                SetPlayerSkin(playerid, playerVariables[playerid][pSkin]); // Set the skin first.
            }
            if(playerVariables[playerid][pSkinSet] == 4) {
                givePlayerWeapons(playerid); // Then give the player their weapons. Seems like a SA-MP bug? Pain in the arse might I add!
                playerVariables[playerid][pSkinSet] = 0;
                TogglePlayerControllable(playerid, true);
            }
        }

        if(playerVariables[playerid][pFreezeTime] != 0) {
            TogglePlayerControllable(playerid, false);
            // if(playerVariables[playerid][pFreezeType] == 5)
                // ApplyAnimation(playerid, "FAT", "IDLE_TIRED", 4.1, 1, 1, 1, 1, 0, 1);
                
            if(playerVariables[playerid][pFreezeTime] > 0) {
                playerVariables[playerid][pFreezeTime]--;
                if(playerVariables[playerid][pFreezeTime] == 0) {
                    if(playerVariables[playerid][pFreezeType] == 5) {
                        SetPlayerDrunkLevel(playerid, 0);
                        ClearAnimations(playerid);
                    }
                    playerVariables[playerid][pFreezeType] = 0;
                    TogglePlayerControllable(playerid, true);
                }
            }

        }

        if(playerVariables[playerid][pPrisonID] > 0) {
            playerVariables[playerid][pPrisonTime]--;

            switch(playerVariables[playerid][pPrisonID]) {
                case 1: {
                    va_GameTextForPlayer(playerid, "~n~~n~~n~~n~~n~~n~~n~ ~r~Prisoned!~n~~w~%d seconds (%d minutes) left", 2000, 3, playerVariables[playerid][pPrisonTime], playerVariables[playerid][pPrisonTime]/60);
                }
                case 2, 3: { // We're going to be using 3 for IC jail, so... yeah
                    va_GameTextForPlayer(playerid, "~n~~n~~n~~n~~n~~n~~n~ ~r~Jailed!~n~~w~%d seconds (%d minutes) left", 2000, 3, playerVariables[playerid][pPrisonTime], playerVariables[playerid][pPrisonTime]/60);
                }
            }

            if(playerVariables[playerid][pPrisonTime] == 1 && playerVariables[playerid][pPrisonID] >= 1) {
                playerVariables[playerid][pPrisonID] = 0;
                playerVariables[playerid][pPrisonTime] = 0;

                SendClientMessage(playerid, COLOR_WHITE, "Your time is up! You have been released from jail/prison.");
                SetPlayerPos(playerid, 738.9963, -1417.2211, 13.5234);
                SetPlayerVirtualWorld(playerid, 0);
                SetPlayerInterior(playerid, 0);
            }
        } 

        if(playerVariables[playerid][pSeconds] >= 3600) {

            playerVariables[playerid][pSeconds] = 0;
            playerVariables[playerid][pPlayingHours]++;

            new
                BankInterest = playerVariables[playerid][pBankMoney] / 1000,
                RandPay = (random(495) + 5) * (playerVariables[playerid][pPlayingHours]/60) + random(5000) + 500,
                TotalPay = BankInterest + RandPay;

            if(playerVariables[playerid][pBankMoney]+playerVariables[playerid][pMoney] > -5000000) {
                SendClientMessage(playerid, COLOR_TEAL, "----------------------------------------------------------------------------");
                SendClientMessage(playerid, COLOR_WHITE, "Your paycheck has arrived; please visit the bank to withdraw your money.");
                playerVariables[playerid][pBankMoney] += TotalPay; 
                new taxamount = ((TotalPay/100) * assetVariables[1][aAssetValue]);
                if(taxamount > 1) {
                    playerVariables[playerid][pBankMoney] -= taxamount;
                    groupVariables[GOVERNMENT_GROUP_ID][gSafe][0] += taxamount;
                    saveGroup(GOVERNMENT_GROUP_ID);
                }
                va_SendClientMessage(playerid, COLOR_GREY, "Paycheck: $%d | Bank balance: $%d | Bank interest: $%d | Tax: $%d (0 percent) | Total earnings: $%d", RandPay, playerVariables[playerid][pBankMoney], BankInterest, taxamount, TotalPay-taxamount);
                SendClientMessage(playerid, COLOR_TEAL, "----------------------------------------------------------------------------");

                savePlayerData(playerid);
            }
            else {
                SendClientMessage(playerid, COLOR_WHITE, "You're too poor to obtain a paycheck.");
            }
        }

        if(GetPlayerMoney(playerid) != playerVariables[playerid][pMoney]) {
            ResetPlayerMoney(playerid);
            GivePlayerMoney(playerid, playerVariables[playerid][pMoney]);
        }
    }
}