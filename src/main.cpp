#include <iostream>
#include "ZoneCounter.h"
#include "Map.h"

//Border data is stored in one array like x1,y1,x2,y2,x3,y3
int  defaultBorders[] = {9,0,10,0,23,0,24,0,8,1,9,1,10,1,22,1,23,1,24,1,7,2,8,2,10,2,11,2,22,2,24,2,25,2,6,3,7,3,10,3,11,3,12,3,21,3,25,3,5,4,6,4,10,4,12,4,13,4,21,4,25,4,4,5,5,5,10,5,14,5,20,5,25,5,26,5,2,6,3,6,10,6,14,6,15,6,20,6,26,6,1,7,2,7,10,7,16,7,19,7,26,7,27,7,0,8,1,8,10,8,17,8,19,8,27,8,0,9,10,9,18,9,27,9,10,10,19,10,27,10,10,11,19,11,20,11,28,11,35,11,10,12,20,12,21,12,28,12,33,12,34,12,10,13,21,13,28,13,29,13,32,13,33,13,10,14,21,14,22,14,29,14,30,14,31,14,32,14,10,15,22,15,23,15,29,15,30,15,10,16,23,16,24,16,28,16,29,16,9,17,10,17,11,17,12,17,13,17,14,17,15,17,16,17,17,17,18,17,19,17,20,17,21,17,22,17,23,17,24,17,25,17,26,17,27,17,8,18,9,18,27,18,7,19,27,19,6,20,7,20,27,20,5,21,6,21,27,21,4,22,5,22,27,22,3,23,4,23,27,23};

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
	counter.ShowSolved();
}