#include "Config.h"

Config::Config(const string PATH){
    cout << "Attempting to open sql database at: " << PATH << endl;

    int quit = sqlite3_open(PATH.c_str(), &m_db);
    cout << "Quit :" << quit << endl;
    cout << m_db << endl;
    if(quit) { 
        std::cerr << "Error open DB " << sqlite3_errmsg(m_db) << std::endl; 
        exit(-1); 
    }
    cout << "SQLite Database Opened Succesfully" << endl;
    PullValues();
}

void Config::Menu(){
    int choice;
    bool reprompt = true;
    while(reprompt){
        bool valid = false;

        while(!valid){
            //print options
            cout << "\nConfiguration Menu" << endl;
            cout << "0. Set All Values" << endl;
            cout << "1. Set Username" << endl;
            cout << "2. Set Server Adress" << endl;
            cout << "3. Set Password" << endl;
            cout << "4. Display Values" << endl;
            cout << "5. Exit" << endl;
            cout << "Enter choice: ";

            //input validation
            bool err = !(cin >> choice);
            valid = true;
            if(err){
                cin.clear();
                valid = false;
                cout << "Invalid input" << endl;
            }
            cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
        }

        //cases
        switch(choice){
            case 0:
                SetValues();
                break;
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
    std::string sql = "CREATE TABLE IF NOT EXISTS DATA("
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
        std::exit(-1);
    } 
    else {
        //std::cout << "Table created Successfully" << std::endl;

        //pull data here
        
        string getLength = "SELECT COUNT(*) FROM DATA";
        int count = 0;
        int rc = sqlite3_exec(m_db, getLength.c_str(), callback, &count, NULL);
        if(rc != SQLITE_OK){
            cerr << "Something went wrong" << endl;
            cerr << rc << endl;
            std::exit(-1);
        }

        if(count == 1){
            sqlite3_stmt *stmt;
            rc = sqlite3_prepare_v2(m_db, GET_DATA.c_str(), GET_DATA.length(), &stmt, NULL);
            if(rc!=SQLITE_OK){
                cerr << "Something went wrong" << endl;
                cerr << rc << endl;
                std::exit(-1);
            }
            rc = sqlite3_step(stmt);
            if(rc == SQLITE_ROW){
                m_username = (char*)sqlite3_column_text(stmt, 0);
                m_serverAdress = (char*)sqlite3_column_text(stmt, 1);
                m_password = (char*)sqlite3_column_text(stmt, 2);
            } else {
                cout << "Something broke, error code:" << rc << endl;
                std::exit(-1);
            }

            sqlite3_finalize(stmt);
        } else {
            //sets default values and uploads them to the database
            cout << "Database is empty creating entries" << endl;
            m_username = "default";
            m_serverAdress = "default";
            m_password = "default";
            string insert = "INSERT INTO DATA (USERNAME, SERVERADRESS, PASSWORD) VALUES (?, ?, ?);";
            sqlite3_stmt *stmt;
            rc = sqlite3_prepare_v2(m_db, insert.c_str(), insert.length(), &stmt, NULL);
            if(rc!=SQLITE_OK){
                cerr << "Something went wrong" << endl;
                cerr << rc << endl;
                std::exit(-1);
            }
            sqlite3_bind_text(stmt, 1, m_username.c_str(), m_username.length(), SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, m_serverAdress.c_str(), m_serverAdress.length(), SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, m_password.c_str(), m_password.length(), SQLITE_STATIC);
            rc = sqlite3_step(stmt);
            if(rc != SQLITE_DONE){
                cerr << "Something went wrong" << endl;
                cerr << rc << endl;
                std::exit(-1);
            }
            sqlite3_finalize(stmt);
        }
        
    }
}

static int callback(void *count, int argc, char **argv, char **azColName) {
    int *c = (int*)count;
    *c = atoi(argv[0]);
    return 0;
}

void Config::SetUsername(){
    cout << "Enter Username: ";
    cin >> m_username;
    UploadUsername();
}

void Config::SetServerAdress(){
    cout << "Enter Server Adress: ";
    cin >> m_serverAdress;
    UploadServerAdress();
}

void Config::SetPassword(){
    cout << "Enter Password: ";
    cin >> m_password;
    UploadPassword();
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

void Config::UploadPassword(){
    string update = "UPDATE DATA SET PASSWORD = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(m_db, update.c_str(), update.length(), &stmt, NULL);
    if(rc!=SQLITE_OK){
        cerr << "Something went wrong" << endl;
        cerr << rc << endl;
        std::exit(-1);
    }
    sqlite3_bind_text(stmt, 1, m_password.c_str(), m_password.length(), SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE){
        cerr << "Something went wrong" << endl;
        cerr << rc << endl;
        std::exit(-1);
    }
    sqlite3_finalize(stmt);
}

void Config::UploadUsername(){
    string update = "UPDATE DATA SET USERNAME = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(m_db, update.c_str(), update.length(), &stmt, NULL);
    if(rc!=SQLITE_OK){
        cerr << "Something went wrong" << endl;
        cerr << rc << endl;
        std::exit(-1);
    }
    sqlite3_bind_text(stmt, 1, m_username.c_str(), m_username.length(), SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE){
        cerr << "Something went wrong" << endl;
        cerr << rc << endl;
        std::exit(-1);
    }
    sqlite3_finalize(stmt);
}

void Config::UploadServerAdress(){
    string update = "UPDATE DATA SET SERVERADRESS = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(m_db, update.c_str(), update.length(), &stmt, NULL);
    if(rc!=SQLITE_OK){
        cerr << "Something went wrong" << endl;
        cerr << rc << endl;
        std::exit(-1);
    }
    sqlite3_bind_text(stmt, 1, m_serverAdress.c_str(), m_serverAdress.length(), SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE){
        cerr << "Something went wrong" << endl;
        cerr << rc << endl;
        std::exit(-1);
    }
    sqlite3_finalize(stmt);
}

void Config::SetValues(){
    SetUsername();
    SetPassword();
    SetServerAdress();
}

Config::~Config(){
    sqlite3_close(m_db);
}