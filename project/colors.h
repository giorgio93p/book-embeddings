#ifndef COLORS
#define COLORS

/*
 * Simple header file that provides us with a function getPageColor, which return a different color for each page number,
 * assuming that we have up to 14 pages. We use it in the GraphScene and PageScene classes.
 */

#include<QColor>


static QColor pageColors[] = {Qt::red,Qt::green,Qt::blue,Qt::cyan,Qt::magenta,Qt::yellow,Qt::gray,
                  Qt::darkRed,Qt::darkGreen,Qt::darkBlue,Qt::darkCyan,Qt::darkMagenta,Qt::darkYellow,Qt::lightGray};
static int pageColors_length = 14;

static    QColor getPageColor(int page){
    return pageColors[page % pageColors_length];
}


#endif // COLORS

