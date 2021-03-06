// Called be the engine: draw the complete map here.
public func InitializeMap(proplist map)
{
	Resize(40, 30);

	var w = map.Wdt, h = map.Hgt;
	// Just a lava lake at the bottom.
	map->Draw("DuroLava:DuroLava", nil, [0, h*4/5, w, h/5]);

	return true;
}
