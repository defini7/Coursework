#pragma once

#include <QMenu>

namespace Menu
{
    enum class DataLoadMode
    {
        Raw,
        File
    };

    class Data : public QMenu
    {
        Q_OBJECT

    public:
        Data(QWidget* parent);

    signals:
        void OnLoadModeSelected(DataLoadMode);

    };
}
