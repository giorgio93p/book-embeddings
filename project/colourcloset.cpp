#include "colourcloset.h"

ColourCloset::ColourCloset()
{

    availableColours = new std::vector<QColor>();
    QColor pageColors[] = {Qt::lightGray,Qt::darkYellow,Qt::darkMagenta,Qt::darkCyan,Qt::darkBlue,
                           Qt::darkGreen,Qt::darkRed,
                           Qt::gray,Qt::yellow,Qt::magenta,Qt::cyan,Qt::blue,Qt::green,Qt::red};
    availableColours->assign(pageColors,pageColors+14);
}

QColor ColourCloset::getPaint() {


    QColor retval;
    if (availableColours->empty()) return Qt::BlankCursor; //attention! this needs an error handling mechanism

    else {
        retval = availableColours->back();
        availableColours->pop_back();
        return retval;
    }
}

void ColourCloset::returnPaint(QColor color) {
    availableColours->push_back(color);
}

void ColourCloset::returnAll() {
    QColor pageColors[] = {Qt::lightGray,Qt::darkYellow,Qt::darkMagenta,Qt::darkCyan,Qt::darkBlue,
                           Qt::darkGreen,Qt::darkRed,
                           Qt::gray,Qt::yellow,Qt::magenta,Qt::cyan,Qt::blue,Qt::green,Qt::red};
    availableColours->assign(pageColors,pageColors+14);
}
