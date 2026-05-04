#define BUILDING_ENTER_DETECT_RANGE 2.0

new workBureauPickup[MAX_PLAYERS];
new workBureauExitPickup[MAX_PLAYERS];

hook OnPlayerConnect(playerid)
{
    workBureauPickup[playerid] = CreatePlayerPickup(playerid, 1239, 0, 1219.1830,-1813.1396,16.5938, 0);
    Create3DTextLabel("Secretaria do Trabalho\nAperte F para entrar", -1, 1219.1830,-1813.1396,16.5938, 25, 0);

    workBureauExitPickup[playerid] = CreatePlayerPickup(playerid, 1239, 0, 1494.4944,1304.1597,1093.2891, 0);
    Create3DTextLabel("Secretaria do Trabalho\nAperte F para sair", -1, 1494.4944,1304.1597,1093.2891, 2, 0);
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