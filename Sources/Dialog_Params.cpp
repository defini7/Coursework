#include "Include/Dialog_Params.hpp"

namespace Dialog
{
    Params::Params() : QDialog(nullptr)
    {
        setWindowTitle("Загрузка параметров");

        QVBoxLayout* vlayout = new QVBoxLayout(this);
        QHBoxLayout* btnLayout = new QHBoxLayout;

        m_Table = new QTableWidget(this);
        m_AddRowButton = new QPushButton("Добавить строку", this);
        m_SubmitButton = new QPushButton("Подтвердить", this);

        btnLayout->addStretch();
        btnLayout->addWidget(m_AddRowButton);
        btnLayout->addWidget(m_SubmitButton);
        btnLayout->addStretch();

        vlayout->addWidget(m_Table);
        vlayout->addLayout(btnLayout);

        m_Table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        m_Table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        setLayout(vlayout);

        QObject::connect(m_AddRowButton, &QPushButton::clicked, [this]()
        {
            int row = m_Table->rowCount();

            m_Table->insertRow(row);

            int paramsCount = Well::GetParams(m_Data->model).size();

            for (int col = 0; col < paramsCount; col++)
            {
                QTableWidgetItem *item = new QTableWidgetItem();

                item->setTextAlignment(Qt::AlignCenter);
                item->setData(Qt::EditRole, "0.0");

                m_Table->setItem(row, col, item);
            }

            UpdateSize();
        });

        QObject::connect(m_SubmitButton, &QPushButton::clicked, [this]()
        {
            m_Data->records.clear();
            m_Data->records.reserve(m_Table->rowCount());

            auto GetItem = [&](int row, int col)
            {
                QTableWidgetItem* item = m_Table->item(row, col);
                QString text = item->text();

                bool ok;
                double value = text.toDouble(&ok);

                if (!ok)
                    QMessageBox::warning(this, "Неправильный ввод", "В ячейку введено не число!");

                return ok ? value : 0.0;
            };

            for (int row = 0; row < m_Table->rowCount(); row++)
            {
                Well::Record record;

                int col = 0;

                switch (m_Data->model)
                {
                case Well::Model::Gas:
                {
                    record.bottomPressure = GetItem(row, col++);
                    record.wellheadTemp = GetItem(row, col++);
                    record.gasFlow = GetItem(row, col++);
                    record.depth = GetItem(row, col++);
                    record.permeability = GetItem(row, col++);
                    record.porosity = GetItem(row, col++);
                }
                break;

                case Well::Model::Condensate:
                {
                    record.bottomPressure = GetItem(row, col++);
                    record.wellheadTemp = GetItem(row, col++);
                    record.gasFlow = GetItem(row, col++);
                    record.condensateContent = GetItem(row, col++);
                    record.depth = GetItem(row, col++);
                    record.permeability = GetItem(row, col++);
                    record.porosity = GetItem(row, col++);
                    record.viscosity = GetItem(row, col++);
                    record.density = GetItem(row, col++);
                }
                break;

                default: reject();

                }

                m_Data->records.append(record);
            }

            accept();
        });
    }

    void Params::UpdateModel(Well::Model model)
    {
        m_Data->model = model;
        m_Data->records.clear();

        if (model == Well::Model::None)
        {
            reject();
            return;
        }

        const QList<QString>& params = Well::GetParams(model);

        m_Table->setRowCount(0);
        m_Table->setColumnCount(params.size());

        m_Table->setHorizontalHeaderLabels(params);
    }

    void Params::SetWellData(Well::Data& wellData)
    {
        m_Data = &wellData;
    }

    void Params::showEvent(QShowEvent* event)
    {
        QDialog::showEvent(event);
        UpdateSize();
    }

    void Params::hideEvent(QHideEvent* event)
    {
        QDialog::hideEvent(event);
        m_Table->setRowCount(0);
    }

    void Params::UpdateSize()
    {
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
