#pragma once

#include <QObject>

#include "Dialog_Params.hpp"
#include "Menu_Data.hpp"
#include "WellData.hpp"

class DataManager : public QObject
{
    Q_OBJECT

public:
    DataManager();

public slots:
    void LoadData(Menu::DataLoadMode mode);
    void SaveData(const QString& fileName);

    void UpdateModel(Well::Model model);
    void SendWellData(const QPoint& menuPos);

signals:
    void OnUpdateUsername(const QString&);
    void OnUpdateModel(Well::Model model);
    void OnWellDataRequest(const Well::Data&, const QPoint& menuPos);

private:
    Dialog::Params m_ParamsDialog;
    Well::Data m_Data;

};
