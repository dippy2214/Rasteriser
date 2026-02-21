#include "FloatTypes.h"

std::string Color::ToString()
{
    std::string output;
    output += std::to_string(r);
    output += ", ";
    output += std::to_string(g);
    output += ", ";
    output += std::to_string(b);
    output += ", ";
    output += std::to_string(a);
    return output;
}
