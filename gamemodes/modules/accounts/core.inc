#include <YSI_Coding/y_hooks>

enum {
    DIALOG_REGISTER,
    DIALOG_LOGIN
}

hook OnPlayerRequestClass(playerid, classid) 
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

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) 
{
    if((dialogid == DIALOG_REGISTER || dialogid == DIALOG_LOGIN) && !response) {
        SendClientMessage(playerid, COLOR_WARNING, "[ORPG] Você foi kickado pois escolheu não prosseguir. Sinta-se a vontade para tentar novamente!");
        Kick(playerid);
    }
    return 0;
}