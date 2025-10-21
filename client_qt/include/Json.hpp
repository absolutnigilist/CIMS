#pragma once
#include <QString>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <vector>

namespace Proto {

    inline QByteArray makeAdd(const std::vector<int>& vals) {
        QJsonObject obj;
        obj["cmd"] = "ADD";
        QJsonArray arr;
        for (int v : vals) arr.append(v);
        obj["values"] = arr;
        return QJsonDocument(obj).toJson(QJsonDocument::Compact) + '\n';
    }

    inline QByteArray makeGet() {
        QJsonObject obj; obj["cmd"] = "GET";
        return QJsonDocument(obj).toJson(QJsonDocument::Compact) + '\n';
    }

    inline QByteArray makeClear() {
        QJsonObject obj; obj["cmd"] = "CLEAR";
        return QJsonDocument(obj).toJson(QJsonDocument::Compact) + '\n';
    }

    struct Reply {
        QString status;            // "ok" | "error"
        QString message;
        std::vector<int> values;   // заполняется для GET
    };

    inline bool parseReplyLine(const QByteArray& line, Reply& out) {
        QJsonParseError err{};
        const QJsonDocument doc = QJsonDocument::fromJson(line, &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) return false;

        const auto obj = doc.object();
        out.status = obj.value("status").toString();
        out.message = obj.value("message").toString();
        out.values.clear();

        if (obj.contains("data") && obj.value("data").isObject()) {
            const auto data = obj.value("data").toObject();
            if (data.contains("values") && data.value("values").isArray()) {
                const auto arr = data.value("values").toArray();
                out.values.reserve(arr.size());
                for (const auto& v : arr) out.values.push_back(v.toInt());
            }
        }
        return true;
    }

} // namespace Proto