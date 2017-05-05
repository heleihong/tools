#include "MainWindow.h"

#include <windows.h>

#include <QTranslator>
#include <QtWidgets/QApplication>

void setupApplication(QApplication &a)
{
    QTranslator *translator = new QTranslator(&a);
    translator->load("plisteditor_en", ":/i18n");
    a.installTranslator(translator);
}

int main(int argc, char *argv[])
{
#ifdef _DEBUG
    AllocConsole();
    FILE* fp = NULL;
    freopen_s(&fp, "CONOUT$", "w+t", stdout);
    freopen_s(&fp, "CONOUT$", "w+t", stderr);
    wprintf_s(L"Command: %s\n\n", GetCommandLineW());
#endif

    QApplication a(argc, argv);
    setupApplication(a);

    MainWindow w;
    w.show();
    return a.exec();
}
