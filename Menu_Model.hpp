#pragma once

#include <QtWidgets>

#include "WellData.hpp"

namespace Menu
{
    class Model : public QMenu
    {
        Q_OBJECT

    public:
        Model(QWidget* parent);

    signals:
        void OnModelChange(Well::Model);

    private:
        void SetModel(Well::Model);

    };
}
