#include "chisqrextratermwidget.h"

ChiSqrExtraTermWidget::ChiSqrExtraTermWidget(int n_constants, int max_n_constants, QWidget* parent)
  : QWidget(parent)
{
  max_constants=max_n_constants;
  n_consts=n_constants;
  createGadgets();
  set_user_def(false);
}


void ChiSqrExtraTermWidget::set_enabled(bool enabled)
{
  enabled_cb->setChecked(enabled);
}

bool ChiSqrExtraTermWidget::get_enabled()
{
  return enabled_cb->isChecked();
}


void ChiSqrExtraTermWidget::set_user_def(bool user_def)
{
  n_constants_Lb->setEnabled(user_def);
  n_constants_SB->setEnabled(user_def);
  constantnames_tableView->setEnabled(user_def);
  constantvalues_tableView->setEnabled(user_def);
  numdiffEd->setEnabled(user_def);
  numdiffLb->setEnabled(user_def);
  funcEd->setEnabled(user_def);
  funcLb->setEnabled(user_def);
}

bool ChiSqrExtraTermWidget::set_n_constants(int n_constants)
{
  if(n_constants>max_constants)
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Error: too many constants"));
    return false;
  }
  n_constants_SB->setValue(n_constants);
  return true;
}

int ChiSqrExtraTermWidget::get_n_constants()
{
  return n_consts;
}

void ChiSqrExtraTermWidget::set_constant_name(int c, std::string name)
{
  if(c>n_consts-1)
  {
    set_n_constants(c+1);
  }
  QModelIndex index = constantnames_model->index(c, 0, QModelIndex());
  constantnames_model->setData(index, QVariant(QString(name.c_str())), Qt::EditRole);
}

std::string ChiSqrExtraTermWidget::get_constant_name(int c)
{
  if(c>n_consts-1)
  {
    return std::string("");
  }
  QModelIndex index = constantnames_model->index(c, 0, QModelIndex());
  QString text=(constantnames_model->data(index, Qt::EditRole)).toString();
  return text.toStdString();
}

void ChiSqrExtraTermWidget::set_constant_value(int c, double value)
{
  if(c>n_consts-1)
  {
    set_n_constants(c+1);
  }
  QModelIndex index = constantvalues_model->index(c, 0, QModelIndex());
  constantvalues_model->setData(index, QVariant(double_to_QString(value)), Qt::EditRole);
}

double ChiSqrExtraTermWidget::get_constant_value(int c)
{
  if(c>n_consts-1)
  {
    return 0.0;
  }
  QModelIndex index = constantvalues_model->index(c, 0, QModelIndex());
  return QString_to_double((constantvalues_model->data(index, Qt::EditRole)).toString());
}



void ChiSqrExtraTermWidget::set_num_diff_step(double num_diff_step)
{
  numdiffEd->setText(double_to_QString(num_diff_step));
}


double ChiSqrExtraTermWidget::get_num_diff_step()
{
  return QString_to_double(numdiffEd->text());
}


void ChiSqrExtraTermWidget::set_function(std::string func)
{
  funcEd->setText(QString(func.c_str()));
}


std::string ChiSqrExtraTermWidget::get_function()
{
  return (funcEd->text()).toStdString();
}




void ChiSqrExtraTermWidget::reset()
{
  numdiffEd->setText(double_to_QString(1e-08));
  funcEd->setText("");
  n_constants_SB->setValue(0);
  for(int row = 0; row < max_constants; ++row)
  {
    QModelIndex index = constantnames_model->index(row, 0, QModelIndex());
    constantnames_model->setData(index, QVariant(QString("")), Qt::EditRole);
  }
  enabled_cb->setChecked(Qt::Unchecked);
//  set_user_def(false);
}

void ChiSqrExtraTermWidget::modified_slot()
{
  constantnames_tableView->clearSelection();
  constantvalues_tableView->clearSelection();
  emit modified();
}

void ChiSqrExtraTermWidget::function_modified_slot()
{
  emit function_modified();
}


void ChiSqrExtraTermWidget::enabled_modified_slot(int newState)
{
  if(newState == Qt::Checked)
  {
    set_user_def(true);
  }
  else if(newState == Qt::Unchecked)
  {
    set_user_def(false);
  }
  emit enabled_modified();
}


void ChiSqrExtraTermWidget::constants_changed_slot(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
  QModelIndex index;
  for(int row=topLeft.row(); row<=bottomRight.row(); ++row)
  {
    index=constantnames_model->index(row, 0, QModelIndex());
    QString new_name=(constantnames_model->data(index, Qt::EditRole)).toString();
    emit constant_changed(row, new_name.toStdString());
  }
}

void ChiSqrExtraTermWidget::updateSizes()
{
  int scrollBarWidth=0;
  if(constantvalues_tableView->verticalScrollBar()->isVisible())
  {
    scrollBarWidth=constantvalues_tableView->verticalScrollBar()->width();
  }
  constantnames_tableView->setColumnWidth(0, constantnames_tableView->width()-constantnames_tableView->verticalHeader()->width()-table_padding);
  constantvalues_tableView->setColumnWidth(0, constantvalues_tableView->width()-scrollBarWidth-table_padding);
}

void ChiSqrExtraTermWidget::resizeEvent(QResizeEvent* event)
{
  updateSizes();
  QWidget::resizeEvent(event);
}


void ChiSqrExtraTermWidget::n_constants_modified_slot(int new_n_constants)
{
  if(new_n_constants>n_consts)
  {
    for(int c=n_consts; c<=new_n_constants-1; ++c)
    {
      constantnames_tableView->setRowHidden(c, false);
      constantvalues_tableView->setRowHidden(c, false);
    }
  }
  else if(new_n_constants<n_consts)
  {
    for(int c=n_consts-1; c>new_n_constants-1; --c)
    {
      constantnames_tableView->setRowHidden(c, true);
      constantvalues_tableView->setRowHidden(c, true);
    }
  }
  n_consts=new_n_constants;
  emit n_constants_modified(new_n_constants);
  updateSizes();
}

void ChiSqrExtraTermWidget::scrollbar_moved(int newpos)
{
  (constantnames_tableView->verticalScrollBar())->setSliderPosition(newpos);
  (constantvalues_tableView->verticalScrollBar())->setSliderPosition(newpos);
}


#ifdef Q_WS_MACX
void ChiSqrExtraTermWidget::constantnames_key_pressed(QKeyEvent* key_event)
{
// fix up/down navigation for Mac OS X
  if(key_event->key()==Qt::Key_Down)
  {
    int row=(constantnames_tableView->currentIndex()).row();
    int column=(constantnames_tableView->currentIndex()).column();
    if( row < n_consts-1 )
    {
      constantnames_tableView->setCurrentIndex(constantnames_model->index(row+1, column, QModelIndex()));
    }
  }
  if(key_event->key()==Qt::Key_Up)
  {
    int row=(constantnames_tableView->currentIndex()).row();
    int column=(constantnames_tableView->currentIndex()).column();
    if( row > 0 )
    {
      constantnames_tableView->setCurrentIndex(constantnames_model->index(row-1, column, QModelIndex()));
    }
  }
}
#endif


#ifdef Q_WS_MACX
void ChiSqrExtraTermWidget::constantvalues_key_pressed(QKeyEvent* key_event)
{
// fix up/down navigation for Mac OS X
  if(key_event->key()==Qt::Key_Down)
  {
    int row=(constantvalues_tableView->currentIndex()).row();
    int column=(constantvalues_tableView->currentIndex()).column();
    if( row < n_consts-1 )
    {
      constantvalues_tableView->setCurrentIndex(constantvalues_model->index(row+1, column, QModelIndex()));
    }
  }
  if(key_event->key()==Qt::Key_Up)
  {
    int row=(constantvalues_tableView->currentIndex()).row();
    int column=(constantvalues_tableView->currentIndex()).column();
    if( row > 0 )
    {
      constantvalues_tableView->setCurrentIndex(constantvalues_model->index(row-1, column, QModelIndex()));
    }
  }
}
#endif


void ChiSqrExtraTermWidget::createGadgets()
{
  vlayout=new QVBoxLayout;

  gridlayout=new QGridLayout;

  enabled_cb=new QCheckBox(tr("Enable additional term in chi^2"), this);
  connect(enabled_cb, SIGNAL(stateChanged(int)), this, SLOT(modified_slot()));
  connect(enabled_cb, SIGNAL(stateChanged(int)), this, SLOT(enabled_modified_slot(int)));

  spacer1=new QSpacerItem(10,10);

  gridlayout->addItem(spacer1, 0, 0);
  gridlayout->addWidget(enabled_cb, 1, 0);

  funcEd=new QLineEdit();
  funcLb=new QLabel("Function:");
  connect(funcEd, SIGNAL(textChanged(const QString&)), this, SLOT(modified_slot()));
  connect(funcEd, SIGNAL(textChanged(const QString&)), this, SLOT(function_modified_slot()));

  gridlayout->addWidget(funcLb, 2, 0);
  gridlayout->addWidget(funcEd, 2, 1);

  numdiffEd=new QLineEdit();
  numdiffEd->setValidator(new QDoubleValidator(1E-99, 1E99, 15, numdiffEd));
  numdiffLb=new QLabel("Numerical differentiation step size:");
  connect(numdiffEd, SIGNAL(textChanged(const QString&)), this, SLOT(modified_slot()));

  gridlayout->addWidget(numdiffLb, 3, 0);
  gridlayout->addWidget(numdiffEd, 3, 1);


  hlayout1=new QHBoxLayout;

  n_constants_SB=new QSpinBox();
  n_constants_SB->setMinimumWidth(60);
  n_constants_SB->setMinimum(0);
  n_constants_SB->setValue(0);
  n_constants_SB->setMaximum(max_constants);
  connect(n_constants_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));
  connect(n_constants_SB, SIGNAL(valueChanged(int)), this, SLOT(n_constants_modified_slot(int)));
  n_constants_Lb=new QLabel("Number of constants in above function:");

  hlayout1->addWidget(n_constants_Lb, Qt::AlignLeft);
  hlayout1->addWidget(n_constants_SB, 1, Qt::AlignRight);

  hlayout2=new QHBoxLayout;
  hlayout2->setSpacing(0);

  constantnames_tableView=new QTableView();
  constantnames_tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  constantnames_model=new QStandardItemModel(max_constants, 1);
  constantnames_model->setHeaderData(0, Qt::Horizontal, "Constant name");
  constantnames_tableView->setModel(constantnames_model);
  constantnames_delegate = new LineEditDelegate(false, false);
  constantnames_tableView->setItemDelegate(constantnames_delegate);
  for(int row = 0; row < max_constants; ++row)
  {
    QModelIndex index = constantnames_model->index(row, 0, QModelIndex());
    constantnames_model->setData(index, QVariant(QString("")), Qt::EditRole);
  }
  constantnames_tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  for(int c=n_consts; c<max_constants; ++c)
  {
    constantnames_tableView->setRowHidden(c, true);
  }
  connect(constantnames_tableView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollbar_moved(int)));
  connect(constantnames_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(modified_slot()));
  connect(constantnames_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(constants_changed_slot(const QModelIndex&, const QModelIndex&)));

#ifdef Q_WS_MACX
  connect(constantnames_delegate, SIGNAL(key_pressed(QKeyEvent*)), this, SLOT(constantnames_key_pressed(QKeyEvent*)));
#endif

  constantvalues_tableView=new QTableView();
  constantvalues_tableView->verticalHeader()->hide();
  constantvalues_model=new QStandardItemModel(max_constants, 1);
  constantvalues_model->setHeaderData(0, Qt::Horizontal, "Value");
  constantvalues_tableView->setModel(constantvalues_model);
  constantvalues_delegate = new LineEditDelegate(false, true);
  constantvalues_tableView->setItemDelegate(constantvalues_delegate);
  for(int row = 0; row < max_constants; ++row)
  {
    QModelIndex index = constantvalues_model->index(row, 0, QModelIndex());
    constantvalues_model->setData(index, QVariant(QString("")), Qt::EditRole);
  }
  constantvalues_tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  for(int c=n_consts; c<max_constants; ++c)
  {
    constantvalues_tableView->setRowHidden(c, true);
  }
  connect(constantvalues_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(modified_slot()));
  connect(constantvalues_tableView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollbar_moved(int)));

#ifdef Q_WS_MACX
  connect(constantvalues_delegate, SIGNAL(key_pressed(QKeyEvent*)), this, SLOT(constantvalues_key_pressed(QKeyEvent*)));
#endif

  hlayout2->addWidget(constantnames_tableView);
  hlayout2->addWidget(constantvalues_tableView);

  vlayout->addLayout(gridlayout);
  vlayout->addLayout(hlayout1);
  vlayout->addLayout(hlayout2);

  setLayout(vlayout);
}
