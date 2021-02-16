#include "qword.h"

void QWord::initializeFileDialog(QFileDialog &dialog,
                              QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;
    if (firstDialog){
        firstDialog = false;
        const QStringList documentsLocations = QStandardPaths::standardLocations
                                            (QStandardPaths::DocumentsLocation);

    dialog.setDirectory(documentsLocations.isEmpty()
                        ? QDir::currentPath() : documentsLocations.last());
    }

    if (acceptMode == QFileDialog::AcceptOpen)
        dialog.setNameFilter("All Word Files (*.docx *.doc)");

    if (acceptMode == QFileDialog::AcceptSave){
        QStringList filters;
        filters << "Word Document(*.docx)"
                << "Word 97-2003 Document(*.doc)"
                << "Any files (*)";
        dialog.setNameFilters(filters);
    }
}

QWord::QWord(QObject *parent) :
    QObject(parent),
    m_flagOpen(false),
    m_pWord(nullptr),
    m_pDocument(nullptr),
    m_pSelection(nullptr)
{
    const QStringList docLocList = QStandardPaths::standardLocations
            (QStandardPaths::DocumentsLocation);
   m_strDefaultPath = ((docLocList.isEmpty())
           ? QDir::currentPath() : docLocList.last()) + "/easyScanFile/Doc.dotx";
}

void QWord::initialWordApplication(bool visible)
{
    m_pWord = new QAxObject(this);
    if (m_pWord->setControl("word.Application")){
        m_flagOpen = true;
        m_pWord->setProperty("Visible", visible);
        m_pDocuments = m_pWord->querySubObject("Documents");
        connect(m_pWord, SIGNAL(Quit()), this, SLOT(setClosed()));
    }else
        emit errors("Please check you have installed Word.");
}

void QWord::setClosed()
{
    m_flagOpen = false;
    delete m_pWord;
    m_pWord = nullptr;
    emit warning("The word has quit!");
}

bool QWord::isOpened()
{
    return m_flagOpen;
}

QAxObject *QWord::getActiveDocument()
{
    if (m_flagOpen)
        return m_pDocument = m_pWord->querySubObject("ActiveDocument");
    return nullptr;
}

QAxObject *QWord::getSelection()
{
    if (m_flagOpen)
        return m_pSelection = m_pWord->querySubObject("Selection");
    return nullptr;
}

QAxObject *QWord::application()
{
    if (m_flagOpen) return m_pWord;
    return nullptr;
}

bool QWord::open(const QString &filePath, bool visible)
{
    if (m_flagOpen){
        m_pWord->setProperty("Visible", visible);
        m_pDocuments->dynamicCall("Open(const QString &)", filePath);
        return true;
    }
    return false;
}

void QWord::create(const QString &templateFile,bool visible)
{
    if( templateFile != "") m_strDefaultPath = templateFile;
    if (m_flagOpen){
        m_pWord->setProperty("Visible", visible);
        m_pDocuments->dynamicCall("Add(QVariant&, QVariant&)",
                                  QVariant(m_strDefaultPath),
                                  QVariant(true));
    }
}

void QWord::insertText(const QString& text)
{
    if (m_flagOpen and getSelection())
        m_pSelection->dynamicCall("TypeText(QString)", text);
}

void QWord::paste()
{
    if (m_flagOpen and getSelection())
        m_pSelection->dynamicCall("Paste()");
}

void QWord::pasteExcelTable()
{
    if (m_flagOpen and getSelection())
        m_pSelection->dynamicCall("PasteExcelTable (bool) (bool) (bool)",
                            false, true, false);
}

void QWord::pastePicture()
{
    if (m_flagOpen){
        try {
            if (getSelection()) {
                m_pSelection->dynamicCall("PasteAndFormat (wdChartPicture)");
            }
        } catch(...) {
            m_flagOpen = false;
        }
    }
}

/*
void QWord::generateMannul()
{
    QString doc = m_pWord->generateDocumentation();
    QFile outFile("application.html");
    outFile.open(QIODevice::WriteOnly|QIODevice::Append);
    QTextStream ts(&outFile);
    ts << doc << Qt::endl;

    doc = m_pDocuments->generateDocumentation();
    QFile DocumentsFile("Documents.html");
    DocumentsFile.open(QIODevice::WriteOnly|QIODevice::Append);
    QTextStream ts2(&DocumentsFile);
    ts2 << doc << Qt::endl;

    doc = m_pDocument->generateDocumentation();
    QFile DocumentFile("Document.html");
    DocumentFile.open(QIODevice::WriteOnly|QIODevice::Append);
    QTextStream ts3(&DocumentFile);
    ts3 << doc << Qt::endl;

    doc = m_pSelection->generateDocumentation();
    QFile selectionFile("Selection.html");
    selectionFile.open(QIODevice::WriteOnly|QIODevice::Append);
    QTextStream ts4(&selectionFile);
    ts4 << doc << Qt::endl;
}
*/
