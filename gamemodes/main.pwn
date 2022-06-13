/*
						Copyright 2010-2011 Frederick Wright

		   Licensed under the Apache License, Version 2.0 (the "License");
		   you may not use this file except in compliance with the License.
		   You may obtain a copy of the License at

		     		http://www.apache.org/licenses/LICENSE-2.0

		   Unless required by applicable law or agreed to in writing, software
		   distributed under the License is distributed on an "AS IS" BASIS,
		   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
		   See the License for the specific language governing permissions and
		   limitations under the License.

		INITIAL SCRIPT:
		    Vortex Roleplay 2 Script

        Rewrited by Filiqp_
            Vortex Roleplay 3 Script

		AUTHOR:
			Frederick Wright [mrfrederickwright@gmail.com]
			Stefan Rosic [streetfire68@hotmail.com]
			Filipqp_ [filipmacheama@gmail.com]

		ADDITIONAL CREDITS:
		    All other unmentioned mapping: JamesC [http://forum.sa-mp.com/member.php?u=97617]
			Gym Map: Marcel_Collins [http://forum.sa-mp.com/showthread.php?p=1537421]
			LS Mall: cessil [http://forum.sa-mp.com/member.php?u=50597]

		MISC INFO:
			gGroupType listing:
				0 - Gangs
				1 - Police
				2 - Government
				3 - Hitmen
				4 - LSFMD

				Reserved group slots
				1 - LSPD
				3 - Government
				4 - LSFMD

				Job Types
				1 - Arms Dealer
				2 - Detective
				3 - Mechanic
				4 - Fisherman
				
				Business Item Types:
				1 - Rope
				2 - Walkie Talkie
				3 - Phonebook
				4 - Mobile Phone Credit
				5 - Mobile Phone
				6 - 5% health increase (food)
				7 - 10% health increase (food)
				8 - 30% health increase (food)
				9 - Purple Dildo
				10 - Small White Vibrator
				11 - Large White Vibrator
				12 - Silver Vibrator
				13 - Flowers
				14 - Cigar(s)
				15 - Sprunk
				16 - Wine
				17 - Beer
				18 - All Skins

			Error Codes:
				01x01 - Attempted to deposit an invalid (negative) amount of money to a house safe.
				01x02 - Attempted to deposit an invalid (negative) amount of materials to a house safe.
				01x03 - Attempted to withdraw an invalid (negative) amount of money from a house safe.
				01x04 - Attempted to withdraw an invalid (negative) amount of materials from a house safe.
				01x05 - No checkpoint reason. The checkpoint handle hasn't had a string defined in getPlayerCheckpointReason()
				01x08 - Too many vehicles spawned (in danger of exceeding MAX_VEHICLES).

			Business Types:
			    0 - None
			    1 - 24/7
				2 - Clothing Store
				3 - Bar
				4 - Sex Shop
				5 - Car Dealership
				6 - Gym
				7 - Restaurant
*/

#define YSI_YES_HEAP_MALLOC

#define DEBUG 
#define SERVER_VERSION  "3.0.0 BETA"
#define SERVER_NAME     "VX-RP" // Would be nice if you kept it as this, so I can see which servers are using this mode easily

#include "modules/core/init.inc"

main() {
    #if defined DEBUG
        print("main() has been called");
    #endif
}

public OnGameModeInit() {
    #if defined DEBUG
        print("OnGameModeInit() has been called");
    #endif 
 

    SetGameModeText(SERVER_NAME" "SERVER_VERSION);
	GetConsoleVarAsString("weburl", szServerWebsite, sizeof(szServerWebsite));

    ShowPlayerMarkers(0);
	EnableStuntBonusForAll(false);
	DisableInteriorEnterExits();
	UsePlayerPedAnims();

    print("-----------------------------------------------------------------");
	print("Script: Vortex Roleplay 2 by Calgon and Brian.");
	print("Script: Rewrited by Filiqp_ for open.mp");
	print("Status: Loaded OnGameModeInit, running version "SERVER_VERSION);
	print("-----------------------------------------------------------------");
	
	if(strfind(SERVER_VERSION, "BETA", true) != -1) {
	    print("-----------------------------------------------------------------");
	    print("WARNING: You are running a BETA version of the script.");
	    print("WARNING: This script is not optimized (or specifically built) for public usage yet.");
	    print("-----------------------------------------------------------------");
	}

    return true;
}

public OnGameModeExit() {
    #if defined DEBUG
        print("OnGameModeExit() has been called");
    #endif  

    return true;
} 

public OnPlayerConnect(playerid) {
    #if defined DEBUG
        printf("public OnPlayerConnect(%d) has been called", playerid);
    #endif

    /*
	    (a) Attempts must be made to protect players from access to explicit content. If your
	        server contains elements that may be considered only suitable for adults, your server
	        must state this fact to the player when they first join.
	*/
	SendClientMessage(playerid, COLOR_LIGHTRED, "WARNING: This server contains explicit content which requires you to be 18+ to play here.");

    SetPlayerMapIcon(playerid, 10, 595.5443, -1250.3405, 18.2836, 52, 0); // idk what are doing this here but i will leave here for now

    /* Mall object removal - 0.3d */
    // Remove the original mall mesh
	RemoveBuildingForPlayer(playerid, 6130, 1117.5859, -1490.0078, 32.7188, 10.0);

	// This is the mall mesh LOD
	RemoveBuildingForPlayer(playerid, 6255, 1117.5859, -1490.0078, 32.7188, 10.0);

	// There are some trees on the outside of the mall which poke through one of the interiors
	RemoveBuildingForPlayer(playerid, 762, 1175.3594, -1420.1875, 19.8828, 0.25);
	RemoveBuildingForPlayer(playerid, 615, 1166.3516, -1417.6953, 13.9531, 0.25);


    return true;
}