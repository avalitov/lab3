#include "name.h"

Name::Name(const QString &line, const int &_family)
{
    add_pair(line);
    m_family  =   _family;
}

int Name::family()
{
    return m_family;
}

void    Name::add(const QString &line)
{
    add_pair(line);
}

void    Name::add_pair(const QString &line)
{
    name.push_back(line);
}

QString Name::value(int index)
{
    return name[index];
}

Name::~Name()
{
    
}

