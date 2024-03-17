#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QGraphicsPixmapItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief Класс MainWindow представляет главное окно приложения.
 *
 * Класс MainWindow является наследником QMainWindow и содержит основную логику приложения.
 * Он управляет графической сценой, виджетами и моделями данных.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса MainWindow.
     *
     * @param parent Родительский виджет.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Деструктор класса MainWindow.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Слот для обработки события отпускания элемента на графической сцене.
     *
     * @param icon Иконка отпущенного на графической сцене элемента.
     */
    void onGraphicViewDrop(QIcon icon);

    /**
     * @brief Слот для обработки события клика по элементу в списке.
     *
     * @param index Индекс выбранного элемента в списке.
     */
    void onListViewClicked(const QModelIndex &index);

    /**
     * @brief Слот для обработки события изменения значения ячейки в таблице.
     *
     * @param topLeft Верхний левый индекс измененной ячейки.
     * @param bottomRight Нижний правый индекс измененной ячейки.
     */
    void onCellChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
    Ui::MainWindow *ui; /**< Указатель на объект пользовательского интерфейса. */
    QGraphicsScene *scene; /**< Указатель на графическую сцену. */
    QGraphicsView *view; /**< Указатель на виджет QGraphicsView. */
    QList<QStandardItemModel*> tableModels; /**< Список моделей данных таблицы. */
    QList<QGraphicsPixmapItem*> icons; /**< Список графических элементов. */

    /**
     * @brief Отрисовка иконки на графической сцене.
     *
     * @param icon Иконка для отрисовки.
     */
    void drawIcon(QIcon icon);

    /**
     * @brief Добавление модели данных таблицы.
     */
    void addTableModel();

    /**
     * @brief Добавление строки [icon] в таблицу QListView.
     */
    void addRow();
};

#endif // MAINWINDOW_H
