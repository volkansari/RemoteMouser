#ifndef INPUTEVENT_H
#define INPUTEVENT_H

#endif // INPUTEVENT_H


#include <QObject>

class InputEventType : public QObject {
    Q_OBJECT

public:

    // Girdi türleri (fare ve klavye)
    enum EventType {
        Mouse,
        Keyboard
    };
    Q_ENUM(EventType)


};
