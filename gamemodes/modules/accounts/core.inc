#include <YSI_Coding/y_hooks>

enum 
{
    DIALOG_REGISTER,
    DIALOG_LOGIN
}

enum PlayerDataEnum
{
    pId,
    pName[MAX_PLAYER_NAME],
    pLevel,
    pMoney,
    Float:pPosX,
    Float:pPosY,
    Float:pPosZ,
    pInterior,
    bool:pIsLoggedIn
}

new playerData[MAX_PLAYERS][PlayerDataEnum];

forward OnQueryDone(playerid);
forward OnPasswordHash(playerid);
forward OnPasswordVerify(playerid, bool:success);
forward OnRegisterDone(playerid);
forward OnPlayerLoginRequest(playerid, inputtext[]);
forward OnPlayerFound(playerid);

hook OnPlayerRequestClass(playerid, classid) 
{
    new playerName[MAX_PLAYER_NAME], query[128];
    GetPlayerName(playerid, playerName, sizeof(playerName));
    
    mysql_format(database, query, sizeof(query), "SELECT name from `users` WHERE `name` = '%e' LIMIT 1", playerName);
    mysql_tquery(database, query, "OnQueryDone", "d", playerid);
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) 
{
    if((dialogid == DIALOG_REGISTER || dialogid == DIALOG_LOGIN) && !response) {
        SendClientMessage(playerid, COLOR_WARNING, "[ORPG] Você foi kickado pois escolheu não prosseguir. Sinta-se a vontade para tentar novamente!");
        Kick(playerid);
    }

    if(dialogid == DIALOG_REGISTER) 
    {
        if(strlen(inputtext) < 6)
        {
            return ShowPlayerDialog(
                playerid,
                DIALOG_REGISTER,
                DIALOG_STYLE_PASSWORD,
                "Registro",
                "A sua senha precisa ter pelo menos 6 caracteres.",
                "Registrar",
                "Cancelar"
            );
        }

        bcrypt_hash(playerid, "OnPasswordHash", inputtext, 12);
        return 1;
    }

    if(dialogid == DIALOG_LOGIN)
    {
        new playerName[MAX_PLAYER_NAME], query[128];
        GetPlayerName(playerid, playerName, sizeof(playerName));

        mysql_format(database, query, sizeof(query), "SELECT password FROM `users` WHERE `name` = '%e'", playerName);
        mysql_tquery(database, query, "OnPlayerLoginRequest", "ds", playerid, inputtext);
        return 1;
    }

    return 0;
}

public OnQueryDone(playerid)
{
    if(cache_num_rows() == 0)
    {
        ShowPlayerDialog(
            playerid,
            DIALOG_REGISTER,
            DIALOG_STYLE_PASSWORD,
            "Registro",
            "Bem-vindo(a)!\n\nDigite uma senha abaixo para criar sua conta.",
            "Registrar",
            "Cancelar"
        );
        return 1;
    }

    ShowPlayerDialog(
        playerid,
        DIALOG_LOGIN,
        DIALOG_STYLE_PASSWORD,
        "Login",
        "Bem-vindo(a) novamente!\n\nDigite sua senha abaixo.",
        "Entrar",
        "Cancelar"
    );

    return 1;
}

public OnPlayerLoginRequest(playerid, inputtext[]) 
{
    new password[128], hash[BCRYPT_HASH_LENGTH];
    cache_get_value_name(0, "password", hash);

    strcopy(password, inputtext);
    bcrypt_verify(playerid, "OnPasswordVerify", password, hash);
}

public OnPasswordHash(playerid) 
{
    new playerName[MAX_PLAYER_NAME], hash[BCRYPT_HASH_LENGTH], query[512];
    GetPlayerName(playerid, playerName, sizeof(playerName));
    bcrypt_get_hash(hash);

    mysql_format(
        database, 
        query, 
        sizeof(query), 
        "INSERT INTO `users` (`name`, `password`, `pos_x`, `pos_y`, `pos_z`) VALUES ('%e', '%e', %f, %f, %f)",
        playerName,
        hash,
        823.4052,
        -1341.5691,
        13.5167
    );
    
    mysql_tquery(database, query, "OnRegisterDone", "d", playerid);
    return 1;
}

public OnPasswordVerify(playerid, bool:success)
{
    if(!success)
    {
        ShowPlayerDialog(
            playerid,
            DIALOG_LOGIN,
            DIALOG_STYLE_PASSWORD,
            "Login",
            "A senha está incorreta.",
            "Entrar",
            "Cancelar"
        );
        return 1;
    }

    new query[128], playerName[MAX_PLAYER_NAME];
    GetPlayerName(playerid, playerName, sizeof(playerName));

    mysql_format(database, query, sizeof(query), "SELECT * FROM `users` WHERE name = '%e'", playerName);
    mysql_tquery(database, query, "OnPlayerFound", "d", playerid);

    SendClientMessage(playerid, COLOR_SUCCESS, "[ORPG] Bem-vindo(a) de volta. Aproveite!");
    return 1;
}

public OnRegisterDone(playerid)
{
    new playerName[MAX_PLAYER_NAME], query[128];
    GetPlayerName(playerid, playerName, sizeof(playerName));

    mysql_format(database, query, sizeof(query), "SELECT * FROM `users` WHERE `name` = '%e' LIMIT 1", playerName);
    mysql_tquery(database, query, "OnPlayerFound", "d", playerid);
    SendClientMessage(playerid, COLOR_SUCCESS, "[ORPG] Seja muito bem-vindo(a) ao servidor. Esperamos que você curta a experiência!");
    return 1;
}

public OnPlayerFound(playerid)
{
    if(cache_num_rows() == 0)
    {
        Kick(playerid);
        return 1;
    }

    cache_get_value_name_int(0, "id", playerData[playerid][pId]);
    cache_get_value_name(0, "name", playerData[playerid][pName], MAX_PLAYER_NAME);
    cache_get_value_name_int(0, "level", playerData[playerid][pLevel]);
    cache_get_value_name_int(0, "money", playerData[playerid][pMoney]);
    cache_get_value_name_float(0, "pos_x", playerData[playerid][pPosX]);
    cache_get_value_name_float(0, "pos_y", playerData[playerid][pPosY]);
    cache_get_value_name_float(0, "pos_z", playerData[playerid][pPosZ]);
    cache_get_value_name_int(0, "p_interior", playerData[playerid][pInterior]);
    playerData[playerid][pIsLoggedIn] = true;

    SetPlayerScore(playerid, playerData[playerid][pLevel]);
    GivePlayerMoney(playerid, playerData[playerid][pMoney]);
    SetPlayerPos(playerid, playerData[playerid][pPosX], playerData[playerid][pPosY], playerData[playerid][pPosZ]);
    SetPlayerInterior(playerid, playerData[playerid][pInterior]);
    
    SetSpawnInfo(playerid, 0, 0, playerData[playerid][pPosX], playerData[playerid][pPosY], playerData[playerid][pPosZ], 90.0);
    SpawnPlayer(playerid);
    return 1;
}