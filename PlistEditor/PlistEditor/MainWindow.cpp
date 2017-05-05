#include "MainWindow.h"

#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    m_plist_editor = new PlistEditor();
    ui.centralWidget->layout()->addWidget(m_plist_editor);
}

MainWindow::~MainWindow()
{

}