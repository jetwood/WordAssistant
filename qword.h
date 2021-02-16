#ifndef QWORD_H
#define QWORD_H

#include <QObject>
#include <QAxObject>
#include <QAxBase>
#include <QStandardPaths>
#include <QFileDialog>
#include <QTextStream>

class QWord : public QObject
{
    Q_OBJECT
public:
    explicit QWord(QObject *parent=nullptr);

private:
    bool m_flagOpen;
    QAxObject *m_pWord;
    QAxObject *m_pDocuments;
    QAxObject *m_pDocument;
    QAxObject *m_pSelection;
    QString m_strDefaultPath;
    QString m_strDebugMessage;

signals:
    void errors(const QString &msg);
    void warning(const QString &msg);

public slots:
    void setClosed();

public:
    static void initializeFileDialog
        (QFileDialog &dialog, QFileDialog::AcceptMode acceptMode);
    void initialWordApplication(bool visible);
    bool isOpened();
    QAxObject *getActiveDocument();
    QAxObject *getSelection();
    QAxObject *application();
    bool open(const QString &filePath="", bool visibile=true);
    void create(const QString& templateFile="", bool visibile=true);
    void insertText(const QString& text);
    void paste();
    void pasteExcelTable();
    void pastePicture();
//    void generateMannul();
};

#endif // QWORD_H
