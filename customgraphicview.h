#ifndef CUSTOMGRAPHICVIEW_H
#define CUSTOMGRAPHICVIEW_H

#include <QGraphicsView>
#include <QListWidget>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>

/**
 * @brief Класс CustomGraphicView представляет собой настраиваемый вид QGraphicsView,
 * генерирующий сигнал при отпускании.
 *
 * Класс CustomGraphicView является наследником QGraphicsView и добавляет сигнал itemDrop,
 * который генерируется при событии отпускания элемента.
 */
class CustomGraphicView : public QGraphicsView
{
    Q_OBJECT

signals:
    /**
     * @brief Сигнал, генерируемый при событии отпускания элемента.
     *
     * @param icon Иконка отпущенного элемента.
     */
    void itemDrop(QIcon icon);

public:
    /**
     * @brief Конструктор класса CustomGraphicView.
     *
     * @param widget Родительский виджет, к которому принадлежит CustomGraphicView.
     */
    explicit CustomGraphicView(QWidget *widget = nullptr);

protected:
    /**
     * @brief Обработчик события отпускания элемента.
     *
     * @param event Событие отпускания элемента.
     */
    void dropEvent(QDropEvent *event) override;

    /**
     * @brief Обработчик события перемещения элемента при перетаскивании.
     *
     * @param event Событие перемещения элемента при перетаскивании.
     */
    void dragMoveEvent(QDragMoveEvent *event) override;

    using QGraphicsView::QGraphicsView;
};

#endif // CUSTOMGRAPHICVIEW_H
