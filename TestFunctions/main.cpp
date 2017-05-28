#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>

QString resInput;

int main(int argc, char *argv[])
{

    QFile ptr_file_0("/home/ivanalejandro/Dropbox/Projectos/CrackCryptor/CrackCryptor_beta/CrackCryptor_beta/input.txt");
    if (!ptr_file_0.open(QIODevice::ReadOnly | QIODevice::Text))
        return 2;

    while (!ptr_file_0.atEnd()) {
        QByteArray line = ptr_file_0.readLine();
        resInput.append(QString(line));
    }

    qDebug() << resInput;

    return 0;
}
