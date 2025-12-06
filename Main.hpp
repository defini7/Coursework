#pragma once

#include <QtWidgets>

#include "WellData.hpp"

#include "Menu_Model.hpp"
#include "Menu_Data.hpp"

class Main : public QMainWindow
{
    Q_OBJECT

public:
    Main();
    ~Main();

    void SetName(const QString& text);
    void SetModel(Well::Model model);

signals:
    void OnLoadModeSelected(Menu::DataLoadMode);
    void OnModelChange(Well::Model);

private:
    QLabel* m_NameLabel;
    QLabel* m_ModelLabel;

    QGraphicsScene* m_GfxScene;
    QGraphicsView* m_GfxSceneView;

    Menu::Model* m_ModelMenu;
    Menu::Data* m_DataMenu;

};
