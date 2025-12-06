#include "UserContext.hpp"

UserContext& UserContext::Get()
{
    static UserContext me;
    return me;
}

UserContext::UserContext() : m_Data("Well", "UserData") {}

QVariant UserContext::Get(QAnyStringView key) const
{
    return m_Data.value(key);
}

void UserContext::Set(QAnyStringView key, const QVariant& value)
{
    m_Data.setValue(key, value);
}
