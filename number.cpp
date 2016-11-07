#include "number.h"

Number::Number(const QString &line)
{
    add_pair(line);
}

void    Number::add(const QString &line)
{
    add_pair(line);
}

void    Number::add_pair(const QString &line)
{
    number.push_back(line);
}

QString Number::value(int index)
{
    return number[index];
}

Number::~Number()
{
    
}

