#include "utility.hpp"

std::vector<std::string> splitString(const std::string &input, char limiter)
{
    std::vector<std::string> result;
    std::string mot;

    for (size_t i = 0; i < input.size(); ++i)
    {
        if (input[i] != limiter){
            mot += input[i];
        }
        else{
            if (!mot.empty()){
                result.push_back(mot);
                mot.clear();
            }
        }
    }
    if (!mot.empty()){
        result.push_back(mot);
    }

    return result;
}