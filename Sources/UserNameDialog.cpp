#include "UserNameDialog.hpp"
#include "UserContext.hpp"

UserNameDialog::UserNameDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Имя пользователя");
    resize(100, 70);

    QVBoxLayout* layout = new QVBoxLayout(this);

    m_NameLabel = new QLabel("Введите имя пользователя: ", this);

    m_NameEdit = new QLineEdit(this);
    m_NameEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[A-Za-zА-Яа-я0-9]*")));

    m_SubmitButton = new QPushButton("Подтвердить", this);

    layout->addWidget(m_NameLabel);
    layout->addWidget(m_NameEdit);
    layout->addWidget(m_SubmitButton);

    connect(m_SubmitButton, &QPushButton::clicked, this, [this]()
    {
        UserContext::Get().Set("name", m_NameEdit->text());
        accept();
    });
}
