#include <YSI_Coding/y_hooks>

new MySQL:gSQL;

hook OnGameModeInit() {
    // mysql_log(ERROR | WARNING);

    #if defined DEBUG
        print("hook OnGameModeInit()_sql has been called");
        // mysql_log();
    #endif

    new MySQLOpt: option_id = mysql_init_options();

	mysql_set_option(option_id, AUTO_RECONNECT, true); // it automatically reconnects when loosing connection to mysql server
    gSQL = mysql_connect("localhost", "root", "", "vortex", option_id); // AUTO_RECONNECT is enabled for this connection handle only

    if (gSQL == MYSQL_INVALID_HANDLE || mysql_errno(gSQL) != 0) {
		print("MySQL connection failed. Server is shutting down.");
		SendRconCommand("exit"); // close the server if there is no connection
		return true;
	}

    print("MySQL connection is successful.");

    return true;
}

hook OnGameModeExit() {
    #if defined DEBUG
        print("hook OnGameModeExit()_sql has been called");
    #endif 

    // save data 

    mysql_close(gSQL);

    return true;
}