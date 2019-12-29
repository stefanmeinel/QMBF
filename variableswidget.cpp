#include "variableswidget.h"

VariablesWidget::VariablesWidget(int n_variables, int max_n_variables, QWidget* parent)
  : QWidget(parent)
{
  max_variables=max_n_variables;
  n_vars=n_variables;
  start_n_vars=n_vars;
  createGadgets();
}

void VariablesWidget::set_user_def(bool user_def)
{
  n_variables_Lb->setEnabled(user_def);
  n_variables_SB->setEnabled(user_def);
  variablenames_tableView->setEnabled(user_def);
}

void VariablesWidget::set_n_variables(int n_variables)
{
  n_variables_SB->setValue(n_variables);
}

int VariablesWidget::get_n_variables()
{
  return n_vars;
}

void VariablesWidget::set_variable_name(int v, std::string name)
{
  if(v>n_vars-1)
  {
    set_n_variables(v+1);
  }
  QModelIndex index = variablenames_model->index(v, 0, QModelIndex());
  variablenames_model->setData(index, QVariant(QString(name.c_str())), Qt::EditRole);
}

std::string VariablesWidget::get_variable_name(int v)
{
  if(v>n_vars-1)
  {
    return std::string("");
  }
  QModelIndex index = variablenames_model->index(v, 0, QModelIndex());
  QString text=(variablenames_model->data(index, Qt::EditRole)).toString();
  return text.toStdString();
}

void VariablesWidget::set_fit_min(int v, std::string value)
{
  if(v>n_vars-1)
  {
    set_n_variables(v+1);
  }
  QModelIndex index = fit_min_max_model->index(v, 0, QModelIndex());
  fit_min_max_model->setData(index, QVariant(QString(value.c_str())), Qt::EditRole);
}

std::string VariablesWidget::get_fit_min(int v)
{
  if(v>n_vars-1)
  {
    return "0.0";
  }
  QModelIndex index = fit_min_max_model->index(v, 0, QModelIndex());
  QString text=(fit_min_max_model->data(index, Qt::EditRole)).toString();
  return text.toStdString();
}

void VariablesWidget::set_fit_max(int v, std::string value)
{
  if(v>n_vars-1)
  {
    set_n_variables(v+1);
  }
  QModelIndex index = fit_min_max_model->index(v, 1, QModelIndex());
  fit_min_max_model->setData(index, QVariant(QString(value.c_str())), Qt::EditRole);
}

std::string VariablesWidget::get_fit_max(int v)
{
  if(v>n_vars-1)
  {
    return "0.0";
  }
  QModelIndex index = fit_min_max_model->index(v, 1, QModelIndex());
  QString text=(fit_min_max_model->data(index, Qt::EditRole)).toString();
  return text.toStdString();
}

void VariablesWidget::reset()
{
  n_variables_SB->setValue(start_n_vars);

  for(int row = 0; row < max_variables; ++row)
  {
    QModelIndex index = variablenames_model->index(row, 0, QModelIndex());
    variablenames_model->setData(index, QVariant(QString("")), Qt::EditRole);
    index = fit_min_max_model->index(row, 0, QModelIndex());
    fit_min_max_model->setData(index, QVariant(QString("")), Qt::EditRole);
    index = fit_min_max_model->index(row, 1, QModelIndex());
    fit_min_max_model->setData(index, QVariant(QString("")), Qt::EditRole);
  }
}

void VariablesWidget::modified_slot()
{
  variablenames_tableView->clearSelection();
  fit_min_max_tableView->clearSelection();
  emit modified();
}

void VariablesWidget::updateSizes()
{
  int scrollBarWidth=0;
  if(fit_min_max_tableView->verticalScrollBar()->isVisible())
  {
    scrollBarWidth=fit_min_max_tableView->verticalScrollBar()->width();
  }
  variablenames_tableView->setColumnWidth(0, variablenames_tableView->width()-variablenames_tableView->verticalHeader()->width()-table_padding);
  fit_min_max_tableView->setColumnWidth(0, (fit_min_max_tableView->width()-scrollBarWidth-table_padding)/2);
  fit_min_max_tableView->setColumnWidth(1, (fit_min_max_tableView->width()-scrollBarWidth-table_padding)/2+(fit_min_max_tableView->width()-scrollBarWidth-table_padding)%2);
}


void VariablesWidget::resizeEvent(QResizeEvent* event)
{
  updateSizes();
  QWidget::resizeEvent(event);
}


void VariablesWidget::n_variables_modified_slot(int new_n_variables)
{
  if(new_n_variables>n_vars)
  {
    for(int v=n_vars; v<=new_n_variables-1; ++v)
    {
      variablenames_tableView->setRowHidden(v, false);
      fit_min_max_tableView->setRowHidden(v, false);
    }
  }
  else if(new_n_variables<n_vars)
  {
    for(int v=n_vars-1; v>new_n_variables-1; --v)
    {
      variablenames_tableView->setRowHidden(v, true);
      fit_min_max_tableView->setRowHidden(v, true);
    }
  }
  n_vars=new_n_variables;
  emit n_variables_modified(new_n_variables);
  updateSizes();
}

void VariablesWidget::variables_changed_slot(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
  QModelIndex index;
  for(int row=topLeft.row(); row<=bottomRight.row(); ++row)
  {
    index=variablenames_model->index(row, 0, QModelIndex());
    QString new_name=(variablenames_model->data(index, Qt::EditRole)).toString();
    emit variable_changed(row, new_name.toStdString());
  }
}

void VariablesWidget::ranges_changed_slot()
{
  emit range_changed();
}


void VariablesWidget::scrollbar_moved(int newpos)
{
  (variablenames_tableView->verticalScrollBar())->setSliderPosition(newpos);
  (fit_min_max_tableView->verticalScrollBar())->setSliderPosition(newpos);
}


#ifdef Q_WS_MACX
void VariablesWidget::variablenames_key_pressed(QKeyEvent* key_event)
{
// fix up/down navigation for Mac OS X
  if(key_event->key()==Qt::Key_Down)
  {
    int row=(variablenames_tableView->currentIndex()).row();
    int column=(variablenames_tableView->currentIndex()).column();
    if( row < n_vars-1 )
    {
      variablenames_tableView->setCurrentIndex(variablenames_model->index(row+1, column, QModelIndex()));
    }
  }
  if(key_event->key()==Qt::Key_Up)
  {
    int row=(variablenames_tableView->currentIndex()).row();
    int column=(variablenames_tableView->currentIndex()).column();
    if( row > 0 )
    {
      variablenames_tableView->setCurrentIndex(variablenames_model->index(row-1, column, QModelIndex()));
    }
  }
}
#endif

#ifdef Q_WS_MACX
void VariablesWidget::fit_min_max_key_pressed(QKeyEvent* key_event)
{
// fix up/down navigation for Mac OS X
  if(key_event->key()==Qt::Key_Down)
  {
    int row=(fit_min_max_tableView->currentIndex()).row();
    int column=(fit_min_max_tableView->currentIndex()).column();
    if( row < n_vars-1 )
    {
      fit_min_max_tableView->setCurrentIndex(fit_min_max_model->index(row+1, column, QModelIndex()));
    }
  }
  if(key_event->key()==Qt::Key_Up)
  {
    int row=(fit_min_max_tableView->currentIndex()).row();
    int column=(fit_min_max_tableView->currentIndex()).column();
    if( row > 0 )
    {
      fit_min_max_tableView->setCurrentIndex(fit_min_max_model->index(row-1, column, QModelIndex()));
    }
  }
}
#endif


void VariablesWidget::createGadgets()
{
  vlayout=new QVBoxLayout;

  hlayout=new QHBoxLayout;

  n_variables_SB=new QSpinBox();
  n_variables_SB->setMinimumWidth(60);
  n_variables_SB->setMinimum(1);
  n_variables_SB->setValue(n_vars);
  n_variables_SB->setMaximum(max_variables);
  connect(n_variables_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));
  connect(n_variables_SB, SIGNAL(valueChanged(int)), this, SLOT(n_variables_modified_slot(int)));
  n_variables_Lb=new QLabel("Number of variables:");

  hlayout->addWidget(n_variables_Lb, Qt::AlignLeft);
  hlayout->addWidget(n_variables_SB, 1, Qt::AlignRight);

  gridlayout=new QGridLayout;
  gridlayout->setSpacing(0);

  variablenames_tableView=new QTableView();
  variablenames_tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  variablenames_model=new QStandardItemModel(max_variables, 1);
  variablenames_model->setHeaderData(0, Qt::Horizontal, "Name");
  variablenames_tableView->setModel(variablenames_model);
  variablenames_delegate = new LineEditDelegate(false, false);
  variablenames_tableView->setItemDelegate(variablenames_delegate);
  for(int row = 0; row < max_variables; ++row)
  {
    QModelIndex index = variablenames_model->index(row, 0, QModelIndex());
    variablenames_model->setData(index, QVariant(QString("")), Qt::EditRole);
  }
  variablenames_tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  for(int v=n_vars; v<max_variables; ++v)
  {
    variablenames_tableView->setRowHidden(v, true);
  }
  connect(variablenames_tableView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollbar_moved(int)));
  connect(variablenames_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(modified_slot()));
  connect(variablenames_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(variables_changed_slot(const QModelIndex&, const QModelIndex&)));

#ifdef Q_WS_MACX
  connect(variablenames_delegate, SIGNAL(key_pressed(QKeyEvent*)), this, SLOT(variablenames_key_pressed(QKeyEvent*)));
#endif

  fit_min_max_tableView=new QTableView();
  fit_min_max_tableView->verticalHeader()->hide();
  fit_min_max_model=new QStandardItemModel(max_variables, 2);
  fit_min_max_model->setHeaderData(0, Qt::Horizontal, "Fitting min");
  fit_min_max_model->setHeaderData(1, Qt::Horizontal, "Fitting max");
  fit_min_max_tableView->setModel(fit_min_max_model);
  fit_min_max_delegate = new LineEditDelegate(false, false);
  fit_min_max_tableView->setItemDelegate(fit_min_max_delegate);
  for(int row = 0; row < max_variables; ++row)
  {
    QModelIndex index = fit_min_max_model->index(row, 0, QModelIndex());
    fit_min_max_model->setData(index, QVariant(QString("")), Qt::EditRole);
    index = fit_min_max_model->index(row, 1, QModelIndex());
    fit_min_max_model->setData(index, QVariant(QString("")), Qt::EditRole);
  }
  fit_min_max_tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  for(int v=n_vars; v<max_variables; ++v)
  {
    fit_min_max_tableView->setRowHidden(v, true);
  }
  connect(fit_min_max_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(modified_slot()));
  connect(fit_min_max_tableView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollbar_moved(int)));
  connect(fit_min_max_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(ranges_changed_slot()));

#ifdef Q_WS_MACX
  connect(fit_min_max_delegate, SIGNAL(key_pressed(QKeyEvent*)), this, SLOT(fit_min_max_key_pressed(QKeyEvent*)));
#endif

  gridlayout->addWidget(variablenames_tableView, 0, 0);
  gridlayout->addWidget(fit_min_max_tableView, 0, 1);
  gridlayout->setColumnStretch(0, 10);
  gridlayout->setColumnStretch(1, 20);

  vlayout->addLayout(hlayout);
  vlayout->addLayout(gridlayout);

  setLayout(vlayout);
}
