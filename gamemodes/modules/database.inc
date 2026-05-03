#include <YSI_Coding/y_hooks>

#define MYSQL_IP "127.0.0.1"
#define MYSQL_USER "root"
#define MYSQL_PASS "passwordhere"
#define MYSQL_DB "openrpg"

new MySQL:database;

hook OnGameModeInit()
{
    database = mysql_connect(MYSQL_IP, MYSQL_USER, MYSQL_PASS, MYSQL_DB);

    if(database)
    {
        print("Database connection was successful.");
    }
    else
    {
        print("Failed to open database connection!");
        SendRconCommand("exit");
    }

    return 1;
}

hook OnGameModeExit()
{
    mysql_close(database);
    return 1;
}