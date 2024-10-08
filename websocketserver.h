#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include "InputEvent.h"
#include "jsonhelper.h"
#include "mouse.h"
#include "keyboard.h"
#include "networkhelper.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#ifdef Q_OS_LINUX
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#endif

class WebSocketServer : public QObject {
    Q_OBJECT

public:
    explicit WebSocketServer(quint16 port, QObject *parent = nullptr);
    ~WebSocketServer();


private slots:
    void onNewConnection();
    void processTextMessage(QString message);
    void socketDisconnected();
    void auth(QString);



private:
    QWebSocketServer *m_pWebSocketServer;
    QWebSocket *pSocket;
    NetworkHelper *networkHelper;
    QWebSocketCorsAuthenticator *corsAuth;
    QList<QWebSocket *> m_clients;
    InputEventType inputEventType;
    JsonHelper *jsonhelper;
    Mouse *mouse;
    Keyboard *keyboard;
    Mouse::Point *point;
    bool isAuthenticated;





#ifdef Q_OS_LINUX
    Display *display; // Linux için X11 Display
#endif

    void handleMouseInput(Mouse::Point*, Mouse::ActionType,Mouse::ClickType);
    void handleKeyboardInput(int key, Keyboard::ActionType);
};

#endif // WEBSOCKETSERVER_H
