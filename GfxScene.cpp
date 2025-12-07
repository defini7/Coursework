#include "GfxScene.hpp"
#include "UserContext.hpp"
#include "WellData.hpp"

#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QLinearGradient>
#include <QRandomGenerator>
#include <QFont>
#include <QColor>
#include <QWidget>

GfxScene::GfxScene(QObject* parent) : QGraphicsScene(parent)
{
    QLinearGradient bgGradient(0, 0, 0, 600);
    bgGradient.setColorAt(0, Qt::cyan);
    bgGradient.setColorAt(0.5, QColor(80, 50, 30));
    bgGradient.setColorAt(1, QColor(50, 30, 20));
    setBackgroundBrush(bgGradient);

    m_Timer = new QTimer(this);
    connect(m_Timer, &QTimer::timeout, this, &GfxScene::Update);
    m_Timer->start(1000.0 / 60.0);
}

void GfxScene::Construct(int width, int height)
{
    clear();
    m_Bubbles.clear();
    m_BubblePoses.clear();
    m_BubbleSpeeds.clear();

    setSceneRect(0, 0, width, height);

    int centerX = width / 2;
    int surfaceY = height * 0.4;

    if (surfaceY < 0) surfaceY = 0;
    if (surfaceY > height - 50) surfaceY = height - 50;

    QLinearGradient groundGradient(0, surfaceY, 0, height);
    groundGradient.setColorAt(0, QColor(101, 67, 33));
    groundGradient.setColorAt(0.5, QColor(139, 90, 43));
    groundGradient.setColorAt(1, QColor(160, 120, 80));

    addRect(0, surfaceY, width, height - surfaceY,
            QPen(Qt::NoPen), QBrush(groundGradient));

    for (int i = 0; i < 5; i++)
    {
        int y = surfaceY + i * 40;

        if (y < height - 20 && y > surfaceY)
            addLine(0, y, width, y, QPen(QColor(80, 50, 30, 150), 2));
    }

    int casingHeight = height - surfaceY;
    if (casingHeight < 50) casingHeight = 50;

    m_Casing = addRect(centerX - 20, surfaceY, 40, casingHeight,
                       QPen(QColor(30, 30, 30), 2),
                       QBrush(QColor(70, 70, 70)));

    QLinearGradient innerGrad(centerX - 15, surfaceY, centerX + 15, height);

    innerGrad.setColorAt(0, QColor(150, 150, 150));
    innerGrad.setColorAt(1, QColor(100, 100, 100));

    addRect(centerX - 15, surfaceY + 10, 30, casingHeight - 20,
            QPen(Qt::NoPen), QBrush(innerGrad));

    int surfaceCasingHeight = height * 0.1;

    if (surfaceCasingHeight > surfaceY)
        surfaceCasingHeight = surfaceY * 0.8;

    m_SurfaceCasing = addRect(centerX - 25, surfaceY - surfaceCasingHeight,
                              50, surfaceCasingHeight,
                              QPen(QColor(40, 40, 40), 3),
                              QBrush(QColor(90, 90, 90)));

    addRect(centerX - 20, surfaceY - surfaceCasingHeight + 5,
            40, surfaceCasingHeight - 10,
            QPen(Qt::NoPen),
            QBrush(QColor(180, 180, 180, 200)));

    addRect(centerX - 100, surfaceY - 20, 25, 25,
            QPen(QColor(40, 40, 40), 2), QBrush(QColor(110, 110, 110)));
    addRect(centerX - 93, surfaceY - 10, 11, 2,
            QPen(Qt::NoPen), QBrush(QColor(180, 180, 180)));
    addLine(centerX - 75, surfaceY - 8, centerX - 20, surfaceY - 8,
            QPen(QColor(90, 90, 90), 4));

    addRect(centerX + 75, surfaceY - 20, 25, 25,
            QPen(QColor(40, 40, 40), 2), QBrush(QColor(110, 110, 110)));
    addRect(centerX + 88, surfaceY - 10, 2, 11,
            QPen(Qt::NoPen), QBrush(QColor(180, 180, 180)));
    addLine(centerX + 20, surfaceY - 8, centerX + 75, surfaceY - 8,
            QPen(QColor(90, 90, 90), 4));

    int lowerValveY = surfaceY + 150;

    if (lowerValveY > height - 50)
        lowerValveY = height - 100;

    if (lowerValveY < surfaceY + 50)
        lowerValveY = surfaceY + 100;

    addRect(centerX - 100, lowerValveY - 12, 25, 25,
            QPen(QColor(50, 50, 50), 2), QBrush(QColor(120, 120, 120)));
    addRect(centerX - 93, lowerValveY - 2, 11, 2,
            QPen(Qt::NoPen), QBrush(QColor(190, 190, 190)));
    addLine(centerX - 75, lowerValveY, centerX - 20, lowerValveY,
            QPen(QColor(100, 100, 100), 4));

    addRect(centerX + 75, lowerValveY - 12, 25, 25,
            QPen(QColor(50, 50, 50), 2), QBrush(QColor(120, 120, 120)));
    addRect(centerX + 88, lowerValveY - 2, 2, 11,
            QPen(Qt::NoPen), QBrush(QColor(190, 190, 190)));
    addLine(centerX + 20, lowerValveY, centerX + 75, lowerValveY,
            QPen(QColor(100, 100, 100), 4));

    int verticalPipeLength = lowerValveY - (surfaceY + 50);

    if (verticalPipeLength > 20)
    {
        addLine(centerX - 88, surfaceY + 50, centerX - 88, lowerValveY,
            QPen(QColor(85, 85, 85), 3));

        addLine(centerX + 88, surfaceY + 50, centerX + 88, lowerValveY,
            QPen(QColor(85, 85, 85), 3));
    }

    QRandomGenerator* rand = QRandomGenerator::global();

    for (int i = 0; i < 15; i++)
    {
        int stoneX = rand->bounded(width - 20) + 10;
        int stoneY = surfaceY + 20;

        int availableHeight = height - surfaceY - 40;

        if (availableHeight > 20)
            stoneY += rand->bounded(availableHeight);
        else
            stoneY += 10;

        if (stoneY > height - 20)
            stoneY = height - 20;

        int stoneSize = 5 + rand->bounded(15);

        QColor stoneColor = QColor(
            70 + rand->bounded(60),
            45 + rand->bounded(40),
            25 + rand->bounded(30)
        );

        addEllipse(stoneX, stoneY, stoneSize, stoneSize,
            QPen(Qt::NoPen), QBrush(stoneColor));
    }

    m_BubbleSpeeds.resize(s_BubbleCount);
    m_BubblePoses.resize(s_BubbleCount);

    for (int i = 0; i < s_BubbleCount; i++)
    {
        double startY = surfaceY + 50;
        double startX = centerX - 5 + i / s_BubbleCount * 2.0;

        QGraphicsEllipseItem* bubble = addEllipse(0, 0, s_BubbleRadius, s_BubbleRadius, QPen(Qt::white, 1));

        bubble->setPos(startX, startY);
        bubble->setOpacity(0.65);

        m_Bubbles.append(bubble);
        m_BubblePoses[i] = { startX, startY };
        m_BubbleSpeeds[i] = 0.5 + QRandomGenerator::global()->bounded(4.5);
    }

    update();
}

void GfxScene::Update()
{
    static double time = 0.0;

    bool validModel;
    int model = UserContext::Get().Get("model").toInt(&validModel);

    if (m_Bubbles.isEmpty() || m_BubbleSpeeds.isEmpty() || !validModel)
        return;

    time += 0.08;

    const int surfaceY = height() * 0.4;

    bool isCondensate = model == static_cast<int>(Well::Model::Condensate);

    for (int i = 0; i < m_Bubbles.size(); i++)
    {
        double bubbleY = m_BubblePoses[i].y() - m_BubbleSpeeds[i];
        double bubbleX = m_BubblePoses[i].x() + sin(time * 3.0 + i) * 2.0;

        if (bubbleY < surfaceY - 100)
        {
            bubbleY = height();
            m_BubbleSpeeds[i] = 1.0 + QRandomGenerator::global()->bounded(1.0);
        }

        if (isCondensate && i > m_Bubbles.size() / 2)
            m_Bubbles[i]->setBrush(QBrush(Qt::darkYellow));
        else
            m_Bubbles[i]->setBrush(QBrush(Qt::cyan));

        m_BubblePoses[i] = { bubbleX, bubbleY };
        m_Bubbles[i]->setOpacity(bubbleY / (height() - surfaceY + 100));
        m_Bubbles[i]->setPos(bubbleX, bubbleY);
    }

    update();
}

void GfxScene::mousePressEvent(QGraphicsSceneMouseEvent* evt)
{
    if (evt->button() == Qt::LeftButton)
    {
        QPointF mouse = evt->scenePos();

        for (int i = 0; auto& pos : m_BubblePoses)
        {
            QPointF d = mouse - pos;

            if (d.x() * d.x() + d.y() * d.y() < s_BubbleRadius * s_BubbleRadius)
            {
                pos.setY(height());
                m_BubbleSpeeds[i] = 1.0 + QRandomGenerator::global()->bounded(1.0);
            }
        }
    }

    QGraphicsScene::mousePressEvent(evt);
}
