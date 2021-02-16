#include "striplistview.h"
#include "ui_striplistview.h"

StripListView::StripListView(QWidget *parent) :
    QListWidget(parent),
    ui(new Ui::StripListView)
{
    ui->setupUi(this);
    setSpacing(2);
    readSettings();
    setIconSize(QSize(28,28));
}

void StripListView::readSettings()
{
    QSettings settings;
    QListWidgetItem * newItem;
    int size = settings.beginReadArray("stripdata");
    for (int i=0; i<size; ++i){
        settings.setArrayIndex(i);
        newItem
           = new QListWidgetItem(QIcon(":/image/bookmark.png"),
                                 settings.value("content").toString(), this);
        newItem->setSizeHint(QSize(256,36));
        addItem(newItem);
    }
    settings.endArray();
}

void StripListView::writeSettings()
{
    QSettings settings;
    settings.beginWriteArray("stripdata");
    for (int i=0; i<count(); ++i){
        settings.setArrayIndex(i);
        settings.setValue("content", item(i)->text());
    }
    settings.endArray();
}

void StripListView::mousePressEvent(QMouseEvent *event)
{

    QListWidgetItem *selectedItem = nullptr;
    if (event->buttons() == Qt::RightButton &&
            (selectedItem = itemAt(event->pos())))
    {
            setCurrentItem(selectedItem);
            selectedItem->setText(QInputDialog::getText(this,
                                            "Input Text",
                                            "Content: ",
                                            QLineEdit::Normal,
                                            selectedItem->text()));

    }
    if (event->buttons() == Qt::LeftButton &&
        (selectedItem = itemAt(event->pos()))){
        setCurrentItem(selectedItem);
    }
}

void StripListView::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit doubleClickTrigger(itemAt(event->pos())->text());
}


void StripListView::addNewStrip()
{
    QListWidgetItem *newItem = new QListWidgetItem(QIcon(":/image/bookmark.png"),
                                                    "<New One>",this);
    newItem->setSizeHint(QSize(256,36));
    addItem(newItem);
}

void StripListView::deleteCurrentStrip()
{
    takeItem(currentRow());
}

StripListView::~StripListView()
{
    delete ui;
}
