#include <YSI_Coding/y_hooks>

new DB:database;

hook OnGameModeInit()
{
    database = DB_Open("openrpg.db");

    if(database)
    {
        print("Database connection was successful.");

        DB_ExecuteQuery(database, "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(24) UNIQUE, password VARCHAR(16), level INTEGER)");
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