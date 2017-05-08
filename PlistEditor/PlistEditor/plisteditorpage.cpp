#include "plisteditorpage.h"

#include <QHeaderView>

#include "commands.h"
#include "lineeditdelegate.h"
#include "plistdatatypedelegate.h"

PlistEditorPage::PlistEditorPage(PlistEditorModel *model, QWidget *parent)
    : QWidget(parent)
    , m_modified_status(false)
{
    initTreeView();

    m_treeExpanded = false;

    m_model = NULL;
    setModel(model);

    m_undoStack = new QUndoStack();
    connect(m_undoStack, &QUndoStack::canUndoChanged, this,
        [&](bool canUndo) 
    {
        if (m_modified_status != canUndo)
        {
            m_modified_status = canUndo;
            editorModifiedStatusChanged(m_modified_status);
        }
    });

    QTreeView *view = m_treeView;

    LineEditDelegate *delegate1 = new LineEditDelegate(view);
    PlistDataTypeDelegate *delegate2 = new PlistDataTypeDelegate(view);
 
    view->setItemDelegateForColumn(0, delegate1);
    view->setItemDelegateForColumn(1, delegate2);
    view->setItemDelegateForColumn(2, delegate1);

    view->expandToDepth(0);

    connect(delegate1, SIGNAL(dataChanged(const QModelIndex&, QString)), 
        this, SLOT(editorDataAboutToBeSet(const QModelIndex &, QString)));
    connect(delegate2, SIGNAL(dataChanged(const QModelIndex&, QString)), 
        this, SLOT(editorDataAboutToBeSet(const QModelIndex &, QString)));

    delegate1->installEventFilter(this);
    delegate2->installEventFilter(this);
    m_treeView->installEventFilter(this);
}

PlistEditorPage::~PlistEditorPage()
{
    clearModel();
    delete m_undoStack;
}

void PlistEditorPage::initTreeView()
{
    QTreeView *treeView = new QTreeView(this);
    treeView->setObjectName(QStringLiteral("treeView"));
    QFont font;
    font.setPointSize(9);
    treeView->setFont(font);
    treeView->setFrameShape(QFrame::NoFrame);
    treeView->setFrameShadow(QFrame::Sunken);
    treeView->setEditTriggers(QAbstractItemView::DoubleClicked | 
        QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked);
    treeView->setProperty("showDropIndicator", QVariant(false));
    treeView->setAlternatingRowColors(true);
    treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    treeView->setAutoExpandDelay(-1);
    treeView->setRootIsDecorated(true);
    treeView->setUniformRowHeights(false);
    treeView->setItemsExpandable(true);
    treeView->setAnimated(false);
    treeView->setAllColumnsShowFocus(false);
    treeView->setExpandsOnDoubleClick(false);
    treeView->header()->setCascadingSectionResizes(false);
    treeView->header()->setDefaultSectionSize(100);
    treeView->header()->setHighlightSections(false);

    m_treeView = treeView;

    QMetaObject::connectSlotsByName(this);
}

QString PlistEditorPage::path()
{
    return m_fileInfo.filePath();
}

void PlistEditorPage::setPath(const QString& path)
{
    m_fileInfo.setFile(path);
}

QString PlistEditorPage::getFileName()
{
    return m_fileInfo.fileName();
}

PlistEditorModel *PlistEditorPage::model()
{
    return m_model;
}

void PlistEditorPage::setModel(PlistEditorModel *m)
{
    if (m_model == m)
        return;

    m_model = m;
    m_treeView->setModel(m_model);
}

void PlistEditorPage::clearModel()
{
    if (m_model) {
        delete m_model;
        m_model = NULL;
    }
    m_treeView->setModel(m_model);
}

QUndoStack *PlistEditorPage::getUndoStack()
{
    return m_undoStack;
}

void PlistEditorPage::on_treeView_expanded()
{
    m_treeView->resizeColumnToContents(0);
}

void PlistEditorPage::on_treeView_collapsed()
{
    on_treeView_expanded();
}

void PlistEditorPage::editorDataAboutToBeSet(const QModelIndex &index, QString val)
{
    QUndoCommand *editCommand = new EditCommand(val, m_model, QModelIndex(index));
    m_undoStack->push(editCommand);
}

//////////////////////////////////////////////////////////////////////////

bool PlistEditorPage::keyPressFilter(QKeyEvent *event)
{
    bool consumed = false;
    int key = event->key();
    switch (key)
    {
    case Qt::Key_Z:
    case Qt::Key_Y:
    {
        if (event->modifiers() == Qt::ControlModifier)
        {
            if (key == Qt::Key_Z)
                m_undoStack->undo();
            else
                m_undoStack->redo();
            consumed = true;
        }
        break;
    }
    default:
        break;
    }
    return consumed;
}

//////////////////////////////////////////////////////////////////////////

void PlistEditorPage::resizeEvent(QResizeEvent *event)
{
    m_treeView->setGeometry(QRect(0, 0, 
        event->size().width(), event->size().height()));
    __super::resizeEvent(event);
}

bool PlistEditorPage::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::KeyPress)
    {
        if (keyPressFilter(static_cast<QKeyEvent*>(ev)))
            return true;
    }
    return __super::eventFilter(obj, ev);
}

void PlistEditorPage::keyPressEvent(QKeyEvent *event)
{
    if (!keyPressFilter(event))
        __super::keyPressEvent(event);
}