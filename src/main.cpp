/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "mainmenu.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    QTranslator translator;

    if (translator.load(a.applicationDirPath() + "/icp2021_en_US")){
        a.installTranslator(&translator);
    }

    MainMenu w;
    w.show();
    return a.exec();
}
