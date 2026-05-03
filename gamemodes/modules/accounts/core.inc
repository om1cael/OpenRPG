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
    pPassword[72],
    pLevel,
    pMoney,
    pPosX,
    pPosY,
    pPosZ
}

new PlayerData[MAX_PLAYERS][PlayerDataEnum];

forward OnPlayerDataLoaded(playerid);

hook OnPlayerRequestClass(playerid, classid) 
{
    new playerName[MAX_PLAYER_NAME], query[128];
    GetPlayerName(playerid, playerName, sizeof(playerName));
    
    mysql_format(database, query, sizeof(query), "SELECT name from `users` WHERE `name` = '%e' LIMIT 1", playerName);
    mysql_tquery(database, query, "OnPlayerDataLoaded", "d", playerid);
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
        return 1;
    }


    return 0;
}

public OnPlayerDataLoaded(playerid)
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