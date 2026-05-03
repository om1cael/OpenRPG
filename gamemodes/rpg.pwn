#include <open.mp>
#include <a_mysql>
#include <samp_bcrypt>

#include "modules/database"
#include "modules/colors"

#include "modules/accounts/core"

main()
{
	printf("Hello World!");
}

public OnGameModeInit()
{
	SetGameModeText("RPG - 2026");
	AddStaticVehicle(522, 823.4052,-1341.5691,13.5167,89.2287, -1, -1);
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    SetPlayerPos(playerid, 1212.4067,-1388.6460,13.2738);
    SetPlayerInterior(playerid, 0);
    SetPlayerFacingAngle(playerid, 180.0);
    SetPlayerCameraPos(playerid, 1212.4067, -1388.6460, 15.2738);
    SetPlayerCameraLookAt(playerid, 1240.0, -1350.0, 14.0); 
	return 1;
}