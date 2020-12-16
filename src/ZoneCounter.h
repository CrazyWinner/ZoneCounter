#pragma once
#include "Interfaces.h"

namespace MertcanOzdemir
{


  class CustomZoneCounter : public ZoneCounterInterface
  {
  private:
    MapInterface *mapToSolve;
    int ZoneCount = 0;
    int* mapData = 0;
  public:
    bool isValidCoordinate(const int x,const int y,const int& mapWidth,const int& mapHeight);
    int *getZoneAt(const int x, const int y,const int& mapWidth);
    ~CustomZoneCounter();
    void Init(MapInterface *map);
    int Solve();
    void ShowSolved();
    bool isThereADirectPath(int x, int y, int j, int i);
    int CheckInterference(int x, int y,const int& mapWidth,const int& mapHeight);
    int CombineZones(int zone1, int zone2,const int& mapWidth, const int& mapHeight) ;
  };
  ZoneCounterInterface *getZoneCounter();

} // namespace MertcanOzdemir