#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlRecord>
#include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QODBC");
    QString connectString = "DRIVER={SQL Server};";
    connectString.append("SERVER=DESKTOP-ING7ICB\\SQLEXPRESS;");   // Hostname,SQL-Server Instance
    connectString.append("DATABASE=qtApp;");  // Schema
    connectString.append("UID=DESKTOP-ING7ICB\\nurs;");           // User
    connectString.append("PWD=;");           // Pass
    connectString.append("Trusted_Connection=Yes;");
    db.setDatabaseName(connectString);

    if(db.open()) {
        qDebug("NICE");
    } else {
        qDebug("LOOS");
    }

    // подгрузка первой страницы при запуске

    OperationCosts = new QSqlTableModel(this, db);
    OperationCosts->setEditStrategy(QSqlTableModel::OnManualSubmit);
    OperationCosts-> setTable("OperationCosts");
    OperationCosts->select();
    ui->OperationCostsTable->setModel(OperationCosts);

    Personnel = new QSqlTableModel(this, db);
    Personnel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    Personnel-> setTable("Personnel");
    Personnel->select();
    ui->PersonnelTable->setModel(Personnel);

    WorkAccounting = new QSqlTableModel(this, db);
    WorkAccounting->setEditStrategy(QSqlTableModel::OnManualSubmit);
    WorkAccounting-> setTable("WorkAccounting");
    WorkAccounting->select();
    ui->WorkAccountingTable->setModel(WorkAccounting);
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}



// Обновление всех таблиц
void MainWindow::on_refresh_tables_clicked()
{
    OperationCosts->revertAll(); // Удаляет изменения
    OperationCosts->clear();
    ui->OperationCostsTable->clearFocus();
    ui->OperationCostsTable->setModel(OperationCosts);
    OperationCosts = new QSqlTableModel(this, db);
    OperationCosts->setEditStrategy(QSqlTableModel::OnManualSubmit);
    OperationCosts-> setTable("OperationCosts");
    OperationCosts->select();
    ui->OperationCostsTable->setModel(OperationCosts);

    Personnel->revertAll();
    Personnel->clear();
    ui->PersonnelTable->clearFocus();
    ui->PersonnelTable->setModel(OperationCosts);
    Personnel = new QSqlTableModel(this, db);
    Personnel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    Personnel-> setTable("Personnel");
    Personnel->select();
    ui->PersonnelTable->setModel(Personnel);

    WorkAccounting->revertAll();
    WorkAccounting->clear();
    ui->WorkAccountingTable->clearFocus();
    ui->WorkAccountingTable->setModel(OperationCosts);
    WorkAccounting = new QSqlTableModel(this, db);
    WorkAccounting->setEditStrategy(QSqlTableModel::OnManualSubmit);
    WorkAccounting-> setTable("WorkAccounting");
    WorkAccounting->select();
    ui->WorkAccountingTable->setModel(WorkAccounting);
}

// Сохранение всех таблиц
void MainWindow::on_save_tables_clicked()
{
    OperationCosts->submitAll(); // Сохраняет изменения
    OperationCosts->clear();
    ui->OperationCostsTable->clearFocus();
    ui->OperationCostsTable->setModel(OperationCosts);
    OperationCosts = new QSqlTableModel(this, db);
    OperationCosts->setEditStrategy(QSqlTableModel::OnManualSubmit);
    OperationCosts-> setTable("OperationCosts");
    OperationCosts->select();
    ui->OperationCostsTable->setModel(OperationCosts);

    Personnel->submitAll();
    Personnel->clear();
    ui->PersonnelTable->clearFocus();
    ui->PersonnelTable->setModel(OperationCosts);
    Personnel = new QSqlTableModel(this, db);
    Personnel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    Personnel-> setTable("Personnel");
    Personnel->select();
    ui->PersonnelTable->setModel(Personnel);

    WorkAccounting->submitAll();
    WorkAccounting->clear();
    ui->WorkAccountingTable->clearFocus();
    ui->WorkAccountingTable->setModel(OperationCosts);
    WorkAccounting = new QSqlTableModel(this, db);
    WorkAccounting->setEditStrategy(QSqlTableModel::OnManualSubmit);
    WorkAccounting-> setTable("WorkAccounting");
    WorkAccounting->select();
    ui->WorkAccountingTable->setModel(WorkAccounting);
}



// VIEW c автоподгрузкой при выборе таба
void MainWindow::on_tabWidget_currentChanged(int index)
{

    if(index == 2) {
        QSqlQueryModel *setquery1 = new QSqlQueryModel;
        setquery1->setQuery("SELECT * FROM WorkDetailsInfo");
        QTableView *tv = new QTableView(this);
        tv->setModel(setquery1);
        ui->view->setModel(setquery1);
    }
}


// Первая задача SQL & ORM
void MainWindow::on_task_1_clicked()
{
    short int year = ui->year->text().toShort();
    if(ui->year->text() != '\0') {
        if(ui->radio_task_1_SQL->isChecked() == true) {

            QSqlQueryModel *setquery1 = new QSqlQueryModel;
            QString sql = QString("SELECT p.ShopNumber, p.SectionNumber, p.FIO, wa.WorkDate, wa.DefectiveParts "
                                  "FROM Personnel p "
                                  "JOIN WorkAccounting wa ON p.WorkerNumber = wa.WorkerNumber "
                                  "WHERE wa.DefectiveParts > 0 "
                                  "AND YEAR(wa.WorkDate) = %1").arg(year);
            setquery1->setQuery(sql);
            QTableView *tv = new QTableView(this);
            tv->setModel(setquery1);
            ui->table_task_1->setModel(setquery1);
        } else if(ui->radio_task_1_ORM->isChecked() == true) {

            QSqlTableModel *person = new QSqlTableModel(this, db);
            person->setEditStrategy(QSqlTableModel::OnManualSubmit);
            person-> setTable("Personnel");
            person->select();

            QSqlTableModel *account = new QSqlTableModel(this, db);
            account->setEditStrategy(QSqlTableModel::OnManualSubmit);
            account-> setTable("WorkAccounting");
            account->select();

            QStandardItemModel *result = new QStandardItemModel(0, 5, this);
            result->setHeaderData(0, Qt::Horizontal, "ShopNumber");
            result->setHeaderData(1, Qt::Horizontal, "SectionNumber");
            result->setHeaderData(2, Qt::Horizontal, "FIO");
            result->setHeaderData(3, Qt::Horizontal, "WorkDate");
            result->setHeaderData(4, Qt::Horizontal, "DefectiveParts");

            QDate Start_date;
            QDate End_date;
            Start_date.setDate((int)year, 1, 1);
            End_date.setDate((int)year+1, 1, 1);

            QModelIndex index;
            int index_x = 0;

            for(int i=0; i < account->rowCount(); i++){
                for(int j = 0; j < person->rowCount(); j++){
                    if(person->record(j).value("WorkerNumber").toInt() == account->record(i).value("WorkerNumber").toInt()){

                        QDate workDate = account->record(i).value("WorkDate").toDate();

                        if(workDate >= Start_date and workDate <= End_date ){

                            if(account->record(i).value("DefectiveParts").toInt() > 0){

                                result->appendRow(NULL);

                                index = result->index(index_x, 0);
                                result->setData(index, person->record(j).value("ShopNumber").toInt());

                                index = result->index(index_x, 1);
                                result->setData(index, person->record(j).value("SectionNumber").toInt());

                                index = result->index(index_x, 2);
                                result->setData(index, person->record(j).value("FIO").toString());

                                index = result->index(index_x, 3);
                                result->setData(index, account->record(i).value("WorkDate").toString());

                                index = result->index(index_x, 4);
                                result->setData(index, account->record(i).value("DefectiveParts").toInt());

                                index_x++;
                            }
                        }
                    }
                }
            }
            ui->table_task_1->setModel(result);
        }
    }
}


// Вторая задача Оконная функция
void MainWindow::on_task_2_buttom_clicked()
{
    if(ui->year_2->text() != '\0') {

        short int qualification = ui->year_2->text().toShort();
        QSqlQueryModel *setquery1 = new QSqlQueryModel;
        QString sql = QString("SELECT p.FIO, p.ProfessionCode, p.WorkerQualification, "
                              "(SELECT COUNT(*) FROM Personnel p2 WHERE p2.ProfessionCode = p.ProfessionCode "
                              "AND p2.ShopNumber = p.ShopNumber AND p2.SectionNumber = p.SectionNumber) AS TotalWorkers, "
                              "(SELECT MAX(p3.WorkerQualification) FROM Personnel p3 WHERE p3.ProfessionCode = p.ProfessionCode) AS MaxQualification "
                              "FROM Personnel p "
                              "WHERE p.WorkerQualification = %1").arg(qualification);
        setquery1->setQuery(sql);
        QTableView *tv = new QTableView(this);
        tv->setModel(setquery1);
        ui->window_func->setModel(setquery1);
    }
}

// Третья SQL
void MainWindow::on_task_3_buttom_clicked()
{
    short int piece_time = ui->plan_qt->text().toShort();
    short int defect = ui->reg_qt->text().toShort();

    if(type_task == 1){
        QSqlQueryModel *setquery1 = new QSqlQueryModel;
        QString sql =QString( "SELECT p.* "
                           "FROM Personnel p "
                           "JOIN OperationCosts oc ON p.ProfessionCode = oc.ProfessionCode "
                           "AND p.WorkerQualification = oc.WorkerQualification "
                           "JOIN WorkAccounting wa ON p.WorkerNumber = wa.WorkerNumber "
                           "AND oc.DetailCode = wa.DetailCode "
                           "AND oc.OperationNumber = wa.OperationNumber "
                           "WHERE oc.PieceTime > %1 " // Параметр :pieceTimeThreshold для PieceTime
                              "AND wa.DefectiveParts > %2").arg(piece_time).arg(defect);
        setquery1->setQuery(sql);
        QTableView *tv = new QTableView(this);
        tv->setModel(setquery1);
        ui->table_task_3->setModel(setquery1);
    } else if(type_task == 2){

        QSqlTableModel *personnelModel = new QSqlTableModel(this, db);
        personnelModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        personnelModel->setTable("Personnel");
        personnelModel->select();

        QSqlTableModel *operationModel = new QSqlTableModel(this, db);
        operationModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        operationModel->setTable("OperationCosts");
        operationModel->select();

        QSqlTableModel *workAccountingModel = new QSqlTableModel(this, db);
        workAccountingModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        workAccountingModel->setTable("WorkAccounting");
        workAccountingModel->select();

        QStandardItemModel *resultModel = new QStandardItemModel(0, personnelModel->columnCount(), this);
        for (int col = 0; col < personnelModel->columnCount(); ++col) {
            resultModel->setHeaderData(col, Qt::Horizontal, personnelModel->headerData(col, Qt::Horizontal));
        }

        for (int personnelRow = 0; personnelRow < personnelModel->rowCount(); ++personnelRow) {
            int professionCode = personnelModel->record(personnelRow).value("ProfessionCode").toInt();
            int workerQualification = personnelModel->record(personnelRow).value("WorkerQualification").toInt();

            for (int operationRow = 0; operationRow < operationModel->rowCount(); ++operationRow) {
                if (operationModel->record(operationRow).value("ProfessionCode").toInt() == professionCode &&
                    operationModel->record(operationRow).value("WorkerQualification").toInt() == workerQualification &&
                    operationModel->record(operationRow).value("PieceTime").toDouble() > 0) {

                    for (int accountRow = 0; accountRow < workAccountingModel->rowCount(); ++accountRow) {
                        if (workAccountingModel->record(accountRow).value("WorkerNumber").toInt() == personnelModel->record(personnelRow).value("WorkerNumber").toInt() &&
                            workAccountingModel->record(accountRow).value("DetailCode").toInt() == operationModel->record(operationRow).value("DetailCode").toInt() &&
                            workAccountingModel->record(accountRow).value("OperationNumber").toInt() == operationModel->record(operationRow).value("OperationNumber").toInt() &&
                            workAccountingModel->record(accountRow).value("DefectiveParts").toInt() > 0) {

                            QList<QStandardItem *> items;
                            for (int column = 0; column < personnelModel->columnCount(); ++column) {
                                QString value = personnelModel->record(personnelRow).value(column).toString();
                                QStandardItem *item = new QStandardItem(value);
                                items << item;
                            }
                            resultModel->appendRow(items);
                        }
                    }
                }
            }
        }

        ui->table_task_3->setModel(resultModel);

    }

}

// Процедурка
void MainWindow::on_proc_clicked()
{
    short int id_worker = ui->id_worker->text().toShort();
    QString start_date = ui->start_date->text();
    QString end_date = ui->end_date->text();
    QSqlQuery query;
    QString sql = QString("DECLARE @output INT;EXEC GetTotalOutputByWorker @WorkerNumber = %1,"
                            " @start_date = '%2', @end_date = '%3',"
                            " @total_output = @output OUTPUT;"
                            "SELECT @output AS 'TotalOutput';").arg(id_worker).arg(start_date).arg(end_date);

    query.exec(sql);
    query.first();
    QString res = query.value(0).toString();
    ui->res_proc->setText(res);
}



// Режим запроса ORM / SQL
void MainWindow::on_radioButton_ORM_clicked()
{
    type_task = 2;
}


void MainWindow::on_radioButton_SQL_clicked()
{
    type_task = 1;
}




// Динамическая таблица --> установка таблицы OperationCosts
void MainWindow::on_dynamic_OperationCosts_change_clicked()
{
    OperationCosts = new QSqlTableModel(this, db);
    OperationCosts->setEditStrategy(QSqlTableModel::OnManualSubmit);
    OperationCosts-> setTable("OperationCosts");
    OperationCosts->select();
    ui->dynamic_table->setModel(OperationCosts);
    ui->dynamic_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->label_2->setText("OperationCosts");
}

// Динамическая таблица --> установка таблицы Personnel
void MainWindow::on_dynamic_Personnel_change_clicked()
{
    Personnel = new QSqlTableModel(this, db);
    Personnel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    Personnel-> setTable("Personnel");
    Personnel->select();
    ui->dynamic_table->setModel(Personnel);
    ui->label_2->setText("Personnel");
}

// Динамическая таблица --> установка таблицы WorkAccounting
void MainWindow::on_dynamic_WorkAccounting_change_clicked()
{
    WorkAccounting = new QSqlTableModel(this, db);
    WorkAccounting->setEditStrategy(QSqlTableModel::OnManualSubmit);
    WorkAccounting-> setTable("WorkAccounting");
    WorkAccounting->select();
    ui->dynamic_table->setModel(WorkAccounting);
    ui->label_2->setText("WorkAccounting");
}

