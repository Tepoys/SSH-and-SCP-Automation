#include "Automation.h"


Automation::Automation(): m_config(Config("")){
}

int Automation::Login(command cmd){
    cout << "Dont tab out during this process\nPress enter to acknowledge" << endl;
    pause();

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
        usleep(50);
        InputString(m_config.GetPassword());
    }

}

void Automation::PressKey(Formula form) {
    // Create an HID hardware event source
    CGEventSourceRef src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);

    // Create a new keyboard key press event
    CGEventRef press = CGEventCreateKeyboardEvent(src, std::get<0>(form), true);

    // Create a new keyboard key release event
    CGEventRef release = CGEventCreateKeyboardEvent(src, get<0>(form), false);

    if(form)


    // Post keyboard press event
    CGEventPost(kCGHIDEventTap, press);

    // Small delay to mimic a real key press (optional)
    usleep(50); // 1 ms delay

    // Post keyboard release event
    CGEventPost(kCGHIDEventTap, release);

    // Release the events and source
    CFRelease(press);
    CFRelease(release);
    CFRelease(src);
}

void Automation::InputString(string str){
    for(int i = 0; i < str.length(); i++){
        PressKey(toCharWrapper(str[i]));
    }
}