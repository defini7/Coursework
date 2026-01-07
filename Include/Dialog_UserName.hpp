#pragma once

#include <QtWidgets>

namespace Dialog
{
    class UserName : public QDialog
    {
    public:
        UserName(QWidget* parent);

    private:
        QLineEdit* m_NameEdit;
        QPushButton* m_SubmitButton;
        QLabel* m_NameLabel;

    };
}
