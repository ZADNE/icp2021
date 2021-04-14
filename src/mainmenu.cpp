#include "mainmenu.h"
#include "ui_mainmenu.h"

#include <QFileDialog>
#include <QDebug>

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu){
    ui->setupUi(this);

    connect(ui->actionOpen_a_library, &QAction::triggered,
            this, &MainMenu::openLibrary);
    connect(ui->actionExit, &QAction::triggered,
            this, &MainMenu::exit);

}

MainMenu::~MainMenu(){
    delete ui;
}

void MainMenu::openLibrary(bool trigered){
    (void)trigered;
    QString folder = QFileDialog::getExistingDirectory(this,
        tr("Open a library"), QDir::currentPath());

    if (!folder.isEmpty()){//If a folder was selected
        ui->libraryExplorer->loadLibrary(folder);
    }
}

void MainMenu::exit(bool trigered){
    (void)trigered;
    emit saveWork();
    QApplication::exit(0);
}
