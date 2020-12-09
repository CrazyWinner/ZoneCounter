
#include <iostream>
#include "ZoneCounter.h"
using namespace MertcanOzdemir;

void CustomMap::SetSize(const int w, const int h)
{
    delete[] mapData; //Delete from memory if it's already allocated so we won't have memory leak
    mapHeight = h;    //Set map height and width
    mapWidth = w;
    mapData = new int[w * h](); //Create an array to hold map data and init with all zeros
}
void CustomMap::SetBorder(const int x, const int y)
{
    //Set an coordinate to -1, -1 represents border
    mapData[y * mapWidth + x] = -1;
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
    mapData[y * mapWidth + x] = 0;
}
bool CustomMap::IsBorder(const int x, const int y)
{
    //Check if a coordinate border (means if it's -1), if it's not a valid coordinate still return it as border because we don't have outer borders
    return !isValidCoordinate(x, y) || mapData[y * mapWidth + x] == -1;
}
CustomMap::~CustomMap()
{
    //Delete allocated data so we won't create memory leak
    delete[] mapData;
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
                std::cout << mapData[y * mapWidth + x] << " ";
            }
        }
        std::cout << std::endl;
    }
}
int *CustomMap::getZoneAt(const int x, const int y)
{
    //Get zone from a coordinate
    return &mapData[y * mapWidth + x];
}

void CustomZoneCounter::Init(MapInterface *map)
{
    mapToSolve = (CustomMap *)map;
};
int CustomZoneCounter::Solve()
{

    //Iterate data and check for interferences, if a coordinate interferes with a zone it's zone number will be set to zone that it interfered
    for (int y = 0; y < mapToSolve->mapHeight; y++)
    {
        for (int x = 0; x < mapToSolve->mapWidth; x++)
        {
            if (!mapToSolve->IsBorder(x, y))
            {                                    //If it's a border don't touch it
                int a = CheckInterference(x, y); //if it's not a border check zone interference, if there is no interference it will return -1
                                                 //if there is a interference it will return the zone interfered
                if (a == -1)
                { //Inteference not detected, create a new zone and increase zone count
                    ZoneCount++;
                    *mapToSolve->getZoneAt(x, y) = ZoneCount;
                }
                else
                {
                    *mapToSolve->getZoneAt(x, y) = a; //Set zone to interfered one
                }
            }
        }
    }
    return ZoneCount; // return the zone count
};
CustomZoneCounter::~CustomZoneCounter() {}
bool CustomMap::isValidCoordinate(int x, int y)
{
    if (x < 0 || y < 0)
        return false;
    if (x > mapWidth - 1 || y > mapHeight - 1)
        return false;
    return true;
}
int CustomZoneCounter::CheckInterference(int x, int y)
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

            if (mapToSolve->isValidCoordinate(x + j, y + i))
            { //We are checking for a 3x3 square so it can exceed the bounds. We need to check it
                if (*mapToSolve->getZoneAt(x + j, y + i) != -1 && *mapToSolve->getZoneAt(x + j, y + i) != 0)
                {                                       // If a zone determined in that coordinate
                    if (isThereADirectPath(x, y, j, i)) //Check if is there a direct path.
                    {

                        /*

                         BUGFIX: This was combining zones at third direct path. It should have been second.


                        */
                        directPaths[directPathCount] = *mapToSolve->getZoneAt(x + j, y + i); //New direct path found
                        directPathCount++;
                        if (directPathCount > 1)
                        {
                            return CombineZones(directPaths[0], directPaths[1]);
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

int CustomZoneCounter::CombineZones(int zone1, int zone2)
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
    for (int i = 0; i < mapToSolve->mapHeight; i++)
    {
        for (int j = 0; j < mapToSolve->mapWidth; j++)
        {
            int *zoneAt = mapToSolve->getZoneAt(j, i);
            if (*zoneAt == 0)
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