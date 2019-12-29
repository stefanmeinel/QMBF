#ifndef SETTINGSMAP_H
#define SETTINGSMAP_H

#include <string>
#include <sstream>
#include <fstream>
#include <map>

using namespace std;

const int d_prec=15;

class settingsmap
{

  public:

    bool exists(string key);
    bool exists(string key, int index);
    bool exists(string key, int index1, int index2);

    void set_string(string key, string value);
    void set_double(string key, double value);
    void set_int(string key, int value);
    void set_bool(string key, bool value);

    void set_string(string key, int index, string value);
    void set_double(string key, int index, double value);
    void set_int(string key, int index, int value);
    void set_bool(string key, int index, bool value);

    void set_string(string key, int index1, int index2, string value);
    void set_double(string key, int index1, int index2, double value);
    void set_int(string key, int index1, int index2, int value);
    void set_bool(string key, int index1, int index2, bool value);

    string get_string(string key);
    double get_double(string key);
    int get_int(string key);
    bool get_bool(string key);

    string get_string(string key, int index);
    double get_double(string key, int index);
    int get_int(string key, int index);
    bool get_bool(string key, int index);

    string get_string(string key, int index1, int index2);
    double get_double(string key, int index1, int index2);
    int get_int(string key, int index1, int index2);
    bool get_bool(string key, int index1, int index2);

    bool load_file(string filename);

    bool save_file(string filename);

  private:

    std::istream& safeGetline(std::istream& is, std::string& t);

    map< string, string > data;

};

#endif
