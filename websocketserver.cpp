#include "websocketserver.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <networkhelper.h>
#include <totpgenerator.h>

WebSocketServer::WebSocketServer(quint16 port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("WebSocket Server"),
                                            QWebSocketServer::NonSecureMode, this)),
    networkHelper(new NetworkHelper),
    mouse(new Mouse),
    keyboard(new Keyboard),
    isAuthenticated(false)  // Authentication durumu
{
#ifdef Q_OS_LINUX
    display = XOpenDisplay(nullptr); // Linux'ta X11 oturumu aç
    if (display == nullptr) {
        qDebug() << "X11 oturumu başlatılamadı.";
    }
#endif

    if (m_pWebSocketServer->listen(networkHelper->getActiveNetworkIP(), port)) {
        m_pWebSocketServer->setMaxPendingConnections(1);
        qDebug() << "WebSocket server başlatıldı. ip" << networkHelper->getActiveNetworkIP().toString();
        qDebug() << "WebSocket server başlatıldı. port" << port;

        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &WebSocketServer::onNewConnection);
    }
}

WebSocketServer::~WebSocketServer() {
    m_pWebSocketServer->close();
    qDeleteAll(m_clients);

#ifdef Q_OS_LINUX
    if (display) {
        XCloseDisplay(display);  // Linux'ta X11 oturumunu kapat
    }
#endif
}

void WebSocketServer::onNewConnection() {
    pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived,
            this, &WebSocketServer::auth);
    connect(pSocket, &QWebSocket::disconnected,
            this, &WebSocketServer::socketDisconnected);
    m_clients << pSocket;
}

void WebSocketServer::auth(QString message) {
    QJsonObject jsonObject = jsonhelper->parseJson(message)["data"].toObject();

    if (jsonObject.contains("key")) {
        QString receivedTotp = jsonObject["key"].toString();
        QString currentTotp = TOTPGenerator::generateTOTP("secret");

        if (receivedTotp == currentTotp) {
            isAuthenticated = true;  // TOTP doğrulandıktan sonra auth durumunu güncelleyin
            connect(pSocket, &QWebSocket::textMessageReceived,
                    this, &WebSocketServer::processTextMessage);
            qDebug() << "Authentication successful!";
        } else {
            pSocket->sendTextMessage("Authentication failed!");
            pSocket->close();
        }
    } else {
        pSocket->sendTextMessage("Invalid message format: 'key' missing.");
        pSocket->close();
    }
}

void WebSocketServer::processTextMessage(QString message) {
    if (!isAuthenticated) {
        qDebug() << "Not authenticated!";
        return;
    }

    qDebug() << "Gelen mesaj:" << message;
    QJsonObject jsonObject = jsonhelper->parseJson(message)["data"].toObject();

    if (!jsonObject.contains("type")) {
        qDebug() << "Eksik 'type' alanı!";
        return;
    }

    InputEventType::EventType type = static_cast<InputEventType::EventType>(jsonObject["type"].toInt());

    // Mouse olaylarını işle
    if (type == InputEventType::Mouse) {
        auto point = std::make_unique<Mouse::Point>();  // Akıllı işaretçi kullanımı
        point->x = jsonObject["pointX"].toString().toDouble();
        point->y = jsonObject["pointY"].toString().toDouble();

        if (!jsonObject.contains("action") || !jsonObject.contains("click")) {
            qDebug() << "Eksik 'action' veya 'click' alanları!";
            return;
        }

        mouse->actiontype = static_cast<Mouse::ActionType>(jsonObject["action"].toString().toInt());
        mouse->clicktype = static_cast<Mouse::ClickType>(jsonObject["click"].toString().toInt());
        handleMouseInput(point.get(), mouse->actiontype, mouse->clicktype);
    }
    // Klavye olaylarını işle
    else if (type == InputEventType::Keyboard) {
        if (!jsonObject.contains("key") || !jsonObject.contains("action")) {
            qDebug() << "Eksik 'key' veya 'action' alanları!";
            return;
        }

        int key = jsonObject["key"].toInt();
        keyboard->actiontype = static_cast<Keyboard::ActionType>(jsonObject["action"].toString().toInt());
        handleKeyboardInput(key, keyboard->actiontype);
    }
}

void WebSocketServer::socketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}



void WebSocketServer::handleMouseInput(Mouse::Point *point, Mouse::ActionType action, Mouse::ClickType click) {
#ifdef Q_OS_WIN
    // Windows'ta mouse hareket ve tıklama
    if (action == Mouse::Click) {
        INPUT input;
        memset(&input, 0, sizeof(INPUT));
        if (click == Mouse::LeftClick) { // Sol tıklama
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            SendInput(1, &input, sizeof(INPUT));
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            SendInput(1, &input, sizeof(INPUT));
        } else if (click == Mouse::MiddleClick) { // Orta tıklama
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
            SendInput(1, &input, sizeof(INPUT));
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
            SendInput(1, &input, sizeof(INPUT));
        } else if (click == Mouse::RightClick) { // Sağ tıklama
            input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
            SendInput(1, &input, sizeof(INPUT));
            input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            SendInput(1, &input, sizeof(INPUT));
        }
    } else {
        SetCursorPos(point->x, point->y);

    }
#endif

#ifdef Q_OS_LINUX
    // Linux'ta mouse hareket ve tıklama (X11)
    if (display != nullptr) {
        XWarpPointer(display, None, DefaultRootWindow(display), 0, 0, 0, 0, point->x, point->y);
        XFlush(display);

        if (click == Mouse::LeftClick) {
            XTestFakeButtonEvent(display, 1, True, 0);
            XTestFakeButtonEvent(display, 1, False, 0);
            XFlush(display);
        } else if (click == Mouse::MiddleClick) { // Orta tıklama
            XTestFakeButtonEvent(display, 2, True, 0);  // Orta butona bas
            XTestFakeButtonEvent(display, 2, False, 0); // Orta butondan bırak
        } else if (click == Mouse::RightClick) { // Sağ tıklama
            XTestFakeButtonEvent(display, 3, True, 0);  // Sağ butona bas
            XTestFakeButtonEvent(display, 3, False, 0); // Sağ butondan bırak
        }
    }
#endif
}

void WebSocketServer::handleKeyboardInput(int key, Keyboard::ActionType action) {
#ifdef Q_OS_WIN
    // Windows'ta klavye girdisi
    INPUT input;
    memset(&input, 0, sizeof(INPUT));
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;

    if (action == Keyboard::Press) {
        input.ki.dwFlags = 0;
    } else if (action == Keyboard::Release) {
        input.ki.dwFlags = KEYEVENTF_KEYUP;
    }

    SendInput(1, &input, sizeof(INPUT));
#endif

#ifdef Q_OS_LINUX
    // Linux'ta klavye girdisi (X11)
    if (display != nullptr) {
        KeyCode keycode = XKeysymToKeycode(display, key);
        if (action == Keyboard::Press) {
            XTestFakeKeyEvent(display, keycode, True, 0);
        } else if (action == Keyboard::Release) {
            XTestFakeKeyEvent(display, keycode, False, 0);
        }
        XFlush(display);
    }
#endif
}
