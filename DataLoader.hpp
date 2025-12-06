#pragma once

#include <QObject>

#include "Dialog_Params.hpp"
#include "Menu_Data.hpp"

class DataLoader : public QObject
{
    Q_OBJECT

public:
    DataLoader();

public slots:
    void LoadData(Menu::DataLoadMode mode);
    void UpdateModel(Well::Model model);

private:
    Dialog::Params m_ParamsDialog;

};
