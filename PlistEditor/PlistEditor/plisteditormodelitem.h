#ifndef PLIST_EDITOR_PLIST_EDITOR_MODEL_ITEM_H_
#define PLIST_EDITOR_PLIST_EDITOR_MODEL_ITEM_H_

#include <QList>
#include <QString>

class PListEditorModelItem
{
public:
    PListEditorModelItem(const QString& name = QStringLiteral("NewItem"),
        const QString& type = QStringLiteral("string"));
    ~PListEditorModelItem();

    PListEditorModelItem *child(int i) const;
    int childCount() const;
    PListEditorModelItem *parent() const;
    int row() const;

    bool addChild(PListEditorModelItem *item);
    bool insertChild(PListEditorModelItem *item, int i);
    void removeChild(int i);
    void removeChild(PListEditorModelItem *item);

    QString name() const;
    void setName(const QString& n);

    QString type() const;
    void setType(const QString& t);

    QString value() const;
    void setValue(const QString& v);

    void setDatas(const QString& n, const QString& t, const QString& v);

private:
    // ÍÑÀëitemµ«²»É¾³ý
    void detachItem(PListEditorModelItem *item);
    auto detachItem(int index)->PListEditorModelItem *;

private:
    QString m_name;
    QString m_type;
    QString m_value;
    QList<PListEditorModelItem*> m_childItems;
    PListEditorModelItem *m_parentItem;
    int m_rowNum;
};

#endif // PLIST_EDITOR_PLIST_EDITOR_MODEL_ITEM_H_