#pragma once
#include <QObject>
#include "PickGroup.h"
#include "Lemniscate.h"

class LemniscateController : QObject {
    Q_OBJECT

    Lemniscate* lemniscate;
    PickGroup* x1Group;
    PickGroup* y1Group;
    PickGroup* x2Group;
    PickGroup* y2Group;
public:
    LemniscateController();
    void setX1Group(PickGroup* group);
    void setY1Group(PickGroup* group);
    void setX2Group(PickGroup* group);
    void setY2Group(PickGroup* group);
    void setLemniscate(Lemniscate* lem);
    Lemniscate *getLemniscate() const;

private slots:
    void x1Changed(int x1);
    void y1Changed(int y1);
    void x2Changed(int x2);
    void y2Changed(int y2);
};
