#include <YSI_Coding/y_hooks>

new Text:textdrawVariables[MAX_TEXTDRAWS];

static 
	Timer: tutorialTimer[MAX_PLAYERS];

static tutorialSkins[73] = {
	0, 1, 2, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17,
	18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 31, 32, 33,	34, 35, 36, 37, 38, 39, 40, 41,
	43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 52, 53,
	54, 55, 56, 57, 58, 59, 60, 61,	62, 63, 64,	66,
	67, 68, 69, 70, 72, 73, 75, 76, 77, 78, 79, 299
};

hook OnGameModeInit() { 
    #if defined DEBUG
        print("hook OnGameModeInit() has been called");
    #endif 

    textdrawVariables[1] = TextDrawCreate(203.000000, 377.000000, "Press ~r~RIGHT~w~ to teleport to the player.~n~Press ~r~LEFT~w~ to disregard the request.");
	TextDrawBackgroundColor(textdrawVariables[1], 255);
	TextDrawFont(textdrawVariables[1], 2);
	TextDrawLetterSize(textdrawVariables[1], 0.190000, 1.200000);
	TextDrawColor(textdrawVariables[1], -1);
	TextDrawSetOutline(textdrawVariables[1], 1);
	TextDrawSetProportional(textdrawVariables[1], true);
	TextDrawSetShadow(textdrawVariables[1], 1);

	textdrawVariables[7] = TextDrawCreate(149.000000, 370.000000, "~n~~n~~g~You can now continue to the next step.");
	TextDrawBackgroundColor(textdrawVariables[7], 255);
	TextDrawFont(textdrawVariables[7], 2);
	TextDrawLetterSize(textdrawVariables[7], 0.290000, 1.200000);
	TextDrawColor(textdrawVariables[7], -1);
	TextDrawSetOutline(textdrawVariables[7], 0);
	TextDrawSetProportional(textdrawVariables[7], true);
	TextDrawSetShadow(textdrawVariables[7], 1);

	textdrawVariables[8] = TextDrawCreate(149.000000, 370.000000, "~n~~n~~r~You must wait a few seconds before continuing...");
	TextDrawBackgroundColor(textdrawVariables[8], 255);
	TextDrawFont(textdrawVariables[8], 2);
	TextDrawLetterSize(textdrawVariables[8], 0.290000, 1.200000);
	TextDrawColor(textdrawVariables[8], -1);
	TextDrawSetOutline(textdrawVariables[8], 0);
	TextDrawSetProportional(textdrawVariables[8], true);
	TextDrawSetShadow(textdrawVariables[8], 1);

	textdrawVariables[2] = TextDrawCreate(149.000000, 370.000000, "Press ~r~left~w~ and ~n~Press ~r~right~w~ arrows to change skins.~n~Press ~r~~k~~VEHICLE_ENTER_EXIT~~w~ to select that skin.");
	TextDrawBackgroundColor(textdrawVariables[2], 255);
	TextDrawFont(textdrawVariables[2], 2);
	TextDrawLetterSize(textdrawVariables[2], 0.390000, 1.200000);
	TextDrawColor(textdrawVariables[2], -1);
	TextDrawSetOutline(textdrawVariables[2], 0);
	TextDrawSetProportional(textdrawVariables[2], true);
	TextDrawSetShadow(textdrawVariables[2], 1);

	textdrawVariables[3] = TextDrawCreate(149.000000, 370.000000, "~w~Press ~r~left~w~ to go back a step~n~press ~r~right~w~ arrow to proceed");
	TextDrawBackgroundColor(textdrawVariables[3], 255);
	TextDrawFont(textdrawVariables[3], 2);
	TextDrawLetterSize(textdrawVariables[3], 0.390000, 1.200000);
	TextDrawColor(textdrawVariables[3], -1);
	TextDrawSetOutline(textdrawVariables[3], 0);
	TextDrawSetProportional(textdrawVariables[3], true);
	TextDrawSetShadow(textdrawVariables[3], 1);

	textdrawVariables[4] = TextDrawCreate(149.000000, 420.000000, "Press ~r~~k~~SNEAK_ABOUT~~w~ to quit the spectator tool."); // Moved it down a little, it was actually fairly obtrusive.
	TextDrawBackgroundColor(textdrawVariables[4], 255);
	TextDrawFont(textdrawVariables[4], 2);
	TextDrawLetterSize(textdrawVariables[4], 0.390000, 1.200000);
	TextDrawColor(textdrawVariables[4], -1);
	TextDrawSetOutline(textdrawVariables[4], 0);
	TextDrawSetProportional(textdrawVariables[4], true);
	TextDrawSetShadow(textdrawVariables[4], 1);

	textdrawVariables[5] = TextDrawCreate(610.0, 420.0, "Type ~r~/stopanim~w~ to stop your animation.");
	TextDrawUseBox(textdrawVariables[5], false);
	TextDrawFont(textdrawVariables[5], 2);
	TextDrawSetShadow(textdrawVariables[5], 0);
    TextDrawSetOutline(textdrawVariables[5], 1);
    TextDrawBackgroundColor(textdrawVariables[5], 0x000000FF);
    TextDrawColor(textdrawVariables[5], 0xFFFFFFFF);
    TextDrawAlignment(textdrawVariables[5], 3);

    return true;
}

hook OnPlayerDisconnect(playerid) {
	#if defined DEBUG
        printf("hook OnPlayerDisconnect(%d)_tutorial has been called", playerid);
    #endif 
	if(playerVariables[playerid][pTutorial] != 0)
		stop tutorialTimer[playerid];
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    #if defined DEBUG
        printf("hook OnPlayerKeyStateChange(%d, %d, %d) has been called", playerid, newkeys, oldkeys);
    #endif 

    if(IsKeyJustDown(KEY_SECONDARY_ATTACK, newkeys, oldkeys)) {
        if(playerVariables[playerid][pTutorial] == 1) {
            playerVariables[playerid][pSkin] = GetPlayerSkin(playerid);
            playerVariables[playerid][pTutorial] = 2;

			SendClientMessage(playerid, COLOR_YELLOW, "Great. You've selected your clothes/skin.");
            TextDrawHideForPlayer(playerid, textdrawVariables[2]);

			Dialog_Show(playerid, DIALOG_GENDER_SELECTION, DIALOG_STYLE_MSGBOX, "SERVER: Gender Selection", "What sex/gender is your character?", "Male", "Female");

			return true;
		}
    }

    return true;
}

hook OnPlayerSpawn(playerid) {
    #if defined DEBUG
        printf("hook OnPlayerSpawn(%d) has been called", playerid);
    #endif 

    if(playerVariables[playerid][pTutorial] == 1) {
		SetPlayerInterior(playerid, 14);
		SetPlayerPos(playerid, 216.9770, -155.4791, 1000.5234);
		SetPlayerFacingAngle(playerid, 267.9681);
		TogglePlayerControllable(playerid, false);
		return true;
	}

    return true;
}  

static Keys, ud, lr;
timer tutorial_Timer[1000](playerid) { 
	if(playerVariables[playerid][pTutorial] == 1) {
	    GetPlayerKeys(playerid, Keys, ud, lr);
		
	    if(lr > 0) {
	        if(playerVariables[playerid][pSkinCount]+1 >= sizeof(tutorialSkins)) {
	            SetPlayerSkin(playerid, 0);
	            playerVariables[playerid][pSkinCount] = 0;
	            PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
	        }
	        else {
		        playerVariables[playerid][pSkinCount]++;
				SetPlayerSkin(playerid, tutorialSkins[playerVariables[playerid][pSkinCount]]);
			}
		}
	    else if(lr < 0) {
	        if(playerVariables[playerid][pSkinCount]-1 < 0) {
	            SetPlayerSkin(playerid, 0);
	            playerVariables[playerid][pSkinCount] = 0;
	            PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
	        }
	        else {
		        playerVariables[playerid][pSkinCount]--;
				SetPlayerSkin(playerid, tutorialSkins[playerVariables[playerid][pSkinCount]]);
			}
		}
	}

    if(playerVariables[playerid][pTutorial] >= 4 && playerVariables[playerid][pTutorial] < 14 && GetPVarInt(playerid, "tutTime") > 0)
		TextDrawShowForPlayer(playerid, textdrawVariables[8]);

	if(playerVariables[playerid][pTutorial] >= 4 && playerVariables[playerid][pTutorial] < 14 && GetPVarInt(playerid, "tutTime") < 1) {
	    GetPlayerKeys(playerid, Keys, ud, lr);

	    if(lr > 0) {
	        playerVariables[playerid][pTutorial]++;
	        tutorialSteps(playerid);
		}
	    else if(lr < 0) {
	        playerVariables[playerid][pTutorial]--;

	        if(playerVariables[playerid][pTutorial] < 5) {
	            playerVariables[playerid][pTutorial] = 5;
	            PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
	        }

	        tutorialSteps(playerid);
		}
	}

	if(GetPVarInt(playerid, "tutTime") > 0) {
		SetPVarInt(playerid, "tutTime", GetPVarInt(playerid, "tutTime")-1);
		if(GetPVarInt(playerid, "tutTime") == 0) {
			TextDrawHideForPlayer(playerid, textdrawVariables[8]);
			TextDrawShowForPlayer(playerid, textdrawVariables[7]);
		}
	}

    return true;
}

function tutorialSteps(playerid) {
	switch(playerVariables[playerid][pTutorial]) {
		case 5: {
			SendClientMessage(playerid, COLOR_YELLOW, "Overview");
			SendClientMessage(playerid, COLOR_WHITE, "This is a roleplay server, which means you act out a character as if it were real.");
			SendClientMessage(playerid, COLOR_WHITE, "Pressing the button to open the textbox (usually T) and simply typing a message,");
			SendClientMessage(playerid, COLOR_WHITE, "will broadcast what you've typed to the people around you as an 'IC' (in character) message.");
			SendClientMessage(playerid, COLOR_WHITE, " ");
			SendClientMessage(playerid, COLOR_WHITE, "Using /b and typing your message (e.g. /b hello) will enclose what you've written in double parenthesis.");
			SendClientMessage(playerid, COLOR_WHITE, "This will broadcast your message to the people around you as an 'OOC' (out of character) message.");
			SendClientMessage(playerid, COLOR_WHITE, " ");
			SendClientMessage(playerid, COLOR_WHITE, "Similarly, using /o has the same purpose as /b, though this time the message will be broadcasted throughout the entire server.");

			SetPVarInt(playerid, "tutTime", 10);
			TextDrawHideForPlayer(playerid, textdrawVariables[7]);
			SendClientMessage(playerid, COLOR_YELLOW, "");
		}
		case 6: {
			clearScreen(playerid);
			SendClientMessage(playerid, COLOR_YELLOW, "Locations:");
			SendClientMessage(playerid, COLOR_YELLOW, "");

			SendClientMessage(playerid, COLOR_YELLOW, "The Bank");
			SendClientMessage(playerid, COLOR_WHITE, "This is the place you'll want to go to make your various monetary transactions.");
			SendClientMessage(playerid, COLOR_WHITE, "The following commands will be useful:");
			SendClientMessage(playerid, COLOR_WHITE, "/balance, /withdraw and /deposit");

			SetPlayerVirtualWorld(playerid, 0);
			SetPlayerInterior(playerid, 0);

			SetPlayerCameraPos(playerid, 608.430480, -1203.073608, 17.801227);
			SetPlayerCameraLookAt(playerid, 594.246276, -1237.907348, 17.801227);
			SetPlayerPos(playerid, 526.8502, -1261.1985, 16.2272-30);

			SetPVarInt(playerid, "tutTime", 4);
			TextDrawHideForPlayer(playerid, textdrawVariables[7]);
			SendClientMessage(playerid, COLOR_YELLOW, "");
		}
		case 7: {
			SendClientMessage(playerid, COLOR_YELLOW, "The Crane");
			SendClientMessage(playerid, COLOR_WHITE, "At the crane, you can drop off vehicles for money.");
			SendClientMessage(playerid, COLOR_WHITE, "Use the command /dropcar to drive the vehicle to the red marker.");

			SetPlayerCameraPos(playerid, 2637.447265, -2226.906738, 16.296875);
			SetPlayerCameraLookAt(playerid, 2651.442626, -2227.208496, 16.296875);
			SetPlayerPos(playerid, 2641.4473, -2226.9067, 16.2969-30);

			SetPVarInt(playerid, "tutTime", 5);
			TextDrawHideForPlayer(playerid, textdrawVariables[7]);
			SendClientMessage(playerid, COLOR_YELLOW, "");
		}
		case 8: {
			SendClientMessage(playerid, COLOR_YELLOW, "Los Santos Police Department");
			SendClientMessage(playerid, COLOR_WHITE, "This is the place where you'll find police officers.");
			SendClientMessage(playerid, COLOR_WHITE, "Inside, you should wait in lobby before being served.");
			SendClientMessage(playerid, COLOR_WHITE, "If you want to apply to the LSPD, please visit our forum.");

			SetPlayerCameraPos(playerid, 1495.273925, -1675.542358, 28.382812);
			SetPlayerCameraLookAt(playerid, 1535.268432, -1675.874023, 13.382812);
			SetPlayerPos(playerid, 2641.4473, -2226.9067, 16.2969-30);

			SetPVarInt(playerid, "tutTime", 6);
			TextDrawHideForPlayer(playerid, textdrawVariables[7]);
			SendClientMessage(playerid, COLOR_YELLOW, "");
		}
		case 9: {
			clearScreen(playerid);
			SendClientMessage(playerid, COLOR_YELLOW, "Jobs:");
			SendClientMessage(playerid, COLOR_YELLOW, "");

			SendClientMessage(playerid, COLOR_WHITE, "Having a job gives you something to do. ");
			SendClientMessage(playerid, COLOR_WHITE, "Your job may also have a skill depending on the job you have.");
			SendClientMessage(playerid, COLOR_WHITE, "All jobs are productive in some way.");

			SetPVarInt(playerid, "tutTime", 2);
			TextDrawHideForPlayer(playerid, textdrawVariables[7]);
			SendClientMessage(playerid, COLOR_YELLOW, "");
		}
		case 10: {
			SendClientMessage(playerid, COLOR_YELLOW, "Mechanic Job");
			SendClientMessage(playerid, COLOR_WHITE, "You can find the Mechanic job near the crane at Ocean Docks.");
			SendClientMessage(playerid, COLOR_WHITE, "A mechanic can repair vehicles, add nitrous and even repaint vehicles.");

			SetPlayerCameraPos(playerid, 2314.167724, -2328.139892, 21.382812);
			SetPlayerCameraLookAt(playerid, 2323.291748, -2321.122314, 13.382812);
			SetPlayerPos(playerid, 2316.1677, -2328.1399, 13.3828-30);

			SetPVarInt(playerid, "tutTime", 5);
			TextDrawHideForPlayer(playerid, textdrawVariables[7]);
			SendClientMessage(playerid, COLOR_YELLOW, "");
		}
		case 11: {
			SendClientMessage(playerid, COLOR_YELLOW, "Arms Dealer Job");
			SendClientMessage(playerid, COLOR_WHITE, "You can find the Arms Dealer job at the front of LS Ammunation.");
			SendClientMessage(playerid, COLOR_WHITE, "An arms dealer goes on material runs to obtain materials.");
			SendClientMessage(playerid, COLOR_WHITE, "They can then use those materials to create weapons.");
			SendClientMessage(playerid, COLOR_WHITE, "There are ten weapon levels.");
			SendClientMessage(playerid, COLOR_WHITE, "Each level unlocks a new weapon. Every 50 weapons levels you up.");

			SetPlayerCameraPos(playerid, 1353.600097, -1301.909790, 19.382812);
			SetPlayerCameraLookAt(playerid, 1361.592285, -1285.515136, 13.382812);
			SetPlayerPos(playerid, 1351.6001, -1285.9098, 13.3828-30);

			SetPVarInt(playerid, "tutTime", 6);
			TextDrawHideForPlayer(playerid, textdrawVariables[7]);
			SendClientMessage(playerid, COLOR_YELLOW, "");
		}
		case 12: {
			SendClientMessage(playerid, COLOR_YELLOW, "Detective Job");
			SendClientMessage(playerid, COLOR_WHITE, "You can find the Detective job near the bank.");
			SendClientMessage(playerid, COLOR_WHITE, "A detective can track people, vehicles and houses");
			SendClientMessage(playerid, COLOR_WHITE, "To track vehicles and houses, however, you'll need to level up.");
			SendClientMessage(playerid, COLOR_WHITE, "As with the arms dealer job, there are 10 levels.");
			SendClientMessage(playerid, COLOR_WHITE, "Every 50 searches levels you up.");

			SetPlayerCameraPos(playerid, 622.514709, -1458.283691, 22.256816);
			SetPlayerCameraLookAt(playerid, 612.514709, -1458.298583, 14.256817);
			SetPlayerPos(playerid, 622.5147, -1458.2837, 14.2568-30);

			SetPVarInt(playerid, "tutTime", 10);
			TextDrawHideForPlayer(playerid, textdrawVariables[7]);
		}
		case 13: {
			SendClientMessage(playerid, COLOR_YELLOW, "Levels");
			SendClientMessage(playerid, COLOR_WHITE, "A very new feature to this script is a levels system.");
			SendClientMessage(playerid, COLOR_WHITE, "");
			SendClientMessage(playerid, COLOR_WHITE, "You can now gain OOC levels which will benefit you throughout the server.");
			SendClientMessage(playerid, COLOR_WHITE, "As of present, levels don't really do much - but future updates shall introduce a bunch of new features!");
			SendClientMessage(playerid, COLOR_WHITE, "");
			SendClientMessage(playerid, COLOR_WHITE, "You can only level up every X hours, and it costs money from your character's bank account.");

			SetPlayerCameraPos(playerid, 622.514709, -1458.283691, 22.256816);
			SetPlayerCameraLookAt(playerid, 612.514709, -1458.298583, 14.256817);
			SetPlayerPos(playerid, 622.5147, -1458.2837, 14.2568-30);

			SetPVarInt(playerid, "tutTime", 10);
			TextDrawHideForPlayer(playerid, textdrawVariables[7]);
		}
		case 14: {
			SendClientMessage(playerid, COLOR_YELLOW, "Conclusion");
			SendClientMessage(playerid, COLOR_WHITE, "Thanks for reading/watching the tutorial, your character will now spawn. ");
			SendClientMessage(playerid, COLOR_WHITE, "If you have any questions or concerns which relate to gameplay on our server, please use "EMBED_GREY"/n"EMBED_WHITE".");
			SendClientMessage(playerid, COLOR_WHITE, "If you wish to obtain help from an official member of staff, please use "EMBED_GREY"/helpme"EMBED_WHITE".");
			SendClientMessage(playerid, COLOR_WHITE, "If you see any players breaking rules, please use "EMBED_GREY"/report"EMBED_WHITE"."); 
			va_SendClientMessage(playerid, COLOR_WHITE, "Last, but not least, please make sure that you register on our community forums: "EMBED_GREY"%s"EMBED_WHITE".", szServerWebsite);
			
			firstPlayerSpawn(playerid);
			TextDrawHideForPlayer(playerid, textdrawVariables[7]);
		}
	}
}

function initiateTutorial(playerid) {
    #if defined DEBUG
        printf("function initiateTutorial(%d) has been called", playerid);
    #endif 

    Dialog_Close(playerid); 
 
    SendClientMessage(playerid, COLOR_TEAL, "----------------------------------------------------------------------------");
	va_SendClientMessage(playerid, COLOR_YELLOW, "Welcome to "SERVER_NAME", %s.", szPlayerName);
	SendClientMessage(playerid, COLOR_WHITE, "Please select your style of clothing from the selection below.");

	playerVariables[playerid][pTutorial] = 1;
	playerVariables[playerid][pVirtualWorld] = playerid+50;

	SetSpawnInfo(playerid, 0, 0, 220.4862, 1822.8994, 7.5387, 268.3423, 0, 0, 0, 0, 0, 0);
	SpawnPlayer(playerid);
	SetPlayerPos(playerid, 220.4862, 1822.8994, 7.5387);
	SetPlayerFacingAngle(playerid, 268.3423);
	TogglePlayerControllable(playerid, false);

	TextDrawShowForPlayer(playerid, textdrawVariables[2]);

	tutorialTimer[playerid] = repeat tutorial_Timer(playerid);

	return true;
}

function firstPlayerSpawn(playerid) {
    #if defined DEBUG
        printf("function firstPlayerSpawn(%d) has been called", playerid);
    #endif 
	
	playerVariables[playerid][pTutorial] = 0;
	playerVariables[playerid][pFirstLogin] = 0;

	playerVariables[playerid][pInterior] = 0;
	playerVariables[playerid][pVirtualWorld] = 0;

	SetSpawnInfo(playerid, 0, playerVariables[playerid][pSkin], playerVariables[playerid][pPos][0], playerVariables[playerid][pPos][1], playerVariables[playerid][pPos][2], 0, 0, 0, 0, 0, 0, 0);
	SpawnPlayer(playerid);

	TextDrawHideForPlayer(playerid, textdrawVariables[3]);
    
	return true;
}

Dialog:DIALOG_GENDER_SELECTION(playerid, response, listitem, inputtext[]) { 
	playerVariables[playerid][pGender] = response == 1 ? 1 : 2;
	playerVariables[playerid][pTutorial] = 3;

	va_SendClientMessage(playerid, COLOR_YELLOW, "Great. We now know that you're a %s", response == 1 ? "man" : "woman"); 

    Dialog_Show(playerid, DIALOG_TUTORIAL_DOB, DIALOG_STYLE_INPUT, "SERVER: Character Age", "Please enter the age of your character.", "Proceed", "Cancel");
}

Dialog:DIALOG_TUTORIAL_DOB(playerid, response, listitem, inputtext[]) {
    if(isnull(inputtext))
        return Dialog_Show(playerid, DIALOG_TUTORIAL_DOB, DIALOG_STYLE_INPUT, "SERVER: Character Age", "Please enter the age of your character.", "Proceed", "Cancel");
    
	new
		Age = strval(inputtext);

	if(Age >= 16 && Age < 122) {
		new
			date[3];

		getdate(date[0], date[1], date[2]);

		playerVariables[playerid][pAge] = date[0] - Age;
 
		va_SendClientMessage(playerid, COLOR_YELLOW, "You have set your character's age to %d (born in %d).", Age, playerVariables[playerid][pAge]);

		if(GetPVarInt(playerid, "quiz") == 1) {
			Dialog_Show(playerid, DIALOG_TUTORIAL_CHOICE, DIALOG_STYLE_MSGBOX, "SERVER: Tutorial", "Do you wish to participate in our server tutorial? This is optional for you and will only take a few minutes.","Yes","No");
		} else {
			playerVariables[playerid][pTutorial] = 4;
			TextDrawShowForPlayer(playerid, textdrawVariables[3]);
			SendClientMessage(playerid, COLOR_WHITE, "You're now participating in our mandatory server tutorial. Please pay close attention to the screen. ");
			SendClientMessage(playerid, COLOR_WHITE, "Please press your RIGHT arrow to proceed through the tutorial.");
		}
	}
	else {
		SendClientMessage(playerid, COLOR_WHITE, "Your character must be older than 16, and can't be older than 122 years old.");
		return Dialog_Show(playerid, DIALOG_TUTORIAL_DOB, DIALOG_STYLE_INPUT, "SERVER: Character Age", "Please enter the age of your character.", "Proceed", "Cancel");
	}
    

    return true;
}

Dialog:DIALOG_TUTORIAL_CHOICE(playerid, response, listitem, inputtext[]) {
    if(!response) {
        SendClientMessage(playerid, COLOR_YELLOW, "Conclusion");
        SendClientMessage(playerid, COLOR_WHITE, "Your character will now spawn. Welcome to the server!");
        SendClientMessage(playerid, COLOR_WHITE, "If you have any questions or concerns which relate to gameplay on our server, please use "EMBED_GREY"/n"EMBED_WHITE".");
        SendClientMessage(playerid, COLOR_WHITE, "If you wish to obtain help from an official member of staff, please use "EMBED_GREY"/helpme"EMBED_WHITE".");
        SendClientMessage(playerid, COLOR_WHITE, "If you see any players breaking rules, please use "EMBED_GREY"/report"EMBED_WHITE".");
        va_SendClientMessage(playerid, COLOR_WHITE, "Last, but not least, please make sure that you register on our community forums: "EMBED_GREY"%s"EMBED_WHITE".", szServerWebsite);
        
        firstPlayerSpawn(playerid);
    } else {
        playerVariables[playerid][pTutorial] = 4;
        TextDrawShowForPlayer(playerid, textdrawVariables[3]);
        SendClientMessage(playerid, COLOR_WHITE, "You're now participating in our mandatory server tutorial. Please pay close attention to the screen. ");
        SendClientMessage(playerid, COLOR_WHITE, "Please press your RIGHT arrow to proceed through the tutorial.");
    }

    return true;
}

Dialog:DIALOG_QUIZ(playerid, response, listitem, inputtext[]) {
    switch(listitem) {
        case 0: {
            SetPVarInt(playerid, "quiz", 1);
            Dialog_Show(playerid, DIALOG_DO_TUTORIAL, DIALOG_STYLE_MSGBOX, "Brilliant!", "You've answered the question successfully.\n\nYou'll be taken to set some character preferences in a few seconds, unless you press 'OK'.", "OK", "");
        }
        case 1, 2, 3:
			Dialog_Show(playerid, DIALOG_DO_TUTORIAL, DIALOG_STYLE_MSGBOX, "Bad luck!", "You've unfortunately failed to answer the question correctly, therefore you're going to have to watch our basic tutorial. \n\nThis box will disappear and you will partake in the tutorial after choosing your character preferences in a few seconds, unless you press 'OK'.", "OK", "");
    }
}

Dialog:DIALOG_DO_TUTORIAL(playerid, response, listitem, inputtext[]) {
    if(playerVariables[playerid][pTutorial] == 0)
        return initiateTutorial(playerid); 

    return true;
}  