#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <sqlite3.h>
//#include "sqlite3.c"

using namespace std;

static int callback(void *count, int argc, char **argv, char **azColName);

const string GET_DATA = "SELECT USERNAME, SERVERADRESS, PASSWORD "
                        "FROM DATA;";

class Config {
    public:
        /// @brief Config - Overloaded Constructor - Initializes an Config object from data from a PATH
        /// @param PATH Path to database
        Config(const string PATH);

        ~Config();

        void Menu();

        string GetUsername();
        string GetServerAdress();
        string GetPassword();
    private:
        void SetUsername();
        void SetServerAdress();
        void SetPassword();
        void UploadUsername();
        void UploadServerAdress();
        void UploadPassword();
        void SetValues();
        void PullValues();
        string m_username;
        string m_serverAdress;
        string m_password;
        sqlite3* m_db;
};

#endif