#include "DataManager.hpp"
#include "UserContext.hpp"
#include "Correlation.hpp"

#include <QtPrintSupport/QtPrintSupport>

DataManager::DataManager()
{
    m_ParamsDialog.SetWellData(m_Data);
}

void DataManager::LoadData(Menu::DataLoadMode mode)
{
    if (mode == Menu::DataLoadMode::Raw)
        m_ParamsDialog.show();
    else // Menu::DataLoadMode::File
    {
        QString fileName = QFileDialog::getOpenFileName(&m_ParamsDialog, "Open File", "", "All Files (*)");

        QFile file(fileName);

        if (file.open(QIODevice::ReadOnly))
        {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            QJsonObject jo = doc.object();

            const QString model = jo["model"].toString();
            const QJsonArray params = jo["params"].toArray();
            const QString username = jo["username"].toString();

            if (params.isEmpty() || model.isEmpty() || username.isEmpty())
            {
                QMessageBox::critical(nullptr, "Загрузка данных", "Отсутствует режим, или параметры, или имя пользователя");
                return;
            }

            if (model == "gas")
                m_Data.model = Well::Model::Gas;
            else if (model == "condensate")
                m_Data.model = Well::Model::Condensate;
            else
            {
                QMessageBox::critical(nullptr, "Загрузка данных", "Неправильный режим");
                return;
            }

            QVector<QVector<double>> series(params.size());

            m_Data.records.clear();

            for (int i = 0; const auto& row : params)
            {
                QJsonArray param = row.toArray();
                series[i].resize(param.size());

                for (int j = 0; auto& v : series[i])
                    v = param[j++].toDouble();

                i++;
            }

            m_Data.records.resize(series[0].size());

            for (int i = 0; i < series.size(); i++)
            {
                switch (m_Data.model)
                {
                case Well::Model::Gas:
                {
                    for (int j = 0; j < series[i].size(); j++)
                    {
                        if      (i == 0) m_Data.records[j].bottomPressure = series[i][j];
                        else if (i == 1) m_Data.records[j].wellheadTemp = series[i][j];
                        else if (i == 2) m_Data.records[j].gasFlow = series[i][j];
                        else if (i == 3) m_Data.records[j].depth = series[i][j];
                        else if (i == 4) m_Data.records[j].permeability = series[i][j];
                        else if (i == 5) m_Data.records[j].porosity = series[i][j];
                    }
                }
                break;

                case Well::Model::Condensate:
                {
                    for (int j = 0; j < series[i].size(); j++)
                    {
                        if      (i == 0) m_Data.records[j].bottomPressure = series[i][j];
                        else if (i == 1) m_Data.records[j].wellheadTemp = series[i][j];
                        else if (i == 2) m_Data.records[j].gasFlow = series[i][j];
                        else if (i == 3) m_Data.records[j].condensateContent = series[i][j];
                        else if (i == 4) m_Data.records[j].depth = series[i][j];
                        else if (i == 5) m_Data.records[j].permeability = series[i][j];
                        else if (i == 6) m_Data.records[j].porosity = series[i][j];
                        else if (i == 7) m_Data.records[j].viscosity = series[i][j];
                        else if (i == 8) m_Data.records[j].density = series[i][j];
                    }
                }
                break;

                }
            }

            emit OnUpdateModel(m_Data.model);
            emit OnUpdateUsername(username);

            UserContext::Get().Set("name", username);
        }
    }
}

void DataManager::SaveData(const QString& fileName)
{
    if (m_Data.model == Well::Model::None || m_Data.model == Well::Model::Count)
    {
        QMessageBox::critical(nullptr, "Сохранение данных", "Не могу сохранить данные: режим отсутствует");
        return;
    }

    QVector<QVector<double>> data;
    ConvertWellDataToSeries(m_Data, data);

    QVector<QVector<double>> matrix;
    ComputeSpearmanMatrix(data, matrix);

    QJsonObject jo;

    jo["username"] = UserContext::Get().Get("name").toString();

    // Assume there are only 2 models: gas & condensate
    jo["model"] = m_Data.model == Well::Model::Gas ? "gas" : "condensate";

    QJsonArray params;

    for (int i = 0; i < data.size(); i++)
    {
        QJsonArray row;

        for (int j = 0; j < data[i].size(); j++)
            row.append(data[i][j]);

        params.append(row);
    }

    jo["params"] = params;

    QJsonArray spearmanMat;

    for (int i = 0; i < matrix.size(); i++)
    {
        QJsonArray row;

        for (int j = 0; j < matrix.size(); j++)
            row.append(matrix[i][j]);

        spearmanMat.append(row);
    }

    jo["spearman_matrix"] = spearmanMat;

    QJsonDocument doc(jo);
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toJson());
        file.close();
    }

    // Save to PDF

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);

    printer.setOutputFileName(fileName.split('.')[0] + ".pdf");

    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageLayout(QPageLayout(QPageSize(QPageSize::A4),
                                      QPageLayout::Portrait,
                                      QMarginsF(30, 30, 30, 30),
                                      QPageLayout::Millimeter));

    QPainter painter(&printer);
    painter.setRenderHint(QPainter::Antialiasing);

    const QList<QString>& paramHeaders = Well::GetParams(m_Data.model);
    int paramCount = paramHeaders.size();

    QVector<QString> strong, moderate, weak, nonlinear;

    for (int i = 0; i < paramCount; i++)
        for (int j = i + 1; j < paramCount; j++)
        {
            double r = matrix[i][j];
            double absR = qAbs(r);

            QString p1 = paramHeaders[i];
            QString p2 = paramHeaders[j];
            QString conn = QString("%1 ↔ %2: r=%3").arg(p1).arg(p2).arg(r);

            if (absR >= 0.9f) strong.append(conn);
            else if (absR >= 0.7f) moderate.append(conn);
            else if (absR >= 0.3f) weak.append(conn);
            else nonlinear.append(conn);
        }

    int y = 0;

    QFont titleFont("Arial", 20, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(50, y += 80, "📊 Отчет");

    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(50, y += 400, "Корреляционный анализ скважин");

    QFont infoFont("Arial", 12);
    painter.setFont(infoFont);
    painter.drawText(50, y += 300, QString("👤 Пользователь: %1").arg(UserContext::Get().Get("name").toString()));

    QString modelName = (m_Data.model == Well::Model::Gas) ? "Газовая" : "Газоконденсатная";
    painter.drawText(50, y += 300, QString("⚙️ Режим: %1").arg(modelName));

    painter.drawText(50, y += 300, QString("📈 Записей: %1").arg(m_Data.records.size()));
    painter.drawText(50, y += 300, QString("📊 Параметров: %1").arg(paramCount));

    painter.setFont(QFont("Arial", 13, QFont::Bold));
    painter.drawText(50, y += 300, "🔴 Сильные связи");

    painter.setFont(QFont("Arial", 11));

    if (!strong.isEmpty())
    {
        for (const auto& conn : strong)
            painter.drawText(60, y += 300, "• " + conn);
    }
    else
        painter.drawText(60, y += 300, "• Отсутствуют");

    painter.setFont(QFont("Arial", 13, QFont::Bold));
    painter.drawText(50, y += 300, "🟡 Умеренные связи");

    painter.setFont(QFont("Arial", 11));

    if (!moderate.isEmpty())
    {
        for (const auto& conn : moderate)
            painter.drawText(60, y += 300, "• " + conn);
    }
    else
        painter.drawText(60, y += 300, "• Отсутствуют");

    painter.setFont(QFont("Arial", 13, QFont::Bold));
    painter.drawText(50, y += 300, "🟢 Слабые связи");

    painter.setFont(QFont("Arial", 11));

    if (!weak.isEmpty())
    {
        for (const auto& conn : weak)
            painter.drawText(60, y += 300, "• " + conn);
    }
    else
        painter.drawText(60, y += 300, "• Отсутствуют");

    painter.setFont(QFont("Arial", 13, QFont::Bold));
    painter.drawText(50, y += 300, "⚪ Нелинейные связи");

    painter.setFont(QFont("Arial", 11));
    painter.drawText(60, y += 300, QString("• %1 связей").arg(nonlinear.size()));

    painter.setFont(QFont("Arial", 10));
    painter.drawText(50, y += 300, QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm"));
}

void DataManager::UpdateModel(Well::Model model)
{
    m_ParamsDialog.UpdateModel(model);
}

void DataManager::SendWellData(const QPoint& menuPos)
{
    emit OnWellDataRequest(m_Data, menuPos);
}
