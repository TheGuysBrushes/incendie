#include "coordonnee.h"

Coordonnee::Coordonnee(int _x, int _y)
    :col(_x), row(_y)
{}

Coordonnee::Coordonnee(const Coordonnee& other)
    :col(other.col), row(other.row)
{}

bool Coordonnee::operator==(const Coordonnee& other) const
{
    return (other.col==col && other.row==row);
}

std::ostream& Coordonnee::print(std::ostream& out) const
{
    out << "(" << col << "," << row << ")";
    return out;
}

unsigned Coordonnee::manhattanDistance(const Coordonnee& other) const
{
    assert(abs(col- other.col) + abs(row - other.row) >= 0);
    return abs(col- other.col) + abs(row - other.row);
}

float Coordonnee::euclideanDistance(const Coordonnee& other) const
{
    assert(sqrt( pow((col - other.col), 2)  + pow((row - other.row), 2)) >= 0.0);
    // square-root(square(x1 - x2) + square(y1 - y2))
    return sqrt( pow((col - other.col), 2)  + pow((row - other.row), 2));
}

