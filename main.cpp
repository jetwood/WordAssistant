#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

/* load chinese translator file */
    QTranslator *translator = new QTranslator;
    translator->load("WordAssistant_zh_CN.qm");
    QApplication::installTranslator(translator);

/* register settings */
    QCoreApplication::setOrganizationName("MySoft");
    QCoreApplication::setOrganizationDomain("Mysoft.com");
    QCoreApplication::setApplicationName("Star Runner");

    MainWindow w;

/* multi-screens settings */
    QObject::connect(&a, &QApplication::screenAdded,
                     &w, &MainWindow::screenAdd);
    QObject::connect(&a, &QApplication::screenRemoved,
                     &w, &MainWindow::screenRemove);
    w.show();
    return a.exec();
}

