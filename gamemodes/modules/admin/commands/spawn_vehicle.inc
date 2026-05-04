@cmd() veiculo(playerid, params[], help)
{
    if(IsNull(params))
    {
        return SendClientMessage(playerid, COLOR_WARNING, "[ADMIN] Uso correto: /veiculo [id]");
    }

    new modelid = strval(params);
    if(modelid < 400 || modelid > 611)
    {
        return SendClientMessage(playerid, COLOR_WARNING, "[ADMIN] Uso correto: /veiculo [400-611]");
    }

    new Float:pX, Float:pY, Float:pZ;
    GetPlayerPos(playerid, pX, pY, pZ);

    new vehicleid = CreateVehicle(modelid, pX, pY, pZ, 0, -1, -1, -1);
    PutPlayerInVehicle(playerid, vehicleid, 0);
    return 1;
}