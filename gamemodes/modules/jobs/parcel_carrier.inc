#include <YSI_Coding\y_hooks>
#include <YSI_Visual/y_commands>

#define PARCEL_BASE_TAX 20
#define PARCEL_PRICE_PER_METER 0.06

new const parcelDeliveryPositions[][DeliveryPos] =
{
    {1956.8379, -2117.6582, 12.9547},
    {1965.4443, -2036.3055, 12.9466},
    {1923.5173, -1789.5076, 12.9538},
    {2005.6587, -1732.9277, 12.9536},
    {2174.0122, -1641.5035, 14.2603},
    {2233.1953, -1296.8811, 23.4010},
    {2095.5652, -1297.6423, 23.3908},
    {2146.5986, -1175.6611, 23.3908},
    {1991.7769, -1448.9084, 12.9606},
    {1359.6753, -1754.6516, 12.9231}
};

hook OnGameModeInit()
{
    CreatePickup(1580, 0, 2199.4150,-2244.0508,13.5469, 0);
    Create3DTextLabel("Entregador de Encomendas\nAperte Y para pegar seu veículo", -1, 2199.4150, -2244.0508, 13.5469, 15, -1);
}

@cmd() carregar(playerid, params[], help)
{
    if(playerData[playerid][pJob] != JOB_PARCEL_CARRIER)
    {
        return SendClientMessage(playerid, COLOR_WARNING, "[ORPG] Você não pode executar este comando.");
    }

    if(playerData[playerid][pJob] == JOB_PARCEL_CARRIER && playerData[playerid][pIsWorking])
    {
        return SendClientMessage(playerid, COLOR_WARNING, "[EMPREGO] Você já está em uma entrega.");
    }

    if(!IsPlayerInRangeOfPoint(playerid, 50.0, 2199.4150,-2244.0508,13.5469))
    {
        return SendClientMessage(playerid, COLOR_WARNING, "[EMPREGO] Você precisa estar no centro de distribuição.");
    }

    if(GetPlayerVehicleID(playerid) != playerData[playerid][pWorkVehicle])
    {
        return SendClientMessage(playerid, COLOR_WARNING, "[EMPREGO] Vocẽ precisa estar no veículo da empresa.");
    }

    SendClientMessage(playerid, COLOR_SUCCESS, "[ENTREGA] Você iniciou os serviços de entrega. Faça 5 entregas para concluir.");
    GetParcelDeliveryJob(playerid);
    return 1;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if((newkeys & KEY_YES) && !(oldkeys & KEY_YES))
    {
        if(playerData[playerid][pJob] == JOB_PARCEL_CARRIER)
        {
            new Float:distance = GetPlayerDistanceFromPoint(playerid, 2199.4150,-2244.0508,13.5469);

            if(distance < 1)
            {
                if(playerData[playerid][pWorkVehicle] != 0)
                {
                    return ShowPlayerDialog(
                        playerid,
                        DIALOG_DELIVERY_JOB_RETURN_VEHICLE,
                        DIALOG_STYLE_MSGBOX,
                        "Devolver",
                        "Você deseja devolver o veículo?\n\nSeu trabalho será cancelado.",
                        "Sim",
                        "Não"
                    );
                }

                playerData[playerid][pWorkVehicle] = CreateVehicle(413, 2196.9231, -2234.0515, 13.5469, 0.0, -1, -1, -1);
                SendClientMessage(playerid, COLOR_SUCCESS, "[EMPREGO] O veículo foi alugado.");
                SendClientMessage(playerid, COLOR_SUCCESS, "[EMPREGO] Digite /carregar para iniciar as entregas.");
            }
        }
    }

    return 1;
}

hook OnPlayerEnterCheckpoint(playerid) 
{
    if(playerData[playerid][pJob] != JOB_PARCEL_CARRIER) return 1;

    if(playerData[playerid][pIsWorking] && GetPlayerVehicleID(playerid) == playerData[playerid][pWorkVehicle] || playerData[playerid][pIsWorking] && !IsPlayerInAnyVehicle(playerid))
    {
        DisablePlayerCheckpoint(playerid);

        PlayerPlaySound(playerid, 1058, 0.0, 0.0, 0.0);
        new payment = PARCEL_BASE_TAX + floatround(GetPlayerDistanceFromPoint(playerid, 2199.4150, -2244.0508, 13.5469) * PARCEL_PRICE_PER_METER, floatround_round);
        new deliveryText[128];

        format(deliveryText, sizeof(deliveryText), "[ENTREGA] Você recebeu: R$%d.", payment);

        SendClientMessage(playerid, COLOR_SUCCESS, "[ENTREGA] Entrega realizada com sucesso!");
        SendClientMessage(playerid, COLOR_SUCCESS, deliveryText);
        GivePlayerMoneyEx(playerid, payment);
        
        deliveryMan[playerid][deliveries]++;

        if(deliveryMan[playerid][deliveries] < 5)
        {
            GetParcelDeliveryJob(playerid);
            return 1;
        }

        SendClientMessage(playerid, COLOR_SUCCESS, "[ENTREGA] Você realizou 5 entregas! Volte ao centro de distribuição para pegar mais encomendas.");

        deliveryMan[playerid][deliveries] = 0;
        playerData[playerid][pIsWorking] = false;
    }

    if(!playerData[playerid][pIsWorking])
    {
        DisablePlayerCheckpoint(playerid);
    }
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_DELIVERY_JOB_RETURN_VEHICLE && response)
    {
        CancelWork(playerid);
        SendClientMessage(playerid, COLOR_SUCCESS, "[EMPREGO] Seu veículo foi devolvido e seu trabalho foi cancelado.");
    }

    return 1;
}

hook OnPlayerDisconnect(playerid)
{
    CancelWork(playerid);
}

stock GetParcelDeliveryJob(playerid)
{
    new randomPoint;
    
    do {
        randomPoint = random(sizeof(parcelDeliveryPositions));
    } while(IsPlayerInRangeOfPoint(playerid, 5.0, parcelDeliveryPositions[randomPoint][dPX], parcelDeliveryPositions[randomPoint][dPY], parcelDeliveryPositions[randomPoint][dPZ]));

    SetPlayerCheckpoint(playerid, parcelDeliveryPositions[randomPoint][dPX], parcelDeliveryPositions[randomPoint][dPY], parcelDeliveryPositions[randomPoint][dPZ], 2);
    playerData[playerid][pIsWorking] = true;

    SendClientMessage(playerid, COLOR_HEADSUP, "[ENTREGA] Foi marcado um checkpoint, entregue no lugar marcado para completar a entrega.");
    return 1;
}