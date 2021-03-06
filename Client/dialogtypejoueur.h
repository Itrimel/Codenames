#ifndef DIALOGTYPEJOUEUR_H
#define DIALOGTYPEJOUEUR_H

#include <QDialog>
#include "Commun/socketcommun.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class DialogTypeJoueur : public QDialog
{
    Q_OBJECT
public:
    DialogTypeJoueur(QWidget*, SocketCommun**);
    ~DialogTypeJoueur();
    void closeFromParent();
public slots:
    void reject() override {};
private:
    SocketCommun** socket;
    Ui::Dialog* ui;
    typeJoueur joueur;
    void closeEvent(QCloseEvent *) override;
private slots:
    void radioClicked();
    void choixFait();
    void finDiag();
};

#endif // DIALOGTYPEJOUEUR_H
