#include <YSI_Coding\y_hooks>
#include "modules/utils"

#define PICKUP_INPUT_DETECT_RANGE 2.0

enum WorkerDetails
{
    wFutureJob,
    bool:wIsGoingForNewJob,
    Float:jobX,
    Float:jobY,
    Float:jobZ
}
new workerDetail[MAX_PLAYERS][WorkerDetails];

new PlayerText3D:workTextId[MAX_PLAYERS];
new workPickup[MAX_PLAYERS];


hook OnGameModeInit()
{
    CreateInteractablePickupWithLabel(
        "Secretaria do Trabalho\nAperte F para entrar",
        1239,
        0,
        1219.1830,
        -1813.1396,
        16.5938,
        25
    );

    CreateInteractablePickupWithLabel(
        "Secretaria do Trabalho\nAperte F para sair",
        1239,
        0,
        1494.4944,
        1304.1597,
        1093.2891,
        2
    );

    CreateInteractablePickupWithLabel(
        "Secretaria do Trabalho\nAperte Y para ver empregos",
        1210,
        0,
        1490.9664,
        1305.3859,
        1093.2964,
        2
    );

    CreateActor(
        141,
        1488.8853,
        1305.3525,
        1093.2964,
        270.0
    );

    return 1;
}

hook OnPlayerConnect(playerid)
{
    return 1;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if((newkeys & KEY_SECONDARY_ATTACK) && !(oldkeys & KEY_SECONDARY_ATTACK))
    {
        if(GetPlayerDistanceFromPoint(playerid, 1219.1830,-1813.1396,16.5938) < PICKUP_INPUT_DETECT_RANGE)
        {
            SetPlayerInterior(playerid, 3);
            SetPlayerPos(playerid, 1494.4944,1304.1597,1093.2891);
            return 1;
        }

        if(GetPlayerDistanceFromPoint(playerid, 1494.4944,1304.1597,1093.2891) < PICKUP_INPUT_DETECT_RANGE)
        {
            SetPlayerInterior(playerid, 0);
            SetPlayerPos(playerid, 1219.1830,-1813.1396,16.5938);
            return 1;
        }

        return 1;
    }

    if((newkeys & KEY_YES) && !(oldkeys & KEY_YES))
    {
        if(GetPlayerDistanceFromPoint(playerid, 1490.9664, 1305.3859, 1093.2964) < PICKUP_INPUT_DETECT_RANGE)
        {
            return ShowPlayerDialog(
                playerid,
                DIALOG_WORK_BUREAU_MAIN,
                DIALOG_STYLE_TABLIST_HEADERS,
                "Empregos",
                "Área\tNível Mínimo\nLogística\t0\nSaúde\t20\nSegurança Pública\t35",
                "Visualizar",
                "Cancelar"
            );
        }
    }

    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_WORK_BUREAU_MAIN && response)
    {
        if(listitem == 0)
        {
            return ShowPlayerDialog(
                playerid,
                DIALOG_WORK_BUREAU_LOGISTICS,
                DIALOG_STYLE_TABLIST_HEADERS,
                "Empregos de Logística",
                "Emprego\tNível Mínimo\nEntregador\t0",
                "Escolher",
                "Cancelar"
            );
        }

        return 1;
    }

    if(dialogid == DIALOG_WORK_BUREAU_LOGISTICS)
    {
        if(!response) return 1;
        
        if(listitem == 0)
        {
            if(playerData[playerid][pJob] == JOB_DELIVERY) 
            {
                SendClientMessage(playerid, COLOR_WARNING, "[SECRETARIA] Você já está com contrato ativo neste emprego.");
                return 1;
            }

            if(workerDetail[playerid][wFutureJob] == JOB_DELIVERY) 
            {
                SendClientMessage(playerid, COLOR_WARNING, "[SECRETARIA] Você já foi encaminhado para este emprego.");
                return 1;
            }

            workerDetail[playerid][wIsGoingForNewJob] = true;

            SendClientMessage(playerid, COLOR_HEADSUP, "[EMPREGO] Você escolheu a profissão de entregador.");
            SendClientMessage(playerid, COLOR_HEADSUP, "[EMPREGO] Vá até o checkpoint para assinar o contrato.");

            SetPlayerCheckpoint(playerid, 1031.8572,-1338.0040,13.7266, 2);

            workPickup[playerid] = CreatePlayerPickup(playerid, 1210, 1, 1031.8572,-1338.0040,13.7266, 0);
            workTextId[playerid] = CreatePlayer3DTextLabel(playerid, "Entregador\nDigite /contrato para assinar o contrato", -1, 1031.8572,-1338.0040,13.7266, 15);
            
            workerDetail[playerid][jobX] = 1031.8572;
            workerDetail[playerid][jobY] = -1338.0040;
            workerDetail[playerid][jobZ] = 13.7266;
            workerDetail[playerid][wFutureJob] = JOB_DELIVERY;
            return 1;
        }

        return 1;
    }

    return 1;
}

@cmd() contrato(playerid, params[], help)
{
    if(!workerDetail[playerid][wIsGoingForNewJob]) 
    {
        SendClientMessage(playerid, COLOR_WARNING, "[EMPREGO] Vocẽ já está empregado.");
        return 1;
    }

    if(IsPlayerInRangeOfPoint(playerid, 5.0, workerDetail[playerid][jobX], workerDetail[playerid][jobY], workerDetail[playerid][jobZ]))
    {
        playerData[playerid][pJob] = workerDetail[playerid][wFutureJob];
        SendClientMessage(playerid, COLOR_SUCCESS, "[EMPREGO] O contrato foi assinado, bem-vindo(a)!");

        DestroyPlayerPickup(playerid, workPickup[playerid]);
        DeletePlayer3DTextLabel(playerid, workTextId[playerid]);

        workerDetail[playerid][wIsGoingForNewJob] = false;
        workerDetail[playerid][wFutureJob] = 0;
        workerDetail[playerid][jobX] = 0;
        workerDetail[playerid][jobY] = 0;
        workerDetail[playerid][jobZ] = 0;
        PlayerPlaySound(playerid, 1058, 0.0, 0.0, 0.0);
    }

    return 1;
}