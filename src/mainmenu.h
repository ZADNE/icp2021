/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE

class MainMenu : public QMainWindow{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu();


public slots:
    void openLibrary(QString libpath);

    void addNew(QString path);
    void renameThis(QString path);
    void deleteThis(QString path);

signals:
    void saveWork();

private:
    Ui::MainMenu *ui;

    QFileInfo lib;

    QString renameDialog(QString what, QString oldName);
    int deleteQuestion(QString title, QString text, QString path);
    void updateNoTabLabel();

protected slots:
    void on_actionOpenLib_triggered();
    void on_actionExit_triggered();
};
#endif // MAINMENU_H
