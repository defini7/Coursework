#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneEvent>

class GfxScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GfxScene(QObject* parent = nullptr);

    void Construct(int width, int height);

private:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;

private slots:
    void Update();

signals:
    void OnRightClick();

private:
    QGraphicsRectItem* m_Casing;
    QGraphicsRectItem* m_SurfaceCasing;

    QTimer* m_Timer;

    QList<QGraphicsEllipseItem*> m_Bubbles;
    QVector<double> m_BubbleSpeeds;
    QVector<QPointF> m_BubblePoses;

    static constexpr int s_BubbleCount = 20;
    static constexpr double s_BubbleRadius = 12;
};
