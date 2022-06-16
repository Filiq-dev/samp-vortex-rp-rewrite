#include <YSI_Coding/y_hooks>

static 
    weatherVariables[2],
    validWeatherIDs[17] = { 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15, 17, 18, 20 },
    gTime[3];
 
hook OnGameModeInit() {
    #if defined DEBUG
        print("hook OnGameModeInit()_weather has been called");
    #endif 

    weatherVariables[0] = validWeatherIDs[random(sizeof(validWeatherIDs))];
	SetWeather(weatherVariables[0]);

    return true;
}

hook OnPlayerConnect(playerid) {
    #if defined DEBUG
        printf("hook OnPlayerConnect(%d)_weather has been called", playerid);
    #endif 

    syncPlayerTime(playerid);
	SetPlayerWeather(playerid, weatherVariables[0]);

    return true;
}

hook OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid) {
	#if defined DEBUG
	    printf("hook OnPlayerInteriorChange(%d, %d, %d)_weather has been called", playerid, newinteriorid, oldinteriorid);
	#endif
	
	if(newinteriorid == 0) {
		SetPlayerWeather(playerid, weatherVariables[0]);
		SetPlayerVirtualWorld(playerid, 0); // Setting their virtual world in interior 0 keeps some annoying VW issues at bay.
	}
	else SetPlayerWeather(playerid, INTERIOR_WEATHER_ID);
    
	return true;
}

hook OnPlayerSpawn(playerid) {
    #if defined DEBUG
        printf("hook OnPlayerSpawn(%d)_weather has been called", playerid);
    #endif 

    if(!GetPlayerInterior(playerid))
		SetPlayerWeather(playerid, weatherVariables[0]);
	else
		SetPlayerWeather(playerid, INTERIOR_WEATHER_ID);

	syncPlayerTime(playerid);
}

function updateWorldTime(playerid) {
	gettime(gTime[0], gTime[1], gTime[2]);

	if(gTime[1] >= 59 && gTime[2] >= 59) {
		weatherVariables[1] += random(3) + 1; // Weather changes aren't regular.

		SetWorldTime(gTime[0]); // Set the world time to keep the worldtime variable updated (and ensure it syncs instantly for connecting players).

		if(weatherVariables[1] >= MAX_WEATHER_POINTS) {
			weatherVariables[0] = validWeatherIDs[random(sizeof(validWeatherIDs))];
            
            if(!GetPlayerInterior(playerid)) SetPlayerWeather(playerid, weatherVariables[0]);
            else SetPlayerWeather(playerid, INTERIOR_WEATHER_ID);
                
			weatherVariables[1] = 0;
		}
	}

	if(gTime[2] >= 59) syncPlayerTime(playerid);
}

stock syncPlayerTime(playerid) {
	if(!GetPlayerInterior(playerid))
		SetPlayerWeather(playerid, weatherVariables[0]);
	else 
        SetPlayerWeather(playerid, INTERIOR_WEATHER_ID);
	
    return SetPlayerTime(playerid, gTime[0], gTime[1]);
}