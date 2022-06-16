#include <YSI_Coding/y_hooks>

#define MAX_JOBS 5

enum jobsE {
    jJobType,
    Float: jJobPosition[3],
    jJobName[32],
    jJobPickupID,
    Text3D:jJobLabelID,
}

new 
    jobVariables[MAX_JOBS][jobsE],
    Iterator:jobs<MAX_JOBS>;

hook OnGameModeInit() {
    #if defined DEBUG
        print("hook OnGameModeInit()_jobs has been called");
    #endif

    mysql_tquery(gSQL, "SELECT * FROM jobs", "loadJobs", "");

    return true;
}

hook OnGameModeExit() {
    #if defined DEBUG
        print("hook OnGameModeExit()_jobs has been called");
    #endif

    // foreach(new i : jobs)
    //     saveJobs(i);

    return true;
}

function loadJobs() {
    #if defined DEBUG
        print("function loadJobs()_jobs has been called");
    #endif

    new x = 0;
    for ( new i, j = cache_num_rows(); i != j; ++i ) {

	    cache_get_value_int(i, "jobID", x);
	    cache_get_value_int(i, "jobType", jobVariables[x][jJobType]);	   

        cache_get_value_name_float(i, "jobPositionX", jobVariables[x][jJobPosition][0]);
        cache_get_value_name_float(i, "jobPositionY", jobVariables[x][jJobPosition][1]);
        cache_get_value_name_float(i, "jobPositionZ", jobVariables[x][jJobPosition][2]);

	    cache_get_value_name(i, "assetName", jobVariables[x][jJobName], 32);

        gString[0] = (EOS);
        format(gString, 50, "Job %s\nType /getjob", jobVariables[x][jJobName]);

        jobVariables[x][jJobLabelID] = CreateDynamic3DTextLabel(gString, COLOR_YELLOW, jobVariables[x][jJobPosition][0], jobVariables[x][jJobPosition][1], jobVariables[x][jJobPosition][2], 100, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 50.0);
        jobVariables[x][jJobPickupID] = CreateDynamicPickup(1239, 23, jobVariables[x][jJobPosition][0], jobVariables[x][jJobPosition][1], jobVariables[x][jJobPosition][2], 0, -1, -1, 50);
        
        Iter_Add(jobs, x);
    } 
    printf("[SQL] Jobs loaded: %d", x); 

    return true;
}