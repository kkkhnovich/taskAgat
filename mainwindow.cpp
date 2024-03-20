#include "mainwindow.h"
#include "./ui_mainwindow.h"

class A {
public:
    int *a;
    A() {
        a = new int(5);
    }
    ~A() {
        qDebug() << "Hi";
    }
};

/**
 * @brief Конструктор класса окна приложения
 * @details Класс предназначен для обработаки всех сигналов и отрисовки всех необходимых
 * элементов.
 * @param родительский виджет
 */
MainForm::MainForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainForm)
{
    A *a = new A();
    ui->setupUi(this);
    //Задание стилей
    ui->iconsGraphics->setStyleSheet("border: 1px solid black;");
    ui->iconsOnSceneList->setStyleSheet("border: 1px solid gray;");
    ui->coordinatesTable->setStyleSheet("border: 1px solid gray;");
    ui->iconsList->setStyleSheet("border: 1px solid gray;");
    ui->iconsOnSceneList->setModel(new QStringListModel());
    ui->iconsGraphics->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    //Конфигурация graphicsView
    iconsScene = new QGraphicsScene(this);
    ui->iconsGraphics->setScene(iconsScene);
    iconsScene->setSceneRect(QRectF(QPointF(0, 0), ui->iconsGraphics->size()));

    //Связь слотов и сигналов
    connect(ui->iconsGraphics, &CustomGraphicView::iconDrop, this, &MainForm::onIconDrop);
    connect(ui->iconsOnSceneList, &QListView::clicked, this, &MainForm::onIconSelected);

    //Задание иконок в listWidget
    addIcon(":/images/icon.png", QSize(64, 64), QColor(255, 0, 0));
    addIcon(":/images/icon.png", QSize(32, 32), QColor(0, 0, 255));
    //Задаём максимальное значение размера иконок
    ui->iconsList->setIconSize(QSize(64, 64));

    //Расрешение на перетаскивание и отпускания в listWidget и graphicsView
    ui->iconsList->setDragEnabled(true);
    ui->iconsGraphics->setAcceptDrops(true);

    //Запрет на редактирование записей в таблице отображаемых иконок
    ui->iconsOnSceneList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
MainForm::~MainForm()
{
    delete ui;
    for (QGraphicsPixmapItem* icon : icons) {
        delete icon;
    }
}

/**
 * @brief Слот для обработки события отпускания элемента на графической сцене.
 * @details Слот вызывает функции рисования иконки на сцене, а также добавления строки в
 * список отображаемых иконок
 * @param icon Иконка, которую пользователь перетянул из списка возможных иконок(ui->iconsList).
 */
void MainForm::onIconDrop(QIcon icon)
{
    drawIcon(icon);
    addRow();
}

/**
 * @brief Слот для обработки события выбора отображаемой иконки в списке
 * @details Функция вызывает изменение модели таблицы координат(coordinatesTable), так, чтобы
 * она отображала координаты, соответствующие выбранной иконке
 * @param index Индекс выбранной иконки
 */
void MainForm::onIconSelected(const QModelIndex &index)
{
    //Инициализация таблицы, если строка выбрана в первый раз
    if (!ui->coordinatesTable->model()) {
        initTableModel();
    }
    //Изменение отображения в зависимости от выбранной строки
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->coordinatesTable->model());
    model->item(0, 1)->setData(icons[index.row()]->pos().x(), Qt::EditRole);
    model->item(1, 1)->setData(icons[index.row()]->pos().y(), Qt::EditRole);
}

/**
* @brief Слот для обработки события изменения координаты выбранного элемента
* @details Функция меняет расположение выбранной иконки на сцене
* @param topLeft Верхний левый индекс измененной ячейки(координаты).
* @param bottomRight Нижний правый индекс измененной ячейки(координаты).
*/
void MainForm::onCoordinateChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    //Обработчик ситуации, когда не выбрана ни одна иконка
    if (ui->iconsOnSceneList->selectionModel()->selectedIndexes().empty()) {
        return;
    }
    //Находим индекс текущей выбранной строки(если строка выбрана) в таблице listView
    QModelIndex selectedIndexes = ui->iconsOnSceneList->selectionModel()->selectedIndexes()[0];
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
        QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->coordinatesTable->model());
        model->item(0, 1)->setData(icon->pos().x(), Qt::EditRole);
        model->item(1, 1)->setData(icon->pos().y(), Qt::EditRole);
    }
}

/**
* @brief Отрисовка иконки на графической сцене.
* @details Функция вызывается при перетаскивании иконки в область сцены. Иконка
* выбранного вида рисуется в координатах 0, 0(в левом верхнем углу).
* @param icon Иконка для отрисовки.
*/
void MainForm::drawIcon(QIcon Icon)
{
    QPixmap pixmap = Icon.pixmap(Icon.actualSize(QSize(64, 64)));
    QGraphicsPixmapItem* pixmapItem = iconsScene->addPixmap(pixmap);
    pixmapItem->setPos(0, 0);
    icons.append(pixmapItem);
}

/**
* @brief Функция инициализации модели для отображения координат
* @details Функция необходима для инициализации модели таблицы координат при первом
* нажатии на икону в списке отображаемых иконок.
*/
void MainForm::initTableModel()
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

    ui->coordinatesTable->setModel(model);
    connect(model, &QAbstractItemModel::dataChanged, this, &MainForm::onCoordinateChanged);
    ui->coordinatesTable->horizontalHeader()->setVisible(false);
    ui->coordinatesTable->verticalHeader()->setVisible(false);
}

/**
* @brief Функция для добавления записи в список отображаемых иконок
* @details Эта функция вызывается при перетаскивании иконки из области возможных иконок.
* Она добавляет запись, соответствующую отображаемой(т.е. нарисованной на сцене) иконки.
*/
void MainForm::addRow()
{
    QStringListModel* model = qobject_cast<QStringListModel*>(ui->iconsOnSceneList->model());
    QStringList itemList = model->stringList();
    itemList.append("[icon]");
    model->setStringList(itemList);
    ui->iconsOnSceneList->setModel(model);
}

/**
 * @brief Функция для добавление элемента в список видов возможных иконок
 * @details Функция задаёт размер и цвет изображения иконки. После чего добавляет её
 * в список возможных иконок.
 * @param filePath Путь к картинке для иконки
 * @param size Размер для отображения иконки
 * @param color Цвет иконки
 */
void MainForm::addIcon(const QString &filePath, const QSize &size, const QColor &color)
{
    QPixmap iconPixmap = QPixmap(filePath).scaled(size);

    //Изменение цвета иконки
    QPainter painter(&iconPixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(iconPixmap.rect(), color);
    painter.end();

    QIcon icon(iconPixmap);
    ui->iconsList->addItem(new QListWidgetItem(icon, "Иконка"));
}
