#ifndef QCARD_H
#define QCARD_H

#include <QWidget>
#include <QPainter>
#include <iostream>
#include <QFontMetrics>

enum typeCarte{
    Rouge,
    Bleu,
    Noir,
    Neutre
};

class QCard : public QWidget
{
    Q_OBJECT
public:
    QCard(typeCarte type, QString mot, QWidget *parent = nullptr):
        mot(mot),
        type(type),
        QWidget(parent){
        //Capitaliser le mot
    };
virtual
    bool hasHeightForWidth() const override{return true;}
    int heightForWidth(int w) const override{return (int)0.7*w;}

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    const QString mot;
    const typeCarte type;

signals:

};

#endif // QCARD_H
