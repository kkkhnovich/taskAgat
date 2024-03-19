#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qlistwidget.h"
#include "qtableview.h"
#include <QGraphicsScene>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QGraphicsView>
#include "customgraphicview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onIconDrop(QIcon icon);
    void onIconSelected(const QModelIndex &index);
    void onCoordinateChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
    Ui::MainWindow *ui; /**< Указатель на объект пользовательского интерфейса. */
    QGraphicsScene *iconsScene; /**< Указатель на графическую сцену, на которой
отображаются все иконки */
    QList<QGraphicsPixmapItem*> icons; /**< Список из объектов QGraphicsPixmapItem, необходимый
для рисования и манипулирования с иконками на графической сцене */
    QTableView* coordinatesTable; /**< Отображение таблицы с координатами иконок */
    QListWidget* iconsList; /**< Список возможных видов иконок */
    QListView* iconsOnSceneList; /**< Список отображаемых иконок, с помощью которого можно
выбирать, координаты какой иконки отображать в таблице */
    CustomGraphicView* iconsGraphics; /**< Указатель на пользовательский тип,который
наследуется от QGraphicView, необходимых для отображения иконок и формирования сигнала
отпускания иконки. */

    void drawIcon(QIcon icon);
    void initTableModel();
    void addRow();
    void addIcon(const QString& filePath, const QSize& size, const QColor& color);
};

#endif // MAINWINDOW_H
