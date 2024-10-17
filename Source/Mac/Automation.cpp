#include "Automation.h"

Automation::Automation(): m_config(Config(DATABASE_PATH)) {
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
                m_config.Menu();
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
    cout << "Enter auth token: ";
    cin >> auth;
    return auth;
}

int Automation::Login(command cmd){ 
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
            case command::SSH:
                execlp("ssh", "ssh", "-t", "bliao1@gl.umbc.edu",(char *) NULL); 
                break;
            case command::SCP:
                execlp("scp", "scp", (char *) NULL);
                break;
        }

        //program fails
        cerr << "Command Execute failed" << endl;
        exit(-1);
    } else {
        //parent
        usleep(200*1000);
        cout << "Enter password: " << m_config.GetPassword() << endl;
        InputString(m_config.GetPassword());
        PressKey(std::make_tuple(52, false));
        usleep(1100*1000);
        InputString(auth);
        PressKey(std::make_tuple(52, false));
        InputString("auth");

        wait(nullptr);
        cout << "Child has finished running" << endl;
        return 0;
    }

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