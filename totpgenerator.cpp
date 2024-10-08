#include "totpgenerator.h"

TOTPGenerator::TOTPGenerator() {}

QString TOTPGenerator:: generateTOTP(const QString &key){
    // Zaman damgasını al
    qint64 currentTime = QDateTime::currentSecsSinceEpoch();
    // 30 saniyelik zaman dilimi
    qint64 timeStep = currentTime / 30;

    // Zaman damgasını byte dizisine dönüştür
    QByteArray timeBytes;
    for (int i = 7; i >= 0; --i) {
        timeBytes.append(static_cast<char>((timeStep >> (i * 8)) & 0xFF));
    }

    // HMAC-SHA1 hesapla
    QByteArray hmac = HMAC_SHA1(key.toUtf8(), timeBytes);
    // HMAC'tan bir kod oluştur
    int offset = static_cast<unsigned char>(hmac[hmac.size() - 1]) & 0x0F;
    int code = (hmac[offset] & 0x7F) << 24 |
               (hmac[offset + 1] & 0xFF) << 16 |
               (hmac[offset + 2] & 0xFF) << 8 |
               (hmac[offset + 3] & 0xFF);

    // 6 haneli OTP kodu oluştur
    code %= 1000000; // 0-999999 aralığında

    // 6 haneli kodu döndür
    return QString::number(code).rightJustified(6, '0');
}

QByteArray TOTPGenerator::HMAC_SHA1(const QByteArray &key, const QByteArray &data) {
    return QCryptographicHash::hash(key + data, QCryptographicHash::Sha1);
}

void TOTPGenerator::generateAndDisplayOTP(QString m_secretKey ) {
    QString otp = generateTOTP(m_secretKey);
    qDebug() << "Current OTP:" << otp;
}
