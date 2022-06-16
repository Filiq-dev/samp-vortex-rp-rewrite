#include <YSI_Coding/y_hooks>

#define MAX_ATMS 25

enum atmE {
	Float: fATMPos[3],
	Float: fATMPosRot[3],
	rObjectId,
	Text3D: rTextLabel,
}

new 
    atmVariables[MAX_ATMS][atmE],
    Iterator:atms<MAX_ATMS>;

hook OnGameModeInit() {
    #if defined DEBUG
        print("hook OnGameModeInit()_atms has been called");
    #endif

    mysql_tquery(gSQL, "SELECT * FROM atms", "loadAtms", "");

    return true;
} 

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    #if defined DEBUG
        printf("hook OnPlayerKeyStateChange(%d, %d, %d)_atms has been called", playerid, newkeys, oldkeys);
    #endif  

    if(IsKeyJustDown(KEY_CROUCH, newkeys, oldkeys)) {
        foreach(new x : atms)
            if(IsPlayerInRangeOfPoint(playerid, 2.0, atmVariables[x][fATMPos][0], atmVariables[x][fATMPos][1], atmVariables[x][fATMPos][2])) {
				Dialog_Show(playerid, DIALOG_ATM_MENU, DIALOG_STYLE_LIST, "SERVER: Automated Teller Machine", "Check Balance\nWithdraw", "OK", "Cancel");
                break;
            }
    }

    return true;
}

function loadAtms() {
    #if defined DEBUG
        print("function loadAtms()_atms has been called");
    #endif

    new x = 0;
    for ( new i, j = cache_num_rows(); i != j; ++i ) {

	    cache_get_value_int(i, "atmId", x);

        cache_get_value_name_float(i, "atmPosX", atmVariables[x][fATMPos][0]);
        cache_get_value_name_float(i, "atmPosY", atmVariables[x][fATMPos][1]);
        cache_get_value_name_float(i, "atmPosZ", atmVariables[x][fATMPos][2]);
        cache_get_value_name_float(i, "atmPosRotX", atmVariables[x][fATMPosRot][0]);
        cache_get_value_name_float(i, "atmPosRotY", atmVariables[x][fATMPosRot][1]);
        cache_get_value_name_float(i, "atmPosRotZ", atmVariables[x][fATMPosRot][2]);

        atmVariables[x][rObjectId] = CreateDynamicObject(2618, atmVariables[x][fATMPos][0], atmVariables[x][fATMPos][1], atmVariables[x][fATMPos][2], atmVariables[x][fATMPosRot][0], atmVariables[x][fATMPosRot][1], atmVariables[x][fATMPosRot][2], -1, -1, -1, 500.0);
        atmVariables[x][rTextLabel] = CreateDynamic3DTextLabel("ATM\n\nWithdraw your cash here!\n\nPress ~k~~PED_DUCK~ to use this ATM.", COLOR_YELLOW, atmVariables[x][fATMPos][0], atmVariables[x][fATMPos][1], atmVariables[x][fATMPos][2], 50.0);
        
        Iter_Add(atms, x);
    } 
    printf("[SQL] Atms loaded: %d", x); 

    return true;
}  

Dialog:DIALOG_ATM_MENU(playerid, response, listitem, inputtext[]) {
    if(!response) return true;

    // Reset the player's position to revoke the player crouching animation for convenience
    GetPlayerPos(playerid, playerVariables[playerid][pPos][0], playerVariables[playerid][pPos][1], playerVariables[playerid][pPos][2]);
    SetPlayerPos(playerid, playerVariables[playerid][pPos][0], playerVariables[playerid][pPos][1], playerVariables[playerid][pPos][2]);
        
    switch(listitem) {
        case 0: {
            gString[0] = (EOS);
            format(gString, 100, "Your bank account balance is currently standing at $%d.", playerVariables[playerid][pBankMoney]);
            Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "SERVER: Automated Teller Machine", gString, "OK", "");
        }
        case 1: 
            Dialog_Show(playerid, DIALOG_ATM_WITHDRAWAL, DIALOG_STYLE_INPUT, "SERVER: ATM", "Please specify the amount of money that you'd like to withdraw.\n\nYou can withdraw up to $10,000 from this ATM.\n\nNote: This ATM charges $2 from any withdrawals you make.", "OK", "Cancel");
    }

    return true;
}

Dialog:DIALOG_ATM_WITHDRAWAL(playerid, response, listitem, inputtext[]) {
    if(!response) return true;

    new
        iWithdrawalAmount = strval(inputtext);

    if(playerVariables[playerid][pBankMoney] > iWithdrawalAmount && iWithdrawalAmount > 1 && iWithdrawalAmount < 10000) {
        playerVariables[playerid][pBankMoney] -= iWithdrawalAmount - 2;
        playerVariables[playerid][pMoney] += iWithdrawalAmount;

        Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "SERVER: Automated Teller Machine", "Your money has been withdrawn. \n\nThank you for using our ATM today!", "OK", "");
    } else {
        SendClientMessage(playerid, COLOR_GREY, "You do not have enough money to withdraw this amount.");
        Dialog_Show(playerid, DIALOG_ATM_WITHDRAWAL, DIALOG_STYLE_INPUT, "SERVER: ATM", "Please select a value of money that you currently have in your bank account.\n\nPlease specify the amount of money that you'd like to withdraw. \n\nYou can withdraw up to $10,000 from this ATM.\n\nNote: This ATM charges $2 from any withdrawals you make.", "OK", "Cancel");
    }
    
    return true;
}