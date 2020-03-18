#ifndef QCARD_H
#define QCARD_H

#include <QWidget>
#include <QPainter>
#include <iostream>
#include <QFontMetrics>
#include <algorithm>

enum typeCarte{
    Rouge,
    Bleu,
    Noir,
    Neutre,
    SaisPas
};

class QCard : public QWidget
{
    Q_OBJECT
public:
    QCard(typeCarte type, QString mot, std::vector<QCard*>* liste_cartes, QWidget *parent = nullptr):
        QWidget(parent),
        mot(mot),
        type(type),
        liste_cartes(liste_cartes){};
    double inline getRightFontSize() const {return rightFontSize;}
virtual
    bool hasHeightForWidth() const override{return true;}
    int heightForWidth(int w) const override{return (int)0.7*w;}

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEvent*) override;
    void leaveEvent(QEvent*) override;

private:
    const QString mot;
    const typeCarte type;
    void findRightFontSize(QString, QRect, QFont);
    double rightFontSize = 1;
    const std::vector<QCard*>* liste_cartes;
    bool isGuessed = false;
    bool drawGuessed = false;

signals:

};

#endif // QCARD_H