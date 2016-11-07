#include "converter.h"

//enum    e_cases_are_the {
//    NOMINATIVE  =   11, // именительный
//    GENITIVE    =   12  // родительный
//};

#define MALE    0   // мужской
#define FEMALE  1   // женский

#define RUBES   -1
#define KOPECKS -2

int Converter::the_choice_of_index_words(int k)
{
    return  ((((k % 10) == 1) && ((k % 100) != 11)) ? /*" пункт"*/0 :
            ((k >= 5) && (k <= 20)) ? /*" пунктов"*/1 :
            ((((k % 10) == 2) || ((k % 10) == 3) || ((k % 10) == 4)) &&
            (((k % 100) != 12) && ((k % 100) != 13) && ((k % 100) != 14))) ? /*" пункта"*/2 :
            /*" пунктов"*/1);
}

QMap<int, Number>   cardinal_number;
QMap<int, Name>     value_to_the_name;
QMap<int, Name>     name_of_exchange;

Converter::Converter(const QString &line, const QString &NDS, bool f, int &max)
{
    if ((NDS.size()  ==  2) && (NDS[0]   ==  '0'))
        nds =   NDS.mid(1);
    else
        nds =   NDS;
    with_a_capital_letter = f;
    initialization(line);
    filling();
    
    QList<int> list = value_to_the_name.keys();
    
    max =   list[list.size() - 1];
    
    max =   max -   (max%3);
}

void    Converter::initialization(const QString &line)
{
    QStringList list    =   line.split(QRegExp("[,.]"));
    
    if (list.size() ==  2) {
        rubes   =   list[0];
        kopecks =   list[1];
    } else {
        rubes   =   list[0];
        kopecks =   "0";
    }
    
    while (rubes[0] == '0')
        rubes = rubes.mid(1);
    
    kopecks.reserve(2);    
    
    if (kopecks.size()  ==  1) {
        kopecks[1]  =   '0';
    } else if (kopecks.size()  ==  0)
        kopecks =   "0";
    
    if (rubes.size()  ==  0)
        rubes =   "0";
}

bool    my_f(int rod,   int order, int the_latter_figure)
{
    int family;
    int index;
    
    if (order/3 ==  0) {
        index   =   rod;
        family  =   name_of_exchange[index].family();
    } else {
        index   =   order;
        family  =   value_to_the_name[index].family();
    }
    
    if ((family   ==  FEMALE) &&
        (((the_latter_figure%10  ==  1) || (the_latter_figure%10  ==  2)) &&
            (the_latter_figure != 11) && (the_latter_figure != 12)))
        return  1;
    else
        return 0;
}

QString space(const QString &res)
{
    if ((res.size() != 0) && (res[res.size() - 1] != ' '))
        return " ";
    return "";
}

QString Converter::processing_number(const int &order, int the_latter_figure, int rod, int num)
{
    QString res =   "";
    
    bool    flag;
    int index   =   find_index((the_latter_figure/100)*100, flag);
    
    if (flag)
        res += space(res) + cardinal_number[index].value(my_f(rod, order/3*3, index));
    
    index  =   find_index(the_latter_figure%100, flag);
    
    if (flag)
        res +=  space(res) + cardinal_number[index].value(my_f(rod, order/3*3, index));
    else {
        index   =   min_find_index(the_latter_figure%100, flag);
        res     +=  space(res) + cardinal_number[index].value(my_f(rod, order/3*3, index));
        
        index   =   find_index(the_latter_figure%10, flag);
        res     +=  space(res) + cardinal_number[index].value(my_f(rod, order/3*3, index));
    }
    
    if ((order != 0) && (order/3 != 0) && (the_latter_figure != 0)) {
        //qDebug() << "order = " << order << ", order/3*3 = " << order/3*3 << ", the_latter_figure = " << the_latter_figure;
        res +=  space(res) + value_to_the_name[(num == 0) ? order/3*3 : num].value(the_choice_of_index_words(the_latter_figure));
    }

    return  res;
}

int     Converter::find_index(const int &n, bool &find)
{
    int index = 0;
    find = false;
    
    for (auto it = cardinal_number.begin(); it != cardinal_number.end(); ++it)
        if (it.key() == n) {
            index   =   it.key();
            find    =   true;
        }
    
    return  index;
}

int     Converter::min_find_index(const int &n, bool &find)
{
    int index = 0;
    find = false;
    
    for (auto it = cardinal_number.begin(); it != cardinal_number.end(); ++it)
        if (it.key() <= n) {
            index   =   it.key();
            find    =   true;
        } else  break;
    
    return  index;
}

QString Converter::Up(QString s)
{
    if (with_a_capital_letter)
        s[1] = s[1].toUpper();
    return s;
}

QString Converter::in_words(QString name_exchange, int index_exchange)
{
    QString number  =   "";
    
    QString res =   "(";
    
    int index   =   0;
    
    int the_latter_figure = 0;
    
    int i = ((name_exchange.size() - 1)%3) + 1;
    bool    once    =   false;
    
    while ((index >= 0) && (name_exchange[index]    !=  '\0')) {
        if (!once)
            once = true;
        else i = 3;
        
        the_latter_figure = 0;
        
        while ((i-->0) && (name_exchange[index]    !=  '\0')) {
            number  +=  name_exchange[index];
            the_latter_figure   +=   name_exchange[index++].digitValue()*pow(10, i);
        }
        
        number  +=  " ";
        
        QString p;
        if ((res.size() != 0) && (res[res.size() - 1] != ' ') &&
            (res[res.size() - 1] != '('))
            p = " ";
        else p = "";
        
        int order   =   name_exchange.size() - index + 1;
        
        if ((order   ==  97) && (the_latter_figure%10   !=  the_latter_figure)) {
//            if (the_latter_figure%100   ==  the_latter_figure) {
                res +=  p + processing_number(order,    the_latter_figure/10,  index_exchange, 98);
                res +=  space(res) + processing_number(order,    the_latter_figure%10,  index_exchange);
        } else
            res +=  p + processing_number(order,    the_latter_figure,  index_exchange);
    }
    
    if ((res.size() != 0) && (res[res.size() - 1] == ' '))
        res.resize(res.size()-1);
        
    if (res[res.size() - 1] == '(')
        res += "ноль";
    
    res +=  ") " + name_of_exchange[index_exchange].value(the_choice_of_index_words(the_latter_figure));
    
    while ( (number[0] == '0' || number[0] == ' ') &&
            (number.toInt() != 0) && (number.size() != 1))
            number  =   number.mid(1);
    
    if (QString::compare(number.left(2), "00") == 0)
        number = number.mid(1);
    
    if (index_exchange  ==  KOPECKS)
        res =   number +
                name_of_exchange[index_exchange].value(the_choice_of_index_words(the_latter_figure));
    else
        res =   number  +   Up(res);
    
    return  res;
}

void    Converter::Nds(QString &rub_nds, QString &kop_nds)
{
    rub_nds =   "";
    
    QVector<QString> tmp;
    
    int i_nds   =   nds.toInt();
    int size    =   rubes.size();
    
    for (int i = 0; i < size;   ++i)
        tmp.push_back(QString::number(rubes[i].digitValue()*i_nds) + QString(size - 1 - i, '0'));
    
    int i = 0;
    
    QVector<QString> vec;
    
    while (tmp.size()) {
        if (tmp[i].size() > 0) {
            if (i == 0) {
                if (rub_nds != "")
                    vec.push_back(rub_nds);
                rub_nds = tmp[i][tmp[i].size() - 1];
            } else
                rub_nds +=  tmp[i][tmp[i].size() - 1];
            
            tmp[i].chop(1);
            i   =   i + 1 < tmp.size() ? i + 1 : 0;
            
        } else if (tmp[i].size() == 0) {
            tmp.remove(i);
            i   =   (i == tmp.size() ? 0 : i);
        }
    }
    
    vec.push_back(rub_nds);
    QVector<int> sum_vec;
    
    for (QString i : vec) {
        int sum = 0;
        for (int k = 0; k < i.size(); ++k)
            sum += i[k].digitValue();
        sum_vec.push_back(sum);
    }
    
    QVector<int> sum_vec1 = sum_vec;
    
    int g = 0;
    for (int i = 0; i < sum_vec.size(); ++i) {
        sum_vec[i] += g;
        g = 0;
        
        if (sum_vec[i] != (sum_vec[i]%10)) {
            g = sum_vec[i]/10;
        }
        sum_vec[i] = sum_vec[i]%10;
    }
    
    g = 0;
    for (int i = 0; i < sum_vec1.size(); ++i) {
        sum_vec1[i] += g + (i == 2 ? 1 : 0);
        g = 0;
        
        if (sum_vec1[i] != (sum_vec1[i]%10)) {
            g = sum_vec1[i]/10;
        }
        sum_vec1[i] = sum_vec1[i]%10;
    }
    
    QString result = "";
    QString result1 = "";
    
    for (int i = sum_vec.size() - 1; i >= 0 ; --i)
        result += QString::number(sum_vec[i]);
    
    for (int i = sum_vec1.size() - 1; i >= 0 ; --i)
        result1 += QString::number(sum_vec1[i]);
    
    int i_kop = result.right(2).toInt() + kopecks.toInt()*i_nds/100.;
    
    if (i_kop < 100) {
        if (i_kop != 0) {
            kop_nds = QString::number(i_kop);
        } else
            kop_nds = "0";
        
        result.chop(2);
        
        while (result[0] == '0')
            result = result.mid(1);
        
        rub_nds = result;
    } else {
        i_kop   -=  100;
        
        if (i_kop != 0) {
            kop_nds = QString::number(i_kop);
        } else
            kop_nds = "0";
        
        result1.chop(2);
        
        while (result1[0] == '0')
            result1 = result1.mid(1);
        
        rub_nds = result1;
    }
    
    if (rub_nds.size()  ==  0)
        rub_nds =   "0";
    
    if (kop_nds.size()  ==  0)
        kop_nds =   "0";
}

QString Converter::result()
{
    QString f = "", h = "";
    Nds(f, h);
    
    return  in_words(rubes,     RUBES)      +   " "     +
            in_words(kopecks,   KOPECKS)    +
            ((nds.toInt() != 0) ? ( ", в том числе НДС "  +   nds +   "% – "    +
                                    in_words(f,     RUBES)      +   " " +
                                    in_words(h,     KOPECKS)) :
                                    (". НДС не облагается"));
}

Converter::~Converter()
{
    
}

void    Converter::filling()
{
    filling_numbers();
    filling_the_value_to_the_name();
    filling_name_of_exchange();
}

void    Converter::filling_numbers()
{
    cardinal_number.insert(0,   Number(""));
    
    cardinal_number.insert(1,   Number("один"));
    cardinal_number[1].add(            "одна");
    
    cardinal_number.insert(2,   Number("два"));
    cardinal_number[2].add(            "две");
    
    cardinal_number.insert(3,   Number("три"));
    cardinal_number.insert(4,   Number("четыре"));
    cardinal_number.insert(5,   Number("пять"));
    cardinal_number.insert(6,   Number("шесть"));
    cardinal_number.insert(7,   Number("семь"));
    cardinal_number.insert(8,   Number("восемь"));
    cardinal_number.insert(9,   Number("девять"));
    cardinal_number.insert(10,  Number("десять"));
    cardinal_number.insert(11,  Number("одиннадцать"));
    cardinal_number.insert(12,  Number("двенадцать"));
    cardinal_number.insert(13,  Number("тринадцать"));
    cardinal_number.insert(14,  Number("четырнадцать"));
    cardinal_number.insert(15,  Number("пятнадцать"));
    cardinal_number.insert(16,  Number("шестнадцать"));
    cardinal_number.insert(17,  Number("семнадцать"));
    cardinal_number.insert(18,  Number("восемнадцать"));
    cardinal_number.insert(19,  Number("девятнадцать"));
    cardinal_number.insert(20,  Number("двадцать"));
    cardinal_number.insert(30,  Number("тридцать"));
    cardinal_number.insert(40,  Number("сорок"));
    cardinal_number.insert(50,  Number("пятьдесят"));
    cardinal_number.insert(60,  Number("шестьдесят"));
    cardinal_number.insert(70,  Number("семьдесят"));
    cardinal_number.insert(80,  Number("восемьдесят"));
    cardinal_number.insert(90,  Number("девяносто"));
    cardinal_number.insert(100, Number("сто"));
    cardinal_number.insert(200, Number("двести"));
    cardinal_number.insert(300, Number("триста"));
    cardinal_number.insert(400, Number("четыреста"));
    cardinal_number.insert(500, Number("пятьсот"));
    cardinal_number.insert(600, Number("шестьсот"));
    cardinal_number.insert(700, Number("семьсот"));
    cardinal_number.insert(800, Number("восемьсот"));
    cardinal_number.insert(900, Number("девятьсот"));
}

void    Converter::filling_the_value_to_the_name()
{
    if (1) {
        value_to_the_name.insert(3, Name("тысяча",  FEMALE));
        value_to_the_name[3].add(        "тысяч");
        value_to_the_name[3].add(        "тысячи");
        
        QString name[]  =
            {"миллион",
             "миллиард",
             "триллион",
             "квадриллион",
             "квинтиллион",
             "секстиллион",
             "септиллион",
             "октиллион",
             "нониллион",
             "дециллион",
             "ундециллион",
             "додециллион",
             "тредециллион",
             "кваттуордециллион",
             "квиндециллион",
             "седециллион",
             "септдециллион",
             "октодециллион",
             "вигинтиллион",
             "анвигинтиллион",
             "дуовигинтиллион",
             "тревигинтиллион",
             "кватторвигинтиллион",
             "квинвигинтиллион",
             "сексвигинтиллион",
             "септемвигинтиллион",
             "октовигинтиллион",
             "новемвигинтиллион",
             "тригинтиллион",
             "антригинтиллион",
             "дуотригинтиллион",
             "третригинтиллион",
             "кваттортригинтиллион",
             "квинтригинтиллион",
             "секстригинтиллион",
             "септемтригинллион",
             "октотригинтиллион",
             "новемтригинтиллион",
             "квадрагинтиллион",
             "анквадрагинтиллион",
             "дуоквадрагинтиллион"
             };
        
        for (int i = 6; i <= int(sizeof(name)/4*3 + 3); i += 3) {
            value_to_the_name.insert(i, Name(name[i/3-2],   MALE));
            value_to_the_name[i].add(        name[i/3-2] + "ов");
            value_to_the_name[i].add(        name[i/3-2] + "а");
        }
        
        if (int(sizeof(name)/4) >=  31) {
            value_to_the_name.insert(98, Name("гугол",   MALE));
            value_to_the_name[98].add(        "гуголов");
            value_to_the_name[98].add(        "гугола");
        }
    }
}

void    Converter::filling_name_of_exchange()
{
    name_of_exchange.insert(RUBES,  Name(   "рубль",    MALE));
    name_of_exchange[RUBES].add(            "рублей");
    name_of_exchange[RUBES].add(            "рубля");
    
    name_of_exchange.insert(KOPECKS,    Name("копейка", FEMALE));
    name_of_exchange[KOPECKS].add(           "копеек");
    name_of_exchange[KOPECKS].add(           "копейки");
}