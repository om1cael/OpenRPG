#include <YSI_Coding/y_hooks>

enum 
{
    DIALOG_REGISTER,
    DIALOG_LOGIN
}

enum PlayerDataEnum
{
    pName[MAX_PLAYER_NAME],
    pPassword[72],
    pLevel,
    pMoney,
    pPosX,
    pPosY,
    pPosZ
}

new PlayerData[MAX_PLAYERS][PlayerDataEnum];

hook OnPlayerRequestClass(playerid, classid) 
{
    new playerName[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, playerName, sizeof(playerName));
    
    new DBResult:player_query = 
        DB_ExecuteQuery(database, "SELECT name FROM users WHERE name = %s", playerName);

    if(DB_GetRowCount(player_query) == 0) 
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
    }
    else
    {
        ShowPlayerDialog(
            playerid,
            DIALOG_LOGIN,
            DIALOG_STYLE_PASSWORD,
            "Login",
            "Bem-vindo(a) novamente!\n\nDigite sua senha abaixo.",
            "Entrar",
            "Cancelar"
        );
    }

    DB_FreeResultSet(player_query);
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) 
{
    if((dialogid == DIALOG_REGISTER || dialogid == DIALOG_LOGIN) && !response) {
        SendClientMessage(playerid, COLOR_WARNING, "[ORPG] Você foi kickado pois escolheu não prosseguir. Sinta-se a vontade para tentar novamente!");
        Kick(playerid);
    }
    return 0;
}