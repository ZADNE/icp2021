#include "mainmenu.h"

#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <QLocale>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    QTranslator translator;

    if (translator.load(a.applicationDirPath() + "/icp2021_en_US")){
        a.installTranslator(&translator);
    } else {
        qDebug() << "Failed to load file with translations.";
    }

    MainMenu w;
    w.show();
    return a.exec();
}
