#pragma once

#include <QtWidgets>

#include "WellData.hpp"

namespace Dialog
{
    class Params : public QDialog
    {
        Q_OBJECT

    public:
        Params();

        void UpdateModel(Well::Model model);

        void showEvent(QShowEvent* event) override;

    private:
        Well::Model m_Model = Well::Model::None;

        QTableWidget* m_Table;
        QPushButton* m_SubmitButton;
        QPushButton* m_AddRawButton;

    };
}
