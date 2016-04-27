/* Hot ice */

func Initialize()
{
}

func InitializePlayer(int plr)
{
	// everything visible
	SetFoW(false, plr);
	SetPlayerViewLock(plr, true);

	// Player positioning. 
	var ls_wdt = LandscapeWidth(), ls_hgt = LandscapeHeight(), border = 20;
	var crew = GetCrew(plr);
	crew->SetPosition(border + Random(ls_wdt - border), border + Random(ls_hgt - border));
	OnPlayerRelaunch(plr);
	return true;
}

func OnPlayerRelaunch(int plr)
{
	var crew = GetCrew(plr);
	crew.MaxContentsCount = 2;
	var wallkit = crew->CreateContents(IceWallKit);
	wallkit->ControlUseStop(crew, crew->GetX(), crew->GetY() + 100);
	crew->CreateContents(WindBag);

	crew.MaxEnergy = 100000;
	crew->DoEnergy(1000);

	AddEffect("Drip", crew, 10, 1, crew);
}

global func FxDripTimer()
{
	for (var i = 0; i < 10; i++)
		InsertMaterial(Material("DuroLava"));
}
