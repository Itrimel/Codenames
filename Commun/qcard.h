#ifndef QCARD_H
#define QCARD_H

#include <QWidget>
#include <QPainter>
#include <iostream>
#include <QFontMetrics>
#include <QMouseEvent>
#include <algorithm>
#include "protocole.h"

class QCard : public QWidget
{
    Q_OBJECT
public:
    QCard(int numero,typeCarte type, QString mot, std::vector<QCard*>* liste_cartes, QWidget *parent = nullptr):
        QWidget(parent),
        mot(mot),
        type(type),
        liste_cartes(liste_cartes),
        num(numero) {};
    double inline getRightFontSize() const {return rightFontSize;}
    void setGuess();
    bool inline getGuess() {return isGuessed;}
    QString inline getMot() {return mot;}
    typeCarte inline getType() {return type;}
    void setType(typeCarte);
virtual
    bool hasHeightForWidth() const override{return true;}
    int heightForWidth(int w) const override{return (int)0.7*w;}

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEvent*) override;
    void leaveEvent(QEvent*) override;
    void mousePressEvent(QMouseEvent*) override;

private:
    const QString mot;
    typeCarte type;
    void findRightFontSize(QString, QRect, QFont);
    double rightFontSize = 1;
    const std::vector<QCard*>* liste_cartes;
    bool isGuessed = false;
    bool drawGuessed = false;
    const int num;

signals:
    void cardClicked(int);
};

#endif // QCARD_H
