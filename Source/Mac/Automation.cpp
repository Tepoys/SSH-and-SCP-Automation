#include "Automation.h"

char** vecToCharArr(std::vector<std::string> vec);
std::vector<std::string> split(const std::string& s, const std::string& delimiter);

 


Automation::Automation() {
    char path[PATH_MAX];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0) {
        std::cerr << "Buffer too small; needed size: " << size << std::endl;
        exit(-1);
    }
    char realPath[PATH_MAX];
    realpath(path, realPath);
    string stringPath = string(realPath);
    uint32_t lastDelim = stringPath.find_last_of("/");


    string fullPath = stringPath.substr(0, lastDelim+1) + DATABASE_PATH;
    m_config = new Config(fullPath);
}

Automation::~Automation(){
    delete m_config;
}

void Automation::Menu(){
    

    int choice;
    bool reprompt = true;
    while(reprompt){
        bool valid = false;
        

        while(!valid){
            // this is a mess, prob going to rewrite
            cout << "\nSSH and SCP auto login\nWhat would you like to do?" << endl;
            cout << "1. SSH" << endl;
            cout << "2. SCP" << endl;
            cout << "3. Configure" << endl;
            cout << "4. Exit" << endl;
            cout << "Enter choice: ";
            
            // input validation
            bool err = !(cin >> choice);
                valid = true;
                if(err){
                    cin.clear();
                    valid = false;
                    cout << "Invalid input" << endl;
                }
                cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
        }

        // cases
        switch(choice){
            case 1:
                Login(command::SSH);
                break;
            case 2:
                Login(command::SCP);
                break;
            case 3:
                m_config->Menu();
                break;
            case 4:
                reprompt = false;
                break;
            default:
                cout << "Invalid choice" << endl;
                break;
        }
    }
    
}

string Automation::AuthPrompt(){
    string auth;
    cout << "Enter auth token (or 1 for duo push): ";
    cin >> auth;
    return auth;
}

int Automation::Login(command cmd){
    string fullCommand = "";
    if(cmd == command::SCP){
        string from = "", to = "", additional = "";
        string custom = "";
        int choice = 0;
        while(cout << "\nPick An Option\n1. Downloading file\n2. Uploading file\n3. Custom scp command\n" && !(cin >> choice) && (choice != 1 && choice != 2 && choice != 3)){
            cin.clear();
            cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input" << endl;
        }
        cin.ignore();
        if(choice != 3){
            int slashR = 0;
            cout << "Include -r?\n";
            cout << "0.No\n1.Yes\n";
            cin >> slashR;
            cin.clear();
            cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
            fullCommand += slashR ? "-r " : "";
        }

        switch(choice){
            case 1:
                cout << "Enter source folder (remote):";
                cin >> from;
                cout << "Enter destination path (local):";
                cin >> to;
                fullCommand += m_config->GetUsername() + "@" + m_config->GetServerAdress() + ":" + from + " " + to;
                break;
            case 2:
                cout << "Enter file path to upload (local):";
                cin >> from;
                cout << "Enter destination path (remote):";
                cin >> to;
                fullCommand += from + " " + m_config->GetUsername() + "@" + m_config->GetServerAdress() + ":" + to;
                break;
            case 3:
                cout << "Enter custom command (only arguments after scp):";
                cin >> custom;
                fullCommand = custom;
                break;
        }
    }

    string auth = AuthPrompt();
    cout << "Dont tab out during this process and dont touch the keyboard untill you are logged in" << endl;
    cin.ignore();

    //open pipe and checks for success
    int p[2];
    if(pipe(p) < 0){
        cerr << "Pipe creation failed";
        return -1;
    }

    //forks process and checks for failure
    pid_t pid = fork();
    if(pid == -1){
        cerr << "Fork failed";
        return pid;
    }
    
    if(pid == 0){
        //child process

        //calls command
        switch(cmd){
            case command::SSH:{
                string dest = m_config->GetUsername() + "@" + m_config->GetServerAdress();
                execlp("ssh", "ssh", "-t", dest.c_str(),(char *) NULL); 
                break;
            }
            case command::SCP:
                cout << "Running command: " << fullCommand << endl;
                execvp("scp", vecToCharArr(split(fullCommand, " ")));
                break;
        }

        //program fails
        cerr << "Command Execute failed" << endl;
        exit(-1);
    } else {
        //parent
        usleep(200*1000);
        //cout << "Enter password: " << m_config->GetPassword() << endl;
        InputString(m_config->GetPassword());
        PressKey(std::make_tuple(52, false));
        usleep(1100*1000);
        InputString(auth);
        PressKey(std::make_tuple(52, false));
        //InputString("auth");

        wait(nullptr);
        cout << "Child has finished running" << endl;
        return 0;
    }

}

// split string
std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    std::string str = s; 
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        tokens.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    tokens.push_back(str);

    return tokens;
}

// convert string vector to null terminated char array with scp at the front
char** vecToCharArr(std::vector<std::string> vec){
    char** arr = new char*[vec.size() + 2];
    arr[0] = new char[4];
    strcpy(arr[0], "scp");
    for(size_t i = 1; i < vec.size() + 1; i++){
        arr[i] = new char[vec[i - 1].length() + 1];
        strcpy(arr[i], vec[i - 1].c_str());
    }
    arr[vec.size() + 1] = nullptr;
    for(size_t i = 0; i < vec.size() + 1; i++){
        cout << arr[i] << endl;
    }
    return arr;
}


void Automation::PressKey(Formula form) {
    // Create an HID hardware event source
    CGEventSourceRef src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);

    // Create a new keyboard key press event
    CGEventRef press = CGEventCreateKeyboardEvent(src, std::get<0>(form), true);

    // Create a new keyboard key release event
    CGEventRef release = CGEventCreateKeyboardEvent(src, std::get<0>(form), false);
    
    //cout << "Keycode: " << std::get<0>(form) << endl;
    //cout << "Shift: " << std::get<1>(form) << endl;

    // if shift needed
    if(std::get<1>(form)){
        // Press shift key
        CGEventSetFlags(press, kCGEventFlagMaskShift);
    }

    // Post keyboard press event
    CGEventPost(kCGHIDEventTap, press);

    // Small delay to mimic a real key press (optional)
    //usleep(100*1000); // 1 ms delay
    //sleep(1);

    // Post keyboard release event
    CGEventPost(kCGHIDEventTap, release);

    // unpress shift
    if(std::get<1>(form)){
        // Release shift key
        CGEventRef shiftKeyUp = CGEventCreateKeyboardEvent(src, (CGKeyCode)56, false);
        CFRelease(shiftKeyUp);
    }

    // Release the events and source
    CFRelease(press);
    CFRelease(release);
    CFRelease(src);
}

void Automation::InputString(string str){
    for(int i = 0; i < str.length(); i++){
        //cout << "Pressed key: " << str[i]  << " After wrapper: " << std::get<0>(toCharWrapper(str[i])) << endl;

        PressKey(toCharWrapper(str[i]));
    }
}