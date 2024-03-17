#include "customgraphicview.h"

CustomGraphicView::CustomGraphicView(QWidget *widget) : QGraphicsView(widget)
{
    setAcceptDrops(true);
}

void CustomGraphicView::dropEvent(QDropEvent *event)
{
    if (event->source() == this) return;
    QListWidget *listwidget = qobject_cast<QListWidget*>(event->source());
    emit itemDrop(listwidget->currentItem()->icon());
}

void CustomGraphicView::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
    event->acceptProposedAction();
}
