#include "mainwidget.h"

MainWidget::MainWidget(bool bayesian, int n_parameters, int max_n_parameters, QWidget* parent)
{
  bayes=bayesian;
  n_parms=n_parameters;
  start_bayes=bayesian;
  start_n_parms=n_parameters;
  max_parameters=max_n_parameters;
  createGadgets();
}

void MainWidget::updateSizes()
{
  param_tableView->setColumnWidth(0, param_tableView->width()-param_tableView->verticalHeader()->width()-table_padding);
  for(int column=0; column<3; ++column)
  {
    if(column<2)
    {
      values_tableView->setColumnWidth(column, (values_tableView->width()-table_padding)/3);
      results_tableView->setColumnWidth(column, (results_tableView->width()-results_tableView->verticalScrollBar()->width()-table_padding)/3);
    }
    else
    {
      values_tableView->setColumnWidth(column, (values_tableView->width()-table_padding)/3+(values_tableView->width()-table_padding)%3);
      results_tableView->setColumnWidth(column, (results_tableView->width()-results_tableView->verticalScrollBar()->width()-table_padding)/3+(results_tableView->width()-results_tableView->verticalScrollBar()->width()-table_padding)%3-2);
    }
  }
}


void MainWidget::resizeEvent(QResizeEvent* event)
{
  updateSizes();
  QWidget::resizeEvent(event);
}

void MainWidget::scrollbar_moved(int newpos)
{
  (param_tableView->verticalScrollBar())->setSliderPosition(newpos);
  (values_tableView->verticalScrollBar())->setSliderPosition(newpos);
  (results_tableView->verticalScrollBar())->setSliderPosition(newpos);
}

void MainWidget::set_bayesian(bool bayesian)
{
  bayes=bayesian;
  if(bayes)
  {
    values_tableView->setColumnHidden(1, false);
    values_tableView->setColumnHidden(2, false);
  }
  else
  {
    values_tableView->setColumnHidden(1, true);
    values_tableView->setColumnHidden(2, true);
  }
  updateSizes();
}

void MainWidget::set_n_parameters(int new_n_parameters)
{
  if(new_n_parameters>n_parms)
  {
    for(int p=n_parms; p<=new_n_parameters-1; ++p)
    {
      param_tableView->setRowHidden(p, false);
      values_tableView->setRowHidden(p, false);
      results_tableView->setRowHidden(p, false);
    }
  }
  else if(new_n_parameters<n_parms)
  {
    for(int p=n_parms-1; p>new_n_parameters-1; --p)
    {
      param_tableView->setRowHidden(p, true);
      values_tableView->setRowHidden(p, true);
      results_tableView->setRowHidden(p, true);
    }
  }
  n_parms=new_n_parameters;
  updateSizes();
}

void MainWidget::set_parameter(int p, std::string name)
{
  QModelIndex index=param_model->index(p, 0, QModelIndex());
  param_model->setData(index, QVariant(QString(name.c_str())), Qt::EditRole);
}

std::string MainWidget::get_data_file()
{
  return (fileEd->text()).toStdString();
}

void MainWidget::set_data_file(std::string name)
{
  fileEd->setText(QString(name.c_str()));
}

int MainWidget::get_data_file_type()
{
  return fileTypeCb->currentIndex();
}

void MainWidget::set_data_file_type(int index)
{
  fileTypeCb->setCurrentIndex(index);
}

std::string MainWidget::get_output_dir()
{
  return (outputdirEd->text()).toStdString();
}

void MainWidget::set_output_dir(std::string name)
{
  outputdirEd->setText(QString(name.c_str()));
}

double MainWidget::get_start_value(int p)
{
  QModelIndex index=values_model->index(p, 0, QModelIndex());
  return QString_to_double((values_model->data(index, Qt::EditRole)).toString());
}

void MainWidget::set_start_value(int p, double value)
{
  QModelIndex index=values_model->index(p, 0, QModelIndex());
  values_model->setData(index, QVariant(double_to_QString(value)), Qt::EditRole);
}

double MainWidget::get_prior(int p)
{
  QModelIndex index=values_model->index(p, 1, QModelIndex());
  return QString_to_double((values_model->data(index, Qt::EditRole)).toString());
}

void MainWidget::set_prior(int p, double value)
{
  QModelIndex index=values_model->index(p, 1, QModelIndex());
  values_model->setData(index, QVariant(double_to_QString(value)), Qt::EditRole);
}

double MainWidget::get_sigma(int p)
{
  QModelIndex index=values_model->index(p, 2, QModelIndex());
  return QString_to_double((values_model->data(index, Qt::EditRole)).toString());
}

void MainWidget::set_sigma(int p, double value)
{
  QModelIndex index=values_model->index(p, 2, QModelIndex());
  values_model->setData(index, QVariant(double_to_QString(value)), Qt::EditRole);
}

void MainWidget::set_result(int p, double value)
{
  QModelIndex index=results_model->index(p, 0, QModelIndex());
  results_model->setData(index, QVariant(double_to_QString(value)), Qt::EditRole);
}

double MainWidget::get_result(int p)
{
  QModelIndex index=results_model->index(p, 0, QModelIndex());
  return QString_to_double((results_model->data(index, Qt::EditRole)).toString());
}


void MainWidget::set_sqrt_cov(int p, double value)
{
  QModelIndex index=results_model->index(p, 1, QModelIndex());
  results_model->setData(index, QVariant(double_to_QString(value)), Qt::EditRole);
}

double MainWidget::get_sqrt_cov(int p)
{
  QModelIndex index=results_model->index(p, 1, QModelIndex());
  return QString_to_double((results_model->data(index, Qt::EditRole)).toString());
}

void MainWidget::set_BS_sigma(int p, double value)
{
  QModelIndex index=results_model->index(p, 2, QModelIndex());
  results_model->setData(index, QVariant(double_to_QString(value)), Qt::EditRole);
}


double MainWidget::get_BS_sigma(int p)
{
  QModelIndex index=results_model->index(p, 2, QModelIndex());
  return QString_to_double((results_model->data(index, Qt::EditRole)).toString());
}


void MainWidget::print(std::string text)
{
  terminal->append(QString(text.c_str())); 
  terminal->moveCursor(QTextCursor::End);
}

void MainWidget::printm(QString text)
{
  terminal->append(text); 
  terminal->moveCursor(QTextCursor::End);
}

void MainWidget::reset()
{
  fileEd->setText(QString(""));
  fileTypeCb->setCurrentIndex(0);
  outputdirEd->setText(QString(""));
  set_bayesian(start_bayes);
  set_n_parameters(start_n_parms);
  for(int row = 0; row < max_parameters; ++row)
  {
    QModelIndex index = param_model->index(row, 0, QModelIndex());
    param_model->setData(index, QVariant(QString("")), Qt::EditRole);

    for(int col=0; col<3; ++col)
    {
      index = values_model->index(row, col, QModelIndex());
      values_model->setData(index, QVariant(QString("")), Qt::EditRole);
      index = results_model->index(row, col, QModelIndex());
      results_model->setData(index, QVariant(QString("")), Qt::EditRole);
    }
  }
  terminal->clear();
  old_start.clear();
  old_prior.clear();
  old_sigma.clear();
}

void MainWidget::reset_tables()
{
  set_n_parameters(start_n_parms);
  for(int row = 0; row < max_parameters; ++row)
  {
    QModelIndex index = param_model->index(row, 0, QModelIndex());
    param_model->setData(index, QVariant(QString("")), Qt::EditRole);

    for(int col=0; col<3; ++col)
    {
      index = values_model->index(row, col, QModelIndex());
      values_model->setData(index, QVariant(QString("")), Qt::EditRole);
      index = results_model->index(row, col, QModelIndex());
      results_model->setData(index, QVariant(QString("")), Qt::EditRole);
    }
  }
}

void MainWidget::save_tables()
{
  QModelIndex index;
  for(int p=0; p<n_parms; ++p)
  {
    index=param_model->index(p, 0, QModelIndex());
    string name=(param_model->data(index, Qt::EditRole)).toString().toStdString();
    if(!name.empty())
    {
      if(get_start_value(p)!=empty_double)
      {
        old_start[name]=get_start_value(p);
      }
      if(get_prior(p)!=empty_double)
      {
        old_prior[name]=get_prior(p);
      }
      if(get_sigma(p)!=empty_double)
      {
        old_sigma[name]=get_sigma(p);
      }
    }
  }
}


void MainWidget::restore_tables()
{
  QModelIndex index;
  for(int p=0; p<n_parms; ++p)
  {
    index=param_model->index(p, 0, QModelIndex());
    string name=(param_model->data(index, Qt::EditRole)).toString().toStdString();
    map< string, double >::iterator iter;

    iter=old_start.find(name);
    if(iter!=old_start.end())
    {
      set_start_value(p, iter->second);
    }

    iter=old_prior.find(name);
    if(iter!=old_prior.end())
    {
      set_prior(p, iter->second);
    }

    iter=old_sigma.find(name);
    if(iter!=old_sigma.end())
    {
      set_sigma(p, iter->second);
    }
  }
}


void MainWidget::clear_results()
{
  for(int row = 0; row < max_parameters; ++row)
  {
    for(int col=0; col<3; ++col)
    {
      QModelIndex index = results_model->index(row, col, QModelIndex());
      results_model->setData(index, QVariant(QString("")), Qt::EditRole);
    }
  }
}


void MainWidget::modified_slot()
{
  param_tableView->clearSelection();
  values_tableView->clearSelection();
  results_tableView->clearSelection();
  emit modified();
}

void MainWidget::data_file_modified_slot()
{
  emit data_file_modified();
}


void MainWidget::select_datafile()
{
  QString fileName=QFileDialog::getOpenFileName(this, tr("Select data file"),
                                                 current_dir,
                                                 tr("Data files (*)"));
  if(!fileName.isEmpty())
  {
     fileEd->setText(fileName);
     current_dir=QFileInfo(fileName).absolutePath();
  }
}


void MainWidget::select_outputdir()
{
  QString dirName=QFileDialog::getExistingDirectory(this, tr("Select output directory"),  current_output_dir, QFileDialog::ShowDirsOnly );
  if(!dirName.isEmpty())
  {
     outputdirEd->setText(dirName);
     current_output_dir=dirName;
  }
}


void MainWidget::fit()
{
  emit start_fit();
}


void MainWidget::bootstrap()
{
  emit start_BS();
}


#ifdef Q_WS_MACX
void MainWidget::values_key_pressed(QKeyEvent* key_event)
{
// fix up/down navigation for Mac OS X
  if(key_event->key()==Qt::Key_Down)
  {
    int row=(values_tableView->currentIndex()).row();
    int column=(values_tableView->currentIndex()).column();
    if( row < n_parms-1 )
    {
      values_tableView->setCurrentIndex(values_model->index(row+1, column, QModelIndex()));
    }
  }
  if(key_event->key()==Qt::Key_Up)
  {
    int row=(values_tableView->currentIndex()).row();
    int column=(values_tableView->currentIndex()).column();
    if( row > 0 )
    {
      values_tableView->setCurrentIndex(values_model->index(row-1, column, QModelIndex()));
    }
  }
}
#endif

#ifdef Q_WS_MACX
void MainWidget::param_key_pressed(QKeyEvent* key_event)
{
// fix up/down navigation for Mac OS X
  if(key_event->key()==Qt::Key_Down)
  {
    int row=(param_tableView->currentIndex()).row();
    int column=(param_tableView->currentIndex()).column();
    if( row < n_parms-1 )
    {
      param_tableView->setCurrentIndex(param_model->index(row+1, column, QModelIndex()));
    }
  }
  if(key_event->key()==Qt::Key_Up)
  {
    int row=(param_tableView->currentIndex()).row();
    int column=(param_tableView->currentIndex()).column();
    if( row > 0 )
    {
      param_tableView->setCurrentIndex(param_model->index(row-1, column, QModelIndex()));
    }
  }
}
#endif

#ifdef Q_WS_MACX
void MainWidget::results_key_pressed(QKeyEvent* key_event)
{
// fix up/down navigation for Mac OS X
  if(key_event->key()==Qt::Key_Down)
  {
    int row=(results_tableView->currentIndex()).row();
    int column=(results_tableView->currentIndex()).column();
    if( row < n_parms-1 )
    {
      results_tableView->setCurrentIndex(results_model->index(row+1, column, QModelIndex()));
    }
  }
  if(key_event->key()==Qt::Key_Up)
  {
    int row=(results_tableView->currentIndex()).row();
    int column=(results_tableView->currentIndex()).column();
    if( row > 0 )
    {
      results_tableView->setCurrentIndex(results_model->index(row-1, column, QModelIndex()));
    }
  }
}
#endif

void MainWidget::createGadgets()
{
  vlayout=new QVBoxLayout();

  gridlayout1=new QGridLayout();

  fileTypeLb=new QLabel("Data file type:");
  fileTypeCb=new QComboBox;
  fileTypeCb->addItem("Text");
  fileTypeCb->addItem("Binary 32 bit (litte-endian)");
  fileTypeCb->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  connect(fileTypeCb, SIGNAL(currentIndexChanged(int)), this, SLOT(modified_slot()));
  connect(fileTypeCb, SIGNAL(currentIndexChanged(int)), this, SLOT(data_file_modified_slot()));

  gridlayout1->addWidget(fileTypeLb, 0, 0);
  gridlayout1->addWidget(fileTypeCb, 0, 1);


  fileLb=new QLabel("Data file:");
  fileEd=new QLineEdit();
  connect(fileEd, SIGNAL(textChanged(const QString&)), this, SLOT(modified_slot()));
  connect(fileEd, SIGNAL(textChanged(const QString&)), this, SLOT(data_file_modified_slot()));

  datafilebutton=new QToolButton();
  datafilebutton->setIcon(QPixmap(":/images/fileopen.png"));
  connect(datafilebutton, SIGNAL(clicked()), this, SLOT(select_datafile()));

  spacer1=new QSpacerItem(60,20);

  fitbutton=new QPushButton("Start fit");
  connect(fitbutton, SIGNAL(clicked()), this, SLOT(fit()));

  gridlayout1->addWidget(fileLb, 1, 0);
  gridlayout1->addWidget(fileEd, 1, 1);
  gridlayout1->addWidget(datafilebutton, 1, 2);
  gridlayout1->addItem(spacer1, 1, 3);
  gridlayout1->addWidget(fitbutton, 1, 4);

  outputdirLb=new QLabel("Output directory:");
  outputdirEd=new QLineEdit();
  connect(outputdirEd, SIGNAL(textChanged(const QString&)), this, SLOT(modified_slot()));

  outputdirbutton=new QToolButton();
  outputdirbutton->setIcon(QPixmap(":/images/fileopen.png"));
  connect(outputdirbutton, SIGNAL(clicked()), this, SLOT(select_outputdir()));

  spacer2=new QSpacerItem(60,20);

  bootbutton=new QPushButton("Start bootstrap");
  connect(bootbutton, SIGNAL(clicked()), this, SLOT(bootstrap()));

  gridlayout1->addWidget(outputdirLb, 2, 0);
  gridlayout1->addWidget(outputdirEd, 2, 1);
  gridlayout1->addWidget(outputdirbutton, 2, 2);
  gridlayout1->addItem(spacer2, 2, 3);
  gridlayout1->addWidget(bootbutton, 2, 4);

  gridlayout2=new QGridLayout();
  gridlayout2->setSpacing(0);

  param_tableView=new QTableView();
  param_tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  param_model=new QStandardItemModel(max_parameters, 1); 
  param_model->setHeaderData(0, Qt::Horizontal, "Parameter");
  param_tableView->setModel(param_model);
  param_delegate = new LineEditDelegate(true, false);
  param_tableView->setItemDelegate(param_delegate);
  param_tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  connect(param_tableView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollbar_moved(int)));
  connect(param_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(modified_slot()));

#ifdef Q_WS_MACX
  connect(param_delegate, SIGNAL(key_pressed(QKeyEvent*)), this, SLOT(param_key_pressed(QKeyEvent*)));
#endif

  values_tableView=new QTableView();
  values_tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  values_model=new QStandardItemModel(max_parameters, 3);
  values_model->setHeaderData(0, Qt::Horizontal, "Start value");
  values_model->setHeaderData(1, Qt::Horizontal, "Prior value");
  values_model->setHeaderData(2, Qt::Horizontal, "Prior sigma");
  values_tableView->setModel(values_model);
  values_delegate=new LineEditDelegate(false, false);
  values_tableView->setItemDelegate(values_delegate);
  values_tableView->verticalHeader()->hide();
  values_tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  connect(values_tableView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollbar_moved(int)));
  connect(values_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(modified_slot()));

#ifdef Q_WS_MACX
  connect(values_delegate, SIGNAL(key_pressed(QKeyEvent*)), this, SLOT(values_key_pressed(QKeyEvent*)));
#endif

  results_tableView=new QTableView();
  results_model=new QStandardItemModel(max_parameters, 3);
  results_model->setHeaderData(0, Qt::Horizontal, "Fit result");
  results_model->setHeaderData(1, Qt::Horizontal, "sqrt(cov)");
  results_model->setHeaderData(2, Qt::Horizontal, "BS 68% error");
  results_tableView->setModel(results_model);
  results_delegate=new LineEditDelegate(true, true);
  results_tableView->setItemDelegate(results_delegate);
  results_tableView->verticalHeader()->hide();
  results_tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  results_tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  connect(results_tableView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollbar_moved(int)));
//  connect(results_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(modified_slot()));

#ifdef Q_WS_MACX
  connect(results_delegate, SIGNAL(key_pressed(QKeyEvent*)), this, SLOT(results_key_pressed(QKeyEvent*)));
#endif

  for(int r=n_parms; r<max_parameters; ++r)
  {
    param_tableView->setRowHidden(r, true);
    values_tableView->setRowHidden(r, true);
    results_tableView->setRowHidden(r, true);
  }

  gridlayout2->addWidget(param_tableView, 0, 0);
  gridlayout2->addWidget(values_tableView, 0, 1);
  gridlayout2->addWidget(results_tableView, 0, 2);

  gridlayout2->setColumnStretch(0, 12);
  gridlayout2->setColumnStretch(1, 30);
  gridlayout2->setColumnStretch(2, 30);


  terminal=new QTextEdit();
  terminal->setReadOnly(true);
  terminal->setLineWrapMode(QTextEdit::NoWrap);
  terminal->setMaximumHeight(200);

  vlayout->addLayout(gridlayout1);
  vlayout->addLayout(gridlayout2);
  vlayout->addWidget(terminal);

  setLayout(vlayout);
}
