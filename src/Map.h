  #pragma once
  #include "Interfaces.h"
  #include <map>
  namespace MertcanOzdemir
{

  class CustomMap : public MapInterface
  {
  private:
     std::map<int,bool> borders;    
     int mapHeight, mapWidth;
  public:
    ~CustomMap();
    void SetSize(const int w, const int h);
    void GetSize(int &width, int &height);
    void SetBorder(const int x, const int y);
    void ClearBorder(const int x, const int y);
    bool IsBorder(const int x, const int y);
    void Show();
  };
  
  }