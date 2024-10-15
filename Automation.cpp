#include <iostream> 
#include <unistd.h> 
#include <fcntl.h>
#include <chrono>
#include <thread>
#include <CoreFoundation/CoreFoundation.h>
#include <Carbon/Carbon.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <ApplicationServices/ApplicationServices.h>



using namespace::std;

const string pass = "#6pLyCZqSyXYwNV";
 
enum command {
    SSH, //Secure Shell
    SCP  //Secure Copy Protocol
};

CGKeyCode charToKeyCode(char c);
void PressKey(CGKeyCode key);
int login(command cmd);

int main() { 
    cout << "Starting program";
    login(SSH);
 
    return 0; 
}


int login(command cmd){
    //create a set of pipes
    int p[2];
    int result = pipe(p);
    if(result == -1){
        cerr<<"Error: Pipe creation failed";
        return result;
    }

    //attempt to set pipe read to non-blocking
    // if(fcntl(p[0], F_SETFL, O_NONBLOCK) == -1){
    //     cerr<<"Error: Set pipe to unblocking failed";
    //     return -1;
    // }

    //fork the processes, 0 = child, 0 < parent
    pid_t pid = fork();
 
    if (pid == 0) {
        // This is the child process

        // if (setsid() == -1) {//lets parent have control over terminal i think
        //     perror("setsid");
        //     exit(1);
        // }

        close(p[1]); //close write end
        dup2(p[0], STDIN_FILENO);//

        //cout.setf(ios::unitbuf);

        //call exec(), takes over current program
        switch(cmd){
            case SSH:
                //Runs ssh command with arguments
                execlp("ssh", "ssh", "-t", "bliao1@gl.umbc.edu",(char *) NULL); 
                break;
            case SCP:
                // SCP command with arguments
                execlp("scp", "scp", (char *) NULL);
                break;
        }
        
        // If execlp fails 
        cerr << "Error executing command" << endl; 
        return 1; 
    } else if (pid > 0) { 
        // This is the parent process 

        close(p[0]); //close read end
        //dup2(p[1], STDIN_FILENO);
        for(int i = 0; i < 5; i++){
            this_thread::sleep_for(chrono::milliseconds(500));
            //write(p[1], pass.c_str(), pass.length() + 1);
            //cout << "hello world";
            CGKeyCode code = charToKeyCode('!');
            if(code >= 0){

              PressKey(code);
              //PressKey((CGKeyCode)0x00);
              //cout << (code) << std::flush;
            } else {
              cerr<<"Code is 0";
            }
        }
        

        cout << "Entered password" << endl;


        //wait(nullptr); // Wait for the child process to finish 
        cout << "\nChild process finished";
        return wait(nullptr);
    } else { 
        // Fork failed 
        std::cerr << "Fork failed" << std::endl; 
        return 1; 
    }
}

void PressKey(CGKeyCode key) {
    // Create an HID hardware event source
    CGEventSourceRef src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);

    // Create a new keyboard key press event
    CGEventRef press = CGEventCreateKeyboardEvent(src, key, true);

    // Create a new keyboard key release event
    CGEventRef release = CGEventCreateKeyboardEvent(src, key, false);

    // Post keyboard press event
    CGEventPost(kCGHIDEventTap, press);

    // Small delay to mimic a real key press (optional)
    usleep(1000); // 1 ms delay

    // Post keyboard release event
    CGEventPost(kCGHIDEventTap, release);

    // Release the events and source
    CFRelease(press);
    CFRelease(release);
    CFRelease(src);
}

char keyCodeToChar(CGKeyCode keyCode, UInt32 modifiers) {
    TISInputSourceRef currentKeyboard = TISCopyCurrentKeyboardLayoutInputSource();
    CFDataRef layoutData = (CFDataRef)TISGetInputSourceProperty(currentKeyboard, kTISPropertyUnicodeKeyLayoutData);
    const UCKeyboardLayout *keyboardLayout = (const UCKeyboardLayout *)CFDataGetBytePtr(layoutData);

    UInt32 deadKeyState = 0;
    UniCharCount maxStringLength = 255;
    UniCharCount actualStringLength = 0;
    UniChar unicodeString[255];

    UCKeyTranslate(keyboardLayout,
                   keyCode,
                   kUCKeyActionDown,
                   modifiers,
                   LMGetKbdType(),
                   kUCKeyTranslateNoDeadKeysMask,
                   &deadKeyState,
                   maxStringLength,
                   &actualStringLength,
                   unicodeString);

    CFRelease(currentKeyboard);

    if (actualStringLength > 0) {
        return unicodeString[0];  // Return the first character
    }

    return 0;
}
CGKeyCode charToKeyCode(char c) {
    for (CGKeyCode keyCode = 0; keyCode < 128; ++keyCode) {
        char result = keyCodeToChar(keyCode, 0);
        if (result == c) {
            return keyCode;
        }
    }
    return -1; // Not found
}