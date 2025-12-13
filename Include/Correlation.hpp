#pragma once

#include <QVector>
#include "WellData.hpp"

void ConvertWellDataToSeries(const Well::Data& in, QVector<QVector<double>>& out);

void ComputeRanks(QVector<QVector<double>> in, QVector<QVector<double>>& out);
void ComputeSpearmanMatrix(const QVector<QVector<double>>& in, QVector<QVector<double>>& out);
