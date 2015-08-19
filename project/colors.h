#ifndef COLORS
#define COLORS

#include<QColor>


static QColor pageColors[] = {Qt::red,Qt::green,Qt::blue,Qt::cyan,Qt::magenta,Qt::yellow,Qt::gray,
                  Qt::darkRed,Qt::darkGreen,Qt::darkBlue,Qt::darkCyan,Qt::darkMagenta,Qt::darkYellow,Qt::lightGray};
static int pageColors_length = 14;

static    QColor getPageColor(int page){
    return pageColors[page % pageColors_length];
}


#endif // COLORS

