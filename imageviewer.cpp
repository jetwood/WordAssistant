#include "imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QGraphicsView(parent),
    m_pOnScreen(nullptr),
    m_rubberBandColor(Qt::blue),
    m_pNewRubberBand(nullptr),
    m_bMousePressed(false),
    m_fBeginPos(0.0, 0.0),
    m_fEndPos(0.0, 0.0)
{
    setWindowFlags(Qt::Window|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    showFullScreen();
    m_pFullScene = new QGraphicsScene(this);
    m_pFullScreenPixmap = new QPixmap();
    setScene(m_pFullScene);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

void ImageViewer::setScreen(QScreen *screen)
{
    setGeometry(screen->geometry());
    m_pOnScreen = screen;
}

void ImageViewer::grabScreen()
{
    m_pFullScene->clear();
    if (m_pOnScreen != nullptr){
        *m_pFullScreenPixmap = m_pOnScreen->grabWindow(0);
        m_pFullScene->addPixmap(*m_pFullScreenPixmap);
    }else
        m_pFullScene->addText("MISS SCREEN PARAMETER !");
}

void ImageViewer::initializeNewRubberBand()
{
    m_pNewRubberBand = new QGraphicsRectItem;
    m_pNewRubberBand->setBrush(m_rubberBandColor);
    m_pNewRubberBand->setOpacity(0.3);
    m_pFullScene->addItem(m_pNewRubberBand);
}

void ImageViewer::mousePressEvent(QMouseEvent *event)
{
    if (QGraphicsItem *item = itemAt(event->pos()))
    {
        QPointF posInScene(mapToScene(event->pos()));
        QPointF posInItem(item->mapFromScene(posInScene));
        m_bMousePressed = true;
        initializeNewRubberBand();
        m_fBeginPos = posInItem;
    }else
        m_fBeginPos = QPointF(0.0, 0.0);
}

void ImageViewer::mouseMoveEvent(QMouseEvent *event)
{
    if (QGraphicsItem *item = itemAt(event->pos()))
    {
        QPointF posInScene(mapToScene(event->pos()));
        QPointF posInItem(item->mapFromScene(posInScene));
        if (m_bMousePressed){
            m_fEndPos = posInItem;
            m_pNewRubberBand->
                    setRect(QRectF(m_fBeginPos, m_fEndPos).normalized());
        }
    }else
        m_fEndPos = QPointF(0.0, 0.0);
}

void ImageViewer::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    if (event->button() == Qt::LeftButton){
        QImage imageData =
                m_pFullScreenPixmap->
                copy(m_pNewRubberBand->rect().x(),
                     m_pNewRubberBand->rect().y(),
                     m_pNewRubberBand->rect().width(),
                     m_pNewRubberBand->rect().height()).toImage();
        emit sendImage(imageData);
        delete m_pNewRubberBand;
        m_pNewRubberBand = nullptr;
    }
    m_bMousePressed = false;
}

void ImageViewer::setRubberBandColor(QColor color)
{
    m_rubberBandColor = color;
}

QScreen *ImageViewer::onScreen()
{
    return m_pOnScreen;
}
