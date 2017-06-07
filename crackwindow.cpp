#include "crackwindow.h"
#include "ui_crackwindow.h"

CrackWindow::CrackWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CrackWindow)
{
    ui->setupUi(this);
}

// CrackCryptor functions declaration

void Reader::errorMsg(QString infText)
{
    QMessageBox err;
    err.setText("Error");
    err.setInformativeText(infText);
    err.setWindowTitle("Error");
    err.setIcon(QMessageBox::Critical);
    err.exec();
}

void Reader::infoMsg(QString infText)
{
    QMessageBox inf;
    inf.setText("Success");
    inf.setInformativeText(infText);
    inf.setWindowTitle("Success");
    inf.setIcon(QMessageBox::Information);
    inf.exec();
}

void Reader::ReadTxtInput(QString txt)
{
    tamanioInput = txt.size();

    resInput = txt.toUtf8().data();
}

void Reader::ReadInput()
{
    ptr_file_0.setFileName(in_filename);
    if (!ptr_file_0.open(QIODevice::ReadWrite))
        return;

    QDataStream in(&ptr_file_0);

    tamanioInput = ptr_file_0.size();
    qDebug() << tamanioInput;

    resInput = (char*) malloc((int)tamanioInput*8);
    in.readRawData(resInput, (int)tamanioInput);

    ptr_file_0.close();
}

void Reader::ReadKey()
{
    ptr_file_1.setFileName(key_filename);
    if (!ptr_file_1.open(QIODevice::ReadOnly))
        return;

    QDataStream in(&ptr_file_1);

    tamanioKey = ptr_file_1.size();
    qDebug() << tamanioKey;

    resKey = (char*) malloc((int)tamanioKey*8);
    in.readRawData(resKey, (int)tamanioKey);

    ptr_file_1.close();
}

void Reader::WriteOutput(int operation)
{
    if (outputType == 0)
    {
        ptr_file_2.setFileName(out_filename);
    }
    if (outputType == 1)
    {
        ptr_file_2.setFileName(in_filename);
    }
    if (outputType == 2)
    {
        QMessageBox msg;
        msg.setText(QString(output));
        msg.setWindowTitle("Message");
        msg.exec();

        return;
    }

    if (!ptr_file_2.open(QIODevice::WriteOnly))
         return;

     QDataStream out(&ptr_file_2);

     if(out.writeRawData(output, tamanioInput) == -1)
     {
         errorMsg("Error writing in file");
         return;
     }else{
         if(operation==1)
             infoMsg("Message crypted  successfully!");
         if(operation==2)
             infoMsg("Message decrypted successfully!");
     }

     ptr_file_2.close();
}

int Reader::compareSize()
{
    enum {GOOD, LONGER};
    if(tamanioInput <= tamanioKey)
        return GOOD;
    if(tamanioInput > tamanioKey)
        return LONGER;
}

void Reader::encryptMessage(int size)
{
    char fc;
    char* final = (char *) malloc(size*8);
    for(int i=0;i<size;i++)
    {
        fc = resInput[i]^resKey[i];
        final[i] = fc;
    }
    output = final;
    for(int i=0;i<size;i++)
    {
        qDebug() << "Msg:" << (int)resInput[i] << "| (" << QChar(resInput[i]) << ") | Key:" << (int)resKey[i] << "| Out:" << (int)output[i] << "(" << QChar(output[i]) << ")";
    }
}

CrackWindow::~CrackWindow()
{
    if((reader.inputType == 1 && reader.in_filename!=NULL && reader.key_filename!=NULL && reader.out_filename!=NULL)||
            (reader.inputType == 0 && reader.in_filename!=NULL && reader.key_filename!=NULL && reader.out_filename!=NULL)){
        free(reader.resInput);
        free(reader.resKey);
    }
    delete ui;
}

void CrackWindow::on_gFile_toggled(bool checked)
{
    reader.inputType = checked;

    if(checked==true)
    {
        ui->iTxtbox->setEnabled(false);

        ui->iLabelMsg->setEnabled(true);
        ui->iBtnSelFile->setEnabled(true);

        ui->gInput->setEnabled(true);
    }
}

void CrackWindow::on_gTextbox_toggled(bool checked)
{
    if(checked==true)
    {
        ui->iTxtbox->setEnabled(true);

        ui->iLabelMsg->setEnabled(false);
        ui->iBtnSelFile->setEnabled(false);

        ui->gInput->setEnabled(false);
    }
}

void CrackWindow::on_iBtnSelFile_clicked()
{
    QString ifn = QFileDialog::getOpenFileName(this,
    tr("Open Input File"), "/home/", tr("Text Files (*.txt)"));
    if(ifn=="")
        return;
    reader.in_filename = ifn;
    QStringList cut = ifn.split("/");
    QString cut_ifn = cut.at(cut.length()-1);
    ui->iBtnSelFile->setText(cut_ifn);
    // qDebug() << ifn;
}

void CrackWindow::on_kBtnSelFile_clicked()
{
    QString kfn = QFileDialog::getOpenFileName(this,
    tr("Open Key File"), "~/", tr("Text Files (*.txt)"));
    if(kfn=="")
        return;
    reader.key_filename = kfn;
    QStringList cut = kfn.split("/");
    QString cut_kfn = cut.at(cut.length()-1);
    ui->kBtnSelFile->setText(cut_kfn);
    // qDebug() << kfn;
}

void CrackWindow::on_oBtnSelFile_clicked()
{
    QString ofn = QFileDialog::getOpenFileName(this,
    tr("Open Output File"), "~/", tr("Text Files (*.txt)"));
    if(ofn=="")
        return;
    reader.out_filename = ofn;
    QStringList cut = ofn.split("/");
    QString cut_ofn = cut.at(cut.length()-1);
    ui->oBtnSelFile->setText(cut_ofn);
    // qDebug() << ofn;
}

void CrackWindow::on_cBtn_clicked()
{
    reader.ReadInput();
    reader.ReadKey();

    if(reader.inputType)
    {
        if (reader.compareSize())
        {
            reader.errorMsg("Sorry, message is larger than key, we can't encrypt it\n");
            return;
        }
        reader.encryptMessage(reader.tamanioInput);
    }
    else
    {
        reader.ReadTxtInput(ui->iTxtbox->toPlainText());
        if (reader.compareSize())
        {
            reader.errorMsg("Sorry, message is larger than key, we can't encrypt it\n");
            return;
        }
        reader.encryptMessage(reader.tamanioInput);
    }

    reader.WriteOutput(1);

}

void CrackWindow::on_dBtn_clicked()
{
    reader.ReadInput();
    reader.ReadKey();

    if (reader.compareSize() == 1)
    {
        reader.errorMsg("Sorry, message is larger than key, we can't encrypt it\n");
        return;
    }

    reader.encryptMessage(reader.tamanioInput);

    reader.WriteOutput(2);
}

void CrackWindow::on_clearBtn_clicked()
{

    ui->gFile->toggle();
    ui->gFile2->toggle();
    reader.status = 0;

    ui->iBtnSelFile->setText("Browse...");
    ui->kBtnSelFile->setText("Browse...");
    ui->oBtnSelFile->setText("Browse...");

    ui->iTxtbox->clear();
}

void CrackWindow::on_actionExit_2_triggered()
{
    this->~CrackWindow();
    exit (0);
}

void CrackWindow::on_actionAbout_CrackCryptor_triggered()
{
    reader.a = new About;
    reader.a->show();
}

void CrackWindow::on_gFile2_toggled(bool checked)
{
    reader.outputType = 0;
    if(checked==true)
    {
        ui->oBtnSelFile->setEnabled(true);
    }
}

void CrackWindow::on_gInput_toggled(bool checked)
{
    reader.outputType = 1;
    if(checked==true)
    {
        ui->oBtnSelFile->setEnabled(false);
    }
}

void CrackWindow::on_gDialog_toggled(bool checked)
{
    reader.outputType = 2;
    if(checked==true)
    {
        ui->oBtnSelFile->setEnabled(false);
    }
}
