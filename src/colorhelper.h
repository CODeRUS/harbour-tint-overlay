#ifndef COLORHELPER_H
#define COLORHELPER_H

#include <QObject>
#include <QColor>

class ColorHelper : public QObject
{
    Q_OBJECT
public:
    explicit ColorHelper(QObject *parent = 0);
    Q_INVOKABLE QString colorString(const QColor &color);
    Q_INVOKABLE int red(const QString &colorName);
    Q_INVOKABLE int green(const QString &colorName);
    Q_INVOKABLE int blue(const QString &colorName);
    Q_INVOKABLE int alpha(const QString &colorName);

};

#endif // COLORHELPER_H
