QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Commun/qcard.cpp \
    Commun/socketcommun.cpp \
    Serveur/main_serveur.cpp \
    Serveur/mainwindowserveur.cpp \
    Serveur/qglobaipdiag.cpp \
    Serveur/widgetconnexion.cpp

HEADERS += \
    Commun/protocole.h \
    Commun/qcard.h \
    Commun/socketcommun.h \
    Serveur/mainwindowserveur.h \
    Serveur/qglobaipdiag.h \
    Serveur/serveur_internet_infos.h \
    Serveur/widgetconnexion.h

FORMS += \
    Serveur/serveur.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

mytarget.target = liste_mots.o
unix{
    contains(QMAKE_HOST.arch,x86_64){
        mytarget.commands = objcopy --input binary --output elf64-x86-64 --binary-architecture i386:x86-64 ../Codenames/Commun/liste_mots.txt liste_mots.o
    }
    contains(QMAKE_HOST.arch,x86){
        mytarget.commands = objcopy --input binary --output elf32-i386 --binary-architecture i386 ../Codenames/Commun/liste_mots.txt liste_mots.o
    }
}
win32 {
    contains(QMAKE_HOST.arch,x86_64){
        mytarget.commands = objcopy.exe --input binary --output elf64-x86-64 --binary-architecture i386:x86-64 ../Codenames/Commun/liste_mots.txt liste_mots.o
    }
    contains(QMAKE_HOST.arch,x86){
        mytarget.commands = objcopy.exe --input binary --output elf32-i386 --binary-architecture i386 ../Codenames/Commun/liste_mots.txt liste_mots.o
    }
}
mytarget.depends = mytarget2 ../Codenames/Commun/liste_mots.txt
mytarget2.commands = @echo Building liste_mots.o

LIBS+=liste_mots.o
QMAKE_EXTRA_TARGETS += mytarget mytarget2
PRE_TARGETDEPS += liste_mots.o
