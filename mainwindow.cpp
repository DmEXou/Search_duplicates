#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->scrollArea->setWidgetResizable(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_direct_button_clicked()
{
    direct_str = QFileDialog::getExistingDirectory(this, "TEST", QDir::currentPath());
}


void MainWindow::on_search_button_clicked()
{
    bool flag = true;
    if(direct_str.isEmpty()){
        QMessageBox::warning(this, "Error", "Path is empty");
        flag = false;
    }
    if(!direct_str.isEmpty()&&format.empty()){
        QMessageBox::warning(this, "Error", "Format not choise");
        flag = false;
    }
    if(flag){
        Core core(direct_str, format);
        if(core.GetSize() == 0){
            QMessageBox::warning(this, "Error", "No duplicate");
        }
        else{
        QString str;
        str += "Found duplicates - " + QString::number(core.get_duplicate().back().counter) + '\n';
        int counter = 1;
        for(const auto& dupl : core.get_duplicate()){
            if(counter == dupl.counter){
                str += '\n';
                ++counter;
            }
            str += QString::fromStdString(dupl.file_path.string());
            str += '\n';
        }
        QLabel *lable = new QLabel;
        lable->setText(str);
        ui->scrollArea->setWidget(lable);
        }
    }
}


void MainWindow::on_all_form_check_toggled(bool checked)
{
    if(checked){
        ui->checkBox_mp3->setDisabled(true);
        ui->checkBox_mp3->setCheckState(Qt::Unchecked);
        ui->checkBox_jpg->setDisabled(true);
        ui->checkBox_jpg->setCheckState(Qt::Unchecked);
        ui->checkBox_wma->setDisabled(true);
        ui->checkBox_wma->setCheckState(Qt::Unchecked);
        ui->checkBox_pdf->setDisabled(true);
        ui->checkBox_pdf->setCheckState(Qt::Unchecked);
        ui->checkBox_doc->setDisabled(true);
        ui->checkBox_doc->setCheckState(Qt::Unchecked);
        format = {".mp3", ".MP3", ".jpg", ".JPG", ".wma", ".pdf", ".doc", ".xls"}; //jpeg JPEG
    }
    else{
        ui->checkBox_mp3->setDisabled(false);
        ui->checkBox_jpg->setDisabled(false);
        ui->checkBox_wma->setDisabled(false);
        ui->checkBox_pdf->setDisabled(false);
        ui->checkBox_doc->setDisabled(false);
        format = {};
    }
}


void MainWindow::on_all_dir_check_toggled(bool checked)
{
    if(checked){
        ui->direct_button->setDisabled(true);
        direct_str = "C:\\";
    }
    else{
        ui->direct_button->setDisabled(false);
        direct_str.clear();
    }
}

void MainWindow::erase_form(const QString& str){
    auto it = std::find(format.begin(), format.end(), str);
    format.erase(it);
}

void MainWindow::on_checkBox_mp3_toggled(bool checked)
{
    if(checked){
        format.push_back(".mp3");
        format.push_back(".MP3");
    }
    else{
        erase_form(".mp3");
        erase_form(".MP3");
    }
    qDebug() << format;
}


void MainWindow::on_checkBox_jpg_toggled(bool checked)
{
    if(checked){
        format.push_back(".jpg");
        format.push_back(".JPG");
    }
    else{
        erase_form(".jpg");
        erase_form(".JPG");
    }
    qDebug() << format;
}


void MainWindow::on_checkBox_wma_toggled(bool checked)
{
    if(checked){
        format.push_back(".wma");
    }
    else{
        erase_form(".wma");
    }
    qDebug() << format;
}


void MainWindow::on_checkBox_pdf_toggled(bool checked)
{
    if(checked){
        format.push_back(".pdf");
    }
    else{
        erase_form(".pdf");
    }
    qDebug() << format;
}


void MainWindow::on_checkBox_doc_toggled(bool checked)
{
    if(checked){
        format.push_back(".doc");
        format.push_back(".xls");
    }
    else{
        erase_form(".doc");
        erase_form(".xls");
    }
    qDebug() << format;
}
void MainWindow::resizeEvent(QResizeEvent *event){
    int width_win = event->size().width() - 330;
    int height_win = event->size().height() - 45;
    ui->scrollArea->setGeometry(ui->scrollArea->geometry().x(),
                                ui->scrollArea->geometry().y(),
                                width_win, height_win);
}
