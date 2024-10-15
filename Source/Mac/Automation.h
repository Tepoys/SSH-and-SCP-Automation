#include <string>
#include <unistd.h>
#include <chrono>
using namespace std;

const string DATABASE_PATH = "";

enum command {
    SSH, //Secure Shell
    SCP  //Secure Copy Protocol
};

class Automation {
    public:
        /// @brief default constructor
        Automation();
        void Menu();
    private:
        int Login(command cmd);
        int Child();
        int Parent();
        int SSH();
        int SCP();
};