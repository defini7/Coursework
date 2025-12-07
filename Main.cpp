#include "Main.hpp"
#include "UserNameDialog.hpp"
#include "UserContext.hpp"

Main::Main() : QMainWindow(nullptr)
{
    setWindowTitle("Курсовая работа");
    resize(800, 600);

    QWidget* central = new QWidget(this);

    m_NameLabel = new QLabel();
    m_ModelLabel = new QLabel();

    m_GfxSceneView = new QGraphicsView(central);
    m_GfxScene = new GfxScene(m_GfxSceneView);

    m_GfxSceneView->setScene(m_GfxScene);

    m_GfxScene->Construct(m_GfxSceneView->width(), m_GfxSceneView->height());

    SetName("-");
    SetModel(Well::Model::None);

    m_GfxSceneView->setRenderHint(QPainter::Antialiasing, true);
    m_GfxSceneView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_GfxSceneView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout* layout = new QVBoxLayout(central);
    QHBoxLayout* labelLayout = new QHBoxLayout(central);

    labelLayout->addWidget(m_NameLabel);
    labelLayout->addWidget(m_ModelLabel);

    layout->addLayout(labelLayout);
    layout->addWidget(m_GfxSceneView);

    central->setLayout(layout);
    setCentralWidget(central);

    m_ModelMenu = new Menu::Model(menuBar());
    menuBar()->addMenu(m_ModelMenu);

    m_DataMenu = new Menu::Data(menuBar());
    menuBar()->addMenu(m_DataMenu);

    QObject::connect(m_ModelMenu, &Menu::Model::OnModelChange, this, [this](Well::Model model)
        {
            SetModel(model);
            emit OnModelChange(model);
        });

    QObject::connect(m_DataMenu, &Menu::Data::OnLoadModeSelected, this, [this](Menu::DataLoadMode mode)
        {
            emit OnLoadModeSelected(mode);
        });

    QObject::connect(m_DataMenu, &Menu::Data::OnSaveSelected, this, [this](const QString& fileName)
        {
            emit OnSaveSelected(fileName);
        });

    QTimer::singleShot(0, this, [this]()
        {
           UserNameDialog userNameDialog(this);

           if (userNameDialog.exec() == QDialog::Accepted)
               SetName(UserContext::Get().Get("name").toString());

           userNameDialog.close();
        });

    m_GfxSceneView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_GfxSceneView, &QGraphicsView::customContextMenuRequested,
        this, [this](const QPoint& pos)
        {
            emit OnContextMenuRequested(m_GfxSceneView->mapToGlobal(pos));
        });
}

Main::~Main()
{

}

void Main::SetName(const QString& text)
{
    m_NameLabel->setText("Имя пользователя: " + text);
}

void Main::SetModel(Well::Model model)
{
    QString strModel;

    switch (model)
    {
    case Well::Model::Gas: strModel = "газовая"; break;
    case Well::Model::Condensate: strModel = "газоконденсатная"; break;
    case Well::Model::None: strModel = "-"; break;
    }

    m_ModelLabel->setText("Режим: " + strModel);
}

void Main::CreateContextMenu(const Well::Data& data, const QPoint& menuPos)
{
    QMenu menu(this);

    for (const auto& param : Well::GetParams(data.model))
        menu.addAction(param)->setEnabled(false);

    menu.exec(menuPos);
}

void Main::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    if (m_GfxScene && m_GfxSceneView)
    {
        QTimer::singleShot(10, this, [this]()
        {
            m_GfxScene->Construct(m_GfxSceneView->width(), m_GfxSceneView->height());
        });
    }
}
