#include "Automation.h"
#include "Config.h"


Automation::Automation(){

}

int login(command cmd){
    int p[2];
    if(pipe(p) < 0){
        cerr << "Pipe creation failed";
        return -1;
    }

    pid_t pid = fork
}