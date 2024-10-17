#ifndef AUTOMATION_H
#define AUTOMATION_H


#include <mach-o/dyld.h>
#include <limits.h>
#include <vector>
#include <string>
#include <unistd.h>
#include <chrono>

#include "Translate.h"
#include "Config.h"

using namespace std;

const string DATABASE_PATH = "data.sqlite3";
const int msKeystroke = 50;

enum command {
    SSH, //Secure Shell
    SCP  //Secure Copy Protocol
};

class Automation {
    public:
        /// @brief default constructor
        Automation();
        ~Automation();
        void Menu();
    private:
        string AuthPrompt();
        int Login(command cmd);
        int SSH();
        int SCP();
        void PressKey(Formula form);
        void InputString(string str);
        Config *m_config;
};

#endif