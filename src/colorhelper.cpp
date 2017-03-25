#include "colorhelper.h"

ColorHelper::ColorHelper(QObject *parent): QObject(parent)
{

}

QString ColorHelper::colorString(const QColor &color)
{
    return color.name(QColor::HexArgb);
}

int ColorHelper::red(const QString &colorName)
{
    QColor color(colorName);
    return color.red();
}

int ColorHelper::green(const QString &colorName)
{
    QColor color(colorName);
    return color.green();
}

int ColorHelper::blue(const QString &colorName)
{
    QColor color(colorName);
    return color.blue();
}

int ColorHelper::alpha(const QString &colorName)
{
    QColor color(colorName);
    return color.alpha();
}
