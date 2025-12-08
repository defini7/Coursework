#pragma once

#include <QSettings>

class UserContext
{
public:
    static UserContext& Get();

    QVariant Get(const QString& key) const;
    void Set(const QString& key, const QVariant& value);

    UserContext(UserContext&&) = delete;
    UserContext(const UserContext&) = delete;

    void operator=(UserContext&&) = delete;
    void operator=(const UserContext&) = delete;

private:
    UserContext();

private:
    QSettings m_Data;

};
