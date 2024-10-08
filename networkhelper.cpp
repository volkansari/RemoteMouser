#include "NetworkHelper.h"
#include <QDebug>

NetworkHelper::NetworkHelper(QObject *parent) : QObject(parent)
{
}

// Aktif olan adaptörün IP adresini döner
QHostAddress NetworkHelper::getActiveNetworkIP()
{
    QList<QHostAddress> activeIPs = getActiveIPAddresses();

    // Eğer aktif bir IP adresi varsa ilkini döner
    if (!activeIPs.isEmpty()) {
        return activeIPs.first();
    }

    throw std::runtime_error("No active IP addresses found");
}

// Aktif ve çalışan adaptörlerin IP adreslerini döner
QList<QHostAddress> NetworkHelper::getActiveIPAddresses()
{
    QList<QHostAddress> ipList;

    // Tüm ağ arayüzlerini al
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    // Her bir arayüzü kontrol et
    foreach (const QNetworkInterface &netInterface, interfaces) {
        // Sadece aktif ve çalışan adaptörleri kontrol et
        if (netInterface.flags().testFlag(QNetworkInterface::IsUp) &&
            netInterface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !netInterface.flags().testFlag(QNetworkInterface::IsLoopBack)) {

            // IP adreslerini al
            QList<QNetworkAddressEntry> entries = netInterface.addressEntries();
            foreach (const QNetworkAddressEntry &entry, entries) {
                // IPv4 adreslerini filtrele
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    ipList.append(entry.ip());
                }
            }
        }
    }

    return ipList;
}
