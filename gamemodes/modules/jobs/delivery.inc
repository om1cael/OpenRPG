#include <YSI_Visual/y_commands>

#define BASE_DELIVERY_TAX 10
#define PRICE_PER_METER 0.03

#define DELIVERY_JOB_BIKE_PRICE 100

enum DeliveryManData
{
    deliveries
}

enum DeliveryPos
{
    Float:dPX,
    Float:dPY,
    Float:dPZ
}

new const deliveryPositions[][DeliveryPos] =
{
    {904.6049, -1447.3535, 13.5525},
    {942.9338,-1479.8563,13.5335},
    {763.7391, -1564.4473, 13.5469},
    {738.9736, -1416.2937, 13.5234},
    {727.6375, -1276.2643, 13.6484},
    {397.9998, -1270.8383, 50.0198},
    {354.3398, -1279.5974, 53.7036},
    {1413.5890, -926.1986, 36.0624},
    {2000.2172, -1119.4436, 26.7813},
    {2087.7517, -1185.2821, 25.9382},
    {1686.7675, -1343.3375, 17.4291}
};

new deliveryMan[MAX_PLAYERS][DeliveryManData];
new playerDeliveryPickup[MAX_PLAYERS];

hook OnPlayerConnect(playerid)
{
    playerDeliveryPickup[playerid] = CreatePlayerPickup(playerid, 1582, 0, 1026.8573, -1342.4838, 13.7266, 0);
    Create3DTextLabel("Entregador\nAperte Y para pegar seu veículo", -1, 1026.8573, -1342.4838, 13.7266, 15, 0);
}

@cmd() trabalhar(playerid, params[], help)
{
    playerData[playerid][pJob] = 1;
    SendClientMessage(playerid, COLOR_SUCCESS, "[EMPREGO] Você agora é um entregador, vá até a marcação no mapa para pegar seu veículo.");
    SetPlayerCheckpoint(playerid, 1026.8573, -1342.4838, 13.7266, 2);
    return 1;
}

@cmd() entregar(playerid, params[], help)
{
    if(playerData[playerid][pJob] != 1)
    {
        SendClientMessage(playerid, COLOR_WARNING, "[ORPG] Você não pode executar este comando.");
        return 1;
    }

    if(playerData[playerid][pJob] == 1 && playerData[playerid][pIsWorking])
    {
        SendClientMessage(playerid, COLOR_WARNING, "[EMPREGO] Você já está em uma entrega.");
        return 1;
    }

    if(GetPlayerVehicleID(playerid) != playerData[playerid][pWorkVehicle])
    {
        SendClientMessage(playerid, COLOR_WARNING, "[EMPREGO] Vocẽ precisa estar no veículo da empresa.");
        return 1;
    }

    SendClientMessage(playerid, COLOR_SUCCESS, "[ENTREGA] Você iniciou os serviços de entrega. Faça 10 entregas para concluir.");
    GetDeliveryJob(playerid);
    return 1;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if((newkeys & KEY_YES) && !(oldkeys & KEY_YES))
    {
        if(playerData[playerid][pJob] == 1)
        {
            new Float:distance = GetPlayerDistanceFromPoint(playerid, 1026.8573, -1342.4838, 13.7266);

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

                ShowPlayerDialog(
                    playerid,
                    DIALOG_DELIVERY_JOB_VEHICLE,
                    DIALOG_STYLE_TABLIST_HEADERS,
                    "Veículo",
                    "Tipo\tCusto\tNível\nBike\tGrátis\t0\nMoto\tR$100\t1",
                    "Alugar",
                    "Cancelar"
                );
            }
        }
    }

    return 1;
}

hook OnPlayerEnterCheckpoint(playerid) 
{
    if(playerData[playerid][pIsWorking] && GetPlayerVehicleID(playerid) == playerData[playerid][pWorkVehicle])
    {
        new payment = BASE_DELIVERY_TAX + floatround(GetPlayerDistanceFromPoint(playerid, 1026.8573, -1342.4838, 13.7266) * PRICE_PER_METER, floatround_round);
        new deliveryText[128];

        format(deliveryText, sizeof(deliveryText), "[ENTREGA] Você recebeu: R$%d.", payment);

        SendClientMessage(playerid, COLOR_SUCCESS, "[ENTREGA] Entrega realizada com sucesso!");
        SendClientMessage(playerid, COLOR_SUCCESS, deliveryText);
        GivePlayerMoney(playerid, payment);
        
        deliveryMan[playerid][deliveries]++;

        if(deliveryMan[playerid][deliveries] < 10)
        {
            GetDeliveryJob(playerid);
            return 1;
        }

        SendClientMessage(playerid, COLOR_SUCCESS, "[ENTREGA] Você realizou 10 entregas! Se quiser, volte ao restaurante para fazer mais.");
        DisablePlayerCheckpoint(playerid);

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
    if(dialogid == DIALOG_DELIVERY_JOB_VEHICLE && response)
    {
        if(listitem == 0)
        {
            playerData[playerid][pWorkVehicle] = CreateVehicle(509, 1026.8573, -1342.4838, 13.7266, 0.0, -1, -1, -1);
        }
        else if(listitem == 1)
        {
            if(playerData[playerid][pLevel] < 1)
            {
                SendClientMessage(playerid, COLOR_WARNING, "[EMPREGO] Você precisa estar pelo menos no nível 1 para usar este veículo.");
                return 1;
            }

            if(playerData[playerid][pMoney] < DELIVERY_JOB_BIKE_PRICE)
            {
                SendClientMessage(playerid, COLOR_WARNING, "[EMPREGO] Você não tem dinheiro suficiente.");
                return 1;
            }

            GivePlayerMoney(playerid, -DELIVERY_JOB_BIKE_PRICE);
            playerData[playerid][pWorkVehicle] = CreateVehicle(462, 1026.8573, -1342.4838, 13.7266, 0.0, -1, -1, -1);
        }
    }
    else if(dialogid == DIALOG_DELIVERY_JOB_RETURN_VEHICLE && response)
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

stock GetDeliveryJob(playerid)
{
    new randomPoint;
    
    do {
        randomPoint = random(sizeof(deliveryPositions));
    } while(IsPlayerInRangeOfPoint(playerid, 5.0, deliveryPositions[randomPoint][dPX], deliveryPositions[randomPoint][dPY], deliveryPositions[randomPoint][dPZ]));

    SetPlayerCheckpoint(playerid, deliveryPositions[randomPoint][dPX], deliveryPositions[randomPoint][dPY], deliveryPositions[randomPoint][dPZ], 2);
    playerData[playerid][pIsWorking] = true;

    SendClientMessage(playerid, COLOR_HEADSUP, "[ENTREGA] Foi marcado um checkpoint, entregue no lugar marcado para completar a entrega.");
    return 1;
}

stock CancelWork(playerid)
{
    DestroyVehicle(playerData[playerid][pWorkVehicle]);
    playerData[playerid][pWorkVehicle] = 0;
    playerData[playerid][pIsWorking] = false;
    deliveryMan[playerid][deliveries] = 0;
    DisablePlayerCheckpoint(playerid);

    return 1;
}