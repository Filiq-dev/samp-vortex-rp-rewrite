enum {
    FINISH_SAVE_BUSINESS
}

function finishQuery(id, query, tick) {

    switch(query) {
        case FINISH_SAVE_BUSINESS: {
            printf("[SQL] Took me %d to save business in DB.", tick-GetTickCount());
        }
    }

    return true;
}