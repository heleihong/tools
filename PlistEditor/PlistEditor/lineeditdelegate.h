#ifndef PLIST_EDITOR_LINE_EDIT_DELEGATE_H_
#define PLIST_EDITOR_LINE_EDIT_DELEGATE_H_

#include <QLineEdit>
#include <QItemDelegate>

class LineEditDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    LineEditDelegate(QObject *parent = Q_NULLPTR);
    ~LineEditDelegate();

    // editing
    QWidget *createEditor(QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEditorData(QWidget *editor, const QModelIndex &index) 
        const Q_DECL_OVERRIDE;

    void setModelData(QWidget *editor, QAbstractItemModel *model, 
        const QModelIndex &index) const Q_DECL_OVERRIDE;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const Q_DECL_OVERRIDE;

signals:
    void dataChanged(const QModelIndex&, QString) const;

private:
    // return validity
    static bool checkValueValidity(const QString& val, const QString &type, int col);
};

#endif // PLIST_EDITOR_LINE_EDIT_DELEGATE_H_