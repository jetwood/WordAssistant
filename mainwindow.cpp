#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::~MainWindow()
{
    ui->centralwidget->writeSettings();
    delete ui;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_eMode(None),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pErrorDialog = new QErrorMessage(this);
    setWindowFlags(Qt::WindowTitleHint
                   |Qt::WindowSystemMenuHint
                   |Qt::WindowCloseButtonHint
                   |Qt::WindowStaysOnTopHint);
    buildScreenWidgetList();
    m_pNetworkManager
            = new NetworkManager(this);
/* programe would check access token's availablity on Internet
 * in every initialization.
 * if access token available, enable actionCaptionText.
 * if not, disable actionCaptionText.
 * caution, disconnecting internet would disable actionCaption Text.
 */
    connect(m_pNetworkManager, &NetworkManager::available,
            ui->actionCaptureText, &QAction::setEnabled);
    QSettings settings;
    settings.beginGroup("apidata");
    QString accessToken =
    m_pNetworkManager->getAccessToken(settings.value("client_id").toString(),
                                 settings.value("client_secret").toString());
    settings.endGroup();
    if (accessToken != "")
        m_pNetworkManager->setAccessToken(accessToken);

    m_pClipboard = QGuiApplication::clipboard();
    m_pWord = new QWord(this);
    createTrayMenuToolBar();
    m_pSettingDialog = new SettingDialog(this);
    m_pTray->show();
    establishConnects();
}

void MainWindow::captureText()
{
    ui->actionCaptureText->setDisabled(true);
    disconnect(shortcut_F1, &GlobalShortCut::activated,
                     this, &MainWindow::captureText);
    m_eMode = Texture;
    foreach (ImageViewer *iterator, m_pListViewer)
        iterator->setRubberBandColor(Qt::blue);
    showImageviewerList();
}

void MainWindow::capturePicture()
{
    ui->actionCapturePicture->setDisabled(true);
    disconnect(shortcut_F3, &GlobalShortCut::activated,
                     this, &MainWindow::captureText);
    m_eMode = Picture;
    foreach (ImageViewer *iterator, m_pListViewer)
        iterator->setRubberBandColor(Qt::green);
    showImageviewerList();
}

void MainWindow::getImageFromViewer(QImage image)
{
    hideImageviewerList();
    QMimeData *pMimeData = new QMimeData;
    if (m_eMode == Texture){
        QByteArray imageData
                = "image=" + imageToBase64(image).toPercentEncoding();
        QString result = m_pNetworkManager->getString(imageData);
        pMimeData->setText(result);
        m_pClipboard->setMimeData(pMimeData, QClipboard::Clipboard);
        if (m_pWord->isOpened())
            m_pWord->insertText(result);
        ui->actionCaptureText->setEnabled(true);
        connect(shortcut_F1, &GlobalShortCut::activated,
                this, &MainWindow::captureText);
    }
    if (m_eMode == Picture){
        pMimeData->setImageData(image);
        m_pClipboard->setMimeData(pMimeData, QClipboard::Clipboard);
        if (m_pWord->isOpened())
            m_pWord->pastePicture();
        ui->actionCapturePicture->setEnabled(true);
        connect(shortcut_F3, &GlobalShortCut::activated,
                this, &MainWindow::capturePicture);
    }
    m_arrMimeDataList.append(pMimeData);
}

void MainWindow::trayFeedBacks(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::DoubleClick:
        this->show();
        break;
    default:
        break;
    }
}

void MainWindow::buildScreenWidgetList()
{
    ImageViewer *pImageViewer;
    for (QScreen *iterator : QGuiApplication::screens()){
        pImageViewer = new ImageViewer(this);
        pImageViewer->setScreen(iterator);
        pImageViewer->hide();
        connect(pImageViewer, &ImageViewer::sendImage,
                this, &MainWindow::getImageFromViewer);
        m_pListViewer.append(pImageViewer);
    }
}

void MainWindow::showImageviewerList()
{
    foreach (ImageViewer *iterator, m_pListViewer){
        iterator->grabScreen();
        iterator->setCursor(Qt::CrossCursor);
        iterator->show();
    }
}

void MainWindow::hideImageviewerList()
{
    foreach (ImageViewer *iterator, m_pListViewer)
        iterator->hide();
}


void MainWindow::showErrorMessage(const QString&msg)
{
    m_pTray->showMessage("Error", msg);
}

void MainWindow::createTrayMenuToolBar()
{
    QActionGroup *pWordFileActionGroup = new QActionGroup(this);
    pWordFileActionGroup->addAction(ui->actionOpenWordFile);
    pWordFileActionGroup->addAction(ui->actionCreateWordFile);

    QActionGroup *pWordOperationActionGroup = new QActionGroup(this);
    pWordOperationActionGroup->addAction(ui->actionCaptureText);
    pWordOperationActionGroup->addAction(ui->actionCapturePicture);
    pWordOperationActionGroup->addAction(ui->actionPasteInWord);

    m_pTray = new QSystemTrayIcon(QIcon(":/image/scanner.png"), this);
    m_pTray->setToolTip("Word Assisant");

    QMenu *pTrayMenu = new QMenu(tr("TrayMenu"), this);
    pTrayMenu->addAction(ui->actionMainMenu);
    pTrayMenu->addSeparator();
    pTrayMenu->addActions(pWordOperationActionGroup->actions());
    pTrayMenu->addSeparator();
    pTrayMenu->addAction(ui->actionQuit);
    m_pTray->setContextMenu(pTrayMenu);
}

void MainWindow::establishConnects()
{
    connect(ui->actionQuit, &QAction::triggered,
            [=](){QCoreApplication::exit();});

    connect(m_pSettingDialog, SIGNAL(errorMsg(const QString)),
            m_pErrorDialog, SLOT(showMessage(const QString&)));
    connect(ui->actionCreateWordFile, &QAction::triggered,
            this, &MainWindow::createWordFile);
    connect(ui->actionOpenWordFile, &QAction::triggered,
           this, &MainWindow::openWordFile);
    connect(ui->actionCaptureText, &QAction::triggered,
            this, &MainWindow::captureText);
    connect(ui->actionCapturePicture, &QAction::triggered,
            this, &MainWindow::capturePicture);
    connect(ui->actionPasteInWord, &QAction::triggered,
            m_pWord, &QWord::paste);

    connect(ui->actionSetting, &QAction::triggered,
            m_pSettingDialog, &SettingDialog::exec);

    connect(m_pTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
            SLOT(trayFeedBacks(QSystemTrayIcon::ActivationReason)));
    connect(ui->actionAddStrip, &QAction::triggered,
            ui->centralwidget, &StripListView::addNewStrip);
    connect(ui->actionSubstractStrip, &QAction::triggered,
            ui->centralwidget, &StripListView::deleteCurrentStrip);
    connect(ui->actionSave, &QAction::triggered,
            ui->centralwidget, &StripListView::writeSettings);
    connect(m_pNetworkManager, &NetworkManager::errors,
            this, &MainWindow::showErrorMessage);
    connect(ui->centralwidget, &StripListView::doubleClickTrigger,
           m_pWord,&QWord::insertText);
    shortcut_F1 = new GlobalShortCut("F1", this);
    connect(shortcut_F1, &GlobalShortCut::activated,
                     this, &MainWindow::captureText);
    shortcut_F3 = new GlobalShortCut("F3", this);
    connect(shortcut_F3, &GlobalShortCut::activated,
                     this, &MainWindow::capturePicture);
    shortcut_F4 = new GlobalShortCut("F4", this);
    connect(shortcut_F4, &GlobalShortCut::activated,
                     m_pWord, &QWord::paste);
    connect(m_pWord, &QWord::errors, this, &MainWindow::showErrorMessage);
}

void MainWindow::createWordFile()
{
    if (!m_pWord->isOpened()){
        m_pWord->initialWordApplication(true);
    }
    m_pWord->create();
}

void MainWindow::openWordFile()
{
    QFileDialog dialog;
    QWord::initializeFileDialog(dialog, QFileDialog::AcceptMode::AcceptOpen);
    if (!m_pWord->isOpened()){
        m_pWord->initialWordApplication(true);
    }
    while(dialog.exec()==QDialog::Accepted
          && !m_pWord->open(dialog.selectedFiles().first())){;};
}

void MainWindow::screenAdd(QScreen* screen)
{
    ImageViewer *pImageViewer = new ImageViewer(this);
    pImageViewer->setScreen(screen);
    pImageViewer->hide();
    connect(pImageViewer, &ImageViewer::sendImage,
            this, &MainWindow::getImageFromViewer);
    m_pListViewer.append(pImageViewer);
}

void MainWindow::screenRemove(QScreen* screen)
{
    foreach (ImageViewer *iterator, m_pListViewer){
        if (iterator->onScreen() == screen){
            iterator->close();
            m_pListViewer.removeOne(iterator);
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    hide();
    m_pTray->show();
    event->ignore();
}
