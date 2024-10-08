#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "qnamespace.h"
class Keyboard
{

public:
    // Klavye tuşları için enum
    enum KeyType {
        Key_A = Qt::Key_A,
        Key_B = Qt::Key_B,
        Key_C = Qt::Key_C,
        Key_D = Qt::Key_D,
        Key_E = Qt::Key_E,
        Key_F = Qt::Key_F,
        Key_G = Qt::Key_G,
        Key_H = Qt::Key_H,
        Key_I = Qt::Key_I,
        Key_J = Qt::Key_J,
        Key_K = Qt::Key_K,
        Key_L = Qt::Key_L,
        Key_M = Qt::Key_M,
        Key_N = Qt::Key_N,
        Key_O = Qt::Key_O,
        Key_P = Qt::Key_P,
        Key_Q = Qt::Key_Q,
        Key_R = Qt::Key_R,
        Key_S = Qt::Key_S,
        Key_T = Qt::Key_T,
        Key_U = Qt::Key_U,
        Key_V = Qt::Key_V,
        Key_W = Qt::Key_W,
        Key_X = Qt::Key_X,
        Key_Y = Qt::Key_Y,
        Key_Z = Qt::Key_Z,
        Key_Enter = Qt::Key_Return,
        Key_Space = Qt::Key_Space,
        Key_Escape = Qt::Key_Escape
    };
    enum ActionType {
        Press,
        Release


    };

public:
    Keyboard();
    ActionType actiontype;
};

#endif // KEYBOARD_H
