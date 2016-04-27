/* Wall kit */
// Modified to draw Ice + not allowed too close to flag

#include WallKit

func Initialize()
{
	SetInfiniteStackCount();
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
		preview->SetColor(0xff80ffff);
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
local BridgeLength = 15;
local BridgeThickness = 5;
local BridgeMaterial = "Ice-ice";
