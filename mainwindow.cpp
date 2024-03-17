#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "customgraphicview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Задание стилей
    ui->graphicsView->setStyleSheet("border: 1px solid black;");
    ui->listView->setStyleSheet("border: 1px solid gray;");
    ui->tableView->setStyleSheet("border: 1px solid gray;");
    ui->listWidget->setStyleSheet("border: 1px solid gray;");
    ui->listView->setModel(new QStringListModel());
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    //Конфигурация graphicsView
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(QRectF(QPointF(0, 0), ui->graphicsView->size()));

    //Связь слотов и сигналов
    connect(ui->graphicsView, &CustomGraphicView::itemDrop, this, &MainWindow::onGraphicViewDrop);
    connect(ui->listView, &QListView::clicked, this, &MainWindow::onListViewClicked);

    //Задание иконки в listWidget
    ui->listWidget->addItem("Иконка");
    QListWidgetItem *item = ui->listWidget->item(0);
    item->setIcon(QIcon(":/images/icon.png"));
    ui->listWidget->setIconSize(QSize(64, 64));

    //Расрешение на перетаскивание и отпускания в listWidget и graphicsView
    ui->listWidget->setDragEnabled(true);
    ui->graphicsView->setAcceptDrops(true);

    //Настройка сеток для масштабирования
    QGridLayout* layout = new QGridLayout(ui->centralwidget);
    layout->addWidget(ui->gridLayoutWidget);
    QVBoxLayout* groupBoxLayout = new QVBoxLayout(ui->groupBox);
    groupBoxLayout->addWidget(ui->listWidget);
    ui->groupBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}
MainWindow::~MainWindow()
{
    delete ui;
    delete view;
    delete scene;
    for (QStandardItemModel* model : tableModels) {
        delete model;
    }
    for (QGraphicsPixmapItem* icon : icons) {
        delete icon;
    }
}

void MainWindow::onGraphicViewDrop(QIcon icon)
{
    drawIcon(icon);
    addTableModel();
    addRow();
}

void MainWindow::onListViewClicked(const QModelIndex &index)
{
    QStandardItemModel* model = tableModels[index.row()];
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setVisible(false);
    ui->tableView->verticalHeader()->setVisible(false);
}

void MainWindow::onCellChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    //Проверяем, можно ли преобразовать в тип double
    if (topLeft.data().canConvert<double>()) {
        //Находим индекс текущей выбранной строки в таблице listView
        QModelIndex selectedIndexes = ui->listView->selectionModel()->selectedIndexes()[0];
        //Проверяем, какая координата(x или y) была изменена
        if (topLeft.row() == 0) {
            icons[selectedIndexes.row()]->setX(topLeft.data().toDouble());
        }
        else if (topLeft.row() == 1) {
            icons[selectedIndexes.row()]->setY(topLeft.data().toDouble());
        }
    }
}

void MainWindow::drawIcon(QIcon Icon)
{
    QPixmap pixmap = Icon.pixmap(Icon.actualSize(QSize(64, 64)));
    QGraphicsPixmapItem* pixmapItem = scene->addPixmap(pixmap);
    pixmapItem->setPos(0, 0);
    icons.append(pixmapItem);
}

void MainWindow::addTableModel()
{
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setItem(0, 0, new QStandardItem("x"));

    //Запрещаем изменение ячейки с надписью x
    model->item(0, 0)->setEnabled(false);

    model->setItem(0, 1, new QStandardItem("0"));
    model->setItem(1, 0, new QStandardItem("y"));

    //Запрещаем изменение ячейки с надписью y
    model->item(1, 0)->setEnabled(false);

    model->setItem(1, 1, new QStandardItem("0"));
    connect(model, &QAbstractItemModel::dataChanged, this, &MainWindow::onCellChanged);
    tableModels.append(model);
}

void MainWindow::addRow()
{
    QStringListModel* model = qobject_cast<QStringListModel*>(ui->listView->model());
    QStringList itemList = model->stringList();
    itemList.append("[icon]");
    model->setStringList(itemList);
    ui->listView->setModel(model);
}
