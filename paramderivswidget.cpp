#include "paramderivswidget.h"

ParamDerivsWidget::ParamDerivsWidget(bool num_diff, int n_functions, int max_n_functions, int n_parameters, int max_n_parameters, QWidget* parent)
  : QWidget(parent)
{
  n_funcs=n_functions;
  max_functions=max_n_functions;
  n_parms=n_parameters;
  max_parameters=max_n_parameters;
  start_n_parms=n_parms;
  start_n_funcs=n_funcs;
  userdef=true;

  createGadgets();

  set_num_diff(num_diff);
}


void ParamDerivsWidget::set_user_def(bool user_def)
{
  userdef=user_def;
  n_parameters_Lb->setEnabled(user_def);
  n_parameters_SB->setEnabled(user_def);
  parder_tableView->setEnabled(user_def);
  for(int f=0; f<n_funcs; ++f)
  {
    parder_tableView->setColumnHidden(1+f, !(userdef && (!numdiff)));
  }
  for(int f=n_funcs; f<max_functions; ++f)
  {
    parder_tableView->setColumnHidden(1+f, true);
  }
}


void ParamDerivsWidget::set_num_diff(bool num_diff)
{
  numdiff=num_diff;
  for(int f=0; f<n_funcs; ++f)
  {
    parder_tableView->setColumnHidden(1+f, !(userdef && (!numdiff)));
  }
  for(int f=n_funcs; f<max_functions; ++f)
  {
    parder_tableView->setColumnHidden(1+f, true);
  }
}



bool ParamDerivsWidget::set_n_parameters(int n_parameters)
{
  if(n_parameters!=n_parms)
  {
    if(n_parameters>max_parameters)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: too many parameters"));
      return false;
    }
    n_parameters_SB->setValue(n_parameters);
  }
  return true;
}

int ParamDerivsWidget::get_n_parameters()
{
  return n_parms;
}

void ParamDerivsWidget::set_n_parameters_dof(int p)
{
  n_parameters_dof_SB->setValue(p);
}

int ParamDerivsWidget::get_n_parameters_dof()
{
  return n_parameters_dof_SB->value();
}


void ParamDerivsWidget::set_n_functions(int n_functions)
{
  if(n_functions!=n_funcs)
  {
    n_functions_modified_slot(n_functions);
  }
}

int ParamDerivsWidget::get_n_functions()
{
  return n_funcs;
}

void ParamDerivsWidget::set_parameter(int p, std::string name)
{
  if(p>n_parms-1)
  {
    set_n_parameters(p+1);
  }
  QModelIndex index = parder_model->index(p, 0, QModelIndex());
  parder_model->setData(index, QVariant(QString(name.c_str())), Qt::EditRole);
}

std::string ParamDerivsWidget::get_parameter(int p)
{
  if(p>n_parms-1)
  {
    return std::string("");
  }
  QModelIndex index = parder_model->index(p, 0, QModelIndex());
  QString text=(parder_model->data(index, Qt::EditRole)).toString();
  return text.toStdString();
}

void ParamDerivsWidget::set_derivative(int f, int p, std::string name)
{
  if(p>n_parms-1)
  {
    set_n_parameters(p+1);
  }
  if(f>n_funcs-1)
  {
    set_n_functions(f+1);
  }
  QModelIndex index = parder_model->index(p, 1+f, QModelIndex());
  parder_model->setData(index, QVariant(QString(name.c_str())), Qt::EditRole);
}

std::string ParamDerivsWidget::get_derivative(int f, int p)
{
  if( (p>n_parms-1) || (f>n_funcs-1) )
  {
    return std::string("");
  }
  QModelIndex index = parder_model->index(p, 1+f, QModelIndex());
  QString text=(parder_model->data(index, Qt::EditRole)).toString();
  return text.toStdString();
}

void ParamDerivsWidget::reset()
{
  n_parameters_SB->setValue(start_n_parms);
//  n_parameters_dof_SB->setValue(0);
  n_functions_modified_slot(start_n_funcs);
  for(int row=0; row < max_parameters; ++row)
  {
    for(int column=0; column < max_functions+1; ++column)
    {
      QModelIndex index = parder_model->index(row, column, QModelIndex());
      parder_model->setData(index, QVariant(QString("")), Qt::EditRole);
    }
  }
}

void ParamDerivsWidget::modified_slot()
{
  parder_tableView->clearSelection();
  emit modified();
}

void ParamDerivsWidget::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);
}

void ParamDerivsWidget::n_parameters_modified_slot(int new_n_parameters)
{
  if(new_n_parameters>n_parms)
  {
    for(int p=n_parms; p<=new_n_parameters-1; ++p)
    {
      parder_tableView->setRowHidden(p, false);
    }
  }
  else if(new_n_parameters<n_parms)
  {
    for(int p=n_parms-1; p>new_n_parameters-1; --p)
    {
      parder_tableView->setRowHidden(p, true);
    }
  }
  n_parms=new_n_parameters;
  emit n_parameters_modified(new_n_parameters);
}

void ParamDerivsWidget::n_functions_modified_slot(int new_n_functions)
{
  if(userdef && (!numdiff))
  {
    if(new_n_functions>n_funcs)
    {
      for(int f=n_funcs; f<=new_n_functions-1; ++f)
      {
        parder_tableView->setColumnHidden(f+1, false);
      }
    }
    else if(new_n_functions<n_funcs)
    {
      for(int f=n_funcs-1; f>new_n_functions-1; --f)
      {
        parder_tableView->setColumnHidden(f+1, true);
      }
    }
  }
  n_funcs=new_n_functions;
  emit n_functions_modified(new_n_functions);
}


void ParamDerivsWidget::parameters_changed_slot(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
  QModelIndex index;
  for(int row=topLeft.row(); row<=bottomRight.row(); ++row)
  {
    for(int col=topLeft.column(); col<=bottomRight.column(); ++col)
    {
      if(col==0)
      {
        index=parder_model->index(row, col, QModelIndex());
        QString new_name=(parder_model->data(index, Qt::EditRole)).toString();
        emit parameter_changed(row, new_name.toStdString());
      }
    }
  }
}


void ParamDerivsWidget::derivatives_changed_slot(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
  QModelIndex index;
  for(int row=topLeft.row(); row<=bottomRight.row(); ++row)
  {
    for(int col=topLeft.column(); col<=bottomRight.column(); ++col)
    {
      if(col>0)
      {
        index=parder_model->index(row, col, QModelIndex());
        QString new_name=(parder_model->data(index, Qt::EditRole)).toString();
        emit derivative_changed(col-1, row, new_name.toStdString());
      }
    }
  }
}


#ifdef Q_WS_MACX
void ParamDerivsWidget::parder_key_pressed(QKeyEvent* key_event)
{
// fix up/down navigation for Mac OS X
  if(key_event->key()==Qt::Key_Down)
  {
    int row=(parder_tableView->currentIndex()).row();
    int column=(parder_tableView->currentIndex()).column();
    if( row < n_parms-1 )
    {
      parder_tableView->setCurrentIndex(parder_model->index(row+1, column, QModelIndex()));
    }
  }
  if(key_event->key()==Qt::Key_Up)
  {
    int row=(parder_tableView->currentIndex()).row();
    int column=(parder_tableView->currentIndex()).column();
    if( row > 0 )
    {
      parder_tableView->setCurrentIndex(parder_model->index(row-1, column, QModelIndex()));
    }
  }
}
#endif


void ParamDerivsWidget::createGadgets()
{
  vlayout=new QVBoxLayout;

  hlayout=new QHBoxLayout;
  hlayout2=new QHBoxLayout;

  n_parameters_SB=new QSpinBox();
  n_parameters_SB->setMinimumWidth(60);
  n_parameters_SB->setMinimum(1);
  n_parameters_SB->setValue(n_parms);
  n_parameters_SB->setMaximum(max_parameters);
  connect(n_parameters_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));
  connect(n_parameters_SB, SIGNAL(valueChanged(int)), this, SLOT(n_parameters_modified_slot(int)));
  n_parameters_Lb=new QLabel("Number of parameters:");

  n_parameters_dof_SB=new QSpinBox();
  n_parameters_dof_SB->setMinimumWidth(60);
  n_parameters_dof_SB->setMinimum(0);
  n_parameters_dof_SB->setValue(1);
  n_parameters_dof_SB->setMaximum(max_parameters);
  connect(n_parameters_dof_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));
  n_parameters_dof_Lb=new QLabel("Number of parameters to be subtracted from d.o.f.:");

  hlayout->addWidget(n_parameters_Lb, Qt::AlignLeft);
  hlayout->addWidget(n_parameters_SB, 1, Qt::AlignRight);

  hlayout2->addWidget(n_parameters_dof_Lb, Qt::AlignLeft);
  hlayout2->addWidget(n_parameters_dof_SB, 1, Qt::AlignRight);

  parder_tableView=new QTableView();
  parder_model=new QStandardItemModel(max_parameters, 1+max_functions);
  parder_model->setHeaderData(0, Qt::Horizontal, "Parameter name");
  for(int f=0; f<max_functions; ++f)
  {
    std::stringstream sst;
    sst << "Derivative of function " << f+1;
    parder_model->setHeaderData(1+f, Qt::Horizontal, sst.str().c_str());
  }
  parder_tableView->setModel(parder_model);
  parder_delegate = new LineEditDelegate(false, false);
  parder_tableView->setItemDelegate(parder_delegate);
  for(int row=0; row < max_parameters; ++row)
  {
    for(int column=0; column < max_functions+1; ++column)
    {
      QModelIndex index = parder_model->index(row, column, QModelIndex());
      parder_model->setData(index, QVariant(QString("")), Qt::EditRole);
    }
  }
  parder_tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  for(int p=n_parms; p<max_parameters; ++p)
  {
    parder_tableView->setRowHidden(p, true);
  }
  parder_tableView->setColumnWidth(0, 140);
  for(int f=0; f<max_functions; ++f)
  {
    parder_tableView->setColumnWidth(1+f, 240);
  }
  for(int f=n_funcs; f<max_functions; ++f)
  {
    parder_tableView->setColumnHidden(1+f, true);
  }
  connect(parder_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(modified_slot()));
  connect(parder_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(parameters_changed_slot(const QModelIndex&, const QModelIndex&)));
  connect(parder_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(derivatives_changed_slot(const QModelIndex&, const QModelIndex&)));

#ifdef Q_WS_MACX
  connect(parder_delegate, SIGNAL(key_pressed(QKeyEvent*)), this, SLOT(parder_key_pressed(QKeyEvent*)));
#endif

  vlayout->addLayout(hlayout);
  vlayout->addLayout(hlayout2);
  vlayout->addWidget(parder_tableView);

  setLayout(vlayout);
}
