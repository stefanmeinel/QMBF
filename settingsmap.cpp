#include "settingsmap.h"


// this getline function can handle LF, CR, and CRLF newline characters (Linux/OS X/Windows)
// from http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
std::istream& settingsmap::safeGetline(std::istream& is, std::string& t)
{
  t.clear();

  // The characters in the stream are read one-by-one using a std::streambuf.
  // That is faster than reading them one-by-one using the std::istream.
  // Code that uses streambuf this way must be guarded by a sentry object.
  // The sentry object performs various tasks,
  // such as thread synchronization and updating the stream state.

  std::istream::sentry se(is, true);
  std::streambuf* sb = is.rdbuf();

  for(;;)
  {
    int c = sb->sbumpc();
    switch (c)
    {
      case '\n':
        return is;
      case '\r':
        if(sb->sgetc() == '\n')
        {
          sb->sbumpc();
        }
        return is;
      case EOF:
        // Also handle the case when the last line has no line ending
        if(t.empty())
        {
          is.setstate(std::ios::eofbit);
        }
        return is;
      default:
        t += (char)c;
    }
  }
}




bool settingsmap::exists(string key)
{
  if(data.find(key)!=data.end())
  {
    return true;
  }
  else
  {
    return false;
  }
}


bool settingsmap::exists(string key, int index)
{
  stringstream key_st;
  key_st << key << "_" << index;
  if(data.find(key_st.str())!=data.end())
  {
    return true;
  }
  else
  {
    return false;
  }
}


bool settingsmap::exists(string key, int index1, int index2)
{
  stringstream key_st;
  key_st << key << "_" << index1 << "_" << index2;
  if(data.find(key_st.str())!=data.end())
  {
    return true;
  }
  else
  {
    return false;
  }
}


void settingsmap::set_string(string key, string value)
{
  data[key]=value;
}

void settingsmap::set_double(string key, double value)
{
  stringstream val_st;
  val_st.precision(d_prec);
  val_st << value;
  data[key]=val_st.str();
}

void settingsmap::set_int(string key, int value)
{
  stringstream val_st;
  val_st << value;
  data[key]=val_st.str();
}

void settingsmap::set_bool(string key, bool value)
{
  stringstream val_st;
  val_st << value;
  data[key]=val_st.str();
}


void settingsmap::set_string(string key, int index, string value)
{
  stringstream key_st;
  key_st << key << "_" << index;
  data[key_st.str()]=value;
}

void settingsmap::set_double(string key, int index, double value)
{
  stringstream key_st;
  key_st << key << "_" << index;
  stringstream val_st;
  val_st.precision(d_prec);
  val_st << value;
  data[key_st.str()]=val_st.str();
}

void settingsmap::set_int(string key, int index, int value)
{
  stringstream key_st;
  key_st << key << "_" << index;
  stringstream val_st;
  val_st << value;
  data[key_st.str()]=val_st.str();
}

void settingsmap::set_bool(string key, int index, bool value)
{
  stringstream key_st;
  key_st << key << "_" << index;
  stringstream val_st;
  val_st << value;
  data[key_st.str()]=val_st.str();
}

void settingsmap::set_string(string key, int index1, int index2, string value)
{
  stringstream key_st;
  key_st << key << "_" << index1 << "_" << index2;
  data[key_st.str()]=value;
}

void settingsmap::set_double(string key, int index1, int index2, double value)
{
  stringstream key_st;
  key_st << key << "_" << index1 << "_" << index2;
  stringstream val_st;
  val_st.precision(d_prec);
  val_st << value;
  data[key_st.str()]=val_st.str();
}

void settingsmap::set_int(string key, int index1, int index2, int value)
{
  stringstream key_st;
  key_st << key << "_" << index1 << "_" << index2;
  stringstream val_st;
  val_st << value;
  data[key_st.str()]=val_st.str();
}

void settingsmap::set_bool(string key, int index1, int index2, bool value)
{
  stringstream key_st;
  key_st << key << "_" << index1 << "_" << index2;
  stringstream val_st;
  val_st << value;
  data[key_st.str()]=val_st.str();
}


string settingsmap::get_string(string key)
{
  return data[key];
}

double settingsmap::get_double(string key)
{
  stringstream val_st;
  val_st << data[key];
  double value;
  val_st >> value;
  return value;
}

int settingsmap::get_int(string key)
{
  stringstream val_st;
  val_st << data[key];
  int value;
  val_st >> value;
  return value;
}

bool settingsmap::get_bool(string key)
{
  stringstream val_st;
  val_st << data[key];
  bool value;
  val_st >> value;
  return value;
}


string settingsmap::get_string(string key, int index)
{
  stringstream key_st;
  key_st << key << "_" << index;
  return data[key_st.str()];
}

double settingsmap::get_double(string key, int index)
{
  stringstream key_st;
  key_st << key << "_" << index;
  stringstream val_st;
  val_st << data[key_st.str()];
  double value;
  val_st >> value;
  return value;
}

int settingsmap::get_int(string key, int index)
{
  stringstream key_st;
  key_st << key << "_" << index;
  stringstream val_st;
  val_st << data[key_st.str()];
  int value;
  val_st >> value;
  return value;
}

bool settingsmap::get_bool(string key, int index)
{
  stringstream key_st;
  key_st << key << "_" << index;
  stringstream val_st;
  val_st << data[key_st.str()];
  bool value;
  val_st >> value;
  return value;
}


string settingsmap::get_string(string key, int index1, int index2)
{
  stringstream key_st;
  key_st << key << "_" << index1 << "_" << index2;
  return data[key_st.str()];
}

double settingsmap::get_double(string key, int index1, int index2)
{
  stringstream key_st;
  key_st << key << "_" << index1 << "_" << index2;
  stringstream val_st;
  val_st << data[key_st.str()];
  double value;
  val_st >> value;
  return value;
}

int settingsmap::get_int(string key, int index1, int index2)
{
  stringstream key_st;
  key_st << key << "_" << index1 << "_" << index2;
  stringstream val_st;
  val_st << data[key_st.str()];
  int value;
  val_st >> value;
  return value;
}

bool settingsmap::get_bool(string key, int index1, int index2)
{
  stringstream key_st;
  key_st << key << "_" << index1 << "_" << index2;
  stringstream val_st;
  val_st << data[key_st.str()];
  bool value;
  val_st >> value;
  return value;
}


bool settingsmap::load_file(string filename)
{
  ifstream input(filename.c_str());
  if(!input)
  {
    return false;
  }
  data.clear();
  while(!input.eof())
  {
    string key;
    safeGetline(input, key);
    string value;
    safeGetline(input, value);
    if(!key.empty())
    {
      data[key]=value;
    }
  }
  input.close();
  return true;
}

bool settingsmap::save_file(string filename)
{
  ofstream output(filename.c_str());
  if(!output)
  {
    return false;
  }
  for(map< string, string >::iterator it=data.begin(); it!=data.end(); ++it)
  {
    output << it->first << endl;
    output << it->second << endl;
  }
  output.close();
  return true;
}
