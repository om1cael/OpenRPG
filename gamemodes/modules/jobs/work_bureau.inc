#include <YSI_Coding\y_hooks>
#include "modules/utils"

#define BUILDING_ENTER_DETECT_RANGE 2.0

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
        if(GetPlayerDistanceFromPoint(playerid, 1219.1830,-1813.1396,16.5938) < BUILDING_ENTER_DETECT_RANGE)
        {
            SetPlayerInterior(playerid, 3);
            SetPlayerPos(playerid, 1494.4944,1304.1597,1093.2891);
            return 1;
        }

        if(GetPlayerDistanceFromPoint(playerid, 1494.4944,1304.1597,1093.2891) < BUILDING_ENTER_DETECT_RANGE)
        {
            SetPlayerInterior(playerid, 0);
            SetPlayerPos(playerid, 1219.1830,-1813.1396,16.5938);
            return 1;
        }
    }

    return 1;
}