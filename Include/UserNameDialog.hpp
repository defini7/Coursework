#pragma once

#include <QtWidgets>

class UserNameDialog : public QDialog
{
public:
    UserNameDialog(QWidget* parent);

private:
    QLineEdit* m_NameEdit;
    QPushButton* m_SubmitButton;
    QLabel* m_NameLabel;

};
