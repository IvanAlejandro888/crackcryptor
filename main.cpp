#include "crackwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CrackWindow w;
    w.show();
    w.on_gFile_toggled(true);
    w.on_gFile2_toggled(true);

    return a.exec();
}
