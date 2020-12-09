#include "Interfaces.h"

namespace MertcanOzdemir
{

  class CustomMap : public MapInterface
  {
  public:
    int *mapData = 0;
    int mapHeight, mapWidth;
    ~CustomMap();
    void SetSize(const int w, const int h);
    void GetSize(int &width, int &height);
    void SetBorder(const int x, const int y);
    void ClearBorder(const int x, const int y);
    bool IsBorder(const int x, const int y);
    int *getZoneAt(const int x, const int y);
    bool isValidCoordinate(const int x, const int y);
    void Show();
  };
  class CustomZoneCounter : public ZoneCounterInterface
  {
  private:
    CustomMap *mapToSolve;
    int ZoneCount = 0;

  public:
    ~CustomZoneCounter();
    void Init(MapInterface *map);
    int Solve();
    bool isThereADirectPath(int x, int y, int j, int i);
    int CheckInterference(int width, int height);
    int CombineZones(int zone1, int zone2);
  };
  ZoneCounterInterface *getZoneCounter();

} // namespace MertcanOzdemir