

//
//  This parser is based on the "Calculator" example from section 6.1 of the book "The C++ Programming Language" by Bjarne Stroustrup
//


#include "parser.h"

using namespace std;

bool parser::in(const string& sub, const string& s)
{
  if(s.find(sub)!=string::npos)
  {
    return true;
  }
  else
  {
    return false;
  }
}


void parser::replace(string& s, const string& old, const string& ne_w)
{
  string::size_type pos=s.find(old);
  while( pos!=string::npos )
  {
    s.replace(pos, old.length(), ne_w);
    pos=s.find(old, pos+ne_w.length());
  }
}


double parser::alternating(double x)
{
  int i=static_cast<int>(x);
  if(i%2==0)
  {
    return 1.0;
  }
  else
  {
    return -1.0;
  }
}

parser::parser(const string& s)
{
  no_of_errors=0;
  st=s; 
  unsigned int i=0;
  while(i<st.length())
  { 
    if(isspace(st[i])) { st.erase(i,1); i--; }
    i++;
  };
  if( in("{", st) || in("&", st) || in("&", st) || in("!", st) || in("}", st)
                  || in("%", st) || in("?", st) || in("[", st) || in(":", st)
                  || in(">", st) || in("#", st) || in("~", st) || in("$", st)
                  || in("]", st) || in("<", st) ) error ("wrong token");

  replace(st, "arcsin", "{");  replace(st, "sinh",   "&");
  replace(st, "sin",    "!");  replace(st, "arccos", "}");
  replace(st, "cosh",   "%");  replace(st, "cos",    "?");
  replace(st, "arctan", "[");  replace(st, "tanh",   ":");
  replace(st, "tan",    ">");  replace(st, "exp",    "#");
  replace(st, "log",    "~");  replace(st, "sqrt",   "$");
  replace(st, "sqr",    "]");  replace(st, "alt",    "<");
}

double parser::error(const string& s)
{
  no_of_errors++;
  cerr << "parser error: " << s << '\n';
  return 1;
}

parser::Token_value parser::get_token()
{
  istringstream str(temp_st);
  temp_st.erase();
  char ch = 0;
  if(!str.get(ch)) return curr_tok = END;
  switch(ch)
  {
    case '\n':
      str >> temp_st; return curr_tok=END;
    case '*': case '/': case '+': case '-': case '(': case ')':
    case '!': case '?': case '#': case '~': case ']': case '$':
    case '>': case '&': case '%': case ':': case '{': case '}':
    case '[': case '<':
      str >> temp_st; return curr_tok=Token_value(ch);
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    case '.':
      str.putback(ch);
      str >> number_value;
      str >> temp_st; 
      return curr_tok=NUMBER;
    default:
      if (isalpha(ch) || (ch=='_'))
      {
        string_value = ch;
        while(str.get(ch) && (isalnum(ch) || (ch=='_')) ) string_value.push_back(ch);
        str.putback(ch);
        str >> temp_st;
        return curr_tok=NAME;
      }
      str >> temp_st;
      error("wrong token");
      return curr_tok=END;
  }
}

double parser::prim(bool get)
{
  double x=0;
  if (get) get_token();
  switch (curr_tok)
  {
    case NUMBER:
    {
      double v = number_value;
      get_token();
      return v;
    }
    case NAME:
    {
      if(table.find(string_value)==table.end())
      {
        no_of_errors++;
        return 0.0;
      }
      double v = table[string_value];
      get_token();
      return v;
    }
    case PLUS:
      return prim(true);
    case MINUS:
      return -prim(true);
    case SINUS:
      return sin(prim(true));
    case ARCSIN:
      return asin(prim(true));
    case COSINUS:
      return cos(prim(true));
    case ARCCOS:
      return acos(prim(true));
    case TAN:
      return tan(prim(true));
    case ARCTAN:
      return atan(prim(true));
    case TANH:
      return tanh(prim(true));
    case SINUSH:
      return sinh(prim(true));
    case COSINUSH:
      return cosh(prim(true));
    case EXP:
      return exp(prim(true));
    case LN:
      return log(prim(true));
    case SQR:
      x=prim(true);
      return x*x;
    case SQRT:
      return sqrt(prim(true));
    case ALT:
      return alternating(prim(true));
    case LP:
    {
      double e = expr(true);
      if (curr_tok != RP) return error(") expected");
      get_token();
      return e;
    }
    default:
      return error("primary expected");
  }
}

double parser::term(bool get)
{
  double left = prim(get);
  for(;;) 
    switch (curr_tok)
    {
      case MUL:
        left *= prim(true);
        break;
      case DIV:
        {
          double d = prim(true);
          if (!(d==0))
          {
            left /= d;
            break;
          }
          return error("division by 0");
        }
      default:
        return left;
    }
}

double parser::expr(bool get)
{
  double left = term(get);
  for(;;) 
    switch (curr_tok)
    {
      case PLUS:
        left += term(true);
        break;
      case MINUS:
        left -= term(true);
        break;
      default:
        return left;
    }
}

double parser::parse(const map<string,double>& val)
{
  if (no_of_errors>0) return 0;
  table=val;
  temp_st=st;
  curr_tok=END;
  number_value=0;
  string_value.erase();
  get_token();
  return expr(false);
}

int parser::get_no_of_errors()
{
  return no_of_errors;
}
