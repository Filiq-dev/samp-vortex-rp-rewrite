#include <YSI_Coding/y_hooks>

#define MAX_ASSETS 10

enum assetsE {
	aAssetName[32],
	aAssetValue,
}

new 
    assetVariables[MAX_ASSETS][assetsE],
    Iterator:assets<MAX_ASSETS>;

hook OnGameModeInit() {
    #if defined DEBUG
        print("hook OnGameModeInit()_assets has been called");
    #endif

    mysql_tquery(gSQL, "SELECT * FROM assets", "loadAssets", "");

    return true;
}

hook OnGameModeExit() {
    #if defined DEBUG
        print("hook OnGameModeExit()_assets has been called");
    #endif

    foreach(new i : assets)
        saveAsset(i);

    return true;
}

function loadAssets() {
    #if defined DEBUG
        print("function loadAssets()_assets has been called");
    #endif

    new x = 0;
    for ( new i, j = cache_num_rows(); i != j; ++i ) {

	    cache_get_value_int(i, "assetID", x);
	    cache_get_value_int(i, "assetValue", assetVariables[x][aAssetValue]);	   
	    cache_get_value_name(i, "assetName", assetVariables[x][aAssetName], 32);

        // x++; de testat
        Iter_Add(assets, x);
    } 
    printf("[SQL] Assets loaded: %d", x); 

    return true;
}

function saveAsset(id) {
    #if defined DEBUG
        print("function saveAsset()_assets has been called");
    #endif

	if(strlen(assetVariables[id][aAssetName]) >= 1) {
        gQuery[0] = (EOS);
		mysql_format(gSQL, gQuery, sizeof(gQuery), "UPDATE assets SET assetName = '%s', assetValue = '%d' WHERE assetID = '%d'", assetVariables[id][aAssetName], assetVariables[id][aAssetValue]);
		mysql_tquery(gSQL, gQuery, "", "");
	}

	return true;
} 

YCMD:listassets(playerid, params[], help) { // de bagat in admin cmds cand o fi
    if(playerVariables[playerid][pAdminLevel] >= 5)
        foreach(new i : assets)
			if(strlen(assetVariables[i][aAssetName]) >= 1)
				va_SendClientMessage(playerid, COLOR_WHITE, "Asset Name: %s | Asset ID: %d | Value: %d", assetVariables[i][aAssetName], i, assetVariables[i][aAssetValue]);

	return true;
}