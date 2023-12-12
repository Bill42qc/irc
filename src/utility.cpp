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

void password_check (std::string serverPassword, std::string clientPassword)
{
    std::cout << "client password = " + clientPassword << std::endl;
    std::cout << "server password = " + serverPassword << std::endl;
    if(serverPassword == clientPassword)
        std::cout <<GRE "Password " + clientPassword +  " Acepted, Now Connecting ..." WHT<< std::endl;
    else
        throw std::runtime_error( RED "PASSWORD" + clientPassword +  " Is WRONG, try again" WHT);//TODO changer pour le code d'erreur
}
