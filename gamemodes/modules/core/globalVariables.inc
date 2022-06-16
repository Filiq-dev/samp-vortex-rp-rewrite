new
    gString[256],
    gQuery[256],
    gLargeQuery[1024],
    gLargeString[1024],
    szServerWebsite[32],
    LSPDObjs[8][3], // 8 sets of doors. 0 = door1, 1 = door2, 2 = status (closed/open)
	LSPDGates[2][2], // Boom gate, garage (1 = status, closed/open).
    adTick;

#define					IsPlayerAuthed(%0)						(playerVariables[%0][pStatus] == 1) 

new VehicleNames[212][] = // Keeping unnecessary bits out (easily calculated integers, etc) for the win
{
	"Landstalker","Bravura","Buffalo","Linerunner","Perennial","Sentinel","Dumper","Firetruck","Trashmaster","Stretch",
	"Manana","Infernus","Voodoo","Pony","Mule","Cheetah","Ambulance","Leviathan","Moonbeam","Esperanto","Taxi",
	"Washington","Bobcat","Mr Whoopee","BF Injection","Hunter","Premier","Enforcer","Securicar","Banshee","Predator",
	"Bus","Rhino","Barracks","Hotknife","Trailer","Previon","Coach","Cabbie","Stallion","Rumpo","RC Bandit", "Romero",
	"Packer","Monster","Admiral","Squalo","Seasparrow","Pizzaboy","Tram","Trailer","Turismo","Speeder","Reefer","Tropic","Flatbed",
	"Yankee","Caddy","Solair","Berkley's RC Van","Skimmer","PCJ-600","Faggio","Freeway","RC Baron","RC Raider",
	"Glendale","Oceanic","Sanchez","Sparrow","Patriot","Quad","Coastguard","Dinghy","Hermes","Sabre","Rustler",
	"ZR-350","Walton","Regina","Comet","BMX","Burrito","Camper","Marquis","Baggage","Dozer","Maverick","News Chopper",
	"Rancher","FBI Rancher","Virgo","Greenwood","Jetmax","Hotring Racer","Sandking","Blista Compact","Police Maverick",
	"Boxville","Benson","Mesa","RC Goblin","Hotring Racer A","Hotring Racer B","Bloodring Banger","Rancher","Super GT",
	"Elegant","Journey","Bike","Mountain Bike","Beagle","Cropduster","Stuntplane","Tanker","Road Train","Nebula","Majestic",
	"Buccaneer","Shamal","Hydra","FCR-900","NRG-500","HPV-1000","Cement Truck","Tow Truck","Fortune","Cadrona","FBI Truck",
	"Willard","Forklift","Tractor","Combine","Feltzer","Remington","Slamvan","Blade","Freight","Streak","Vortex","Vincent",
	"Bullet","Clover","Sadler","Firetruck","Hustler","Intruder","Primo","Cargobob","Tampa","Sunrise","Merit","Utility",
	"Nevada","Yosemite","Windsor","Monster A","Monster B","Uranus","Jester","Sultan","Stratum","Elegy","Raindance","RC Tiger",
	"Flash","Tahoma","Savanna","Bandito","Freight","Trailer","Kart","Mower","Duneride","Sweeper","Broadway",
	"Tornado","AT-400","DFT-30","Huntley","Stafford","BF-400","Newsvan","Tug","Trailer","Emperor","Wayfarer",
	"Euros","Hotdog","Club","Trailer","Trailer","Andromada","Dodo","RCCam","Launch","Police Car (LSPD)","Police Car (SFPD)",
	"Police Car (LVPD)","Police Ranger","Picador","S.W.A.T. Van","Alpha","Phoenix","Glendale","Sadler","Luggage Trailer A",
	"Luggage Trailer B","Stair Trailer","Boxville","Farm Plow","Utility Trailer"
};

new Float:JailSpawns[4][3] = {

	{ 227.46, 110.0, 999.02 },
	{ 223.15, 110.0, 999.02 },
	{ 219.25, 110.0, 999.02 },
	{ 216.39, 110.0, 999.02 }
}; 

new WeaponNames[47][] = // As below
{
	"punch","brass knuckles","golf club","nitestick","knife","baseball bat","shovel","pool cue","katana","chainsaw","purple dildo","small white vibrator","large white vibrator","silver vibrator",
	"bouquet of flowers","cane","grenade","tear gas grenade","molotov cocktail","jetpack"," "," ","Colt .45","silenced Colt .45","Desert Eagle","12-gauge shotgun","sawn-off shotgun","SPAS-12",
	"Micro Uzi","MP5","AK-47","M4A1","TEC-9","rifle","sniper rifle","rocket launcher","heatseeker","flamethrower","minigun","satchel charge","detonator","spray can","fire extinguisher",
	"camera","nightvision goggles", "thermal goggles","parachute"
}; 

#define MAX_ASSETS 10
#define MAX_GROUPS          (20)
#define GROUP_VIRTUAL_WORLD (20000)
#define GOVERNMENT_GROUP_ID (4)
enum groupE {
	gGroupName[64],
	gGroupType,
	Float: gGroupExteriorPos[3],
	Float: gGroupInteriorPos[3],
	gGroupHQInteriorID,
	gGroupPickupID,
	Float: gSafePos[3],
	gSafePickupID,
	Text3D: gSafeLabelID,
	Text3D: gGroupLabelID,
	gGroupHQLockStatus,
	gSafe[2], // 0-1: Money, mats. pot, cocaine out for now
	gswatInv,
	gGroupMOTD[128],
	gGroupRankName1[32], // 4d arrays aren't supported in pawn, so I'll have to continue it like this...
	gGroupRankName2[32],
	gGroupRankName3[32],
	gGroupRankName4[32],
	gGroupRankName5[32],
	gGroupRankName6[32],
}

enum assetsE {
	aAssetName[32],
	aAssetValue,
}

new 
    assetVariables[MAX_ASSETS][assetsE];


new groupVariables[MAX_GROUPS][groupE];