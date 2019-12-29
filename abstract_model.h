#ifndef ABSTRACT_MODEL_H
#define ABSTRACT_MODEL_H

#include <vector>
#include <string>

class abstract_model
{
  public:

    virtual ~abstract_model() { }

    virtual int get_n_functions() = 0;
    virtual int get_n_variables() = 0;
    virtual int get_n_parameters() = 0;
    virtual int get_n_constants() = 0;

    virtual void set_parameters(const std::vector< double >& parameters) = 0;
    virtual void set_constants(const std::vector< double >& constants) = 0;

    virtual double eval_function(int function, const std::vector< double >& arguments) = 0;
    virtual double eval_derivative(int function, int parameter, const std::vector< double >& arguments) = 0;

    virtual std::string get_function_name(int function) = 0;
    virtual std::string get_parameter_name(int parameter) = 0;
    virtual std::string get_variable_name(int variable) = 0;
    virtual std::string get_constant_name(int constant) = 0;

    virtual int no_of_errors() = 0;
};

#endif
