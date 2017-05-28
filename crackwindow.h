#ifndef CRACKWINDOW_H
#define CRACKWINDOW_H

#include "libraries.h"
#include "about.h"

namespace Ui {
class CrackWindow;
}

class Reader {
public:
    About *a;

    int inputType;
    int status;
    QString in_filename;
    QString key_filename;
    QString out_filename;
    QFile ptr_file_0;
    QFile ptr_file_1;
    QFile ptr_file_2;
    QTextStream ptr_stream_0;
    QTextStream ptr_stream_1;
    QTextStream ptr_stream_2;
    char* resKey;
    char* resInput;
    qint16 tamanioInput;
    qint16 tamanioKey;
    void errorMsg(QString infText);
    void infoMsg(QString infText);
    void ReadTxtInput(QString txt);
    void ReadInput();
    void ReadKey();
    void WriteOutput(int operation);
    char * output;
    void getInput();
    int compareSize();
    void encryptMessage(int size);
};

class CrackWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CrackWindow(QWidget *parent = 0);
    Reader reader;    ~CrackWindow();

public slots:
    void on_gFile_toggled(bool checked);

    void on_gTextbox_toggled(bool checked);

private slots:
    void on_iBtnSelFile_clicked();

    void on_kBtnSelFile_clicked();

    void on_oBtnSelFile_clicked();

    void on_cBtn_clicked();

    void on_dBtn_clicked();

    void on_clearBtn_clicked();

    void on_actionExit_2_triggered();

    void on_actionAbout_CrackCryptor_triggered();

private:
    Ui::CrackWindow *ui;
};

#endif // CRACKWINDOW_H
