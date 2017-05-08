#include "PlistEditor.h"

#include <QFileDialog>
#include <QMenu>
#include <QMimeData>
#include <QtXml/QtXml>

#include "plisteditormodelparser.h"

PlistEditor::PlistEditor(QWidget * parent) 
    : QTabWidget(parent) 
{
    this->setAcceptDrops(true);
    this->setMovable(true);
}

PlistEditor::~PlistEditor() 
{
	
}

void PlistEditor::openFiles(const QStringList& list)
{
    QStringList file_list = list;
    if (file_list.isEmpty())
    {
        file_list = QFileDialog::getOpenFileNames(
            this, tr("Select files to open"),
            "", "Property list (*.plist)");
    }

    for (int i = 0; i < file_list.size(); ++i)
        openPlist(file_list[i]);
}

void PlistEditor::openPlist(const QString& filePath)
{
    if (!filePath.isEmpty())
    {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly))
        {
            createPage(PListEditorModelParser::fromPlist(&file), filePath);
            file.close();
        }
    }
}

PlistEditorPage *PlistEditor::createPage(PlistEditorModel *model, const QString& filePath)
{
    // create tab
    PlistEditorPage *editorPage = new PlistEditorPage(model);
    
    QString _filePath = filePath;
    if (_filePath.isEmpty())
    {
        int c = this->count() + 1;
        _filePath = tr("new %1").arg(c);
    }
    else
    {
        editorPage->setPath(_filePath);
    }

    // add tab
    addTab(editorPage, _filePath);
    // set as active
    setCurrentWidget(editorPage);

    // connects
    connect(editorPage, &PlistEditorPage::editorModifiedStatusChanged,
        this, &PlistEditor::pageModifiedStatusChanged);

    return editorPage;
}

bool PlistEditor::hasPages()
{
    return count() > 0 ? true : false;
}

PlistEditorPage *PlistEditor::getPage(int i)
{
    if (i < 0 && i >= count())
        return Q_NULLPTR;
    return static_cast<PlistEditorPage*>(widget(i));
}

PlistEditorPage *PlistEditor::getCurrentPage()
{
    return getPage(currentIndex());
}

void PlistEditor::createMenu()
{
    QMenu contextMenu;
    contextMenu.addAction(tr("Close"), this, SLOT(closeCurrentPage()));
    contextMenu.addAction(tr("Close All BUT this "), this, SLOT(closeAllPagesButThis()));
    contextMenu.addAction(tr("Close All to the Left"), this, SLOT(closeAllPagesToLeft()));
    contextMenu.addAction(tr("Close All to the Right"), this, SLOT(closeAllPagesToRight()));
    contextMenu.addAction(tr("Close All"), this, SLOT(closeAllPages()));
    contextMenu.popup(QCursor::pos());
    contextMenu.exec();
}

void PlistEditor::closeCurrentPage()
{
    PlistEditorPage* page = getCurrentPage();
    if (!page)
        return;

    QUndoStack *page_stack = page->getUndoStack();
    Q_ASSERT(page_stack != Q_NULLPTR);
    if (page_stack->canUndo())
    {

    }
}

void PlistEditor::closeAllPagesButThis()
{
}

void PlistEditor::closeAllPagesToLeft()
{
}

void PlistEditor::closeAllPagesToRight()
{
}

void PlistEditor::closeAllPages()
{
}

//////////////////////////////////////////////////////////////////////////

void PlistEditor::pageModifiedStatusChanged(bool modified)
{
    PlistEditorPage *page = qobject_cast<PlistEditorPage*>(sender());
    if (!page)
        return;
    
    int index = indexOf(page);
    Q_ASSERT(index != -1);
    if (index == -1)
        return;

    QString tabName = tabText(index);
    if (modified)
        setTabText(index, tabName.append("*"));
    else
    {
        Q_ASSERT(tabName.endsWith(QLatin1String("*")));
        setTabText(index, tabName.left(tabName.size() - 1));
    }
}

//////////////////////////////////////////////////////////////////////////

void PlistEditor::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData* mimeData = event->mimeData();

    if (mimeData->hasFormat("text/uri-list"))
        event->accept();

    // application/xml application/x-plist
}

void PlistEditor::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) return;

    QStringList list;
    for (int i = 0; i < urls.size(); ++i)
    {
        list.append(urls.at(i).toLocalFile());
    }

    openFiles(list);
}

void PlistEditor::contextMenuEvent(QContextMenuEvent * event)
{
    if (hasPages() && event->reason() == QContextMenuEvent::Mouse)
    {
        int i = tabBar()->tabAt(event->pos());
        if (i != -1)
        {
            setCurrentIndex(i);
            createMenu();
        }
    }
}