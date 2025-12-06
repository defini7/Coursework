#include "DataLoader.hpp"

DataLoader::DataLoader()
{

}

void DataLoader::LoadData(Menu::DataLoadMode mode)
{
    if (mode == Menu::DataLoadMode::Raw)
    {
        m_ParamsDialog.show();
    }
    else // Menu::DataLoadMode::File
    {

    }
}

void DataLoader::UpdateModel(Well::Model model)
{
    m_ParamsDialog.UpdateModel(model);
}
