

//
//  This parser is based on the "Calculator" example from section 6.1 of the book "The C++ Programming Language" by Bjarne Stroustrup
//


#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <cctype>
#include <cmath>

using namespace std;

class parser
{
  public:
    parser(const string&);                 // constructor: parameter is the function-string
                                           // to parse - for example "sin(x)*(1+cos(y))".

                                           // Allowed are: 
                                           //   +, -, *, /, (, ),
                                           //   exp(...),    log(...),
                                           //   sin(...),    cos(...),    tan(...),
                                           //   sinh(...),   cosh(...),   tanh(...),
                                           //   arcsin(...), arccos(...), arctan(...),
                                           //   sqr(...),    sqrt(...),   alt(...).

    double parse(const map<string,double>&);

                                           // Returns the value of the function. The arguments
                                           // have to be stored in a map<string,double> with the
                                           // variables as strings and their values as doubles.
    int get_no_of_errors();

  private:
    enum Token_value
    {
      NAME,         NUMBER,        END,
      PLUS='+',     MINUS='-',     MUL='*',     DIV='/',
      LP='(',       RP=')',
      SINUS='!',    COSINUS='?',   TAN='>',
      SINUSH='&',   COSINUSH='%',  TANH=':',    ARCSIN='{',
      ARCCOS='}',   ARCTAN='[',
      EXP='#',      LN='~',
      SQR=']',      SQRT='$',      ALT='<'
    };
    string st, temp_st;
    int no_of_errors;
    Token_value curr_tok;
    double number_value;
    string string_value;
    map<string,double> table;
    double error(const string&);
    Token_value get_token();
    double expr(bool);
    double prim(bool);
    double term(bool);
    bool in(const string& sub, const string& s);
    void replace(string& s, const string& old, const string& ne_w);
    double alternating(double x);
};

#endif
