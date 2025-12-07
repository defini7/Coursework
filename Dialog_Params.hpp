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
        void SetWellData(Well::Data& wellData);

        void showEvent(QShowEvent* event) override;
        void hideEvent(QHideEvent* event) override;

    private:
        void UpdateSize();

    private:
        QTableWidget* m_Table;
        QPushButton* m_SubmitButton;
        QPushButton* m_AddRowButton;

        Well::Data* m_Data;

    };
}
