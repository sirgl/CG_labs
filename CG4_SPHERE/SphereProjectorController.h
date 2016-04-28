#ifndef SPHEREPROJECTORCONTROLLER_H
#define SPHEREPROJECTORCONTROLLER_H

#include <QObject>
#include "PickGroup.h"
#include "SphereProjector.h"
#include "ComboBoxGroup.h"

class SphereProjectorController : public QObject
{
    Q_OBJECT

    PickGroup* xGroup;
    PickGroup* yGroup;
    PickGroup* scaleGroup;

    ComboBoxGroup* filtrationGroup;
    SphereProjector* sphereProjector;
public:
    SphereProjectorController();
    void setXGroup(PickGroup* group);
    void setYGroup(PickGroup* group);
    void setScaleGroup(PickGroup* group);

    SphereProjector *getSphereProjector() const;
    void setSphereProjector(SphereProjector *projector);

    ComboBoxGroup *getFiltrationGroup() const;
    void setFiltrationGroup(ComboBoxGroup *group);

private slots:
    void xChanged(int x);
    void yChanged(int y);
    void scaleChanged(int scale);
    void filtrationChanged(QString filtration);
public slots:
    void xyOffsetChanged(int dx, int dy);
};

#endif // SPHEREPROJECTORCONTROLLER_H
