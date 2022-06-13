#include <YSI_Coding/y_hooks>

hook OnPlayerConnect(playerid) {
    #if defined DEBUG
        printf("hook OnPlayerConnect(%d) has been called", playerid);
    #endif 

    // reset player data
	static const empty_player[playervEnum];
	playerVariables[playerid] = empty_player;
 
    GetPlayerIp(playerid, playerVariables[playerid][pConnectionIP], 16);  

    inline checkBanPlayer() {
        #if defined DEBUG
            printf("inline checkBanPlayer(%d) has been called", playerid);
        #endif
         
        if(cache_num_rows() >= 1) {
            SendClientMessage(playerid, COLOR_RED, "You're banned from this server.");
            Kick(playerid);
        } else {   
 
            gQuery[0] = (EOS);
            mysql_format(gSQL, gQuery, sizeof(gQuery), "SELECT * FROM `playeraccounts` WHERE `playerName` = '%e' LIMIT 1", getName(playerid));
            mysql_tquery(gSQL, gQuery, "checkAccountUsername", "d", playerid); 
        }
    }

    // Query if the player is banned or not, then continue with other auth code after the thread goes through 
    MySQL_TQueryInline(gSQL, using inline checkBanPlayer, "SELECT `banID` FROM `bans` WHERE `IPBanned` = '%s'", playerVariables[playerid][pConnectionIP]);
    
    return true;
} 

hook OnPlayerDisconnect(playerid, reason) {
    if(playerVariables[playerid][pStatus] == 1) {
        savePlayerData(playerid);
    }
}

function checkAccountUsername(playerid) {
    #if defined DEBUG
        printf("function checkAccountUsername(%d) has been called", playerid);
    #endif

    if(cache_num_rows() == 0) { 
        /*
            if somebody wants to have roleplay names can enable this
            new
                charCounts[5];

            GetPlayerName(playerid, getName(playerid), MAX_PLAYER_NAME);

            for(new n; n < MAX_PLAYER_NAME; n++) {
                switch(getName(playerid)[n]) {
                    case '[', ']', '.', '$', '(', ')', '@', '=': charCounts[1]++;
                    case '_': charCounts[0]++;
                    case '0' .. '9': charCounts[2]++;
                    case 'a' .. 'z': charCounts[3]++;
                    case 'A' .. 'Z': charCounts[4]++;
                }
            }
            if(charCounts[0] == 0 || charCounts[0] >= 3) {
                SendClientMessage(playerid, COLOR_GREY, "Your name is not valid. {FFFFFF}Please use an underscore and a first/last name (i.e. Mark_Edwards).");
                invalidNameChange(playerid);
            }
            else if(charCounts[1] >= 1) {
                SendClientMessage(playerid, COLOR_GREY, "Your name is not valid, as it contains symbols. {FFFFFF}Please use a roleplay name.");
                invalidNameChange(playerid);
            }
            else if(charCounts[2] >= 1) {
                SendClientMessage(playerid, COLOR_GREY, "Your name is not valid, as it contains numbers. {FFFFFF}Please use a roleplay name.");
                invalidNameChange(playerid);
            }
            else if(charCounts[3] == strlen(getName(playerid)) - 1) {
                SendClientMessage(playerid, COLOR_GREY, "Your name is not valid, as it is lower case. {FFFFFF}Please use a roleplay name (i.e. Dave_Meniketti).");
                invalidNameChange(playerid);
            }
            else if(charCounts[4] == strlen(getName(playerid)) - 1) {
                SendClientMessage(playerid, COLOR_GREY, "Your name is not valid, as it is upper case. {FFFFFF}Please use a roleplay name (i.e. Dave_Jones).");
                invalidNameChange(playerid);
            }
            else {
        
        */
        SendClientMessage(playerid, COLOR_GENANNOUNCE, "SERVER: {FFFFFF}Welcome to "SERVER_NAME".");
        SendClientMessage(playerid, COLOR_GENANNOUNCE, "SERVER: {FFFFFF}You aren't registered yet. Please enter your desired password in the dialog box to register.");
 
        Dialog_Show(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "SERVER: Registration", "Welcome to the "SERVER_NAME" Server.\n\nPlease enter your desired password below!", "Register", "Cancel");
        // }
    }
    else { 
        cache_get_value_name(0, "playerPassword", playerVariables[playerid][pPassword], 129); 

        playerVariables[playerid][Cache_ID] = cache_save();

        SendClientMessage(playerid, COLOR_GENANNOUNCE, "SERVER: {FFFFFF}Welcome to "SERVER_NAME".");
        SendClientMessage(playerid, COLOR_GENANNOUNCE, "SERVER: {FFFFFF}You already have a registered account, please enter your password into the dialog box.");

        Dialog_Show(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "SERVER: Login", "Welcome to the "SERVER_NAME" Server.\n\nPlease enter your password below!", "Login", "Cancel");
    }

    return true;
}

function finishLogin(playerid) {
    #if defined DEBUG
        printf("function finishLogin(%d) has been called", playerid);
    #endif 

    for(new i = 0; i < 25; i++)
        SendClientMessage(playerid, COLOR_WHITE, " ");

    DeletePVar(playerid, "LA");

    playerVariables[playerid][pStatus] = 1;

    adminSecurity(playerid);

    inline timestampConnect() {
        #if defined DEBUG
            printf("inline timestampConnect(%d) has been called", playerid);
        #endif 
        if(cache_num_rows() == 0)
            return SendClientMessage(playerid, COLOR_GENANNOUNCE, "SERVER:"EMBED_WHITE" Welcome to the server!");

        new timestamp[50];
        cache_get_value(0, "conTS", timestamp); 

        va_SendClientMessage(playerid, COLOR_GENANNOUNCE, "SERVER:"EMBED_WHITE" Welcome back %s, you last visited us on %s.", getName(playerid), timestamp);
        
        if(playerVariables[playerid][pGroup] >= 1) {
            // gString[0] = (EOS);
            // format(gString, sizeof(gString), "(Group) "EMBED_WHITE"%s from your group has just logged in.", getName(playerid));
            // SendToGroup(playerVariables[playerid][pGroup], COLOR_GENANNOUNCE, gString);

            // gString[0] = (EOS);
            // format(gString, sizeof(gString), "(Group) MOTD: "EMBED_WHITE"%s", groupVariables[playerVariables[playerid][pGroup]][gGroupMOTD]);
            // SendClientMessage(playerid, COLOR_GENANNOUNCE, gString);
        }
    } 
    MySQL_TQueryInline(gSQL, using inline timestampConnect, "SELECT `conTS` FROM `playerconnections` WHERE `conPlayerID` = '%d' ORDER BY `conId` DESC LIMIT 1", playerVariables[playerid][pInternalID]);

    gQuery[0] = (EOS);
    mysql_format(gSQL, gQuery, sizeof(gQuery), "INSERT INTO playerconnections (conName, conIP, conPlayerID) VALUES('%s', '%s', %d)", playerVariables[playerid][pNormalName], playerVariables[playerid][pConnectionIP], playerVariables[playerid][pInternalID]);
    mysql_tquery(gSQL, gQuery, "", "");

    gQuery[0] = (EOS);
    mysql_format(gSQL, gQuery, sizeof(gQuery), "UPDATE playeraccounts SET playerStatus = '1' WHERE playerID = %d", playerVariables[playerid][pInternalID]);
    mysql_tquery(gSQL, gQuery, "", "");
    
    // gQuery[0] = (EOS);
    // mysql_format(gSQL, gQuery, sizeof(gQuery), "SELECT * FROM playervehicles WHERE pvOwnerId = %d", playerVariables[playerid][pInternalID]);
    // mysql_tquery(gSQL, gQuery, THREAD_LOAD_PLAYER_VEHICLES, playerid);  
        
    if(playerVariables[playerid][pFirstLogin] >= 1) {
        // Dialog to send player in to quiz and prevent any other code for the player from being executed, as they have to complete the quiz/tutorial first.
        return Dialog_Show(playerid, DIALOG_QUIZ, DIALOG_STYLE_LIST, "What is roleplay in SA-MP?", "A type of gamemode where you realistically act out a character\nAn STD\nA track by Jay-Z\nA type of gamemode where you just kill people", "Select", "");
    }

    SetSpawnInfo(playerid, 0, playerVariables[playerid][pSkin], playerVariables[playerid][pPos][0], playerVariables[playerid][pPos][1], playerVariables[playerid][pPos][2], 0, 0, 0, 0, 0, 0, 0);
    SpawnPlayer(playerid);

    if(playerVariables[playerid][pWarrants] > 0) {
        // SetPlayerWantedLevel(playerid, playerVariables[playerid][pWarrants]);
        SendClientMessage(playerid, COLOR_HOTORANGE, "You're still a wanted man! Your criminal record has been reinstated.");
    }

    // format(szQueryOutput, sizeof(szQueryOutput), "SELECT * FROM `banksuspensions` WHERE `playerID` = %d", playerVariables[playerid][pInternalID]);
    // mysql_query(szQueryOutput, THREAD_BANK_SUSPENSION, playerid); de facut asta cand fac sistemul de banca

    //     if(playerVariables[playerid][pCarModel] > 0)
    //         SpawnPlayerVehicle(playerid);
 
    SetPlayerScore(playerid, playerVariables[playerid][pLevel] == 0 ? 1 : playerVariables[playerid][pLevel]); // if the level is 0 then set level 1 or level from database

    return true;
}  

Dialog:DIALOG_REGISTER(const playerid, const response, const listitem, inputtext[]) {
    if(strlen(inputtext) < 1)
        return Dialog_Show(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "SERVER: Registration", "Your password must exceed 1 character!\n\nWelcome to the "SERVER_NAME" Server.\n\nPlease enter your desired password below!", "Register", "Cancel");

    new wpHash[129];  
    WP_Hash(wpHash, sizeof(wpHash), inputtext);

    inline setUserID() {
        #if defined DEBUG
            printf("inline setUserID(%d) has been called", playerid);
        #endif 
        playerVariables[playerid][pInternalID] = cache_insert_id();
    } 
    MySQL_TQueryInline(gSQL, using inline setUserID, "INSERT INTO playeraccounts (playerName, playerPassword) VALUES('%e', '%s')", getName(playerid), wpHash);

    SendClientMessage(playerid, COLOR_WHITE, "SERVER: Your account is now registered!"); 
    submitToAdmins(COLOR_HOTORANGE, "AdmWarn: {FFFFFF}%s just registered.", getName(playerid));

    finishLogin(playerid); 

    return true;
}

Dialog:DIALOG_LOGIN(playerid, response, listitem, inputtext[]) {
    if(!response) return Kick(playerid); 

    new wpHash[129];  
    WP_Hash(wpHash, sizeof(wpHash), inputtext); 

    if(strcmp(wpHash, playerVariables[playerid][pPassword]) == 0) {
        Dialog_Close(playerid);

        cache_set_active(playerVariables[playerid][Cache_ID]);
        getPlayerData(playerid);
        cache_delete(playerVariables[playerid][Cache_ID]);
        // playerVariables[playerid][Cache_ID] = MYSQL_INVALID_CACHE; maybe we need this in the future

        finishLogin(playerid); 
    } else {
        SetPVarInt(playerid, "LA", GetPVarInt(playerid, "LA") + 1); 

        if(GetPVarInt(playerid, "LA") > MAX_LOGIN_ATTEMPTS) {
            SendClientMessage(playerid, COLOR_RED, "You have used all available login attempts.");  
            submitToAdmins(COLOR_HOTORANGE, "AdmWarn: {FFFFFF}IP %s has been banned (%d failed 3 attempts on account %s).", playerVariables[playerid][pConnectionIP], MAX_LOGIN_ATTEMPTS, getName(playerid));

            // #if defined IPBan 
            //     IPBan(playerVariables[playerid][pConnectionIP], "Exceeded maximum login attempts."); activate after make bans
            // #endif
            Kick(playerid);

        } else {
            Dialog_Show(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "SERVER: Login", "Welcome to the "SERVER_NAME" Server.\n\nPlease enter your password below!", "Login", "Cancel");

            va_SendClientMessage(playerid, COLOR_HOTORANGE, "Incorrect password. You have %d remaining login attempts left.", MAX_LOGIN_ATTEMPTS - GetPVarInt(playerid, "LA"));
        }
    }

    return true;
} 

function getPlayerData(playerid) { 
    #if defined DEBUG
        printf("function getPlayerData(%d) has been called", playerid);
    #endif 

    new is_banned;

    cache_get_value_int(0, "playerBanned", is_banned);

    if(is_banned) { 

        SendClientMessage(playerid, COLOR_RED, "You are banned from this server.");  
        submitToAdmins(COLOR_HOTORANGE, "AdmWarn: {FFFFFF}%s has attempted to evade their account ban (using IP %s).", getName(playerid), playerVariables[playerid][pConnectionIP]);

        // format(gString, sizeof(gString), "Attempted ban evasion (%s).", getName(playerid)); 
        // IPBan(playerIP, gString); comeback

        Kick(playerid);
    }

    playerVariables[playerid][pBanned] = is_banned;

    cache_get_value(0, "playerPassword", playerVariables[playerid][pPassword]);   
    cache_get_value(0, "playerEmail", playerVariables[playerid][pEmail]);
    cache_get_value(0, "playerAdminName", playerVariables[playerid][pAdminName]);
    cache_get_value(0, "playerAccent", playerVariables[playerid][pAccent]);
    cache_get_value(0, "playerName", playerVariables[playerid][pNormalName]);

    cache_get_value_int(0, "playerSkin", playerVariables[playerid][pSkin]);
    cache_get_value_int(0, "playerMoney", playerVariables[playerid][pMoney]);
    cache_get_value_int(0, "playerBankMoney", playerVariables[playerid][pBankMoney]);
    cache_get_value_int(0, "playerVIP", playerVariables[playerid][pVIP]);
    cache_get_value_int(0, "playerSeconds", playerVariables[playerid][pSeconds]);
    cache_get_value_int(0, "playerAdminLevel", playerVariables[playerid][pAdminLevel]); 
    if(playerVariables[playerid][pAdminLevel] > 0) {
        new pinadmin;
        cache_get_value_int(0, "playerAdminPIN", pinadmin);
        SetPVarInt(playerid, "pAdminPIN", pinadmin);

        Iter_Add(Admin, playerid);
    }

    cache_get_value_float(0, "playerPosX", playerVariables[playerid][pPos][0]);
    cache_get_value_float(0, "playerPosY", playerVariables[playerid][pPos][1]);
    cache_get_value_float(0, "playerPosZ", playerVariables[playerid][pPos][2]);
    cache_get_value_float(0, "playerHealth", playerVariables[playerid][pHealth]);
    cache_get_value_float(0, "playerArmour", playerVariables[playerid][pArmour]);

    cache_get_value_float(0, "playerCarPosX", playerVariables[playerid][pCarPos][0]);
    cache_get_value_float(0, "playerCarPosY", playerVariables[playerid][pCarPos][1]);
    cache_get_value_float(0, "playerCarPosZ", playerVariables[playerid][pCarPos][2]);
    cache_get_value_float(0, "playerCarPosZAngle", playerVariables[playerid][pCarPos][3]);
    
    cache_get_value_int(0, "playerInterior", playerVariables[playerid][pInterior]);
    cache_get_value_int(0, "playerVirtualWorld", playerVariables[playerid][pVirtualWorld]); 
    cache_get_value_int(0, "playerID", playerVariables[playerid][pInternalID]);
    cache_get_value_int(0, "playerCarLicensePlate", playerVariables[playerid][pCarLicensePlate]);
    cache_get_value_int(0, "playerJob", playerVariables[playerid][pJob]);
    cache_get_value_int(0, "playerWeapon0", playerVariables[playerid][pWeapons][0]);
    cache_get_value_int(0, "playerWeapon1", playerVariables[playerid][pWeapons][1]);
    cache_get_value_int(0, "playerWeapon2", playerVariables[playerid][pWeapons][2]);
    cache_get_value_int(0, "playerWeapon3", playerVariables[playerid][pWeapons][3]);
    cache_get_value_int(0, "playerWeapon4", playerVariables[playerid][pWeapons][4]);
    cache_get_value_int(0, "playerWeapon5", playerVariables[playerid][pWeapons][5]);
    cache_get_value_int(0, "playerWeapon6", playerVariables[playerid][pWeapons][6]);
    cache_get_value_int(0, "playerWeapon7", playerVariables[playerid][pWeapons][7]);
    cache_get_value_int(0, "playerWeapon8", playerVariables[playerid][pWeapons][8]);
    cache_get_value_int(0, "playerWeapon9", playerVariables[playerid][pWeapons][9]);
    cache_get_value_int(0, "playerWeapon10", playerVariables[playerid][pWeapons][10]);
    cache_get_value_int(0, "playerWeapon11", playerVariables[playerid][pWeapons][11]);
    cache_get_value_int(0, "playerWeapon12", playerVariables[playerid][pWeapons][12]);
    cache_get_value_int(0, "playerJobSkill1", playerVariables[playerid][pJobSkill][0]);
    cache_get_value_int(0, "playerJobSkill2", playerVariables[playerid][pJobSkill][1]);
    cache_get_value_int(0, "playerMaterials", playerVariables[playerid][pMaterials]);
    cache_get_value_int(0, "playerGroup", playerVariables[playerid][pGroup]);
    cache_get_value_int(0, "playerGroupRank", playerVariables[playerid][pGroupRank]);
    cache_get_value_int(0, "playerHours", playerVariables[playerid][pPlayingHours]);
    cache_get_value_int(0, "playerWarning1", playerVariables[playerid][pWarning1]);
    cache_get_value_int(0, "playerWarning2", playerVariables[playerid][pWarning2]);
    cache_get_value_int(0, "playerWarning3", playerVariables[playerid][pWarning3]);
    cache_get_value_int(0, "playerHospitalized", playerVariables[playerid][pHospitalized]);
    cache_get_value_int(0, "playerFirstLogin", playerVariables[playerid][pFirstLogin]);
    cache_get_value_int(0, "playerGender", playerVariables[playerid][pGender]);
    cache_get_value_int(0, "playerPrisonID", playerVariables[playerid][pPrisonID]);
    cache_get_value_int(0, "playerPrisonTime", playerVariables[playerid][pPrisonTime]);
    cache_get_value_int(0, "playerPhoneNumber", playerVariables[playerid][pPhoneNumber]);
    cache_get_value_int(0, "playerPhoneBook", playerVariables[playerid][pPhoneBook]);
    cache_get_value_int(0, "playerHelperLevel", playerVariables[playerid][pHelper]);
    cache_get_value_int(0, "playerDropCarTimeout", playerVariables[playerid][pDropCarTimeout]);
    cache_get_value_int(0, "playerRope", playerVariables[playerid][pRope]);
    cache_get_value_int(0, "playerAdminDuty", playerVariables[playerid][pAdminDuty]);
    cache_get_value_int(0, "playerCrimes", playerVariables[playerid][pCrimes]);
    cache_get_value_int(0, "playerArrests", playerVariables[playerid][pArrests]);
    cache_get_value_int(0, "playerWarrants", playerVariables[playerid][pWarrants]);
    cache_get_value_int(0, "playerLevel", playerVariables[playerid][pLevel]);
    cache_get_value_int(0, "playerAge", playerVariables[playerid][pAge]);
    cache_get_value_int(0, "playerCarModel", playerVariables[playerid][pCarModel]);
    cache_get_value_int(0, "playerCarMod0", playerVariables[playerid][pCarMods][0]);
    cache_get_value_int(0, "playerCarMod1", playerVariables[playerid][pCarMods][1]);
    cache_get_value_int(0, "playerCarMod2", playerVariables[playerid][pCarMods][2]);
    cache_get_value_int(0, "playerCarMod3", playerVariables[playerid][pCarMods][3]);
    cache_get_value_int(0, "playerCarMod4", playerVariables[playerid][pCarMods][4]);
    cache_get_value_int(0, "playerCarMod5", playerVariables[playerid][pCarMods][5]);
    cache_get_value_int(0, "playerCarMod6", playerVariables[playerid][pCarMods][6]);
    cache_get_value_int(0, "playerCarMod7", playerVariables[playerid][pCarMods][7]);
    cache_get_value_int(0, "playerCarMod8", playerVariables[playerid][pCarMods][8]);
    cache_get_value_int(0, "playerCarMod9", playerVariables[playerid][pCarMods][9]);
    cache_get_value_int(0, "playerCarMod10", playerVariables[playerid][pCarMods][10]);
    cache_get_value_int(0, "playerCarMod11", playerVariables[playerid][pCarMods][11]);
    cache_get_value_int(0, "playerCarMod12", playerVariables[playerid][pCarMods][12]);
    cache_get_value_int(0, "playerCarColour1", playerVariables[playerid][pCarColour][0]);
    cache_get_value_int(0, "playerCarColour2", playerVariables[playerid][pCarColour][1]);
    cache_get_value_int(0, "playerCarWeapon1", playerVariables[playerid][pCarWeapons][0]);
    cache_get_value_int(0, "playerCarWeapon2", playerVariables[playerid][pCarWeapons][1]);
    cache_get_value_int(0, "playerCarWeapon3", playerVariables[playerid][pCarWeapons][2]);
    cache_get_value_int(0, "playerCarWeapon4", playerVariables[playerid][pCarWeapons][3]);
    cache_get_value_int(0, "playerCarWeapon5", playerVariables[playerid][pCarWeapons][4]);
    cache_get_value_int(0, "playerCarTrunk1", playerVariables[playerid][pCarTrunk][0]);
    cache_get_value_int(0, "playerCarTrunk2", playerVariables[playerid][pCarTrunk][1]);
    cache_get_value_int(0, "playerPhoneCredit", playerVariables[playerid][pPhoneCredit]);
    cache_get_value_int(0, "playerCarPaintJob", playerVariables[playerid][pCarPaintjob]);
    cache_get_value_int(0, "playerCarLock", playerVariables[playerid][pCarLock]);
    cache_get_value_int(0, "playerFightStyle", playerVariables[playerid][pFightStyle]);
    cache_get_value_int(0, "playerWalkieTalkie", playerVariables[playerid][pWalkieTalkie]);  
}

function savePlayerData(playerid) {
    #if defined DEBUG
        printf("function savePlayerData(%d) has been called", playerid);
    #endif 
    
	if(playerVariables[playerid][pStatus] >= 1 || playerVariables[playerid][pStatus] == -1) {
		new saveQuery[3500];

		/* 
            if(playerVariables[playerid][pCarModel] >= 1 && doesVehicleExist(playerVariables[playerid][pCarID])) {
                GetVehiclePos(playerVariables[playerid][pCarID], playerVariables[playerid][pCarPos][0], playerVariables[playerid][pCarPos][1], playerVariables[playerid][pCarPos][2]);
                GetVehicleZAngle(playerVariables[playerid][pCarID], playerVariables[playerid][pCarPos][3]);

                for(new i = 0; i < 13; i++) {
                    playerVariables[playerid][pCarMods][i] = GetVehicleComponentInSlot(playerVariables[playerid][pCarID], i);
                }
            } 
        */

		if(playerVariables[playerid][pAdminDuty] == 0 && playerVariables[playerid][pEvent] == 0) {
			GetPlayerHealth(playerid, playerVariables[playerid][pHealth]);
			GetPlayerArmour(playerid, playerVariables[playerid][pArmour]);
		}

		// If they're not in an event and not spectating, current pos is saved. Otherwise, they'll be set back to the pos they last used /joinevent or /spec.
		if(playerVariables[playerid][pSpectating] == INVALID_PLAYER_ID && playerVariables[playerid][pEvent] == 0) {
			GetPlayerPos(playerid, playerVariables[playerid][pPos][0], playerVariables[playerid][pPos][1], playerVariables[playerid][pPos][2]);
			playerVariables[playerid][pInterior] = GetPlayerInterior(playerid);
			playerVariables[playerid][pVirtualWorld] = GetPlayerVirtualWorld(playerid); // If someone disconnects while spectating.
		}

		format(saveQuery, sizeof(saveQuery), "UPDATE playeraccounts SET playerBanned = '%d', playerSeconds = '%d', playerSkin = '%d', playerMoney = '%d', playerBankMoney = '%d'", playerVariables[playerid][pBanned], playerVariables[playerid][pSeconds], playerVariables[playerid][pSkin], playerVariables[playerid][pMoney], playerVariables[playerid][pBankMoney]);
		format(saveQuery, sizeof(saveQuery), "%s, playerInterior = '%d', playerVirtualWorld = '%d', playerHealth = '%f', playerArmour = '%f', playerPosX = '%f', playerPosY = '%f', playerPosZ = '%f'", saveQuery, playerVariables[playerid][pInterior], playerVariables[playerid][pVirtualWorld], playerVariables[playerid][pHealth], playerVariables[playerid][pArmour], playerVariables[playerid][pPos][0], playerVariables[playerid][pPos][1], playerVariables[playerid][pPos][2]);
		format(saveQuery, sizeof(saveQuery), "%s, playerAccent = '%s', playerAdminLevel = '%d', playerJob = '%d', playerWeapon0 = '%d', playerWeapon1 = '%d', playerWeapon2 = '%d', playerWeapon3 = '%d'", saveQuery, playerVariables[playerid][pAccent], playerVariables[playerid][pAdminLevel], playerVariables[playerid][pJob], playerVariables[playerid][pWeapons][0], playerVariables[playerid][pWeapons][1], playerVariables[playerid][pWeapons][2], playerVariables[playerid][pWeapons][3]);
		format(saveQuery, sizeof(saveQuery), "%s, playerWeapon4 = '%d', playerWeapon5 = '%d', playerWeapon6 = '%d', playerWeapon7 = '%d', playerWeapon8 = '%d', playerWeapon9 = '%d', playerWeapon10 = '%d'", saveQuery, playerVariables[playerid][pWeapons][4], playerVariables[playerid][pWeapons][5], playerVariables[playerid][pWeapons][6], playerVariables[playerid][pWeapons][7], playerVariables[playerid][pWeapons][8], playerVariables[playerid][pWeapons][9], playerVariables[playerid][pWeapons][10]);
		format(saveQuery, sizeof(saveQuery), "%s, playerWeapon11 = '%d', playerWeapon12 = '%d', playerJobSkill1 = '%d', playerJobSkill2 = '%d', playerMaterials = '%d', playerHours = '%d', playerLevel = '%d'", saveQuery, playerVariables[playerid][pWeapons][11], playerVariables[playerid][pWeapons][12], playerVariables[playerid][pJobSkill][0], playerVariables[playerid][pJobSkill][1], playerVariables[playerid][pMaterials], playerVariables[playerid][pPlayingHours], playerVariables[playerid][pLevel]);
		format(saveQuery, sizeof(saveQuery), "%s, playerWarning1 = '%d', playerWarning2 = '%d', playerWarning3 = '%d', playerHospitalized = '%d', playerFirstLogin = '%d', playerAdminName = '%s', playerPrisonTime = '%d', playerPrisonID = '%d', playerPhoneNumber = '%d'", saveQuery, playerVariables[playerid][pWarning1], playerVariables[playerid][pWarning2], playerVariables[playerid][pWarning3], playerVariables[playerid][pHospitalized], playerVariables[playerid][pFirstLogin], playerVariables[playerid][pAdminName],
		playerVariables[playerid][pPrisonTime], playerVariables[playerid][pPrisonID], playerVariables[playerid][pPhoneNumber]);
		format(saveQuery, sizeof(saveQuery), "%s, playerCarPaintJob = '%d', playerCarLock = '%d', playerStatus = '%d', playerGender = '%d', playerFightStyle = '%d', playerCarWeapon1 = '%d', playerCarWeapon2 = '%d', playerCarWeapon3 = '%d', playerCarWeapon4 = '%d', playerCarWeapon5 = '%d', playerCarLicensePlate = '%s'", saveQuery, playerVariables[playerid][pCarPaintjob], playerVariables[playerid][pCarLock],
		playerVariables[playerid][pStatus], playerVariables[playerid][pGender], playerVariables[playerid][pFightStyle], playerVariables[playerid][pCarWeapons][0], playerVariables[playerid][pCarWeapons][1], playerVariables[playerid][pCarWeapons][2], playerVariables[playerid][pCarWeapons][3], playerVariables[playerid][pCarWeapons][4], playerVariables[playerid][pCarLicensePlate]);
		format(saveQuery, sizeof(saveQuery), "%s, playerCarModel = '%d', playerCarColour1 = '%d', playerCarColour2 = '%d', playerCarPosX = '%f', playerCarPosY = '%f', playerCarPosZ = '%f', playerCarPosZAngle = '%f', playerCarMod0 = '%d', playerCarMod1 = '%d', playerCarMod2 = '%d', playerCarMod3 = '%d', playerCarMod4 = '%d', playerCarMod5 = '%d', playerCarMod6 = '%d'", saveQuery, playerVariables[playerid][pCarModel], playerVariables[playerid][pCarColour][0], playerVariables[playerid][pCarColour][1],
		playerVariables[playerid][pCarPos][0], playerVariables[playerid][pCarPos][1], playerVariables[playerid][pCarPos][2], playerVariables[playerid][pCarPos][3], playerVariables[playerid][pCarMods][0], playerVariables[playerid][pCarMods][1], playerVariables[playerid][pCarMods][2], playerVariables[playerid][pCarMods][3], playerVariables[playerid][pCarMods][4], playerVariables[playerid][pCarMods][5], playerVariables[playerid][pCarMods][6]);
		format(saveQuery, sizeof(saveQuery), "%s, playerCarTrunk1 = '%d', playerCarTrunk2 = '%d', playerPhoneCredit = '%d', playerWalkieTalkie = '%d'", saveQuery, playerVariables[playerid][pCarTrunk][0], playerVariables[playerid][pCarTrunk][1], playerVariables[playerid][pPhoneCredit], playerVariables[playerid][pWalkieTalkie]);
		format(saveQuery, sizeof(saveQuery), "%s, playerPhoneBook = '%d', playerGroup = '%d', playerGroupRank = '%d', playerIP = '%s', playerDropCarTimeout = '%d', playerRope = '%d', playerAdminDuty = '%d', playerCrimes = '%d', playerArrests = '%d', playerWarrants = '%d', playerAge = '%d', playerCarMod7 = '%d', playerCarMod8 = '%d', playerCarMod9 = '%d', playerCarMod10 = '%d', playerCarMod11 = '%d', playerCarMod12 = '%d'", saveQuery, playerVariables[playerid][pPhoneBook],
		playerVariables[playerid][pGroup], playerVariables[playerid][pGroupRank], playerVariables[playerid][pConnectionIP], playerVariables[playerid][pDropCarTimeout], playerVariables[playerid][pRope], playerVariables[playerid][pAdminDuty], playerVariables[playerid][pCrimes], playerVariables[playerid][pArrests], playerVariables[playerid][pWarrants], playerVariables[playerid][pAge], playerVariables[playerid][pCarMods][7], playerVariables[playerid][pCarMods][8],
		playerVariables[playerid][pCarMods][9], playerVariables[playerid][pCarMods][10], playerVariables[playerid][pCarMods][11], playerVariables[playerid][pCarMods][12]);

		if(playerVariables[playerid][pHelper] > 0)
		    format(saveQuery, sizeof(saveQuery), "%s, playerHelperLevel = %d", saveQuery, playerVariables[playerid][pHelper]);
		    
		if(playerVariables[playerid][pAdminLevel] > 0)
		    format(saveQuery, sizeof(saveQuery), "%s, playerAdminPIN = %d", saveQuery, GetPVarInt(playerid, "pAdminPIN"));
		    
		format(saveQuery, sizeof(saveQuery), "%s WHERE playerID = '%d'", saveQuery, playerVariables[playerid][pInternalID]);
		mysql_tquery(gSQL, saveQuery, "", "");
	}

	return true;
}