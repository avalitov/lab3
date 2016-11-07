#ifndef NAME_H
#define NAME_H

#include <QVector>
#include <QPair>
#include <QString>
#include <QMap>

class Name
{
    QVector<QString>    name;
    int m_family;
public:
    Name() {}
    Name      (const QString &, const int &_family);
    void    add (const QString &);
    
    QString value(int index = 0);
    
    int family();
    ~Name();
private:
    void    add_pair(const QString &);
};

#endif // NAME_H
