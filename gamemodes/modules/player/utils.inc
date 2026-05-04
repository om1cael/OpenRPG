stock GivePlayerMoneyEx(playerid, amount) 
{
    ResetPlayerMoney(playerid);

    playerData[playerid][pMoney] += amount;
    GivePlayerMoney(playerid, playerData[playerid][pMoney]);
}