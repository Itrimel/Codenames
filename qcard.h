#ifndef QCARD_H
#define QCARD_H

#include <QWidget>
#include <QPainter>

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
        QWidget(parent){};

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    const QString mot;
    const typeCarte type;

signals:

};

#endif // QCARD_H
