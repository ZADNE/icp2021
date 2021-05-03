/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef BLOCKEDITOR_H
#define BLOCKEDITOR_H

#include <QWidget>
#include <QFileInfo>

class BlockEditSignaller: public QObject{
    Q_OBJECT
signals:
    void blockEdited(QString otherPath);
    void blockRenamed(QString oldPath, QString newPath);
};

///
/// \brief Abstract block editor
///
class BlockEditor: public QWidget{
    Q_OBJECT
public:
    explicit BlockEditor(QWidget *parent = nullptr);
    virtual ~BlockEditor();

    static void setLibPath(QString libPath);

    void setFilePath(QString relPath, bool loadFile);
    QString filePath() const;

    bool hasUnsavedChanges() { return m_unsavedChanges; }

    static BlockEditSignaller sig;

signals:
    void havingUnsavedChanges(BlockEditor* editor);
    void withoutUnsavedChanges(BlockEditor* editor);
    void editBlock(QString path);//Request to edit other block
    void blockEdited(QString otherPath);//Notification that other block was edited
    void blockRenamed(QString oldPath, QString newPath);//Notification that other block was renamed

public slots:
    void saveWork();
    void buildWork();
    void editedWork();

protected:
    virtual void load() = 0;
    virtual void save() = 0;
    virtual void build() = 0;

private:
    QString m_relPath;

    bool m_unsavedChanges = false;

    static QString m_libPath;
};

#endif // BLOCKEDITOR_H
