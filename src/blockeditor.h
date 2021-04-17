/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#ifndef BLOCKEDITOR_H
#define BLOCKEDITOR_H

#include <QWidget>

class BlockEditor: public QWidget{
    Q_OBJECT
public:
    explicit BlockEditor(QWidget *parent = nullptr);


    void setFilePath(QString filePath);
    QString filePath() const;

public slots:
    void saveWork();

private:
    QString m_filePath;
};

#endif // BLOCKEDITOR_H
