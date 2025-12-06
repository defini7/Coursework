#include "Menu_Model.hpp"
#include "UserContext.hpp"

namespace Menu
{
    Model::Model(QWidget* parent) : QMenu(parent)
    {
        setTitle("Model");

        QAction* gasAct = addAction("Gas");
        QAction* condAct = addAction("Condensate");

        QObject::connect(gasAct, &QAction::triggered, [&]()
        {
            SetModel(Well::Model::Gas);
        });

        QObject::connect(condAct, &QAction::triggered, [&]()
        {
            SetModel(Well::Model::Condensate);
        });
    }

    void Model::SetModel(Well::Model model)
    {
        UserContext::Get().Set("model", static_cast<int>(model));
        emit OnModelChange(model);
    }
}
