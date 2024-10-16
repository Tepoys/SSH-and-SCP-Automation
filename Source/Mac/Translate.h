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

/// @brief Provides a lookup map for key equivalences
/// @return A map of special characters to their base equivalents
static map<char, char> getKeyEquivalenceLookup() {
    return map<char, char>({
        std::make_pair('~', '`'),
        std::make_pair('!', '1'),
        std::make_pair('@', '2'),
        std::make_pair('#', '3'),
        std::make_pair('$', '4'),
        std::make_pair('%', '5'),
        std::make_pair('^', '6'),
        std::make_pair('&', '7'),
        std::make_pair('*', '8'),
        std::make_pair('(', '9'),
        std::make_pair(')', '0'),
        std::make_pair('_', '-'),
        std::make_pair('+', '='),
        std::make_pair('{', '['),
        std::make_pair('}', ']'),
        std::make_pair('|', '\\'),
        std::make_pair(':', ';'),
        std::make_pair('\"', '\''),
        std::make_pair('<', ','),
        std::make_pair('>', '.'),
        std::make_pair('?', '/')
    });
}
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