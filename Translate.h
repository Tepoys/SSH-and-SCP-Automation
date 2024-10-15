#ifndef TRANSLATE_H
#define TRANSLATE_H


#include <CoreFoundation/CoreFoundation.h>
#include <Carbon/Carbon.h>

using namespace std;

char keyCodeToChar(CGKeyCode keyCode, UInt32 modifiers);
CGKeyCode charToKeyCode(char c);

#endif