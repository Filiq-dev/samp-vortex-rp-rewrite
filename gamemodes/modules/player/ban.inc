function checkIfPlayerIsBanned(playerid) {
    #if defined DEBUG
        printf("function checkIfPlayerIsBanned(%d) has been called", playerid);
    #endif

}

function scriptBan(playerid, reason[]) {  

    inline finishBan() {
	    playerVariables[playerid][pBanned] = 1; 
   	    va_SendClientMessageToAll(COLOR_LIGHTRED, "Ban: %s has been banned, reason: %s", getName(playerid), reason);
        adminLog("Ban: %s has been banned, reason: %s", getName(playerid), reason);
	    Kick(playerid);
    } 
    
    MySQL_TQueryInline(gSQL, using inline finishBan, "INSERT INTO bans(playerNameBanned,playerBanReason,IPBanned) VALUES('%s','%s','%s')", getName(playerid), reason, playerVariables[playerid][pConnectionIP]);

	return 1;
}

stock IPBan(ip[], reason[], name[] = "Nobody") {
    gQuery[0] = (EOS);

	mysql_format(gSQL, gQuery, sizeof(gQuery), "INSERT INTO bans(playerNameBanned,playerBanReason,IPBanned) VALUES('%s','%s','%s')", name, reason, ip);
	mysql_tquery(gSQL, gQuery, "", "");

	return true;
} 