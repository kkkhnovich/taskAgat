#ifndef CUSTOMGRAPHICVIEW_H
#define CUSTOMGRAPHICVIEW_H

#include <QGraphicsView>
#include <QListWidget>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>

class CustomGraphicView : public QGraphicsView
{
    Q_OBJECT

signals:
    void iconDrop(QIcon icon);

public:
    explicit CustomGraphicView(QWidget *widget = nullptr);

protected:
    void dropEvent(QDropEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    using QGraphicsView::QGraphicsView;
};

#endif // CUSTOMGRAPHICVIEW_H
