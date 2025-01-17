#include "Translate.h"


/*
*   KeyCodeToChar and charToKeyCode is from https://stackoverflow.com/questions/1918841/how-to-convert-ascii-character-to-cgkeycode
*   writen by Michael https://stackoverflow.com/users/217476/michael
*/

//------------------------------------------//

//changes CGKeycode to char
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

//------------------------------------------//

Formula toCharWrapper(char c){
    map<char, char> map = getKeyEquivalenceLookup();
    if(map.find(c) == map.end()){
        if(isupper(c)){
            // cout << "Found" << endl;
            return std::make_tuple(charToKeyCode(tolower(c)), true);
        } else {
            // cout << "Not Found" << endl;
            return std::make_tuple(charToKeyCode(c), false);
        }
    } else{
        // cout << "Found" << endl;
        return std::make_tuple(charToKeyCode(map[c]), true);
    }
}