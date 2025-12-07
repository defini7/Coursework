#pragma once

#include <QtWidgets>

#include "WellData.hpp"

#include "Menu_Model.hpp"
#include "Menu_Data.hpp"

#include "GfxScene.hpp"

class Main : public QMainWindow
{
    Q_OBJECT

public:
    Main();
    ~Main();

    void SetName(const QString& text);
    void SetModel(Well::Model model);

public slots:
    void CreateContextMenu(const Well::Data& data, const QPoint& menuPos);

signals:
    void OnLoadModeSelected(Menu::DataLoadMode);
    void OnModelChange(Well::Model);
    void OnSaveSelected(const QString&);
    void OnContextMenuRequested(const QPoint&);

private:
    void resizeEvent(QResizeEvent*) override;
    void keyPressEvent(QKeyEvent*) override;

private:
    QLabel* m_NameLabel;
    QLabel* m_ModelLabel;

    GfxScene* m_GfxScene;
    QGraphicsView* m_GfxSceneView;

    Menu::Model* m_ModelMenu;
    Menu::Data* m_DataMenu;

};
