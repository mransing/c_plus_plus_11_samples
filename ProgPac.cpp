/* A simple progmac game. The player can be on any point on 2x2 grid, facing any one the four direction. A player can rotate eithr to left or to right, and can take a step of one point. This simple program takes a point and one string of instructions as input. The instructions are as follows : -
   s - go one step ahead 
   l - rotate left
   r - rotate right
*/

#include <unordered_map>
#include <iostream>
#include <functional>
#include <vector>
#include <numeric>

enum Direction {Up, Down, Left, Right};

struct Point 
{
  int x;
  int y;
};

struct Position
{
  Point pp;
  Direction dr;
};

//Use of hash_map
namespace std{
  template<>
  class hash<Direction>
  { 
  public:
    size_t
    operator()(const Direction& __x) const
    { return (int)__x; }
  };
} 

//New Initialization List 
//This map is for printing the direction, can consider as show in haskell
std::unordered_map<Direction,std::string> mapDirStr = {{Up,"Up"},{Right,"Right"},{Left,"Left"},{Down,"Down"}};

//This is if left rotated in which direction will it end
std::unordered_map<Direction,Direction> mapLeft = {{Up,Left},{Left,Down},{Down,Right},{Right,Up}};

//Similary for right
std::unordered_map<Direction,Direction> mapRight = {{Left,Up},{Down,Left},{Right,Down},{Up,Right}};

//A simple function to print the posistion, same as show in haskell
std::string printp(const Position& p)
{
  char buf[100];
  sprintf(buf,"x-> %d y-> %d ", p.pp.x, p.pp.y);
  auto retStr = std::string(buf) + " " + mapDirStr[p.dr] + " ";
  return retStr;
}

Position goleft(const Position& p)
{
  Position pt = {p.pp,mapLeft[p.dr]};
  return pt;
};


Position goright(const Position& p)
{
  Position pt = {p.pp,mapRight[p.dr]};
  return pt;
};

//Lambda functions
//A map of the actions about what to do for particular direction for going straight
std::unordered_map<Direction,std::function<Point(Point)> > mapDirFunc = 
  { {Up, [](const Point& p)
	 {Point pt = {p.x, p.y + 1}; return pt;} },
    {Down, [](const Point& p)
	 {Point pt = {p.x, p.y - 1}; return pt;}},
    {Left, [](const Point& p)
	 {Point pt = {p.x - 1, p.y}; return pt;}},
    {Right, [](const Point& p)
	 {Point pt = {p.x + 1, p.y}; return pt;}}
  };

Position gostr(const Position& p)
{
  Position pt = {(mapDirFunc[p.dr])(p.pp), p.dr};
  return pt;
};

//std::function template
//A map of which action to take on which character 
std::unordered_map<char,std::function<Position(Position)> > mapCharFuncs= {{'l',goleft},{'r',goright},{'s',gostr}};

//Similar to map in map in haskell, given a list of characters return the list of actions.
std::vector<std::function<Position(Position)> > getVecFuncPos(const std::string& str)
{
  std::vector<std::function<Position(Position)> > vec;
  for(auto p=str.begin();p!=str.end();++p)
    {
      vec.push_back(mapCharFuncs[*p]);
    }
  return vec;
};
 
int main()
{
  //These are inputs 
  Position ps = {{1,2},Up};
  std::string initStr="slssrsss";

  //Get the list of actions given the list of characters
  auto vecFuncs = getVecFuncPos(initStr);
  //Same as fold in haskell, given the list of action and initial position, perform the actions on position in sequence
  Position fin = std::accumulate(vecFuncs.begin(),vecFuncs.end(),ps,
				 [](const Position& ps, std::function<Position(Position)> func){return func(ps);});

  std::cout << "Inital point "  + printp(ps) + " after the actions " + initStr + " becomes " + printp(fin) << std::endl;
  return 0;
}
