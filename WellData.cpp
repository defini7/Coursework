#include "WellData.hpp"

namespace Well
{
    Data::Data() {}

    const QVector<Record>& Data::GetRecords() const
    {
        return m_Records;
    }

    void Data::AddRecord(const Record& record)
    {
        m_Records.push_back(record);
    }

    void Data::UpdateModel(Model model)
    {
        m_Model = model;
    }
}
