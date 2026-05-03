#include <YSI_Coding/y_hooks>

new DB:database;

hook OnGameModeInit()
{
    database = DB_Open("openrpg.db");

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
    if(DB_Close(database))
    {
        database = DB:0;
    }
    return 1;
}