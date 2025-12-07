#include "Menu_Data.hpp"

#include <QFileDialog>

namespace Menu
{
    Data::Data(QWidget* parent) : QMenu(parent)
    {
        setTitle("Параметры");

        QAction* rawAct = addAction("Ввести");
        QAction* fileAct = addAction("Загрузить с файла");
        QAction* saveAct = addAction("Сохранить в файл");

        QObject::connect(rawAct, &QAction::triggered, [this]()
        {
            emit OnLoadModeSelected(DataLoadMode::Raw);
        });

        QObject::connect(fileAct, &QAction::triggered, [this]()
        {
            emit OnLoadModeSelected(DataLoadMode::File);
        });

        QObject::connect(saveAct, &QAction::triggered, [this]()
        {
            QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "All Files (*.*);;Text Files (*.txt)");

            if (!fileName.isEmpty())
                emit OnSaveSelected(fileName);
        });
    }
}
