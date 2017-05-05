#ifndef PLIST_EDITOR_DOM_PARSER_H_
#define PLIST_EDITOR_DOM_PARSER_H_

#include <QtXml/QDomDocument>

#include "PlistEditorModel.h"

class PListEditorModelParser
{
public:
    PListEditorModelParser();
    ~PListEditorModelParser();

    static PlistEditorModel *fromPlist(QIODevice *plist);

private:
    static void parseArrayElement(const QDomElement& element, PListEditorModelItem *item);
    static void parseDictElement(const QDomElement& element, PListEditorModelItem *item);
    static void parseElement(const QDomElement &node, PListEditorModelItem *item);
};

#endif // PLIST_EDITOR_DOM_PARSER_H_