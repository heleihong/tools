#include "PlistEditor.h"

#include <QFileDialog>
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
    this->addTab(editorPage, _filePath);
    // set as active
    this->setCurrentWidget(editorPage);

    return editorPage;
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
