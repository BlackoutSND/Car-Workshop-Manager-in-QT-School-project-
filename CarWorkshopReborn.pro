QT       += core gui sql printsupport pdf

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    carpartsadder.cpp \
    employeeadder.cpp \
    employeetimeslots.cpp \
    main.cpp \
    mainwindow.cpp \
    print.cpp \
    ticketviewer.cpp \
    timepicker.cpp

HEADERS += \
    car.h \
    carParts.h \
    carpartsadder.h \
    employee.h \
    employeeadder.h \
    employeetimeslots.h \
    mainwindow.h \
    print.h \
    ticket.h \
    ticketviewer.h \
    timepicker.h \
    utilities.h

FORMS += \
    carpartsadder.ui \
    employeeadder.ui \
    employeetimeslots.ui \
    mainwindow.ui \
    print.ui \
    ticketviewer.ui \
    timepicker.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    carWorkshop.db \
    db/carWorkshop.db \
    egui-qt.md \
    imgs/tstLogo.png \
    imgs/tstLogoRes.png

RESOURCES += \
    resources.qrc
