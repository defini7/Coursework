#include "Main.hpp"
#include "UserNameDialog.hpp"
#include "UserContext.hpp"

Main::Main() : QMainWindow(nullptr)
{
    setWindowTitle("Coursework");
    resize(800, 600);

    QWidget* central = new QWidget(this);

    m_NameLabel = new QLabel();
    m_ModelLabel = new QLabel();

    m_GfxScene = new QGraphicsScene(0, 0, 800, 600, central);
    m_GfxSceneView = new QGraphicsView(m_GfxScene, central);

    m_GfxSceneView->fitInView(m_GfxScene->sceneRect(), Qt::KeepAspectRatio);

    // TODO: Remove it
    m_GfxScene->setForegroundBrush(QColor(255, 255, 255, 127));

    SetName("-");
    SetModel(Well::Model::None);

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

    QTimer::singleShot(0, this, [this]()
    {
        UserNameDialog* userNameDialog = new UserNameDialog(this);

        if (userNameDialog->exec() == QDialog::Accepted)
            SetName(UserContext::Get().Get("name").toString());

        delete userNameDialog;
    });
}

Main::~Main()
{

}

void Main::SetName(const QString& text)
{
    m_NameLabel->setText("Name: " + text);
}

void Main::SetModel(Well::Model model)
{
    QString strModel;

    switch (model)
    {
    case Well::Model::Gas: strModel = "gas"; break;
    case Well::Model::Condensate: strModel = "condensate"; break;
    case Well::Model::None: strModel = "-"; break;
    }

    m_ModelLabel->setText("Model: " + strModel);
}
