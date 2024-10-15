#ifndef TRANSLATE_H
#define TRANSLATE_H


#include <iostream>
#include <map>
#include <tuple>
#include <CoreFoundation/CoreFoundation.h>
#include <Carbon/Carbon.h>


using namespace std;

/// @brief Keycode and boolean for if shift needs to be pressed
typedef std::tuple<CGKeyCode, bool> Formula;

static map<char, char> KEY_EQUIVELENCE_LOOKUP = {
    { '~', '`' },
    { '!', '1' },
    { '@', '2' },
    { '#', '3' },
    { '$', '4' },
    { '%', '5' },
    { '^', '6' },
    { '&', '7' },
    { '*', '8' },
    { '(', '9' },
    { ')', '0' },
    { '_', '-' },
    { '+', '=' },
    { '{', '[' },
    { '}', ']' },
    { '|', '\\' },
    { ':', ';' },
    { '\"', '\'' },
    { '<', ',' },
    { '>', '.' },
    { '?', '/' },
};
/// @brief Changes CGKeyCode to char
/// @param keyCode the keycode
/// @return character equivilence of keycode
char keyCodeToChar(CGKeyCode keyCode, UInt32 modifiers);

/// @brief Changes character to CGKeyCode
/// @param c the character to convert
/// @return the converted CGKeyCode
CGKeyCode charToKeyCode(char c);

/// @brief Changes character to formula
/// @param c character to convert
/// @return Tuple of CGKeyCode and bool, bool is for if shift needs to be pressed
Formula toCharWrapper(char c);

#endif