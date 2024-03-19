#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Изменение имен графических компонентов
    coordinatesTable = ui->tableView;
    iconsList = ui->listWidget;
    iconsOnSceneList = ui->listView;
    iconsGraphics = ui->graphicsView;

    //Задание стилей
    iconsGraphics->setStyleSheet("border: 1px solid black;");
    iconsOnSceneList->setStyleSheet("border: 1px solid gray;");
    coordinatesTable->setStyleSheet("border: 1px solid gray;");
    iconsList->setStyleSheet("border: 1px solid gray;");
    iconsOnSceneList->setModel(new QStringListModel());
    iconsGraphics->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    //Конфигурация graphicsView
    iconsScene = new QGraphicsScene(this);
    iconsGraphics->setScene(iconsScene);
    iconsScene->setSceneRect(QRectF(QPointF(0, 0), iconsGraphics->size()));

    //Связь слотов и сигналов
    connect(iconsGraphics, &CustomGraphicView::iconDrop, this, &MainWindow::onIconDrop);
    connect(iconsOnSceneList, &QListView::clicked, this, &MainWindow::onIconSelected);

    //Задание иконок в listWidget
    addIcon(":/images/icon.png", QSize(64, 64), QColor(255, 0, 0));
    addIcon(":/images/icon.png", QSize(32, 32), QColor(0, 0, 255));
    //Задаём максимальное значение размера иконок
    iconsList->setIconSize(QSize(64, 64));

    //Расрешение на перетаскивание и отпускания в listWidget и graphicsView
    iconsList->setDragEnabled(true);
    iconsGraphics->setAcceptDrops(true);

    //Запрет на редактирование записей в таблице отображаемых иконок
    iconsOnSceneList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //Настройка сеток для масштабирования
    QGridLayout* layout = new QGridLayout(ui->centralwidget);
    layout->addWidget(ui->gridLayoutWidget);
    QVBoxLayout* groupBoxLayout = new QVBoxLayout(ui->groupBox);
    groupBoxLayout->addWidget(iconsList);
    ui->groupBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}
MainWindow::~MainWindow()
{
    delete ui;
    for (QGraphicsPixmapItem* icon : icons) {
        delete icon;
    }
}

/**
 * @brief Слот для обработки события отпускания элемента на графической сцене.
 * @details Слот вызывает функции рисования иконки на сцене, а также добавление строки в
 * список отображаемых иконок
 * @param icon Иконка, которую пользователь перетянул из списка возможных иконок(iconsList).
 */
void MainWindow::onIconDrop(QIcon icon)
{
    drawIcon(icon);
    addRow();
}

/**
 * @brief Слот для обработки события выбора отображаемой иконки в списке iconsOnSceneList
 * @details Функция вызывает изменение модели таблицы координат(coordinatesTable), так чтобы
 * она отображала координаты, соответствующие выбранной иконке
 * @param index Номер выбранной иконки
 */
void MainWindow::onIconSelected(const QModelIndex &index)
{
    //Инициализация таблицы, если строка выбрана в первый раз
    if (!coordinatesTable->model()) {
        initTableModel();
    }
    //Изменение отображения в зависимости от выбранной строки
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(coordinatesTable->model());
    model->item(0, 1)->setData(icons[index.row()]->pos().x(), Qt::EditRole);
    model->item(1, 1)->setData(icons[index.row()]->pos().y(), Qt::EditRole);
}

/**
* @brief Слот для обработки события изменения координаты выбранного элемента
* @details Функция меняет расположение выбранной иконки на сцене
* @param topLeft Верхний левый индекс измененной ячейки(координаты).
* @param bottomRight Нижний правый индекс измененной ячейки(координаты).
*/
void MainWindow::onCoordinateChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    //Обработчик ситуации, когда не выбрана ни одна иконка
    if (iconsOnSceneList->selectionModel()->selectedIndexes().empty()) {
        return;
    }
    //Находим индекс текущей выбранной строки(если строка выбрана) в таблице listView
    QModelIndex selectedIndexes = iconsOnSceneList->selectionModel()->selectedIndexes()[0];
    //Находи иконку, соответствующую выбранному индексу
    QGraphicsPixmapItem* icon = icons[selectedIndexes.row()];
    //Проверяем, можно ли преобразовать в тип double
    if (topLeft.data().canConvert<double>()) {
        //Проверяем, какая координата(x или y) была изменена
        if (topLeft.row() == 0) {
            icon->setX(topLeft.data().toDouble());
        }
        else if (topLeft.row() == 1) {
            icon->setY(topLeft.data().toDouble());
        }
    }
    //Возвращаем предыдущее значение, если был введен мусор
    else {
        QStandardItemModel* model = qobject_cast<QStandardItemModel*>(coordinatesTable->model());
        model->item(0, 1)->setData(icon->pos().x(), Qt::EditRole);
        model->item(1, 1)->setData(icon->pos().y(), Qt::EditRole);
    }
}

/**
* @brief Отрисовка иконки на графической сцене.
* @details Функция вызывается при перетаскивании иконки в область сцены. Иконка
* выбранного вида рисуется в координатах 0, 0.
* @param icon Иконка для отрисовки.
*/
void MainWindow::drawIcon(QIcon Icon)
{
    QPixmap pixmap = Icon.pixmap(Icon.actualSize(QSize(64, 64)));
    QGraphicsPixmapItem* pixmapItem = iconsScene->addPixmap(pixmap);
    pixmapItem->setPos(0, 0);
    icons.append(pixmapItem);
}

/**
* @brief Функция инициализаии модели для отображения координат
* @details Функция необходима для инициализации модели таблицы координат, при первом
* нажатии на икону в списке отображаемых иконок.
*/
void MainWindow::initTableModel()
{
    QStandardItemModel* model = new QStandardItemModel(2, 2, this);

    model->setItem(0, 0, new QStandardItem("x"));
    //Запрещаем изменение ячейки с надписью x
    model->item(0, 0)->setEnabled(false);
    model->setItem(0, 1, new QStandardItem("0"));

    model->setItem(1, 0, new QStandardItem("y"));
    //Запрещаем изменение ячейки с надписью y
    model->item(1, 0)->setEnabled(false);
    model->setItem(1, 1, new QStandardItem("0"));

    coordinatesTable->setModel(model);
    connect(model, &QAbstractItemModel::dataChanged, this, &MainWindow::onCoordinateChanged);
    coordinatesTable->horizontalHeader()->setVisible(false);
    coordinatesTable->verticalHeader()->setVisible(false);
}

/**
* @brief Функция для добавления записи в список отображаемых иконок
* @details Эта функция вызывается при перетаскивании иконки из области возможных иконок.
* Она добавляет запись, соответствующую отображаемой(т.е. нарисованной на сцене) иконки.
*/
void MainWindow::addRow()
{
    QStringListModel* model = qobject_cast<QStringListModel*>(iconsOnSceneList->model());
    QStringList itemList = model->stringList();
    itemList.append("[icon]");
    model->setStringList(itemList);
    iconsOnSceneList->setModel(model);
}

/**
 * @brief Функция для добавление элемента в список возможных иконок
 * @details Функция задаёт размер и цвет изображения иконки. После чего добавляет её
 * в список возможных иконок.
 * @param filePath Путь к картинке для иконки
 * @param size Размер для отображения иконки
 * @param color Цвет иконки
 */
void MainWindow::addIcon(const QString &filePath, const QSize &size, const QColor &color)
{
    QPixmap originalPixmap(filePath);
    QPixmap scaledPixmap = originalPixmap.scaled(size);

    QPixmap coloredPixmap(scaledPixmap.size());
    coloredPixmap.fill(Qt::transparent);

    QPainter painter(&coloredPixmap);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawPixmap(coloredPixmap.rect(), scaledPixmap);

    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(coloredPixmap.rect(), color);
    painter.end();

    QIcon icon(coloredPixmap);
    iconsList->addItem(new QListWidgetItem(icon, "Иконка"));
}
