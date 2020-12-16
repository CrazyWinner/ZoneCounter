
#include <iostream>
#include "ZoneCounter.h"
using namespace MertcanOzdemir;



void CustomZoneCounter::Init(MapInterface *map)
{
    mapToSolve = map;
    delete[] mapData;
    int mapWidth,mapHeight;
    map->GetSize(mapWidth,mapHeight);
    mapData = new int[mapWidth*mapHeight]();
    
};
int CustomZoneCounter::Solve()
{
    int mapHeight,mapWidth;
    mapToSolve->GetSize(mapWidth, mapHeight);

    //Iterate data and check for interferences, if a coordinate interferes with a zone it's zone number will be set to zone that it interfered
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            if (!mapToSolve->IsBorder(x, y))
            {                                    //If it's a border don't touch it
                int a = CheckInterference(x, y,mapWidth,mapHeight); //if it's not a border check zone interference, if there is no interference it will return -1
                                                 //if there is a interference it will return the zone interfered
                if (a == -1)
                { //Inteference not detected, create a new zone and increase zone count
                    ZoneCount++;
                    *getZoneAt(x, y,mapWidth) = ZoneCount;
  
                }
                else
                {
                    *getZoneAt(x, y,mapWidth) = a; //Set zone to interfered one

                }
            }
        }
    }
    return ZoneCount; // return the zone count
};
CustomZoneCounter::~CustomZoneCounter() {}

int *CustomZoneCounter::getZoneAt(const int x, const int y,const int& mapWidth)
{
    //Get zone from a coordinate
    return &mapData[y * mapWidth + x];
}

bool CustomZoneCounter::isValidCoordinate(const int x,const int y,const int& mapWidth,const int& mapHeight)
{
    if (x < 0 || y < 0)
        return false;
    if (x > mapWidth - 1 || y > mapHeight - 1)
        return false;
    return true;
}
int CustomZoneCounter::CheckInterference(int x, int y,const int& mapWidth,const int& mapHeight)
{

    //Interference means there is a direct path to other zone in 3x3 square.
    /*
          If we are checking for coordinate c 
          
          x x x
          x c x
          x x x 
          We will check this square 

          1 1 x
          x c x
          x x x
          For example we have interference with only 1 zone. But what if we have 2?

          1 1 x
          x c 2
          x x 2
          We have interference with 1 and 2 at the same coordinate. So they must be the same zone.
          We will combine to zones to the lower one, in this case we will combine second and first zone to first zone
        */

    int directPathCount = 0;
    int directPaths[2];
    for (int i = -1; i <= 1; i++)
    { //check an 3x3 square around our coordinate
        for (int j = -1; j <= 1; j++)
        { //this will iterate from x-1,y-1 to x+1,y+1

            if (isValidCoordinate(x + j, y + i,mapWidth,mapHeight))
            { //We are checking for a 3x3 square so it can exceed the bounds. We need to check it
                if (!mapToSolve->IsBorder(x+j,y+i) && *getZoneAt(x + j, y + i,mapWidth) != 0)
                {                                       // If a zone determined in that coordinate
                    if (isThereADirectPath(x, y, j, i)) //Check if is there a direct path.
                    {

                        /*

                         BUGFIX: This was combining zones at third direct path. It should have been second.


                        */
                        directPaths[directPathCount] = *getZoneAt(x + j, y + i,mapWidth); //New direct path found
                        directPathCount++;
                        if (directPathCount > 1)
                        {
                            return CombineZones(directPaths[0], directPaths[1],mapWidth,mapHeight);
                        } //If there is more than one direct paths replace the lower number zone one with other
                    }
                }
            }
        }
    }
    if (directPathCount == 0)
        return -1;         //if there is no direct path return -1
    return directPaths[0]; //If there is only one direct path return that
}
void CustomZoneCounter::ShowSolved()
{
    int mapWidth,mapHeight;
    mapToSolve->GetSize(mapWidth,mapHeight);
    //A basic command line representation of map.
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            if (mapToSolve->IsBorder(x, y))
            {
                //Show borders with - character
                std::cout << "-"
                          << " ";
            }
            else
            {
                //Show zones with zone number
                std::cout << mapData[y * mapWidth + x] << " ";
            }
        }
        std::cout << std::endl;
    }
}
int CustomZoneCounter::CombineZones(int zone1, int zone2,const int& mapWidth, const int& mapHeight) 
{
    if (zone1 == zone2)
        return zone1; //If both direct paths lead to same zone do nothing.
    int x1 = (zone1 < zone2) ? zone1 : zone2; //x1 should be the lower number zone
    int x2 = (zone1 < zone2) ? zone2 : zone1; //x2 should be the higher number zone
    ZoneCount--;                              //We are replacing one zone with another so our zone count should decrease
    // we need to combine two zones and change the ones those higher than our higher number zone.

    /*
            If we have zones 
            1 2 3 4 5 6
            And if we combine zone 1 and 4 we should end up with
            1 2 3 1 4 5

            */
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            int *zoneAt = getZoneAt(j, i,mapWidth);
            if (*zoneAt == 0 && !mapToSolve->IsBorder(j,i))
            {
                return x1;
            } //If we reach a zero we're done
            if (*zoneAt == x2)
            {
                *zoneAt = x1;
            }
            else if (*zoneAt > x2)
            {
                *zoneAt = *zoneAt - 1;
            }
        }
    }
    return x1;
}
bool CustomZoneCounter::isThereADirectPath(int x, int y, int j, int i)
{
    /*
 
        
        - - - 
        - 0 -
        1 - - 
        This one doesn't have a direct path because we have a border in left side.
        ///////////////////////////////////////////////////////
        
        
        - - - 
        - 0 0
        - - 1
        But this one does. We have a direct path to zone 1. The next coordinate is not determined but it's not important. 
        The important thing is we have a direct path. 

     */

    if (j == 0 || i == 0)
        return true; //Left right up and down movement will be direct path no matter what
    if (j == 1 && i == 1)
        return (!mapToSolve->IsBorder(x + 1, y) || !mapToSolve->IsBorder(x, y + 1)); //But if we go diagonal, we need to check the coordinates in between
    if (j == -1 && i == 1)
        return (!mapToSolve->IsBorder(x - 1, y) || !mapToSolve->IsBorder(x, y + 1));
    if (j == 1 && i == -1)
        return (!mapToSolve->IsBorder(x + 1, y) || !mapToSolve->IsBorder(x, y - 1));
    if (j == -1 && i == -1)
        return (!mapToSolve->IsBorder(x - 1, y) || !mapToSolve->IsBorder(x, y - 1));
    /*
       - - - 
       - 0 0
       - - 1
       For example if we need to check is there a path to x+1,y+1

       - - -
       - 0 x
       - x 1
       We need to check x's. If one of them is not border we have a direct path

      */

    return false;
}