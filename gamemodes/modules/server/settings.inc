#define DEBUG 
#define SERVER_VERSION  "3.0.0 BETA"
#define SERVER_NAME     "VX-RP" // Would be nice if you kept it as this, so I can see which servers are using this mode easily 

#define MAX_LOGIN_ATTEMPTS      (3)
#define MAX_TEXTDRAWS           (10)
#define INTERIOR_WEATHER_ID	    (1) // Outdoor weather is used inside interiors too, blame San Andreas.
#define MAX_WEATHER_POINTS      (9)
#define HOUSE_VIRTUAL_WORLD     (10000) 
#define SYNTAX_MESSAGE          "Syntax: {FFFFFF}"

#define function%0(%1) forward%0(%1); public%0(%1)

native WP_Hash(buffer[], len, const str[]);