#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>
#include <QVariantMap>

class JsonHelper {
public:
    // JSON string'ini parse ederek bir QJsonObject döndürür
    static QJsonObject parseJson(const QString& jsonString) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
        if (jsonDoc.isObject()) {
            return jsonDoc.object();
        } else {
            return QJsonObject(); // Boş obje döner
        }
    }

    // QJsonObject'ten JSON string'i oluşturur
    static QString toJsonString(const QJsonObject& jsonObject) {
        QJsonDocument jsonDoc(jsonObject);
        return jsonDoc.toJson(QJsonDocument::Compact);
    }

    // Bir QVariantMap'ten QJsonObject oluşturur
    static QJsonObject mapToJsonObject(const QVariantMap& map) {
        return QJsonObject::fromVariantMap(map);
    }

    // QJsonObject'i QVariantMap'e çevirir
    static QVariantMap jsonObjectToMap(const QJsonObject& jsonObject) {
        return jsonObject.toVariantMap();
    }

    // QJsonObject'ten bir key'e karşılık gelen değeri döner
    static QJsonValue getValueFromJson(const QJsonObject& jsonObject, const QString& key) {
        if (jsonObject.contains(key)) {
            return jsonObject.value(key);
        } else {
            return QJsonValue();
        }
    }

    // JSON string'i içerisinden belirli bir key'in değerini döner
    static QJsonValue getValueFromJsonString(const QString& jsonString, const QString& key) {
        QJsonObject jsonObject = parseJson(jsonString);
        return getValueFromJson(jsonObject, key);
    }

    // QJsonArray oluşturma
    static QJsonArray createJsonArray(const QList<QVariant>& list) {
        QJsonArray jsonArray;
        for (const QVariant& item : list) {
            jsonArray.append(QJsonValue::fromVariant(item));
        }
        return jsonArray;
    }

    // QJsonArray'den QVariantList oluşturma
    static QList<QVariant> jsonArrayToList(const QJsonArray& jsonArray) {
        QList<QVariant> list;
        for (const QJsonValue& value : jsonArray) {
            list.append(value.toVariant());
        }
        return list;
    }
};

#endif // JSONHELPER_H
