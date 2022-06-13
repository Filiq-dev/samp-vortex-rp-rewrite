#include <YSI_Coding/y_hooks>

new g_MysqlRaceCheck[MAX_PLAYERS];

hook OnPlayerConnect(playerid) {
    #if defined DEBUG
        printf("hook OnPlayerConnect(%d) has been called", playerid);
    #endif

    g_MysqlRaceCheck[playerid]++;

    // reset player data
	static const empty_player[playervEnum];
	playerVariables[playerid] = empty_player;
 
    GetPlayerIp(playerid, playerVariables[playerid][pConnectionIP], 16); 
    GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);  

    // // Query if the player is banned or not, then continue with other auth code after the thread goes through
    gQuery[0] = (EOS);
    mysql_format(gSQL, gQuery, sizeof(gQuery), "SELECT `banID` FROM `bans` WHERE `IPBanned` = '%s'", playerVariables[playerid][pConnectionIP]);
    mysql_tquery(gSQL, gQuery, "checkBanPlayer", "dd", playerid, g_MysqlRaceCheck[playerid]);  
    
    return true;
}

function checkBanPlayer(const playerid, race_check) {
    #if defined DEBUG
        printf("function checkBanPlayer(%d) has been called", playerid);
    #endif

    /*	race condition check:
		player A connects -> SELECT query is fired -> this query takes very long
		while the query is still processing, player A with playerid 2 disconnects
		player B joins now with playerid 2 -> our laggy SELECT query is finally finished, but for the wrong player

		what do we do against it?
		we create a connection count for each playerid and increase it everytime the playerid connects or disconnects
		we also pass the current value of the connection count to our OnPlayerDataLoaded callback
		then we check if current connection count is the same as connection count we passed to the callback
		if yes, everything is okay, if not, we just kick the player
	*/
	if (race_check != g_MysqlRaceCheck[playerid]) return Kick(playerid);

    // The query worked. We know there's no (serious) MySQL problems, so we won't display the error dialog.
    SetPVarInt(playerid, "bcs", 1); 

    if(cache_num_rows() >= 1) {
        SendClientMessage(playerid, COLOR_RED, "You're banned from this server.");
        Kick(playerid);
    } else {  
        GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);  

        // Continue with the rest of the auth code...
        gQuery[0] = (EOS);
        mysql_format(gSQL, gQuery, sizeof(gQuery), "SELECT * FROM `playeraccounts` WHERE `playerName` = '%e' LIMIT 1", szPlayerName);
        mysql_tquery(gSQL, gQuery, "checkAccountUsername", "dd", playerid, race_check); 
    }

    return true;
}

function checkAccountUsername(playerid, race_check) {
    #if defined DEBUG
        printf("function checkAccountUsername(%d) has been called", playerid);
    #endif

    /*	race condition check:
		player A connects -> SELECT query is fired -> this query takes very long
		while the query is still processing, player A with playerid 2 disconnects
		player B joins now with playerid 2 -> our laggy SELECT query is finally finished, but for the wrong player

		what do we do against it?
		we create a connection count for each playerid and increase it everytime the playerid connects or disconnects
		we also pass the current value of the connection count to our OnPlayerDataLoaded callback
		then we check if current connection count is the same as connection count we passed to the callback
		if yes, everything is okay, if not, we just kick the player
	*/
	if (race_check != g_MysqlRaceCheck[playerid]) return Kick(playerid);

    if(cache_num_rows() == 0) { 
        /*
            if somebody wants to have roleplay names can enable this
            new
                charCounts[5];

            GetPlayerName(playerid, szPlayerName, MAX_PLAYER_NAME);

            for(new n; n < MAX_PLAYER_NAME; n++) {
                switch(szPlayerName[n]) {
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
            else if(charCounts[3] == strlen(szPlayerName) - 1) {
                SendClientMessage(playerid, COLOR_GREY, "Your name is not valid, as it is lower case. {FFFFFF}Please use a roleplay name (i.e. Dave_Meniketti).");
                invalidNameChange(playerid);
            }
            else if(charCounts[4] == strlen(szPlayerName) - 1) {
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
        new test;
        cache_get_value_int(0, "playerID", test);
        cache_get_value_name(0, "playerPassword", playerVariables[playerid][pPassword], 129);

        printf("%d %s test", test, playerVariables[playerid][pPassword]);

        playerVariables[playerid][Cache_ID] = cache_save();

        SendClientMessage(playerid, COLOR_GENANNOUNCE, "SERVER: {FFFFFF}Welcome to "SERVER_NAME".");
        SendClientMessage(playerid, COLOR_GENANNOUNCE, "SERVER: {FFFFFF}You already have a registered account, please enter your password into the dialog box.");

        Dialog_Show(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "SERVER: Login", "Welcome to the "SERVER_NAME" Server.\n\nPlease enter your password below!", "Login", "Cancel");
    }

    return true;
}

function finishLogin(playerid) {
    #if defined DEBUG
        printf("function checkCredentials(%d) has been called", playerid);
    #endif 

    for(new i = 0; i < 10; i++)
        SendClientMessage(playerid, COLOR_WHITE, " ");

    DeletePVar(playerid, "LA");

    playerVariables[playerid][pStatus] = 1;

    if(playerVariables[playerid][pAdminLevel] > 0) {
        gQuery[0] = (EOS);
        mysql_format(gSQL, gQuery, sizeof(gQuery), "SELECT conIP from playerconnections WHERE conPlayerID = %d AND conIP = '%s'", playerVariables[playerid][pInternalID], playerVariables[playerid][pConnectionIP]);
        mysql_tquery(gSQL, gQuery, "adminSecurity", "d", playerid);
        
        if(GetPVarInt(playerid, "pAdminPIN") == 0)
            Dialog_Show(playerid, DIALOG_SET_ADMIN_PIN, DIALOG_STYLE_INPUT, "SERVER: Admin PIN creation", "The system has detected you do not yet have an admin PIN set.\n\nThis is a new compulsory security measure.\n\nPlease set a four digit pin:", "OK", "");
    }
    
    gQuery[0] = (EOS);
    mysql_format(gSQL, gQuery, sizeof(gQuery), "SELECT `conTS` FROM `playerconnections` WHERE `conPlayerID` = '%d' ORDER BY `conId` DESC LIMIT 1", playerVariables[playerid][pInternalID]);
    mysql_tquery(gSQL, gQuery, "timestampConnect", "d", playerid);

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
    // mysql_query(szQueryOutput, THREAD_BANK_SUSPENSION, playerid); ????

//     if(playerVariables[playerid][pCarModel] > 0)
//         SpawnPlayerVehicle(playerid);

    if(playerVariables[playerid][pLevel] > 0)
        SetPlayerScore(playerid, playerVariables[playerid][pLevel]);

    if(playerVariables[playerid][pAdminDuty] == 1 && playerVariables[playerid][pAdminLevel] < 1) {
        playerVariables[playerid][pAdminLevel] = 0;
        playerVariables[playerid][pAdminDuty] = 0;
        playerVariables[playerid][pAdminName][0] = '*';
        SendClientMessage(playerid, COLOR_HOTORANGE, "You're no longer an administrator.");
    }

    if(playerVariables[playerid][pAdminLevel] > 0 && playerVariables[playerid][pAdminDuty] > 1)
            SetPlayerName(playerid, playerVariables[playerid][pAdminName]);

    return true;
}

function timestampConnect(playerid) {
    if(cache_num_rows() == 0)
        return SendClientMessage(playerid, COLOR_GENANNOUNCE, "SERVER:"EMBED_WHITE" Welcome to the server!");

    new timestamp[50];
    cache_get_value(0, "conTS", timestamp);
    
    gString[0] = (EOS);
    format(gString, sizeof(gString), "SERVER:"EMBED_WHITE" Welcome back %s, you last visited us on %s.", szPlayerName, timestamp);
    SendClientMessage(playerid, COLOR_GENANNOUNCE, gString);
    
    if(playerVariables[playerid][pGroup] >= 1) {
        // gString[0] = (EOS);
        // format(gString, sizeof(gString), "(Group) "EMBED_WHITE"%s from your group has just logged in.", szPlayerName);
        // SendToGroup(playerVariables[playerid][pGroup], COLOR_GENANNOUNCE, gString);

        // gString[0] = (EOS);
        // format(gString, sizeof(gString), "(Group) MOTD: "EMBED_WHITE"%s", groupVariables[playerVariables[playerid][pGroup]][gGroupMOTD]);
        // SendClientMessage(playerid, COLOR_GENANNOUNCE, gString);
    }

    return true;
}

function adminSecurity(playerid) {
    if(!cache_num_rows()) {
        if(GetPVarInt(playerid, "pAdminPIN") == 0)
            return true;
            
        SetPVarInt(playerid, "pAdminFrozen", 1);
        
        Dialog_Show(playerid, DIALOG_ADMIN_PIN, DIALOG_STYLE_INPUT, "SERVER: Admin authentication verification", "The system has recognised that you have connected with an IP that you've never used before.\n\nPlease confirm your admin PIN to continue:", "OK", "Cancel");
    }

    return true;
}  

Dialog:DIALOG_ADMIN_PIN(playerid, response, listitem, inputtext[]) {
    if(strlen(inputtext) != 4)
        return Dialog_Show(playerid, DIALOG_ADMIN_PIN, DIALOG_STYLE_INPUT, "SERVER: Admin authentication verification", "Incorrect PIN!\n\nPlease confirm your admin PIN to continue:", "OK", "Cancel");

    if(strval(inputtext) == GetPVarInt(playerid, "pAdminPIN")) {
        DeletePVar(playerid, "pAdminFrozen");
        SendClientMessage(playerid, COLOR_GENANNOUNCE, "SERVER:{FFFFFF} You've entered the correct PIN.");
        SetPVarInt(playerid, "pAdminPINConfirmed", ADMIN_PIN_TIMEOUT);
        /*
            if(GetPVarType(playerid, "doCmd") != 0 || GetPVarType(playerid, "doCmdParams") != 0) {
                new
                    szCommand[28],
                    szCommandParams[100];
                    
                GetPVarString(playerid, "doCmd", szCommand, sizeof(szCommand));
                GetPVarString(playerid, "doCmdParams", szCommandParams, sizeof(szCommandParams));
                
                for(new i = 0; i < strlen(szCommand); i++) {
                    tolower(szCommand[i]);
                }
                
                format(gString, sizeof(gString), "cmd_%s", szCommand);
                CallLocalFunction(gString, "ds", playerid, szCommandParams);

                DeletePVar(playerid, "doCmd");
                DeletePVar(playerid, "doCmdParams");
            }
        */
    } else {
        SetPVarInt(playerid, "LA", GetPVarInt(playerid, "LA") + 1);

        if(GetPVarInt(playerid, "LA") > MAX_LOGIN_ATTEMPTS) {
            SendClientMessage(playerid, COLOR_RED, "You have used all available login attempts.");
 
            // format(gString, sizeof(gString), "AdmWarn: {FFFFFF}Admin %s has been banned (%d failed 3 admin PIN attempts).", szPlayerName, MAX_LOGIN_ATTEMPTS);
            // submitToAdmins(gString, COLOR_HOTORANGE);

            // scriptBan(playerid, "Maximum admin PIN attempts exceeded.");
            return true;
        } else {
            Dialog_Show(playerid, DIALOG_ADMIN_PIN, DIALOG_STYLE_INPUT, "SERVER: Admin authentication verification", "Incorrect PIN!\n\nThe system has recognised that you have connected with an IP that you've never used before.\n\nPlease confirm your admin PIN to continue:", "OK", "Cancel");
            
            format(gString, sizeof(gString), "Incorrect admin PIN. You have %d remaining login attempts left.", MAX_LOGIN_ATTEMPTS - GetPVarInt(playerid, "LA"));
            SendClientMessage(playerid, COLOR_HOTORANGE, gString);
            
            return true;
        }
    }

    return true;
}

Dialog:DIALOG_REGISTER(const playerid, const response, const listitem, inputtext[]) {
    if(strlen(inputtext) < 1)
        return Dialog_Show(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "SERVER: Registration", "Your password must exceed 1 character!\n\nWelcome to the "SERVER_NAME" Server.\n\nPlease enter your desired password below!", "Register", "Cancel");

    new
        wpHash[129]; 

    WP_Hash(wpHash, sizeof(wpHash), inputtext);

    mysql_format(gSQL, gLargeQuery, sizeof(gLargeQuery), "INSERT INTO playeraccounts (playerName, playerPassword) VALUES('%e', '%s')", szPlayerName, wpHash);
    mysql_tquery(gSQL, gLargeQuery, "", "");

    SendClientMessage(playerid, COLOR_WHITE, "SERVER: Your account is now registered!"); 
    // send admin message & log for registration add this in future

    finishLogin(playerid); 

    return true;
}

Dialog:DIALOG_LOGIN(playerid, response, listitem, inputtext[]) {
    if(!response) return Kick(playerid); 

    new
        wpHash[129]; 

    WP_Hash(wpHash, sizeof(wpHash), inputtext); 

    if(strcmp(wpHash, playerVariables[playerid][pPassword]) == 0) {
        Dialog_Close(playerid);

        cache_set_active(playerVariables[playerid][Cache_ID]);

        getPlayerData(playerid);

        cache_delete(playerVariables[playerid][Cache_ID]);
        playerVariables[playerid][Cache_ID] = MYSQL_INVALID_CACHE;

        finishLogin(playerid); 
    } else {
        SetPVarInt(playerid, "LA", GetPVarInt(playerid, "LA") + 1); 

        if(GetPVarInt(playerid, "LA") > MAX_LOGIN_ATTEMPTS) {
            SendClientMessage(playerid, COLOR_RED, "You have used all available login attempts.");  

            // format(gString, sizeof(gString), "AdmWarn: {FFFFFF}IP %s has been banned (%d failed 3 attempts on account %s).", playerVariables[playerid][pConnectionIP], MAX_LOGIN_ATTEMPTS, szPlayerName);
            // submitToAdmins(gString, COLOR_HOTORANGE);
            // after make admin sistem come back here

            // #if defined IPBan 
            //     IPBan(playerVariables[playerid][pConnectionIP], "Exceeded maximum login attempts."); activate after make bans
            // #endif
            Kick(playerid);

        } else {
            Dialog_Show(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "SERVER: Login", "Welcome to the "SERVER_NAME" Server.\n\nPlease enter your password below!", "Login", "Cancel");
            
            format(gString, sizeof(gString), "Incorrect password. You have %d remaining login attempts left.", MAX_LOGIN_ATTEMPTS - GetPVarInt(playerid, "LA"));
            SendClientMessage(playerid, COLOR_HOTORANGE, gString);
        }
    }

    return true;
} 

function getPlayerData(playerid) {
    new is_banned;

    cache_get_value_int(0, "playerBanned", is_banned);

    if(is_banned) { 

        SendClientMessage(playerid, COLOR_RED, "You are banned from this server."); 

        // format(gString, sizeof(gString), "AdmWarn: {FFFFFF}%s has attempted to evade their account ban (using IP %s).", szPlayerName, playerVariables[playerid][pConnectionIP]);
        // submitToAdmins(gString, COLOR_HOTORANGE); come back when admins system is done

        // format(gString, sizeof(gString), "Attempted ban evasion (%s).", szPlayerName); 
        // IPBan(playerIP, gString); 

        Kick(playerid);
    }

    // new stringDB[100];

    playerVariables[playerid][pBanned] = is_banned;

    cache_get_value(0, "playerPassword", playerVariables[playerid][pPassword]);   
    cache_get_value(0, "playerEmail", playerVariables[playerid][pEmail]);
    cache_get_value(0, "playerAdminName", playerVariables[playerid][pAdminName]);

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
    
    cache_get_value_int(0, "playerAccent", playerVariables[playerid][pAccent]);
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