#include "qcard.h"

void QCard::paintEvent(QPaintEvent* event){

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.scale(width(),height());
    QColor couleur_bord;
    switch(type){
    case typeCarte::Rouge :
        couleur_bord = Qt::red;
        break;
    case typeCarte::Bleu :
        couleur_bord = Qt::blue;
        break;
    case typeCarte::Noir :
        couleur_bord = Qt::black;
        break;
    case typeCarte::Neutre :
        couleur_bord.setRgbF(230.0/255.0,230.0/255.0,0);
    }


    QPainterPath contour;
    contour.moveTo(0.5,0);
    contour.lineTo(0.95,0);
    contour.arcTo(0.9,0,0.1,0.1,90,-90);
    contour.lineTo(1,0.95);
    contour.arcTo(0.9,0.9,0.1,0.1,0,-90);
    contour.lineTo(0.05,1);
    contour.arcTo(0,0.9,0.1,0.1,-90,-90);
    contour.lineTo(0,0.05);
    contour.arcTo(0,0,0.1,0.1,180,-90);
    contour.closeSubpath();

    painter.setPen(QPen(couleur_bord,3));
    painter.drawPath(contour);
}
