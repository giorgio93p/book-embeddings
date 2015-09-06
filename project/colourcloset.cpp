#include "colourcloset.h"

ColourCloset::ColourCloset()
{

    //this creates the vector with the colours that can be "given".

    availableColours = new std::vector<QColor>();
    QColor pageColors[] = {Qt::lightGray,Qt::darkYellow,Qt::darkMagenta,Qt::darkCyan,Qt::darkBlue,
                           Qt::darkGreen,Qt::darkRed,Qt::gray,Qt::yellow,Qt::magenta,Qt::cyan,
                           Qt::blue,Qt::green,Qt::red};





    availableColours->assign(pageColors,pageColors+14);
}

QColor ColourCloset::getPaint() {




    QColor retval;


    if (availableColours->empty()) return Qt::BlankCursor; //attention! this needs an error handling mechanism



    else {
        //get the top of the colour stack and give a colour to the caller

        retval = availableColours->back();
        availableColours->pop_back();
        return retval;
    }
}

void ColourCloset::returnPaint(QColor color) {
    availableColours->push_back(color);

    //simply add it to the available colours stack
}

void ColourCloset::returnAll() {
    QColor pageColors[] = {Qt::lightGray,Qt::darkYellow,Qt::darkMagenta,Qt::darkCyan,Qt::darkBlue,
                           Qt::darkGreen,Qt::darkRed,
                           Qt::gray,Qt::yellow,Qt::magenta,Qt::cyan,Qt::blue,Qt::green,Qt::red};
    availableColours->assign(pageColors,pageColors+14);

    //just what the consructor does
}
