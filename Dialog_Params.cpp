#include "Dialog_Params.hpp"

namespace Dialog
{
    Params::Params() : QDialog(nullptr)
    {
        setWindowTitle("Load parameters");

        QVBoxLayout* layout = new QVBoxLayout(this);
        QHBoxLayout *btnLayout = new QHBoxLayout;

        m_Table = new QTableWidget(this);
        m_AddRawButton = new QPushButton("Add raw", this);
        m_SubmitButton = new QPushButton("Submit", this);

        btnLayout->addStretch();
        btnLayout->addWidget(m_AddRawButton);
        btnLayout->addWidget(m_SubmitButton);
        btnLayout->addStretch();

        layout->addWidget(m_Table);
        layout->addLayout(btnLayout);

        m_Table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        m_Table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        setLayout(layout);
    }

    void Params::UpdateModel(Well::Model model)
    {
        m_Model = model;

        if (model == Well::Model::None)
        {
            reject();
            return;
        }

        const QList<QString>& params = Well::g_ParamHeaders[static_cast<int>(model) - 1];

        m_Table->setRowCount(1);
        m_Table->setColumnCount(params.size());

        m_Table->setHorizontalHeaderLabels(params);
    }

    void Params::showEvent(QShowEvent* event)
    {
        QDialog::showEvent(event);

        QTimer::singleShot(0, this, [this]()
        {
            m_Table->resizeColumnsToContents();
            m_Table->updateGeometry();

            layout()->update();
            layout()->activate();

            adjustSize();
        });
    }
}
