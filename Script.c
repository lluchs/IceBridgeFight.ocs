/* Hot ice */

static DEBUG;

func Initialize()
{
	DEBUG = false;
	Meteor->SetChance(10);
	CreateEffect(CheckLandscapeFill, 10, 200);
}

func InitializePlayer(int plr)
{
	// everything visible
	SetFoW(false, plr);
	SetPlayerViewLock(plr, true);

	return true;
}

func OnPlayerRelaunch(int plr)
{
	var crew = GetCrew(plr);

	// Player positioning. 
	var ls_wdt = LandscapeWidth(), ls_hgt = LandscapeHeight(), border = 20;
	var crew = GetCrew(plr);
	crew->SetPosition(border + Random(ls_wdt - border), border + Random(ls_hgt * 4/5 - border));

	crew.MaxContentsCount = 2;
	var wallkit = crew->CreateContents(IceWallKit);
	wallkit->ControlUseStop(crew, crew->GetX(), crew->GetY() + 200);
	crew->CreateContents(WindBag);

	crew.MaxEnergy = 100000;
	crew->DoEnergy(1000);

	crew->CreateEffect(CheckTrapped, 10, 100);
}

static const Drip = new Global {
	Construction = func(object target, pxs)
	{
		this.pxs = pxs;
	},

	Timer = func(object target)
	{
		for (var i = 0; i < this.pxs; i++)
			target->InsertMaterial(Material("DuroLava"));
	},
};

static const CheckTrapped = new Global {
	Construction = func(object target)
	{
		this.target = target;
	},

	Timer = func(object target)
	{
		var step = 30;
		var x = target->GetX(), y = target->GetY();
		var tx = LandscapeWidth() / 2 / step * step + x % step, ty = LandscapeHeight() / step * step - 3*step + y % step;
		target->AsyncAStarMap({x=x, y=y}, {x=tx, y=ty}, step, {
			callback = this,
			max_time = 50,
		});
	},

	Done = func(path, checked_points)
	{
		if (path)
		{
			if (DEBUG)
				for (var pt in path)
					CreateParticle("Magic", pt.x, pt.y, 0, 0, 50);
			if (this.drip_effect)
			{
				RemoveEffect(nil, this.target, this.drip_effect);
				this.drip_effect = nil;
			}
		}
		else
		{
			if (!this.drip_effect)
				this.drip_effect = this.target->CreateEffect(Drip, 10, 1, 2);

			// The drip effect always creates PXS. Due to the PXS limit, it's a lot more efficient
			// to add material to an existing lava lake. Not making the player wait a long time for
			// death is obviously a good thing :)
			var lava = Material("DuroLava");
			for (var pt in checked_points)
				if (GetMaterial(pt.x, pt.y) == lava)
				{
					if (DEBUG) CreateParticle("Magic", pt.x, pt.y, 0, 0, 50, {R=255, G=0, B=0});
					for (var i = 0; i < 50; i++)
						if (!InsertMaterial(lava, pt.x, pt.y))
							break;
				}
				else
				{
					if (DEBUG) CreateParticle("Magic", pt.x, pt.y, 0, 0, 50, {R=0, G=255, B=0});
				}
		}
	},
};

static const CheckLandscapeFill = new Global {
	Construction = func()
	{
		this.blocked = 0;
	},

	Timer = func(target)
	{
		var step = 80;
		var x = Random(LandscapeWidth()), y = 0;
		var tx = x, ty = LandscapeHeight()*9/10 / step * step + y % step;
		target->AsyncAStarMap({x=x, y=y}, {x=tx, y=ty}, step, {
			callback = this,
			max_time = 50,
		});
	},

	Done = func(path, checked_points)
	{
		if (path)
		{
			if (DEBUG)
				for (var pt in path)
					CreateParticle("Magic", pt.x, pt.y, 0, 0, 50);
			this.blocked = 0;
		}
		else
		{
			// Someone blocked the landscape, try to free it using meteors!
			this.blocked++;
			for (var i = 0; i < this.blocked; i++)
			{
				var x = Random(LandscapeWidth());
				var y = 0;
				var size = RandomX(60, 90);
				var xdir = RandomX(-22, 22);
				var ydir = RandomX(28, 36);
				LaunchMeteor(x, y, size, xdir, ydir);
			}
		}
	},
};
