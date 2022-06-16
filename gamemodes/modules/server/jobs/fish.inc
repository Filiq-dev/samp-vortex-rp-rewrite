#include <YSI_Coding/y_hooks>

static fishNames[5][] = {
	"Carp", "Bass", "Cod", "Plaice", "Tuna"
};

static fishMoney[5] = {
	1000, 750, 250, 900, 500
};

static Timer:onFishing[MAX_PLAYERS];

hook OnPlayerDisconnect(playerid, reason) {
    if(onFishing[playerid]) 
        stop onFishing[playerid];
}

timer startFishing[12000](playerid) {
    playerVariables[playerid][pFishing]++;
    if(playerVariables[playerid][pFishing] == 10) {

        new randFish = random(5);

        playerVariables[playerid][pFish] = randFish; 

        va_SendClientMessage(playerid, COLOR_WHITE, "You have reeled in a %s.", fishNames[randFish]);
        va_SendClientMessage(playerid, COLOR_WHITE, "The fish that you collected is worth $%d. To sell your fish, please visit a 24/7.", fishMoney[randFish]);

        playerVariables[playerid][pJobDelay] = 900;
    } 
}

YCMD:fish(playerid, params[], help) {
	if(!(playerVariables[playerid][pJob] == 4 && IsPlayerInAnyVehicle(playerid) && playerVariables[playerid][pFishing] == 0)) return SendClientMessage(playerid, COLOR_GREY, "You're required to be in a boat, have the fisherman's job and not to actively be fishing.");
    if(playerVariables[playerid][pJobDelay] != 0) return va_SendClientMessage(playerid, COLOR_GREY, "You must wait %d minutes (%d seconds) until you can go fishing again.", playerVariables[playerid][pJobDelay] / 60, playerVariables[playerid][pJobDelay]); 
    if(!IsABoat(GetPlayerVehicleID(playerid))) return SendClientMessage(playerid, COLOR_GREY, "This vehicle cannot be used for fishing.");

    SendClientMessage(playerid, COLOR_WHITE, "You're now fishing. It will take a few seconds to reel your fish in."); 

    playerVariables[playerid][pFishing] = 1; 
    onFishing[playerid] = defer startFishing(playerid);
    // playerVariables[playerid][pFishingBar] = CreateProgressBar(258.00, 137.00, 131.50, 3.19, COLOR_LIGHT, 100.0); // There's a bug people have noticed which I've not been able to fix; other players often see the textdraws when people are fishing
    // ShowProgressBarForPlayer(playerid, playerVariables[playerid][pFishingBar]);
    
	return true;
}

stock getFishName(fish) {
    // if(fish < 0 && fish > 4) return -1;

    return fishNames[fish];
}

stock getFishMoney(fish) {
    if(fish < 0 && fish > 4) return -1;

    return fishMoney[fish];
} 


