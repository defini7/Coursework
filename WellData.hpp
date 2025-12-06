#pragma once

#include <QVector>
#include <QObject>

namespace Well
{
    enum class Model
    {
        None,
        Gas,
        Condensate,
        Count
    };

    inline constexpr int g_ModelCount = static_cast<int>(Model::Count) - 1;

    struct Record
    {
        double bottomPressure;   // Давление забоя, МПа
        double wellheadTemp;     // Устьевая температура, °С
        double gasFlow;          // Расход газа, тыс. м³/сут
        double condensateContent; // Содержание конденсата, кг/м³
        double depth;            // Глубина, м
        double permeability;     // Проницаемость пласта
        double porosity;         // Пористость пласта
        double viscosity;        // Вязкость конденсата
        double density;          // Плотность конденсата
    };

    // Here we don't take into account Model::None
    inline const QList<QString> g_ParamHeaders[g_ModelCount] =
    {
        { "Bottom pressure", "Wellhead temperature", "Gas flow",
        "Depth", "Permeability", "Porosity" },

        { "Bottom pressure", "Wellhead temperature", "Gas flow",
        "Condensate content", "Depth", "Permeability",
        "Porosity", "Viscosity", "Density" }
    };

    class Data : QObject
    {
        Q_OBJECT

    public:
        Data();

        const QVector<Record>& GetRecords() const;

    public slots:
        void AddRecord(const Record&);
        void UpdateModel(Model);

    private:
        QVector<Record> m_Records;
        Model m_Model;

    };
}
