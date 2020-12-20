#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "delegate.h"

#include "QTableView"//добавлено для вывода БД в таблице
#include "QStandardItemModel"//добавлено для вывода БД в таблице
#include "QStandardItem"//добавлено для вывода БД в таблице


#include "QCheckBox"//добавлено для вывода QCheckBox в таблице
#include "QRadioButton"//добавлено для вывода QRadioButton в таблице
#include "QStyledItemDelegate"

#include "QDebug"



int columndelegatenumber=5;//номер колонки для делегата, выбираем созданную таблицу в самой бд со значениями int 0

/*для исправления ошибки QSqlDatabasePrivate::addDatabase: duplicate connection name 'qt_sql_default_connection', old connection removed
подключать драйвер для БД (обычно доступно несколько драйверов, их список содержится в стандартном методе QSqlDatabase::drivers();)
следует в глобальной переменной static (видно везде в одном файле) или extern (видно везде во всех файлах) */
static QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");// db - это статическая глобальная переменная, которую можно использовать только в этом файле

// Создаём элемент, который будет выполнять роль чекбокса,делаем его static
//static QTableWidgetItem *item = new QTableWidgetItem();

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
ui->setupUi(this);
tableviewfunction();//основная функция, выводящая таблицу в tableView
connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(forworktableview()));//получение значений всех ячеек в tableview через выбранные checkbox и radiobutton


}

MainWindow::~MainWindow()
{
delete ui;
}




void MainWindow::tableviewfunction(){//основная функция, выводящая таблицу в tableView
    //основная функция, выводящая таблицу

    //подключить БД по ее адресу на компьютере
    db.setDatabaseName("/home/mishanyya/databaseexample/examplebdfordbwork");
    //открывает базу данных, указанную в db.setDatabaseName("/home/mishanyya/databaseexample/examplebdfordbwork");
    db.open();

    //СЛЕДУЕТ СОЗДАТЬ в таблице отдельное поле/колонку со значениями типа bool или integer

    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("basetable");//в модель помещается таблица из БД
    model->select();

    QTableView *tableView = ui->tableView;
    CheckBoxDelegate *delegate = new CheckBoxDelegate();

     //Устанавливаем модель в представление
       tableView->setModel(model);
       //Устанавливаем делегат в столбец
       tableView->setItemDelegateForColumn(columndelegatenumber, delegate);
       //Внешний вид представления
       tableView->resizeColumnsToContents();
       tableView->verticalHeader()->hide();
       tableView->horizontalHeader()->setStretchLastSection(true);
}


void MainWindow::forworktableview(){//получение значения ячейки в tableview через checkbox
int countrows=ui->tableView->model()->rowCount();

    //Получаем данные
    for (int row =0; row < countrows; ++row) {      
            QModelIndex index = ui->tableView->model()->index(row, columndelegatenumber, QModelIndex());//установка индекса - columndelegatenumber колонка в каждой строке

            QVariant value=ui->tableView->model()->data(index).toBool();//поместить в переменную значение ячейки
            //так как там установлен делегат для checkbox должны быть значения
            //1 - если нажат checkbox и 0 - если не нажат checkbox
            //qDebug() << "информация = " << value;

            if(value==1){
                QModelIndex index1 = ui->tableView->model()->index(row, 0, QModelIndex());//установка индекса - 0 колонка в каждой строке
                QVariant value1=ui->tableView->model()->data(index1);//поместить в переменную значение ячейки
                qDebug() << "выбрано значение = " << value1;
                //ВАЖНО:
//так как данные 0 и 1 сохраняются в БД, в дальнейшем их надо проверять в таблице БД, хотя при удалении, может и не надо
            }
    }
}
