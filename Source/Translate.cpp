#include "Translate.h"

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