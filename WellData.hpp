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
        { "Давление забоя, МПа", "Устьевая температура, °С", "Расход газа, тыс. м³/сут",
        "Глубина, м", "Проницаемость пласта", "Пористость пласта" },

        { "Давление забоя, МПа", "Устьевая температура, °С", "Расход газа, тыс. м³/сут",
        "Содержание конденсата, кг/м³", "Глубина, м", "Проницаемость пласта",
        "Пористость пласта", "Вязкость конденсата", "Плотность конденсата" }
    };

    inline const QList<QString>& GetParams(Model model)
    {
        return g_ParamHeaders[static_cast<int>(model) - 1];
    }

    struct Data
    {
        QVector<Record> records;
        Model model = Model::None;
    };
}
