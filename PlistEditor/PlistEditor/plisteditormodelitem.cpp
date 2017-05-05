#include "plisteditormodelitem.h"

PListEditorModelItem::PListEditorModelItem(const QString& name, const QString& type)
{
    m_rowNum = -1;
    m_parentItem = Q_NULLPTR;

    m_name = name;
    m_type = type;
}

PListEditorModelItem::~PListEditorModelItem()
{
    while (!m_childItems.isEmpty())
        delete m_childItems.takeFirst();
}

PListEditorModelItem *PListEditorModelItem::child(int i) const
{
    if (m_childItems.count() > i)
        return m_childItems.at(i);

    return Q_NULLPTR;
}

int PListEditorModelItem::childCount() const
{
    return m_childItems.size();
}

PListEditorModelItem *PListEditorModelItem::parent() const
{
    return m_parentItem;
}

int PListEditorModelItem::row() const
{
    return m_rowNum;
}

bool PListEditorModelItem::addChild(PListEditorModelItem *item)
{
    return insertChild(item, childCount());
}

bool PListEditorModelItem::insertChild(PListEditorModelItem *item, int index)
{
    if (index < 0 || index > childCount())
        return false;
    
    if (item->parent())
        parent()->detachItem(item);
    
    m_childItems.insert(index, item);
    item->m_parentItem = this;
    int count = 0;
    for (auto _iter = m_childItems.begin() + index;
        _iter != m_childItems.end(); _iter++)
    {
        (*_iter)->m_rowNum = index + count;
        count++;
    }

    return true;
}

void PListEditorModelItem::removeChild(int i)
{
    if (i >= 0 && i < childCount())
    {
        PListEditorModelItem *item = m_childItems[i];
        m_childItems.removeAt(i);
        delete item;
    }
}

void PListEditorModelItem::removeChild(PListEditorModelItem *item)
{
    removeChild(item->row());
}

QString PListEditorModelItem::name() const
{
    return m_name;
}

void PListEditorModelItem::setName(const QString& n)
{
    m_name = n;
}

QString PListEditorModelItem::type() const
{
    return m_type;
}

void PListEditorModelItem::setType(const QString& t)
{
    m_type = t;
}

QString PListEditorModelItem::value() const
{
    return m_value;
}

void PListEditorModelItem::setValue(const QString& v)
{
    m_value = v;
}

void PListEditorModelItem::setDatas(const QString& n, const QString& t, const QString& v)
{
    m_name = n;
    m_type = t;
    m_value = v;
}

//////////////////////////////////////////////////////////////////////////

void PListEditorModelItem::detachItem(PListEditorModelItem *item)
{
    detachItem(item->row());
}

auto PListEditorModelItem::detachItem(int index) -> PListEditorModelItem *
{
    if (index < 0 || index >= childCount())
        return Q_NULLPTR;

    PListEditorModelItem *item = m_childItems[index];

    m_childItems.removeAt(index);
    if (m_childItems.size() > index)
    {
        int count = 0;
        for (auto _iter = m_childItems.begin() + index; 
            _iter != m_childItems.end(); _iter++)
        {
            (*_iter)->m_rowNum = index + count;
            count++;
        }
    }
    return item;
}