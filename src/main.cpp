#include <iostream>
#include "ZoneCounter.h"

//Border data is stored in one array like x1,y1,x2,y2,x3,y3
int  defaultBorders[] = {33,0,35,0,33,1,35,1,33,2,35,2,33,3,35,3,33,4,35,4,33,5,35,5,33,6,35,6,33,7,35,7,32,8,35,8,32,9,34,9};

int main()
{

	MertcanOzdemir::CustomMap *map = new MertcanOzdemir::CustomMap();
	map->SetSize(36, 24);
	for (int i = 0; i < sizeof(defaultBorders) / sizeof(int) / 2; i++)
	{
		map->SetBorder(defaultBorders[i * 2], defaultBorders[i * 2 + 1]);
	}
	MertcanOzdemir::CustomZoneCounter counter;
	counter.Init(map);
	int zoneCount = counter.Solve();

	map->Show();
	std::cout << "There are " << zoneCount << " zones" << std::endl;
}