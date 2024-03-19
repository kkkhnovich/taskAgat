#include "customgraphicview.h"

CustomGraphicView::CustomGraphicView(QWidget *widget) : QGraphicsView(widget)
{
    setAcceptDrops(true);
}
/**
 * @brief Обработка события отпускания иконки над областью для отображения
 * @details Функция генерирует сигнал, вызывающий добавление иконки в список
 * и рисование элемента
 * @param event Событие отпускания
 */
void CustomGraphicView::dropEvent(QDropEvent *event)
{
    if (event->source() == this) return;
    QListWidget *listwidget = qobject_cast<QListWidget*>(event->source());
    emit iconDrop(listwidget->currentItem()->icon());
}
/**
 * @brief Обработка события перемещения иконки
 * @details Функция необходима для разрешения перемещения по области для отображения
 * @param event Событие перемещения при перетаскивании
 */
void CustomGraphicView::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
    event->acceptProposedAction();
}
