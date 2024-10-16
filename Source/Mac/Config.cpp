#include "Config.h"

Config::Config(const string PATH){
    cout << "SQL Path: " << PATH << endl;

    int quit = sqlite3_open(PATH.c_str(), &m_db);
    cout << "Quit :" << quit << endl;
    cout << m_db << endl;
    if(quit) { 
        std::cerr << "Error open DB " << sqlite3_errmsg(m_db) << std::endl; 
        exit(-1); 
    }
    PullValues();
}

void Config::Menu(){
    int choice;
    bool reprompt = true;
    while(reprompt){
        bool valid = false;

        while(!valid){
            cout << "0. Set All Values" << endl;
            cout << "1. Set Username" << endl;
            cout << "2. Set Server Adress" << endl;
            cout << "3. Set Password" << endl;
            cout << "4. Display Values" << endl;
            cout << "5. Exit" << endl;
            cout << "Enter choice: ";
            try{
                cin >> choice;
                valid = true;
            } catch(...){
                cout << "Invalid input" << endl;
            }
        }
        switch(choice){
            case 1:
                SetUsername();
                break;
            case 2:
                SetServerAdress();
                break;
            case 3:
                SetPassword();
                break;
            case 4:
                cout << "Username: " << m_username << endl;
                cout << "Server Adress: " << m_serverAdress << endl;
                cout << "Password not displayed for security reasons." << endl;
                break;
            case 5:
                reprompt = false;
                break;
            default:
                cout << "Invalid choice" << endl;
                break;
        }
        
    }

}

void Config::PullValues(){
    //sql command to execute
    std::string sql = "CREATE TABLE DATA("
                        "USERNAME TEXT PRIMARY KEY     NOT NULL, "
                        "SERVERADRESS           TEXT    NOT NULL, "
                        "PASSWORD          TEXT     NOT NULL);";
    int exit = 0;
    char* messaggeError; 
    exit = sqlite3_exec(m_db, sql.c_str(), NULL, 0, &messaggeError); 

    //error check
    if (exit != SQLITE_OK) { 
        std::cerr << "Error Create Table" << std::endl; 
        sqlite3_free(messaggeError); 
    } 
    else
        std::cout << "Table created Successfully" << std::endl;
}

void Config::SetUsername(){
    cout << "Enter Username: ";
    cin >> m_username;
}

void Config::SetServerAdress(){
    cout << "Enter Server Adress: ";
    cin >> m_serverAdress;
}

void Config::SetPassword(){
    cout << "Enter Password: ";
    cin >> m_password;
}

string Config::GetPassword(){
    return m_password;
}
string Config::GetUsername(){
    return m_username;
}

string Config::GetServerAdress(){
    return m_serverAdress;
}

void Config::SetValues(){
    SetUsername();
    SetPassword();
    SetServerAdress();
}

Config::~Config(){
    sqlite3_close(m_db);
}