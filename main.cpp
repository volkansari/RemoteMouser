#include <QCoreApplication>
#include <QrCodeGenerator.h>
#include "mainwindow.h"
#include "websocketserver.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();

    WebSocketServer server(9000);
    
    return app.exec();
}
