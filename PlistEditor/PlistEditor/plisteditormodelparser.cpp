#include "plisteditormodelparser.h"

#include <QDateTime>
#include <QDebug>

#include <plist/plistparser.h>

QString transformType(const QVariant::Type& type)
{
    QString ret_type;
    switch (type)
    {
    case QVariant::Bool:
        ret_type = QLatin1String("boolean");
        break;
    case QVariant::LongLong:
        ret_type = QLatin1String("integer");
        break;
    case QVariant::Double:
        ret_type = QLatin1String("real");
        break;
    case QVariant::Map:
        ret_type = QLatin1String("dict");
        break;
    case QVariant::List:
        ret_type = QLatin1String("array");
        break;
    case QVariant::String:
        ret_type = QLatin1String("string");
        break;
    case QVariant::ByteArray:
        ret_type = QLatin1String("data");
        break;
    case QVariant::DateTime:
        ret_type = QLatin1String("date");
        break;
    default:
        qDebug() << QObject::tr("file:%1, function:%2, Warning: Invalid QVariant::Type.")
            .arg(__FILE__).arg(__FUNCTION__);
        break;
    }
    return ret_type;
}

QString transformValue(const QVariant& value)
{
    QString ret_value;
    int type = value.type();
    switch (type)
    {
    case QVariant::Map:
    case QVariant::List:
    {
        int item_num = 0;
        if (type == QVariant::Map)
            item_num = value.toMap().size();
        else
            item_num = value.toList().size();
        ret_value = QObject::tr("(%1 item)", "", item_num).arg(item_num);

        break;
    }
    case QVariant::ByteArray:
        ret_value = QLatin1String("...");
        break;
    default:
        ret_value = value.toString();
        break;
    }
    return ret_value;
}

PListEditorModelItem *addItemToModel(const QString& key, const QString& value,
    const QString& value_type, PListEditorModelItem *item)
{
    PListEditorModelItem *child = new PListEditorModelItem();
    item->addChild(child);
    child->setDatas(key, value_type, value);
    return child;
}

void parseElement(const QVariant &var, PListEditorModelItem *item)
{
    QString key;
    QString value;
    QString value_type;

    int type = var.type();
    switch (type)
    {
    case QVariant::Map:
    {
        QMap<QString, QVariant> map = var.toMap();
        for (auto it = map.begin(); it != map.end(); ++it)
        {
            key = it.key();
            value = transformValue(it.value());
            value_type = transformType(it.value().type());
            PListEditorModelItem *child =
                addItemToModel(key, value, value_type, item);
            parseElement(it.value(), child);
        }
    }
    break;
    case QVariant::List:
    {
        QList<QVariant> list = var.toList();
        for (auto it : list)
        {
            value = transformValue(it);
            value_type = transformType(it.type());
            PListEditorModelItem *child =
                addItemToModel(key, value, value_type, item);
            parseElement(it, child);
        }
    }
    break;
    default:
        break;
    }
}

bool parseValue(const QDomElement& node, QString *value, QString *value_type)
{
    Q_ASSERT(value);
    Q_ASSERT(value_type);
    if (!value_type || !value)
        return false;

    QString tagName = node.tagName();
    if (tagName == QLatin1String("dict") ||
        tagName == QLatin1String("array"))
    {
        value->clear();
        *value_type = tagName;
    }
    else
    {
        QVariant var;
        if (tagName == QLatin1String("string")) {
            var = node.text();
        }
        else if (tagName == QLatin1String("data")) {
            var = QByteArray::fromBase64(node.text().toUtf8());
        }
        else if (tagName == QLatin1String("integer")) {
            var = node.text().toLongLong();
        }
        else if (tagName == QLatin1String("real")) {
            var = node.text().toDouble();
        }
        else if (tagName == QLatin1String("true")) {
            var = true;
        }
        else if (tagName == QLatin1String("false")) {
            var = false;
        }
        else if (tagName == QLatin1String("date")) {
            var = QDateTime::fromString(node.text(), Qt::ISODate);
        }
        else
        {
            qDebug() << "PListParser Warning: Invalid tag found: " << node.tagName();
            return false;
        }

        *value = transformValue(var);
        *value_type = transformType(var.type());
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////

PListEditorModelParser::PListEditorModelParser() {}
PListEditorModelParser::~PListEditorModelParser() {}

void PListEditorModelParser::parseArrayElement(const QDomElement& node, PListEditorModelItem *item)
{
    QDomNodeList children = node.childNodes();
    for (int i = 0; i < children.count(); i++)
    {
        QDomElement value_node = children.at(i).toElement();
        if (value_node.isNull())
            continue;

        QString key;
        QString value;
        QString value_type;
        if (parseValue(value_node, &value, &value_type))
        {
            PListEditorModelItem *child = new PListEditorModelItem();
            item->addChild(child);
            parseElement(children.at(i + 1).toElement(), child);

            QString tagName = value_node.tagName();
            if (tagName == QLatin1String("dict") ||
                tagName == QLatin1String("array"))
            {
                int item_num = child->childCount();
                value = QObject::tr("(%1 item)", "", item_num).arg(item_num);
            }

            child->setDatas(key, value_type, value);
        }
    }
}

void PListEditorModelParser::parseDictElement(const QDomElement& node, PListEditorModelItem *item)
{
    QDomNodeList children = node.childNodes();
    for (int i = 0; i <= children.count() - 2; i++) 
    {
        QDomElement key_node = children.at(i).toElement();
        if (!key_node.isNull() && key_node.tagName() == QLatin1String("key"))
        {
            QDomElement value_node = children.at(i + 1).toElement();
            if (value_node.isNull())
                continue;            
            
            QString key = key_node.text();
            QString value;
            QString value_type;
            if (parseValue(value_node, &value, &value_type))
            {
                PListEditorModelItem *child = new PListEditorModelItem();
                item->addChild(child);
                parseElement(children.at(i + 1).toElement(), child);

                QString tagName = value_node.tagName();
                if (tagName == QLatin1String("dict") ||
                    tagName == QLatin1String("array"))
                {
                    int item_num = child->childCount();
                    value = QObject::tr("(%1 item)", "", item_num).arg(item_num);
                }

                child->setDatas(key, value_type, value);
                i++; // Ìø¹ývalue
            }
        }
    }
}

void PListEditorModelParser::parseElement(const QDomElement &node, PListEditorModelItem *item)
{
    QString tagName = node.tagName();
    if (tagName == QLatin1String("dict")) {
        parseDictElement(node, item);
    }
    else if (tagName == QLatin1String("array")) {
        parseArrayElement(node, item);
    }
}

PlistEditorModel *PListEditorModelParser::fromPlist(QIODevice *plist)
{
    Q_ASSERT(plist);
    if (!plist)
        return Q_NULLPTR;

    QDomDocument doc;
    QString errorMessage;
    int errorLine;
    int errorColumn;
    bool success = doc.setContent(PListParser::readPlistXML(plist), false, &errorMessage, &errorLine, &errorColumn);
    if (!success) 
    {
        qDebug() << "PListParser Warning: Could not parse PList file!";
        qDebug() << "Error message: " << errorMessage;
        qDebug() << "Error line: " << errorLine;
        qDebug() << "Error column: " << errorColumn;
        return Q_NULLPTR;
    }

    QDomElement root = doc.documentElement();
    // check consistency
    if (root.tagName() != QLatin1String("plist"))
    {
        qWarning("The file is not a vaild plist!");
        return Q_NULLPTR;
    }

    if (root.attribute(QStringLiteral("version"), QStringLiteral("1.0")) != QLatin1String("1.0")) {
        qDebug() << "PListParser Warning: plist is using an unknown format version, parsing might fail unexpectedly";
    }

    // create model
    PlistEditorModel *model = new PlistEditorModel();

    // create root item in model
    PListEditorModelItem *item = new PListEditorModelItem(QStringLiteral("plist"));
    model->getRoot()->addChild(item);

    QDomNodeList root_children = root.childNodes();
    // get dict element
    if (root_children.size() <= 0)
        return model;
    
    for (int i = 0; i < root_children.count(); i++)
    {
        QDomElement child = root_children.at(i).toElement();
        if (child.isNull())
            continue;

        QString tagName = child.tagName();
        if (tagName == QLatin1String("dict") ||
            tagName == QLatin1String("array"))
        {
            parseElement(child, item);

            int item_num = item->childCount();
            item->setType(tagName);
            item->setValue(QObject::tr("(%1 item)", "", item_num).arg(item_num));
            break;
        }
        else
        {
            QString value;
            QString value_type;
            if (parseValue(child, &value, &value_type))
            {
                item->setValue(value);
                item->setType(value_type);
                break;
            }
        }
    }

    return model;
}