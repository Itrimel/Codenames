#include "qcard.h"


QFont static findRightFontSize(QString mot, QRect rect, QFont font){
    double w,h,scale;
    QFontMetrics test(font);
    QRect temp = test.boundingRect(mot);
    w=temp.width();
    h=temp.height();
    scale=0.95*std::min(rect.width()/w,rect.height()/h);
    font.setPointSizeF(font.pointSizeF()*scale);
    return font;
}

void QCard::paintEvent(QPaintEvent* event){

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();
    painter.scale(0.5*width(),0.5*height());
    painter.translate(1,1);
    QColor couleur_bord,couleur_centre;
    switch(type){
    case typeCarte::Rouge :
        couleur_bord = Qt::red;
        couleur_centre = QColor(255,201,201);
        break;
    case typeCarte::Bleu :
        couleur_bord = Qt::blue;
        couleur_centre= QColor(201,201,255);
        break;
    case typeCarte::Noir :
        couleur_bord = Qt::black;
        couleur_centre = QColor(184,184,184);
        break;
    case typeCarte::Neutre :
        couleur_bord = QColor(255,255,0);
        couleur_centre = QColor(237,237,178);
    }

    QPainterPath contour;
    double scale=0.92, rounding = 0.3;
    rounding*=scale;
    contour.moveTo(0,scale);
    contour.lineTo(scale-rounding,scale);
    contour.arcTo(scale-2*rounding,scale-2*rounding,2*rounding,2*rounding,-90,90);
    contour.lineTo(scale,-scale+rounding);
    contour.arcTo(scale-2*rounding,-scale,2*rounding,2*rounding,0,90);
    contour.lineTo(-scale+rounding,-scale);
    contour.arcTo(-scale,-scale,2*rounding,2*rounding,90,90);
    contour.lineTo(-scale,scale-rounding);
    contour.arcTo(-scale,scale-2*rounding,2*rounding,2*rounding,180,90);
    contour.lineTo(0,scale);

    painter.setPen(QPen(couleur_bord,0.03));
    painter.fillPath(contour,couleur_centre);
    painter.drawPath(contour);

    painter.restore();
    QRect zone_texte = QRect(width()/5.0,height()/5.0,width()*3/5.0,height()*3/5.0);
    painter.setPen(type==Neutre ? QColor(181,132,0) : couleur_bord);
    painter.setFont(findRightFontSize(mot,zone_texte,painter.font()));
    painter.drawText(zone_texte,Qt::AlignCenter,mot);
    //painter.drawText(QPointF(width()/2.0,height()/2.0),mot);

}
