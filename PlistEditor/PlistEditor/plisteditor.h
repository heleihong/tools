#ifndef PLIST_EDITOR_PLIST_EDITOR_H_
#define PLIST_EDITOR_PLIST_EDITOR_H_

#include <QTabWidget>
#include <QDrag>
#include <QDropEvent>

#include "PlistEditorModel.h"
#include "plisteditorpage.h"

class PlistEditor : public QTabWidget 
{
	Q_OBJECT

public:
    PlistEditor(QWidget * parent = Q_NULLPTR);
	~PlistEditor();

    void openFiles(const QStringList& list = QStringList());
    void openPlist(const QString& filePath);

    PlistEditorPage *createPage(PlistEditorModel *model, const QString& filePath = "");
    bool hasPages();

    PlistEditorPage *getPage(int i);
    PlistEditorPage *getCurrentPage();

private:
    void createMenu();
    void closeCurrentPage();
    void closeAllPagesButThis();
    void closeAllPagesToLeft();
    void closeAllPagesToRight();
    void closeAllPages();

private Q_SLOTS:
    void pageModifiedStatusChanged(bool modified);

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void contextMenuEvent(QContextMenuEvent * event) Q_DECL_OVERRIDE;
};

#endif // PLIST_EDITOR_PLIST_EDITOR_H_