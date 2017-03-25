#ifndef CONFIGVIEW_H
#define CONFIGVIEW_H

#include <QObject>
#include <QQuickView>
#include <QColor>
#include <qpa/qplatformnativeinterface.h>
#include <QGuiApplication>
#include <mlite5/MGConfItem>
#include <QColor>

class ConfigView : public QObject
{
    Q_OBJECT
public:
    explicit ConfigView(QQuickView *parent = 0);

public slots:
    void setColor(qreal red, qreal green, qreal blue, qreal opacity);
    void setColor(const QColor &color);

private slots:
    void updateColor();

private:
    MGConfItem *dconf;

};

#endif // CONFIGVIEW_H
