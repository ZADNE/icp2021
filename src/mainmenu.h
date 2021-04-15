/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu();


public slots:
    void on_actionOpenLib_triggered();
    void on_actionExit_triggered();

    void addNew(QString folderPath);
    void editFile(QString filePath);
    void deleteFolder(QString folderPath);
    void deleteFile(QString filePath);

signals:
    void saveWork();

private:
    Ui::MainMenu *ui;

    int deleteQuestion(QString title, QString text, QString path);
};
#endif // MAINMENU_H
