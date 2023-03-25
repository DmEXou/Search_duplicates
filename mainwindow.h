#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QVector>
#include <algorithm>
#include <QLabel>
#include <QString>
#include <QScrollArea>
#include <QResizeEvent>
#include <QTextCodec>

#include "core_search.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_direct_button_clicked();

    void on_search_button_clicked();

    void on_all_form_check_toggled(bool checked);

    void on_all_dir_check_toggled(bool checked);

    void on_checkBox_mp3_toggled(bool checked);

    void on_checkBox_jpg_toggled(bool checked);

    void on_checkBox_wma_toggled(bool checked);

    void on_checkBox_pdf_toggled(bool checked);

    void on_checkBox_doc_toggled(bool checked);

    void erase_form(const QString&);

private:
    QString direct_str;
    QVector<QString> format;
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
