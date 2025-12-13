#include "UserContext.hpp"

UserContext& UserContext::Get()
{
    static UserContext me;
    return me;
}

UserContext::UserContext() : m_Data("Well", "UserData") {}

QVariant UserContext::Get(const QString& key) const
{
    return m_Data.value(key);
}

void UserContext::Set(const QString& key, const QVariant& value)
{
    m_Data.setValue(key, value);
}
