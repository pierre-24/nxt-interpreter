//
// Created by pierre on 13/04/2020.
//

#include <iostream>
#include "Simulation.h"
#include "Environment.h"
#include "Map.h"

Simulation *createSimulationFromMap(const std::string &path) {
    std::ifstream f;
    f.open(path);
    if (!f.is_open())
        throw std::runtime_error("cannot open file");

    return createSimulationFromStreamMap(f);
}

enum TokenType {
    Digit,
    SepNum,
    WhiteCell,
    BlackCell,
    WallCell,
    WS,
    EOF_
};

std::string tokenName(const int& type) {
    switch (type) {
        case Digit:
            return "Digit";
        case SepNum:
            return "SepNum";
        case WhiteCell:
            return "WhiteCell";
        case BlackCell:
            return "BlackCell";
        case WallCell:
            return "WallCell";
        case WS:
            return "WS";
        case EOF_:
            return "EOF";
        default:
            return "unknown token?";
    }
}

struct Token {
    TokenType type;
    int value;
};

Simulation *createSimulationFromStreamMap(std::istream& istream) { /* parser */
    auto parser = new MapParser(istream);
    return parser->createSimulation();
}

Token MapParser::getNextToken(std::istream &istream) {
    if(istream.eof())
        return {TokenType::EOF_, '\0'};

    int val = istream.get();

    if (isdigit(val))
        return {TokenType::Digit, val - '0'};

    switch (val) {
        case ' ':
        case '\n':
            return {TokenType::WS, val};
        case 0:
        case -1: // what the hell?
            return {TokenType::EOF_, '\0'};
        case ';':
            return {TokenType::SepNum, val};
        case 'o':
            return {TokenType::WhiteCell, val};
        case 'x':
            return {TokenType::BlackCell, val};
        case '*':
            return {TokenType::WallCell, val};
        default: {
            throw std::runtime_error(std::string("invalid character `") + char(val) + std::string("`"));
        }
    }
}

void MapParser::makeTokenList(std::istream &istream) {
    Token t;
    bool running = true;

    while(running) {
        t = getNextToken(istream);
        tokens.push_back(t);

        if(t.type == TokenType::EOF_) {
            running = false;
        }
    }
}

MapParser::MapParser(std::istream &istream) {
    makeTokenList(istream);
}

Simulation *MapParser::createSimulation() {
    rule_simulation();
    rule_robot_position();

    auto* env = new Environment(map_width, map_height, MapConstant::CellWidth, MapConstant::WallHeight);
    rule_map(env);

    skipWhiteSpace();
    eat(TokenType::EOF_);

    auto* simu = new Simulation(env);
    return simu;
}

unsigned int MapParser::rule_number() {
    int value = 0;
    if(tokens.front().type != TokenType::Digit)
        throw std::runtime_error(std::string("expected a digit, got ") + tokenName(tokens.front().type));

    bool running = true;
    while(running) {
        value = value * 10 + tokens.front().value;
        tokens.pop_front();
        if(tokens.front().type != TokenType::Digit)
            running = false;
    }

    return value;
}

int MapParser::rule_cell_value() {
    Token t = tokens.front();

    if (t.type != TokenType::WallCell && t.type != TokenType::WhiteCell && t.type != TokenType::BlackCell)
        throw std::runtime_error(std::string("expected a cell value, got ") + tokenName(t.type));

    tokens.pop_front();
    return t.type;
}

void MapParser::rule_simulation() {
    skipWhiteSpace();
    map_width = rule_number();
    skipWhiteSpace();
    eat(TokenType::SepNum);
    skipWhiteSpace();
    map_height = rule_number();
}

void MapParser::rule_robot_position() {
    skipWhiteSpace();
    robot_x = float(rule_number()) + MapConstant::CellWidth / 2;
    skipWhiteSpace();
    eat(TokenType::SepNum);
    skipWhiteSpace();
    robot_z = float(rule_number())  + MapConstant::CellWidth / 2;
}

void MapParser::rule_map(Environment *env) {
    unsigned expected_cells = map_width * map_height;

    for (int i = 0; i < expected_cells ; ++i) {
        skipWhiteSpace();
        int val = rule_cell_value();

        unsigned x = i % map_width, z = unsigned(i / map_width);

        if (val == TokenType::BlackCell)
            env->setCellShade(x, z, MapConstant::BlackCellShade);
        else if(val == TokenType::WallCell)
            env->setCellIsWall(x, z, true);
    }
}

void MapParser::skipWhiteSpace() {
    while(tokens.front().type == TokenType::WS)
        tokens.pop_front();
}

void MapParser::eat(int type) {
    if(tokens.front().type != type)
        throw std::runtime_error(std::string("expected ") + tokenName(type) + std::string(" got ") + tokenName(tokens.front().type));

    tokens.pop_front();
}
