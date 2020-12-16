#include<iostream>
#include "Map.h"
using namespace MertcanOzdemir;
void CustomMap::SetSize(const int w, const int h)
{
    mapData.clear();
    mapHeight = h;    //Set map height and width
    mapWidth = w;
}
void CustomMap::SetBorder(const int x, const int y)
{
    //Set an coordinate to -1, -1 represents border
    mapData[y * mapWidth + x] = true;
}
void CustomMap::GetSize(int &width, int &height)
{
    //set output according to data
    width = mapWidth;
    height = mapHeight;
}
void CustomMap::ClearBorder(const int x, const int y)
{
    //Set an coordinate to 0, 0 means zone note determined
    mapData[y * mapWidth + x] = false;
}

CustomMap::~CustomMap()
{
    //Delete allocated data so we won't create memory leak
    mapData.clear();
}
bool CustomMap::IsBorder(const int x, const int y){
  return mapData[y * mapWidth + x];

}
void CustomMap::Show()
{
    //A basic command line representation of map.
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            if (IsBorder(x, y))
            {
                //Show borders with - character
                std::cout << "-"
                          << " ";
            }
            else
            {
                //Show zones with zone number
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
}
