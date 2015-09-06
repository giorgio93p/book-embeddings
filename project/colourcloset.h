#ifndef COLOURCLOSET_H
#define COLOURCLOSET_H
#include <QColor>
#include <vector>

//this is a closet with colours
//each pageview created will take an available colour for its edges
//when a pageview "dies" it gives back its colour - the returned colour becomes available again.
//


class ColourCloset
{
public:
    ColourCloset();  //class constructor
    QColor getPaint();//returns a colour from the ones available
    void returnPaint(QColor); //make given colour available again
    void returnAll();        //makes all colours available
private:

    std::vector<QColor> *availableColours;

};

#endif // COLOURCLOSET_H
