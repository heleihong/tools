#ifndef MainWindow_H
#define MainWindow_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

#include <QDropEvent>

#include "plisteditor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

private:
    PlistEditor *m_plist_editor;

private:
    Ui::MainWindow ui;
};

#endif // MainWindow_H
