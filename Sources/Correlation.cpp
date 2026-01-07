#include "Include/Correlation.hpp"

void ConvertWellDataToSeries(const Well::Data& in, QVector<QVector<double>>& out)
{
    int paramsCount = Well::GetParams(in.model).size();

    out.clear();
    out.resize(paramsCount);

    for (auto& var : out)
        var.reserve(in.records.size());

    for (const auto& r : in.records)
    {
        int i = 0;

        switch (in.model)
        {
        case Well::Model::Gas:
        {
            out[i++].push_back(r.bottomPressure);
            out[i++].push_back(r.wellheadTemp);
            out[i++].push_back(r.gasFlow);
            out[i++].push_back(r.depth);
            out[i++].push_back(r.permeability);
            out[i  ].push_back(r.porosity);
        }
        break;

        case Well::Model::Condensate:
        {
            out[i++].push_back(r.bottomPressure);
            out[i++].push_back(r.wellheadTemp);
            out[i++].push_back(r.gasFlow);
            out[i++].push_back(r.condensateContent);
            out[i++].push_back(r.depth);
            out[i++].push_back(r.permeability);
            out[i++].push_back(r.porosity);
            out[i++].push_back(r.viscosity);
            out[i  ].push_back(r.density);
        }
        break;

        default: return;

        }
    }
}

void ComputeRanks(QVector<QVector<double>> in, QVector<QVector<double>>& out)
{
    out.clear();
    out.resize(in.size());

    for (int i = 0; i < in.size(); i++)
    {
        auto values = in[i];

        const int valuesCount = values.size();

        QVector<int> indices(valuesCount);

        std::iota(indices.begin(), indices.end(), 0);
        std::sort(indices.begin(), indices.end(), [&](int a, int b) { return values[a] < values[b]; });

        out[i].resize(valuesCount);

        int j = 0;
        while (j < valuesCount)
        {
            double start = j;
            double value = values[indices[j]];

            while (j < valuesCount && values[indices[j]] == value)
                j++;

            double rank = (start + 1.0 + j) / 2.0;

            for (int k = start; k < j; k++)
                out[i][indices[k]] = rank;
        }
    }
}

void ComputeSpearmanMatrix(const QVector<QVector<double>>& in, QVector<QVector<double>>& out)
{
    QVector<QVector<double>> ranks;
    ComputeRanks(in, ranks);

    const int paramsCount = in.size();
    const int seriesCount = in[0].size();

    out.clear();
    out.resize(paramsCount);

    for (int i = 0; i < paramsCount; i++)
    {
        out[i].resize(paramsCount);

        for (int j = 0; j < paramsCount; j++)
        {
            double sum = 0.0;

            for (int k = 0; k < ranks[i].size(); k++)
            {
                double d = ranks[i][k] - ranks[j][k];
                sum += d * d;
            }

            out[i][j] = 1.0 - 6.0 * sum / (seriesCount * (seriesCount * seriesCount - 1.0));
        }
    }
}
