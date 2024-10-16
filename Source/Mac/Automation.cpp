#include "Automation.h"

Automation::Automation(): m_config(Config(DATABASE_PATH)) {
}

void Automation::Menu(){
    

    int choice;
    bool reprompt = true;
    while(reprompt){
        bool valid = false;
        

        while(!valid){

            cout << "\nSSH and SCP auto login\nWhat would you like to do?" << endl;
            cout << "1. SSH" << endl;
            cout << "2. SCP" << endl;
            cout << "3. Configure" << endl;
            cout << "4. Exit" << endl;
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
            case 6:
                InputString("Hello world!");
                break;
            default:
                cout << "Invalid choice" << endl;
                break;
        }
    }
    
}

int Automation::Login(command cmd){ 
    cout << "Dont tab out during this process\nPress enter to acknowledge" << endl;
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
        usleep(1500*1000);
        cout << "Enter password: " << m_config.GetPassword() << endl;
        InputString(m_config.GetPassword());
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

    // if shift needed
    if(std::get<1>(form)){
        // Press shift key
        CGEventSetFlags(press, kCGEventFlagMaskShift);
    }

    // Post keyboard press event
    CGEventPost(kCGHIDEventTap, press);

    // Small delay to mimic a real key press (optional)
    //usleep(1000); // 1 ms delay
    sleep(1);

    // Post keyboard release event
    CGEventPost(kCGHIDEventTap, release);

    // unpress shift
    if(std::get<1>(form)){
        // Release shift key
        CGEventSetFlags(release, kCGEventFlagMaskShift);
    }

    // Release the events and source
    CFRelease(press);
    CFRelease(release);
    CFRelease(src);
}

void Automation::InputString(string str){
    for(int i = 0; i < str.length(); i++){
        cout << "Pressed key: " << str[i]  << " After wrapper: " << toCharWrapper(str[i]) << endl;

        PressKey(toCharWrapper(str[i]));
    }
}