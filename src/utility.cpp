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
void addSpaceAfterKeywords(std::string& input) {
    // List of keywords to detect
    const char* keywords[] = {"NICK", "USER", "CAP"};

    // Iterate through each keyword
    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); ++i) {
        const char* keyword = keywords[i];
        size_t pos = 0;
        while ((pos = input.find(keyword, pos)) != std::string::npos) {
            // Check if the character after the keyword is not already a space
            if (pos + strlen(keyword) < input.length() &&
                !std::isspace(input[pos + strlen(keyword)])) {
                // Insert a space after the keyword
                input.insert(pos + strlen(keyword), " ");
            }
            // Move the position past the last occurrence to avoid an infinite loop
            pos += strlen(keyword) + 1;
        }
    }
}

void removeNewlines(std::string& str) {
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
}

void removeReturn(std::string& str) {
    size_t pos;
    
    // Remove carriage return characters '\r'
    while ((pos = str.find('\r')) != std::string::npos) {
        str.erase(pos, 1);
    }
}

void removeCRTL(std::string& str) {
    size_t pos;
    
    // Remove carriage return characters '\r'
    while ((pos = str.find('\r')) != std::string::npos) {
        str.erase(pos, 1);
    }

    // Remove newline characters '\n'
    while ((pos = str.find('\n')) != std::string::npos) {
        str.erase(pos, 1);
    }
}

bool password_check (std::string serverPassword, std::string clientPassword)
{
    std::cout << "client password before remove nl = " + clientPassword << std::endl;
    removeCRTL(clientPassword);
    std::cout << "client password = " + clientPassword << std::endl;
    std::cout << "server password = " + serverPassword << std::endl;
    if(serverPassword == clientPassword){
        std::cout <<"Password " + clientPassword +  " Acepted, Now Connecting ..." << std::endl;
        return true;
    }
        
    else
        throw std::runtime_error( "PASSWORD" + clientPassword +  " Is WRONG, try again" );//TODO changer pour le code d'erreur
         return false;
}
