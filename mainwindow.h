#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* official libs */
#include <QMainWindow>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QClipboard>
#include <QList>
#include <QMimeData>
#include <QMenu>
#include <QActionGroup>
#include <QErrorMessage>

/* developer libs */
#include "networkmanager.h"
#include "imageviewer.h"
#include "qword.h"
#include "imagetobase64.h"
#include "globalshortcut.h"
#include "settingdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    typedef enum{
        None = 0,
        Texture,
        Picture
    }Mode;
    /* contructor */
    MainWindow(QWidget *parent = nullptr);

    /* destructor */
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    /* capture a image from screen and get text in image.
     * - triggered by QAction */
    void captureText();

    /* capture a image from screen.
     * - triggered by QAction */
    void capturePicture();

    /* process image data capture from imageviewer.
     * - connect every Imageviewer, triggered by sendImage(); */
    void getImageFromViewer(QImage image);

    /* Tray's operations include trigger, double click */
    void trayFeedBacks(QSystemTrayIcon::ActivationReason reason);

    /* create a new word file. */
    void createWordFile();

    /* open a word file. */
    void openWordFile();

    /* debug information. */
    void showErrorMessage(const QString &msg);

    /* application will create a imageviewer every screen user have.
     * - connect QApplication, triggered by QApplication's signal
     * screenAdded(); */
    void screenAdd(QScreen* screen);

    /* application will remove a imageviewer when a screen moved or closed.
     * - connect QApplication, triggered by QApplication's signal
     * screenRemoved(); */
    void screenRemove(QScreen* screen);

private:
    /* create the n widgets, the n equal the number of screens the user owned.
     * The widgets is class Imageviewer.
     * the Imageviewer is to screenshot as background.
     */
    void buildScreenWidgetList();

    /* show imageviewer when capture screen. */
    void showImageviewerList();

    /* hide imageviewer when capture screen finished */
    void hideImageviewerList();

    /* initial Tray, menu and toolbar */
    void createTrayMenuToolBar();

    /* establish all general connects in this application */
    void establishConnects();

    /* MODE on Picture or Texture.
     * Picture : use Rubberband to capture a image.
     * Texture : use Rubberband to capture a texture. */
    Mode m_eMode;

    /* imagevier list. */
    QList<ImageViewer*> m_pListViewer;

    /* clipboard history recode. */
    QList<QMimeData*> m_arrMimeDataList;

    /* The Clipboard. */
    QClipboard *m_pClipboard;

    /* The class word is to control word programe.*/
    QWord *m_pWord;

    /* The Tray icon */
    QSystemTrayIcon *m_pTray;

    /* setting dialog */
    SettingDialog *m_pSettingDialog;

    /* error dialog */
    QErrorMessage *m_pErrorDialog;

    /* network manager is a park to process network data. */
    NetworkManager *m_pNetworkManager;

    /* shortCut F1 */
    GlobalShortCut *shortcut_F1;

    /* shortCut F2 */
    GlobalShortCut *shortcut_F3;

    /* shortCut F3 */
    GlobalShortCut *shortcut_F4;

    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
