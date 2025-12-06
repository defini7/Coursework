#include "Main.hpp"
#include "DataLoader.hpp"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Main window;
    DataLoader dataLoader;

    QObject::connect(&window, &Main::OnLoadModeSelected, &dataLoader, &DataLoader::LoadData);
    QObject::connect(&window, &Main::OnModelChange, &dataLoader, &DataLoader::UpdateModel);

    window.show();

    return app.exec();
}
