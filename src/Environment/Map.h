//
// Created by pierre on 13/04/2020.
//

#include <string>
#include <fstream>
#include <list>

class Simulation;
class Environment;
struct Token;

namespace MapConstant {
    const float BlackCellShade = .9f;
    const float CellWidth = 1.f;
    const float WallHeight = .75f;
}

class MapParser {
    std::list<Token> tokens;

    unsigned map_width;
    unsigned map_height;
    float robot_x;
    float robot_z;

    // lexer
    Token getNextToken(std::istream& istream);
    void makeTokenList(std::istream& istream);

    void skipWhiteSpace();

    void eat(int type);

    // parser
    unsigned int rule_number(); // number := Digit+
    int rule_cell_value(); // cell_value := BlackCell | WhiteCell | WallCell

    void rule_simulation(); // simulation := number SepNum number
    void rule_robot_position(); // robot_position := number SepNum number
    void rule_map(Environment *env); // map := cell_value*

public:
    MapParser(std::istream& istream);

    Simulation* createSimulation(); // simulation_env := simulation robot_position map

    float getRobotX() const { return robot_x;}

    float getRobotZ() const { return robot_z; }
};

Simulation* createSimulationFromMap(const std::string& path);
Simulation* createSimulationFromStreamMap(std::istream& istream);