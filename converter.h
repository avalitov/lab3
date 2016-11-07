#ifndef CONVERTER_H
#define CONVERTER_H

#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QDebug>
#include "number.h"
#include "name.h"

class Converter
{
    QString rubes;
    QString kopecks;
    QString nds;
    bool with_a_capital_letter;
public:
    Converter(const QString &, const QString &NDS, bool f, int &max);
    QString result();
    QString in_words(QString name_exchange, int index_exchange);
    void    Nds(QString &rub_nds, QString &kop_nds);
    
    ~Converter();
private:
    void    initialization      (const QString &);
    QString processing_number   (const int &, int the_latter_figure, int rod, int num = 0);
    QString Up(QString s);
private:
    void    filling();
    void    filling_numbers();
    void    filling_the_value_to_the_name();
    void    filling_name_of_exchange();
    
    int find_index(const int&, bool &);
    int min_find_index(const int&, bool &);
    
    int the_choice_of_index_words(int k);
};

#endif // CONVERTER_H
