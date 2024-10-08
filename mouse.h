#ifndef MOUSE_H
#define MOUSE_H

class Mouse
{

public:
    enum ClickType {
        LeftClick,
        MiddleClick,
        RightClick,

    };

    enum ActionType {
        Move,
        Click,
        Press,
        Release


    };

    struct Point{
        double x;
        double y;

    };

public:
    Mouse();
    ActionType actiontype;
    ClickType clicktype;

};

#endif // MOUSE_H
