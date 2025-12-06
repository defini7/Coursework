#include "Menu_Data.hpp"

namespace Menu
{
    Data::Data(QWidget* parent) : QMenu(parent)
    {
        setTitle("Data");

        QAction* rawAct = addAction("Load Raw");
        QAction* fileAct = addAction("Load File");

        QObject::connect(rawAct, &QAction::triggered, [this]()
        {
            emit OnLoadModeSelected(DataLoadMode::Raw);
        });

        QObject::connect(fileAct, &QAction::triggered, [this]()
        {
            emit OnLoadModeSelected(DataLoadMode::File);
        });
    }
}
