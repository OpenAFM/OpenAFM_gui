#ifndef ALIGNBOX_H
#define ALIGNBOX_H

#include <QtGui>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>
#include <QGraphicsWidget>
#include <QDebug>
#include <QGraphicsScene>


class ResizeHandle : public QGraphicsRectItem
{
public:
    explicit ResizeHandle(Qt::Alignment alignment, QGraphicsItem* parent = 0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

private:
    QPointF offset;
    Qt::Alignment align;
};


class AlignWidget : public QGraphicsWidget
{

public:
    AlignWidget(QGraphicsItem* parent = 0);

public:

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void gridUpdate(QPainter* painter, int gridSize);
    void mouseMoveEvent( QGraphicsSceneMouseEvent *event);
    void setGeometry(const QRectF& rect);

private:

    void createHandle(Qt::Alignment align);
    void adjustHandle(Qt::Alignment align, ResizeHandle* handle);
    QHash<Qt::Alignment, ResizeHandle*> resizeHandles;
};

#endif
