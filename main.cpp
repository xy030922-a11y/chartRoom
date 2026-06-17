#include "MainWindow.h"

#include <QApplication>


int main(int argc, char* argv[])
{
    // 整体 DPI / UI 缩放：1.75 倍
    qputenv("QT_SCALE_FACTOR", "1.5");

    // 字体 DPI：96 * 1.5 = 144
    qputenv("QT_FONT_DPI", "144");


    QApplication app(argc, argv);
    
    MainWindow w;

    w.resize(800 ,600);
    w.show();


    return app.exec();
}
