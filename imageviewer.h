#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

/* offical libs */
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QApplication>
#include <QScreen>
#include <QPointF>
#include <QMouseEvent>
#include <QGraphicsItem>
#include <QBrush>

class ImageViewer : public QGraphicsView
{
    Q_OBJECT
public:
    ImageViewer(QWidget *parent=nullptr);
    void setBouncy(bool bouncy);
    void setScreen(QScreen *screen);
    void grabScreen();
    void setRubberBandColor(QColor color);
    QScreen *onScreen();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void sendImage(QImage image);

private:
    void initializeNewRubberBand();
    QScreen *m_pOnScreen;
    QColor m_rubberBandColor;
    QGraphicsScene *m_pFullScene;
    QGraphicsRectItem *m_pNewRubberBand;
    QPixmap *m_pFullScreenPixmap;
    bool m_bMousePressed;
    QPointF m_fBeginPos;
    QPointF m_fEndPos;
};

#endif // IMAGEVIEWER_H
