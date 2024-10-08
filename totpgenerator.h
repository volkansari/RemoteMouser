#ifndef TOTPGENERATOR_H
#define TOTPGENERATOR_H

#include <QTimer>
#include <QDateTime>
#include <QCryptographicHash>
#include <QByteArray>
#include <QDebug>
#include <QObject>


class TOTPGenerator : public QObject {
    Q_OBJECT

public:
    TOTPGenerator(const QString &key, QObject *parent = nullptr)
        : QObject(parent), m_secretKey(key) {
        // 30 saniyelik döngü
        QTimer *timer = new QTimer(this);
        //connect(timer, &QTimer::timeout, this, &TOTPGenerator::generateAndDisplayOTP);
        timer->start(1000); // Her saniyede bir güncelle
        generateAndDisplayOTP(m_secretKey); // İlk OTP'yi oluştur
    }

private slots:
    void generateAndDisplayOTP(QString m_secretKey);

public:
    TOTPGenerator();
    QString m_secretKey;
    QString static generateTOTP(const QString &key);
    QByteArray static HMAC_SHA1(const QByteArray&, const QByteArray&);
};

#endif // TOTPGENERATOR_H
