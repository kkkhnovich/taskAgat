#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainForm;
}
QT_END_NAMESPACE

class MainForm : public QMainWindow
{
    Q_OBJECT

public:
    MainForm(QWidget *parent = nullptr);
    ~MainForm();

private slots:
    void onIconDrop(QIcon icon);
    void onIconSelected(const QModelIndex &index);
    void onCoordinateChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
    Ui::MainForm *ui; /**< Указатель на объект пользовательского интерфейса. */
    QGraphicsScene *iconsScene; /**< Указатель на графическую сцену, на которой
отображаются все иконки */
    QList<QGraphicsPixmapItem*> icons; /**< Список из объектов QGraphicsPixmapItem, необходимый
для рисования и манипулирования с иконками на графической сцене */

    void drawIcon(QIcon icon);
    void initTableModel();
    void addRow();
    void addIcon(const QString& filePath, const QSize& size, const QColor& color);
};

#endif // MAINWINDOW_H
