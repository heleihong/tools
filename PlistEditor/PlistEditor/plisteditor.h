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

    PlistEditorPage *createPage(PlistEditorModel *model, const QString& filePath = "");

    void openFiles(const QStringList& list = QStringList());
    void openPlist(const QString& filePath);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
	
};

#endif // PLIST_EDITOR_PLIST_EDITOR_H_