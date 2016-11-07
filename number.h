#ifndef NUMBER_H
#define NUMBER_H

#include <QVector>
#include <QPair>
#include <QString>
#include <QMap>

class Number
{
    QVector<QString>    number;
public:
    Number      () {}
    Number      (const QString &);
    void    add (const QString &);
    
    QString value(int index = 0);
    
    ~Number();
private:
    void    add_pair(const QString &);
};

#endif // NUMBER_H
