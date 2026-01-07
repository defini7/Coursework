#include "Include/Main.hpp"
#include "Include/DataManager.hpp"
#include "Include/UserContext.hpp"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Main window;
    DataManager dataManager;

    UserContext::Get().Set("model", static_cast<int>(Well::Model::None));

    QObject::connect(&window, &Main::OnLoadModeSelected, &dataManager, &DataManager::LoadData);
    QObject::connect(&window, &Main::OnSaveSelected, &dataManager, &DataManager::SaveData);
    QObject::connect(&window, &Main::OnModelChange, &dataManager, &DataManager::UpdateModel);
    QObject::connect(&window, &Main::OnContextMenuRequested, &dataManager, &DataManager::SendWellData);

    QObject::connect(&dataManager, &DataManager::OnUpdateModel, &window, &Main::SetModel);
    QObject::connect(&dataManager, &DataManager::OnUpdateUsername, &window, &Main::SetName);
    QObject::connect(&dataManager, &DataManager::OnWellDataRequest, &window, &Main::CreateContextMenu);

    window.show();

    return app.exec();
}
