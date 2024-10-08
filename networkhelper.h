#ifndef NETWORKHELPER_H
#define NETWORKHELPER_H

#include <QObject>
#include <QNetworkInterface>

class NetworkHelper : public QObject
{
    Q_OBJECT
public:
    explicit NetworkHelper(QObject *parent = nullptr);

    // Aktif olan ağ adaptörünün IP adresini döner
    QHostAddress getActiveNetworkIP();
    // Ağ arayüzlerini filtreleyip, aktif olan adaptörün IP adresini bulur
    static QList<QHostAddress> getActiveIPAddresses();

private:

};

#endif // NETWORKHELPER_H
