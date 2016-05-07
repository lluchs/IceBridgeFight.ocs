/* Wall kit */
// Modified to draw Ice + not allowed too close to flag

#include WallKit

func Initialize()
{
	SetInfiniteStackCount();
}

public func ControlUseStop(object clonk, int x, int y, ...)
{
	StopPreview(clonk);
	if (!IsIceBridgeAllowed(clonk, x, y))
	{
		clonk->PlaySoundDoubt();
		return true;
	}
	else
	{
		return inherited(clonk, x, y, ...);
	}
}


private func IsIceBridgeAllowed(object clonk, int x, int y)
{
	var c = Offset2BridgeCoords(clonk, x, y);
	// Don't allow ice bridges at the top of the landscape.
	return clonk->GetY() + Min(c.y1, c.y2) > 30;
}

private func SetPreview(object clonk, int x, int y, ...)
{
	if (!preview) AddTimer(this.UpdateIcePreviewColor, 3);
	this.ice_last_clonk = clonk;
	this.ice_last_x = x;
	this.ice_last_y = y;
	var r = inherited(clonk, x, y, ...);
	UpdateIcePreviewColor();
	return r;
}

private func UpdateIcePreviewColor()
{
	if (preview)
	{
		var ok = IsIceBridgeAllowed(this.ice_last_clonk, this.ice_last_x, this.ice_last_y);
		if (ok)
			preview->SetColor(0xff80ffff);
		else
			preview->SetColor(0xffff0000);
	}
	return true;
}

private func StopPreview(object clonk, ...)
{
	RemoveTimer(this.UpdateIcePreviewColor);
	return inherited(clonk, ...);
}


/* Status */

local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local BridgeLength = 30;
local BridgeThickness = 5;
local BridgeMaterial = "Ice-ice";
