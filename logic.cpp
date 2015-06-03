#include <string>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <map>
#include <iostream>
#include <boost/algorithm/string.hpp>


//Acrtan...
//
using namespace std;
using namespace boost::algorithm;

enum Token_value : char
{
    NAME,             NUMBER,                 END,
    PLUS='+',         MINUS='-',              MUL='*',        DIV='/',
    PRINT=';',        ASSIGN='=',             LP='(',         RP=')',
    POW='^',
    SIN, COS, TAN, ABS, LN, LOG
};

enum Number_value : char
{
    NUM0='0', NUM1='1', NUM2='2',
    NUM3='3', NUM4='4', NUM5='5',
    NUM6='6', NUM7='7', NUM8='8',
    NUM9='9', NUMP='.',
};

Token_value curr_Tok = PRINT;                                                                       // Останнє, що сидить у літералі
double number_Value;                                                                                        // Число у виразі
std::string string_Value;                                                                                      // Тіпа ім’я
std::map<std::string, double> Table;                                                               // Таблица констант

char s[100];
char *ss=s;



double expr (bool get);

Token_value get_Token()
{



    //    while(isspace(*ss));      //Пропускаємо передні пробіли           РОЗІБРАТИСЯ
    //    {
    //        if(!*ss)
    //            return curr_Tok=END;
    //        ss++;
    //    }

    switch(*ss)
    {
    case 0:  return curr_Tok=END;

    case PRINT:                                                                                                  //тіпа коли ентер то кінець виразу, теба рах(к)увати
    case '\n':
        return curr_Tok=PRINT;
    case MUL:
    case DIV:
    case PLUS:
    case MINUS:
    case POW :
    case LP:
    case RP:
    case ASSIGN:
        //Якщо символ дії - вертаємо символ дії
        curr_Tok=Token_value(*ss);
        ss++;
        return curr_Tok;

    case NUM0: case NUM1: case NUM2: case NUM3: case NUM4:
    case NUM5: case NUM6: case NUM7: case NUM8: case NUM9:  //Якщо якась  цифра..
    case NUMP:
        //Вертаємо її назад..
        number_Value=atof(ss);                                                       //Зчитуємо число..
        while(isdigit(*ss)||*ss=='.')                                                  //пересовуємо вказівник далі
            ss++;
        return  curr_Tok=NUMBER;

    default:                                                                                                         //Якщо ж це не дія і не число, то..
        if(isalpha(*ss))
        {
            string_Value=*ss;
            while(*ss)
            {
                ss++;
                if(isalpha(*ss))
                    string_Value.push_back(*ss);
                else break;

            }
            //ss--;                                                                                                       //Вертаємо лишній символ

          to_lower(string_Value);
            if(string_Value=="sin")             //Шукаємо функцію
                return curr_Tok=SIN;
            if(string_Value=="cos")
                return curr_Tok=COS;
            if(string_Value=="tan")
                return curr_Tok=TAN;
            if(string_Value=="abs")
                return curr_Tok=ABS;
            if(string_Value=="ln")
                return curr_Tok=LN;
            if(string_Value=="log")
                return curr_Tok=LOG;

            return curr_Tok = NAME;        //Якщо ніц нема, то є змінна, я тє атвєчаю
        }
        //Якщо Голуб ввів хуйню, то Голуб - хуй прога вивалиться
        return curr_Tok = PRINT;
    }

}


double prim(bool get)
{
    if(get)
        get_Token();

    switch(curr_Tok)
    {
    case NUMBER :
    {
        double v=number_Value;
        get_Token();                                //Захавав число - переводь на некст, щоб прімару не вивалив еррор
        return v;
    }
    case NAME :
    {
        double &v =Table[string_Value];
        if(get_Token()==ASSIGN)
            v=exp(true);
        return v;
    }

    case MINUS :
        return -prim(true);

    case LP :
    {
        double e = expr(true);
        if(curr_Tok!=RP)
        {
            cout<<"Error occured. Where is \')\'";
            return 0;
        }
        get_Token();
        return e;
    }

    case SIN:
        return sin(prim(true));         //Кажись, я накосячив і це має бути у терм

    case COS:
        return cos(prim(true));

    case TAN:
        return tan(prim(true));

    case ABS :
        return fabs(prim(true));
    case LN:
        return log(prim(true));

    case LOG:
        return log10(prim(true));

    default :
        cout<<"Error occured. Primary expected";
    }
}


double term(bool get)
{
    double left=prim(get);

    for(;;)                                             //Ганяємося, поки не буде кінець цим операціям і ми не попадемо на дефолт
    {
        switch(curr_Tok)
        {
        case MUL :
            left *= prim(true);
            break;
        case DIV :
            if(double d=prim(true))     //Хітрожопо обробляємо ділення на 0
            {
                left /=d;
                break;
            }
            cout<<"Error occured. Some rakal divide by 0";
            return 1;
        case POW :
            left=pow(left,prim(true));
            break;

        default :
            return left;
        }

    }
}


double expr (bool get) {
    double left = term( get);

    for ( ; ; )                                                                                     //Аналогічно як і у терм
    {
        switch (curr_Tok) {
        case PLUS:
            left += term( true);
            break;
        case MINUS:
            left -= term(true);
            break;
        default:
            return left;
        }
    }
}

double xAt(double x)
{
    Table["x"]=x;
    return expr(true);
}

int init()
{

    Table["pi"]=3.14159265359;

    return 0;
}
