#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
using namespace std;

class Config {
    public:
        /// @brief Config - Overloaded Constructor - Initializes an Config object from data from a PATH
        /// @param PATH Path to database
        Config(const string PATH);

        string GetUsername();
        string GetServerAdress();
        string GetPassword();
    private:
        string m_username;
        string m_serverAdress;
        string m_password;
};

#endif