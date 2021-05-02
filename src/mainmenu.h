/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE

///
/// \brief Main widget representing whole UI
///
class MainMenu : public QMainWindow{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

signals:
    void saveWork();

public slots:
    void openLibraryDialog();
    void exitProgram();
    void openLibrary(QString libpath);

    void addNew(QString path);
    void renameThis(QString path);
    void deleteThis(QString path);

private:
    Ui::MainMenu *ui;

    QString renameDialog(QString what, QString oldName);
    int deleteQuestion(QString title, QString text, QString path);
    void updateNoTabLabel();

    QString m_libPath;
};
#endif // MAINMENU_H
