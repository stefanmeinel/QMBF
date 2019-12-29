bool MainWindow::set_plot_data()
{
  int n_functions=file_data[0][0].size();
  int n_data_points=file_data[0].size();
  int n_data_sets=file_data.size();

  int start_n_data;
  int stop_n_data;
  int fit_n_data_sets;

  if(fitsetWidget->get_restrict_data())
  {
    start_n_data=fitsetWidget->get_data_start();
    stop_n_data=fitsetWidget->get_data_stop();

    fit_n_data_sets=stop_n_data-start_n_data+1;

    if (stop_n_data>n_data_sets)
    {
      QMessageBox::warning(this, tr("QMBF"),
                   tr("Range of data sets exceeds data file"));
      return false;
    }

    if (fit_n_data_sets<10)
    {
      QMessageBox::warning(this, tr("QMBF"),
                   tr("Range of data sets too small"));
      return false;
    }
  }
  else
  {
    start_n_data=1;
    stop_n_data=n_data_sets;
    fit_n_data_sets=n_data_sets;
  }

  int bin_size=fitsetWidget->get_bin();
  if(fit_n_data_sets/bin_size<10)
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Error: Bin size too large"));
    return false;
  }
  vector< double > plotting_data_temp_1(n_functions, 0.0);
  vector< vector< double > > plotting_data_temp_2(n_data_points, plotting_data_temp_1);

  plotting_data.clear();
  plotting_data.resize(fit_n_data_sets/bin_size, plotting_data_temp_2);

  for(int m=0; m<n_data_points; ++m)
  {
    for(int f=0; f<n_functions; ++f)
    {
      for(int b=0; b<fit_n_data_sets/bin_size; ++b)
      {
        // average over entries e in bin b
        plotting_data[b][m][f]=0.0;
        for(int e=0; e<bin_size; ++e)
        {
          plotting_data[b][m][f]+=file_data[start_n_data-1+b*bin_size+e][m][f];
        }
        plotting_data[b][m][f]/=bin_size;
      }
    }
  }
  return true;
}




void MainWindow::plot_data()
{
  string dir=mainWidget->get_output_dir();
  if(dir.empty())
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("No output directory selected"));
    return;
  }
  if(*(dir.end()-1)!='/')
  {
    dir.append("/");
  }
  string filename;
  string simple_filename;
  string svg_filename;
  if(curFile.isEmpty())
  {
    simple_filename="untitled.agr";
    filename=dir+simple_filename;
    svg_filename=dir+"untitled.svg";
  }
  else
  {
    simple_filename=strippedName(curFile).toStdString()+".agr";
    filename=dir+simple_filename;
    svg_filename=dir+strippedName(curFile).toStdString()+".svg";
  }
  if(need_to_load_data_file)
  {
    if(!load_data_file())
    {
      return;
    }
  }
  if(!set_plot_data())
  {
    return;
  }
  int n_variables=file_arguments[0].size();
  int plot_var=plot_set_dialog->get_plot_var();

  for(int v=0; v<n_variables; ++v)
  {
    if(v!=plot_var)
    {
      if(plot_set_dialog->get_variable_value(v)==empty_double)
      {
        QMessageBox::warning(this, tr("QMBF"),
                                   tr("Enter fixed values for other variables in plot settings dialog first"));
        return;
      }
    }
  }

  double sign=1.0;
  if(plot_set_dialog->get_neg())
  {
    sign=-1.0;
  }

  int n_functions=plotting_data[0][0].size();
  int n_data_points=plotting_data[0].size();
  int n_data_sets=plotting_data.size();

  vector< double > zero_temp(n_functions, 0.0);
  vector< vector< double > > average(n_data_points, zero_temp);

  for(int n=0; n<n_data_sets; ++n)
  {
    for(int m=0; m<n_data_points; ++m)
    {
      for(int f=0; f<n_functions; ++f)
      {
        average[m][f]+=plotting_data[n][m][f];
      }
    }
  }
  for(int m=0; m<n_data_points; ++m)
  {
    for(int f=0; f<n_functions; ++f)
    {
      average[m][f]/=n_data_sets;
    }
  }
  vector< vector< double > > sigma(n_data_points, zero_temp);
  double normalization;
  if(fitsetWidget->get_bootstrap_normalization())
  {
    normalization=double(n_data_sets-1);
  }
  else
  {
    normalization=double(n_data_sets)*double(n_data_sets-1);
  }
  for(int m=0; m<n_data_points; ++m)
  {
    for(int f=0; f<n_functions; ++f)
    {
      double temp=0.0;
      for(int n=0; n<n_data_sets; ++n)
      {
        temp+=(plotting_data[n][m][f]-average[m][f])*(plotting_data[n][m][f]-average[m][f]);
      }
      sigma[m][f]=sqrt(temp/normalization);
    }
  }
  double plot_x_min;
  double plot_x_max;
  if(plot_set_dialog->data_range())
  {
    plot_x_min=file_arguments[0][plot_var];
    plot_x_max=file_arguments[n_data_points-1][plot_var];
    for(int m=0; m<n_data_points; ++m)
    {
      if(file_arguments[m][plot_var]>plot_x_max)
      {
        plot_x_max=file_arguments[m][plot_var];
      }
      if(file_arguments[m][plot_var]<plot_x_min)
      {
        plot_x_min=file_arguments[m][plot_var];
      }
    }
  }
  else
  {
    plot_x_min=plot_set_dialog->get_min();
    plot_x_max=plot_set_dialog->get_max();
  }

  ofstream output(filename.c_str());
  if(!output)
  {
    QMessageBox::warning(this, tr("QMBF"),
                         tr("Cannot write file %1:\n")
                         .arg(QString(filename.c_str())));
    return;
  }

  output.precision(15);
  bool log_axis=plot_set_dialog->get_log();

  if(!plot_set_dialog->data_range_y())
  {
    output << "@version 50100" << endl;
    output << "@    world " << plot_x_min << ", " << plot_set_dialog->get_min_y() << ", " << plot_x_max << ", " << plot_set_dialog->get_max_y() << endl;
    output << "@    xaxis  tick on" << endl;
    double majortick=(plot_x_max-plot_x_min)/10000000.0;  // this will enable autoticks because it is too small
    output << "@    xaxis  tick major " << majortick << endl;
    if(!plot_set_dialog->get_log())
    {
      output << "@    yaxis  tick on" << endl;
      double majorticky=(plot_set_dialog->get_max_y()-plot_set_dialog->get_min_y())/10000000.0;  // this will enable autoticks because it is too small
      output << "@    yaxis  tick major " << majorticky << endl;
    }
  }

  output << "@    xaxes scale Normal" << endl;
  if(log_axis)
  {
    output << "@    yaxes scale Logarithmic" << endl;
  }
  else
  {
    output << "@    yaxes scale Normal" << endl;
  }
  for(int f=0; f<n_functions; ++f)
  {
    output << "@    s" << f << " line type 0" << endl;
    output << "@    s" << f << " symbol 2" << endl;
    output << "@    s" << f << " symbol size 0.30000" << endl;
    output << "@    s" << f << " symbol color " << f+1 << endl;
    output << "@    s" << f << " symbol pattern 1" << endl;
    output << "@    s" << f << " symbol fill color " << f+1 << endl;
    output << "@    s" << f << " symbol fill pattern 1" << endl;
  }
  for(int f=0; f<n_functions; ++f)
  {
    output << "@target G0.S" << f << endl;
    output << "@type xydy" << endl;
    for(int m=0; m<n_data_points; ++m)
    {
      if( (file_arguments[m][plot_var]>=plot_x_min) && (file_arguments[m][plot_var]<=plot_x_max) )
      {
        bool plotpoint=true;
        for(int v=0; v<n_variables; ++v)
        {
          if(v!=plot_var)
          {
            if(abs(file_arguments[m][v]-plot_set_dialog->get_variable_value(v))>0.0000000001)
            {
              plotpoint=false;
            }
          }
        }
        if(plotpoint)
        {
          output << file_arguments[m][plot_var] << " " << sign*average[m][f] << " " << sigma[m][f] << endl;
        }
      }
    }
    output << "&" << endl;
  }
  output.close();

  QApplication::setOverrideCursor(Qt::WaitCursor);

  QString program = (plot_set_dialog->get_grace()).c_str();
  QStringList arguments;

  if(plot_set_dialog->get_viewplot()==1)
  {
    arguments << simple_filename.c_str();

    QProcess *grace_process = new QProcess(this);
    grace_process->setWorkingDirectory(QString(dir.c_str()));
    grace_process->start(program, arguments);

  }
  else
  {

    arguments << "-hdevice" << "SVG" << "-hardcopy" << simple_filename.c_str();

    QProcess *grace_process = new QProcess(this);
    grace_process->setWorkingDirectory(QString(dir.c_str()));
    grace_process->start(program, arguments);

    grace_process->waitForFinished();
    if(grace_process->exitCode()!=0)
    {
      QApplication::restoreOverrideCursor();
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Grace Error"));
      return;
    }
    PlotView* pv=new PlotView(QString(svg_filename.c_str()));
    pv->show();

  }

  QApplication::restoreOverrideCursor();
}

void MainWindow::plot_all()
{
  string dir=mainWidget->get_output_dir();
  if(dir.empty())
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("No output directory selected"));
    return;
  }
  if(*(dir.end()-1)!='/')
  {
    dir.append("/");
  }
  string filename;
  string simple_filename;
  string svg_filename;
  if(curFile.isEmpty())
  {
    simple_filename="untitled.agr";
    filename=dir+simple_filename;
    svg_filename=dir+"untitled.svg";
  }
  else
  {
    simple_filename=strippedName(curFile).toStdString()+".agr";
    filename=dir+simple_filename;
    svg_filename=dir+strippedName(curFile).toStdString()+".svg";
  }
  if(need_to_fit)
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Fit data first"));
    return;
  }
  if(!set_plot_data())
  {
    return;
  }
  int n_variables=file_arguments[0].size();
  int plot_var=plot_set_dialog->get_plot_var();
  for(int v=0; v<n_variables; ++v)
  {
    if(v!=plot_var)
    {
      if(plot_set_dialog->get_variable_value(v)==empty_double)
      {
        QMessageBox::warning(this, tr("QMBF"),
                                   tr("Enter fixed values for other variables in plot settings dialog first"));
        return;
      }
    }
  }

  double sign=1.0;
  if(plot_set_dialog->get_neg())
  {
    sign=-1.0;
  }

  int n_functions=plotting_data[0][0].size();
  int n_data_points=plotting_data[0].size();
  int n_data_sets=plotting_data.size();


  vector< double > zero_temp(n_functions, 0.0);
  vector< vector< double > > average(n_data_points, zero_temp);

  for(int n=0; n<n_data_sets; ++n)
  {
    for(int m=0; m<n_data_points; ++m)
    {
      for(int f=0; f<n_functions; ++f)
      {
        average[m][f]+=plotting_data[n][m][f];
      }
    }
  }
  for(int m=0; m<n_data_points; ++m)
  {
    for(int f=0; f<n_functions; ++f)
    {
      average[m][f]/=n_data_sets;
    }
  }
  vector< vector< double > > sigma(n_data_points, zero_temp);
  double normalization;
  if(fitsetWidget->get_bootstrap_normalization())
  {
    normalization=double(n_data_sets-1);
  }
  else
  {
    normalization=double(n_data_sets)*double(n_data_sets-1);
  }
  for(int m=0; m<n_data_points; ++m)
  {
    for(int f=0; f<n_functions; ++f)
    {
      double temp=0.0;
      for(int n=0; n<n_data_sets; ++n)
      {
        temp+=(plotting_data[n][m][f]-average[m][f])*(plotting_data[n][m][f]-average[m][f]);
      }
      sigma[m][f]=sqrt(temp/normalization);
    }
  }
  double plot_x_min;
  double plot_x_max;
  if(plot_set_dialog->data_range())
  {
    plot_x_min=file_arguments[0][plot_var];
    plot_x_max=file_arguments[n_data_points-1][plot_var];
    for(int m=0; m<n_data_points; ++m)
    {
      if(file_arguments[m][plot_var]>plot_x_max)
      {
        plot_x_max=file_arguments[m][plot_var];
      }
      if(file_arguments[m][plot_var]<plot_x_min)
      {
        plot_x_min=file_arguments[m][plot_var];
      }
    }
  }
  else
  {
    plot_x_min=plot_set_dialog->get_min();
    plot_x_max=plot_set_dialog->get_max();
  }
  ofstream output(filename.c_str());
  if(!output)
  {
    QMessageBox::warning(this, tr("QMBF"),
                         tr("Cannot write file %1:\n")
                         .arg(QString(filename.c_str())));
    return;
  }
  output.precision(15);
  bool log_axis=plot_set_dialog->get_log();

  if(!plot_set_dialog->data_range_y())
  {
    output << "@version 50100" << endl;
    output << "@    world " << plot_x_min << ", " << plot_set_dialog->get_min_y() << ", " << plot_x_max << ", " << plot_set_dialog->get_max_y() << endl;
    output << "@    xaxis  tick on" << endl;
    double majortick=(plot_x_max-plot_x_min)/10000000.0;  // this will enable autoticks because it is too small
    output << "@    xaxis  tick major " << majortick << endl;
    if(!plot_set_dialog->get_log())
    {
      output << "@    yaxis  tick on" << endl;
      double majorticky=(plot_set_dialog->get_max_y()-plot_set_dialog->get_min_y())/10000000.0;  // this will enable autoticks because it is too small
      output << "@    yaxis  tick major " << majorticky << endl;
    }
  }

  output << "@    xaxes scale Normal" << endl;
  if(log_axis)
  {
    output << "@    yaxes scale Logarithmic" << endl;
  }
  else
  {
    output << "@    yaxes scale Normal" << endl;
  }
  for(int f=0; f<n_functions; ++f)
  {
    output << "@    s" << f << " line type 0" << endl;
    output << "@    s" << f << " symbol 2" << endl;
    output << "@    s" << f << " symbol size 0.30000" << endl;
    output << "@    s" << f << " symbol color " << f+1 << endl;
    output << "@    s" << f << " symbol pattern 1" << endl;
    output << "@    s" << f << " symbol fill color " << f+1 << endl;
    output << "@    s" << f << " symbol fill pattern 1" << endl;
    output << "@    s" << n_functions+f << " line color " << f+1 << endl;
  }

  for(int f=0; f<n_functions; ++f)
  {
    output << "@target G0.S" << f << endl;
    output << "@type xydy" << endl;
    for(int m=0; m<n_data_points; ++m)
    {
      if( (file_arguments[m][plot_var]>=plot_x_min) && (file_arguments[m][plot_var]<=plot_x_max) )
      {
        bool plotpoint=true;
        for(int v=0; v<n_variables; ++v)
        {
          if(v!=plot_var)
          {
            if(abs(file_arguments[m][v]-plot_set_dialog->get_variable_value(v))>0.0000000001)
            {
              plotpoint=false;
            }
          }
        }
        if(plotpoint)
        {
          output << file_arguments[m][plot_var] << " " << sign*average[m][f] << " " << sigma[m][f] << endl;
        }
      }
    }
    output << "&" << endl;
  }

  bool continuous=plot_set_dialog->continuous();
  int steps=1000;

  for(int f=0; f<n_functions; ++f)
  {
    output << "@target G0.S" << n_functions+f << endl;
    output << "@type xy" << endl;
    if(continuous)
    {
      vector< double > arg(n_variables);
      for(int v=0; v<n_variables; ++v)
      {
        if(v!=plot_var)
        {
          arg[v]=plot_set_dialog->get_variable_value(v);
        }
      }
      for(int step=0; step<steps; ++step)
      {
        arg[plot_var]=plot_x_min+double(step)/double(steps)*(plot_x_max-plot_x_min);
        output << arg[plot_var] << " " << sign*(_fitter->eval_function(f, arg)) << endl;
      }
    }
    else
    {
      for(int m=0; m<n_data_points; ++m)
      {
        if( (file_arguments[m][plot_var]>=plot_x_min) && (file_arguments[m][plot_var]<=plot_x_max) )
        {
          bool plotpoint=true;
          for(int v=0; v<n_variables; ++v)
          {
            if(v!=plot_var)
            {
              if(abs(file_arguments[m][v]-plot_set_dialog->get_variable_value(v))>0.0000000001)
              {
                plotpoint=false;
              }
            }
          }
          if(plotpoint)
          {
            output << file_arguments[m][plot_var] << " " << sign*(_fitter->eval_function(f, file_arguments[m])) << endl;
          }
        }
      }
    }
    output << "&" << endl;
  }

  output.close();

  QApplication::setOverrideCursor(Qt::WaitCursor);

  QString program = (plot_set_dialog->get_grace()).c_str();
  QStringList arguments;

  if(plot_set_dialog->get_viewplot()==1)
  {
    arguments << simple_filename.c_str();

    QProcess *grace_process = new QProcess(this);
    grace_process->setWorkingDirectory(QString(dir.c_str()));
    grace_process->start(program, arguments);

  }
  else
  {

    arguments << "-hdevice" << "SVG" << "-hardcopy" << simple_filename.c_str();

    QProcess *grace_process = new QProcess(this);
    grace_process->setWorkingDirectory(QString(dir.c_str()));
    grace_process->start(program, arguments);

    grace_process->waitForFinished();
    if(grace_process->exitCode()!=0)
    {
      QApplication::restoreOverrideCursor();
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Grace Error"));
      return;
    }
    PlotView* pv=new PlotView(QString(svg_filename.c_str()));
    pv->show();

  }

  QApplication::restoreOverrideCursor();
}




void MainWindow::plot_start()
{
  string dir=mainWidget->get_output_dir();
  if(dir.empty())
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("No output directory selected"));
    return;
  }
  if(*(dir.end()-1)!='/')
  {
    dir.append("/");
  }
  string filename;
  string simple_filename;
  string svg_filename;
  if(curFile.isEmpty())
  {
    simple_filename="untitled.agr";
    filename=dir+simple_filename;
    svg_filename=dir+"untitled.svg";
  }
  else
  {
    simple_filename=strippedName(curFile).toStdString()+".agr";
    filename=dir+simple_filename;
    svg_filename=dir+strippedName(curFile).toStdString()+".svg";
  }


  if(need_to_reset_fitter)
  {
    if(!reset_fitter())
    {
      return;
    }
  }

  vector< double > constants(constsWidget->get_n_constants(), 0.0);
  for(int c=0; c<constsWidget->get_n_constants(); ++c)
  {
    double val=constsWidget->get_constant_value(c);
    if(val==empty_double)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: constant value missing"));
      return;
    }
    constants[c]=val;
  }
  _model->set_constants(constants);

  for(int p=0; p<parderWidget->get_n_parameters(); ++p)
  {
    double val=mainWidget->get_start_value(p);
    if(val==empty_double)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: start value missing"));
      return;
    }
    _fitter->set_starting_value(p, val);
  }
  if(need_to_load_data_file)
  {
    if(!load_data_file())
    {
      return;
    }
  }
  if(!set_plot_data())
  {
    return;
  }


  int n_variables=file_arguments[0].size();
  int plot_var=plot_set_dialog->get_plot_var();
  for(int v=0; v<n_variables; ++v)
  {
    if(v!=plot_var)
    {
      if(plot_set_dialog->get_variable_value(v)==empty_double)
      {
        QMessageBox::warning(this, tr("QMBF"),
                                   tr("Enter fixed values for other variables in plot settings dialog first"));
        return;
      }
    }
  }

  double sign=1.0;
  if(plot_set_dialog->get_neg())
  {
    sign=-1.0;
  }

  int n_functions=plotting_data[0][0].size();
  int n_data_points=plotting_data[0].size();
  int n_data_sets=plotting_data.size();


  vector< double > zero_temp(n_functions, 0.0);
  vector< vector< double > > average(n_data_points, zero_temp);

  for(int n=0; n<n_data_sets; ++n)
  {
    for(int m=0; m<n_data_points; ++m)
    {
      for(int f=0; f<n_functions; ++f)
      {
        average[m][f]+=plotting_data[n][m][f];
      }
    }
  }
  for(int m=0; m<n_data_points; ++m)
  {
    for(int f=0; f<n_functions; ++f)
    {
      average[m][f]/=n_data_sets;
    }
  }
  vector< vector< double > > sigma(n_data_points, zero_temp);
  double normalization;
  if(fitsetWidget->get_bootstrap_normalization())
  {
    normalization=double(n_data_sets-1);
  }
  else
  {
    normalization=double(n_data_sets)*double(n_data_sets-1);
  }
  for(int m=0; m<n_data_points; ++m)
  {
    for(int f=0; f<n_functions; ++f)
    {
      double temp=0.0;
      for(int n=0; n<n_data_sets; ++n)
      {
        temp+=(plotting_data[n][m][f]-average[m][f])*(plotting_data[n][m][f]-average[m][f]);
      }
      sigma[m][f]=sqrt(temp/normalization);
    }
  }

  double plot_x_min;
  double plot_x_max;
  if(plot_set_dialog->data_range())
  {
    plot_x_min=file_arguments[0][plot_var];
    plot_x_max=file_arguments[n_data_points-1][plot_var];
    for(int m=0; m<n_data_points; ++m)
    {
      if(file_arguments[m][plot_var]>plot_x_max)
      {
        plot_x_max=file_arguments[m][plot_var];
      }
      if(file_arguments[m][plot_var]<plot_x_min)
      {
        plot_x_min=file_arguments[m][plot_var];
      }
    }
  }
  else
  {
    plot_x_min=plot_set_dialog->get_min();
    plot_x_max=plot_set_dialog->get_max();
  }
  ofstream output(filename.c_str());
  if(!output)
  {
    QMessageBox::warning(this, tr("QMBF"),
                         tr("Cannot write file %1:\n")
                         .arg(QString(filename.c_str())));
    return;
  }
  output.precision(15);
  bool log_axis=plot_set_dialog->get_log();

  if(!plot_set_dialog->data_range_y())
  {
    output << "@version 50100" << endl;
    output << "@    world " << plot_x_min << ", " << plot_set_dialog->get_min_y() << ", " << plot_x_max << ", " << plot_set_dialog->get_max_y() << endl;
    output << "@    xaxis  tick on" << endl;
    double majortick=(plot_x_max-plot_x_min)/10000000.0;  // this will enable autoticks because it is too small
    output << "@    xaxis  tick major " << majortick << endl;
    if(!plot_set_dialog->get_log())
    {
      output << "@    yaxis  tick on" << endl;
      double majorticky=(plot_set_dialog->get_max_y()-plot_set_dialog->get_min_y())/10000000.0;  // this will enable autoticks because it is too small
      output << "@    yaxis  tick major " << majorticky << endl;
    }
  }

  output << "@    xaxes scale Normal" << endl;
  if(log_axis)
  {
    output << "@    yaxes scale Logarithmic" << endl;
  }
  else
  {
    output << "@    yaxes scale Normal" << endl;
  }
  for(int f=0; f<n_functions; ++f)
  {
    output << "@    s" << f << " line type 0" << endl;
    output << "@    s" << f << " symbol 2" << endl;
    output << "@    s" << f << " symbol size 0.30000" << endl;
    output << "@    s" << f << " symbol color " << f+1 << endl;
    output << "@    s" << f << " symbol pattern 1" << endl;
    output << "@    s" << f << " symbol fill color " << f+1 << endl;
    output << "@    s" << f << " symbol fill pattern 1" << endl;
    output << "@    s" << n_functions+f << " line color " << f+1 << endl;
  }

  for(int f=0; f<n_functions; ++f)
  {
    output << "@target G0.S" << f << endl;
    output << "@type xydy" << endl;
    for(int m=0; m<n_data_points; ++m)
    {
      if( (file_arguments[m][plot_var]>=plot_x_min) && (file_arguments[m][plot_var]<=plot_x_max) )
      {
        bool plotpoint=true;
        for(int v=0; v<n_variables; ++v)
        {
          if(v!=plot_var)
          {
            if(abs(file_arguments[m][v]-plot_set_dialog->get_variable_value(v))>0.0000000001)
            {
              plotpoint=false;
            }
          }
        }
        if(plotpoint)
        {
          output << file_arguments[m][plot_var] << " " << sign*average[m][f] << " " << sigma[m][f] << endl;
        }
      }
    }
    output << "&" << endl;
  }

  bool continuous=plot_set_dialog->continuous();
  int steps=1000;

  for(int f=0; f<n_functions; ++f)
  {
    output << "@target G0.S" << n_functions+f << endl;
    output << "@type xy" << endl;
    if(continuous)
    {
      vector< double > arg(n_variables);
      for(int v=0; v<n_variables; ++v)
      {
        if(v!=plot_var)
        {
          arg[v]=plot_set_dialog->get_variable_value(v);
        }
      }
      for(int step=0; step<steps; ++step)
      {
        arg[plot_var]=plot_x_min+double(step)/double(steps)*(plot_x_max-plot_x_min);
        output << arg[plot_var] << " " << sign*(_fitter->eval_function_start(f, arg)) << endl;
      }
    }
    else
    {
      for(int m=0; m<n_data_points; ++m)
      {
        if( (file_arguments[m][plot_var]>=plot_x_min) && (file_arguments[m][plot_var]<=plot_x_max) )
        {
          bool plotpoint=true;
          for(int v=0; v<n_variables; ++v)
          {
            if(v!=plot_var)
            {
              if(abs(file_arguments[m][v]-plot_set_dialog->get_variable_value(v))>0.0000000001)
              {
                plotpoint=false;
              }
            }
          }
          if(plotpoint)
          {
            output << file_arguments[m][plot_var] << " " << sign*(_fitter->eval_function_start(f, file_arguments[m])) << endl;
          }
        }
      }
    }
    output << "&" << endl;
  }

  output.close();

  QApplication::setOverrideCursor(Qt::WaitCursor);

  QString program = (plot_set_dialog->get_grace()).c_str();
  QStringList arguments;

  if(plot_set_dialog->get_viewplot()==1)
  {
    arguments << simple_filename.c_str();

    QProcess *grace_process = new QProcess(this);
    grace_process->setWorkingDirectory(QString(dir.c_str()));
    grace_process->start(program, arguments);

  }
  else
  {

    arguments << "-hdevice" << "SVG" << "-hardcopy" << simple_filename.c_str();

    QProcess *grace_process = new QProcess(this);
    grace_process->setWorkingDirectory(QString(dir.c_str()));
    grace_process->start(program, arguments);

    grace_process->waitForFinished();
    if(grace_process->exitCode()!=0)
    {
      QApplication::restoreOverrideCursor();
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Grace Error"));
      return;
    }
    PlotView* pv=new PlotView(QString(svg_filename.c_str()));
    pv->show();

  }

  QApplication::restoreOverrideCursor();

}



void MainWindow::plot_eff_mass()
{
  int n_variables=varsWidget->get_n_variables();
  if(n_variables>1)
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Effective mass plotting is for 1 variable only"));
    return;
  }

  string dir=mainWidget->get_output_dir();
  if(dir.empty())
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("No output directory selected"));
    return;
  }
  if(*(dir.end()-1)!='/')
  {
    dir.append("/");
  }

  if(plot_set_dialog->get_eff_mass()=="")
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Please enter formula for effective mass parameter (menu: Plot -> Options)"));
    return;
  }

  if(need_to_fit)
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Fit data first"));
    return;
  }
  if(!set_plot_data())
  {
    return;
  }

  string filename;
  string simple_filename;
  string svg_filename;
  if(curFile.isEmpty())
  {
    simple_filename="untitled_em.agr";
    filename=dir+simple_filename;
    svg_filename=dir+"untitled_em.svg";
  }
  else
  {
    simple_filename=strippedName(curFile).toStdString()+"_em.agr";
    filename=dir+simple_filename;
    svg_filename=dir+strippedName(curFile).toStdString()+"_em.svg";
  }

  map<string, double> fit_results;

  for(int p=0; p<parderWidget->get_n_parameters(); ++p)
  {
    fit_results[parderWidget->get_parameter(p)]=mainWidget->get_result(p);
  }

  parser effective_mass_parser(plot_set_dialog->get_eff_mass());

  double E1=effective_mass_parser.parse(fit_results);

  if(effective_mass_parser.get_no_of_errors()!=0)
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Error parsing the formula for the effective mass parameter (see menu: Plot -> Options)"));
    return;
  }


  int n_functions=plotting_data[0][0].size();
  int n_data_points=plotting_data[0].size();
  int n_data_sets=plotting_data.size();


  QApplication::setOverrideCursor(Qt::WaitCursor);



  vector< double > zero_temp(n_functions, 0.0);
  vector< vector< double > > corr_average(n_data_points, zero_temp);

  for(int n=0; n<n_data_sets; ++n)
  {
    for(int m=0; m<n_data_points; ++m)
    {
      for(int f=0; f<n_functions; ++f)
      {
        corr_average[m][f]+=plotting_data[n][m][f];
      }
    }
  }
  for(int m=0; m<n_data_points; ++m)
  {
    for(int f=0; f<n_functions; ++f)
    {
      corr_average[m][f]/=n_data_sets;
    }
  }


  int bssamples=500;

  gsl_rng_default_seed=0;
  gsl_rng_env_setup();
  const gsl_rng_type* Trng=gsl_rng_mt19937;
  gsl_rng* rng=gsl_rng_alloc(Trng);

  vector< int > bootconfig_temp(n_data_sets, 0);
  vector< vector< int > > bootconfig(bssamples, bootconfig_temp);


  for(int boot=0; boot<bssamples; ++boot)
  {
    for(unsigned int b=0; b<n_data_sets; ++b)
    {
      bootconfig[boot][b]=static_cast<int>(gsl_rng_uniform(rng)*n_data_sets);
    }
  }

  gsl_rng_free(rng);


  vector< double > eff_mass_tmp_1(n_functions, 0.0);
  vector< vector< double > > eff_mass_tmp_2(n_data_points, eff_mass_tmp_1);
  vector< vector< vector< double > > > eff_mass(bssamples, eff_mass_tmp_2);

  vector< vector< double > > eff_mass_average(n_data_points, eff_mass_tmp_1);
  vector< vector< double > > eff_mass_sigma(n_data_points, eff_mass_tmp_1);







  for(int boot=0; boot<bssamples; ++boot)
  {
    vector< double > zero_temp(n_functions, 0.0);
    vector< vector< double > > average(n_data_points, zero_temp);

    for(int n=0; n<n_data_sets; ++n)
    {
      for(int m=0; m<n_data_points; ++m)
      {
        for(int f=0; f<n_functions; ++f)
        {
          average[m][f]+=plotting_data[bootconfig[boot][n]][m][f];
        }
      }
    }

    for(int m=0; m<n_data_points-1; ++m)
    {
      for(int f=0; f<n_functions; ++f)
      {
        if(average[m][f]/average[m+1][f]>0)
        {
          eff_mass[boot][m][f]=log(average[m][f]/average[m+1][f]);
        }
        else
        {
          eff_mass[boot][m][f]=0;
        }
      }
    }
  }

  for(int boot=0; boot<bssamples; ++boot)
  {
    for(int m=0; m<n_data_points-1; ++m)
    {
      for(int f=0; f<n_functions; ++f)
      {
        eff_mass_average[m][f]+=eff_mass[boot][m][f];
      }
    }
  }

  for(int m=0; m<n_data_points-1; ++m)
  {
    for(int f=0; f<n_functions; ++f)
    {
       eff_mass_average[m][f]/=bssamples;
    }
  }

  double normalization=1.0;
  if(fitsetWidget->get_bootstrap_normalization())
  {
    normalization=sqrt(n_data_sets);
  }

  for(int m=0; m<n_data_points-1; ++m)
  {
    for(int f=0; f<n_functions; ++f)
    {
      vector< double > range_temp(bssamples, 0.0);
      for(int boot=0; boot<bssamples; ++boot)
      {
        range_temp[boot]= eff_mass[boot][m][f];
      }
      stable_sort(range_temp.begin(), range_temp.end());
      eff_mass_sigma[m][f]=normalization*(range_temp[static_cast<int>(0.841345*bssamples)]-range_temp[static_cast<int>(0.158655*bssamples)])/2.0;
    }
  }

  double plot_x_min;
  double plot_x_max;
  if(plot_set_dialog->data_range())
  {
    plot_x_min=file_arguments[0][0];
    plot_x_max=file_arguments[n_data_points-1][0];
    for(int m=0; m<n_data_points; ++m)
    {
      if(file_arguments[m][0]>plot_x_max)
      {
        plot_x_max=file_arguments[m][0];
      }
      if(file_arguments[m][0]<plot_x_min)
      {
        plot_x_min=file_arguments[m][0];
      }
    }
  }
  else
  {
    plot_x_min=plot_set_dialog->get_min();
    plot_x_max=plot_set_dialog->get_max();
  }

  ofstream output(filename.c_str());
  if(!output)
  {
    QMessageBox::warning(this, tr("QMBF"),
                         tr("Cannot write file %1:\n")
                         .arg(QString(filename.c_str())));
    return;
  }
  output.precision(15);

  if(!plot_set_dialog->data_range_y())
  {
    output << "@version 50100" << endl;
    output << "@    world " << plot_x_min << ", " << plot_set_dialog->get_min_em_y() << ", " << plot_x_max << ", " << plot_set_dialog->get_max_em_y() << endl;
    double majortickx=(plot_x_max-plot_x_min)/10000000.0;  // this will enable autoticks because it is too small
    output << "@    xaxis  tick on" << endl;
    output << "@    xaxis  tick major " << majortickx << endl;
    double majorticky=(plot_set_dialog->get_max_em_y()-plot_set_dialog->get_min_em_y())/10000000.0;  // this will enable autoticks because it is too small
    output << "@    yaxis  tick on" << endl;
    output << "@    yaxis  tick major " << majorticky << endl;
  }

  output << "@    xaxes scale Normal" << endl;
  output << "@    yaxes scale Normal" << endl;

  for(int f=0; f<n_functions; ++f)
  {
    output << "@    s" << f << " line type 0" << endl;
    output << "@    s" << f << " symbol 2" << endl;
    output << "@    s" << f << " symbol size 0.30000" << endl;
    output << "@    s" << f << " symbol color " << f+1 << endl;
    output << "@    s" << f << " symbol pattern 1" << endl;
    output << "@    s" << f << " symbol fill color " << f+1 << endl;
    output << "@    s" << f << " symbol fill pattern 1" << endl;
  }

  for(int f=0; f<n_functions; ++f)
  {
    output << "@target G0.S" << f << endl;
    output << "@type xydy" << endl;
    for(int m=0; m<n_data_points-1; ++m)
    {
      if( (file_arguments[m][0]>=plot_x_min) && (file_arguments[m][0]<=plot_x_max) )
      {
        if(corr_average[m][f]/corr_average[m+1][f]>0)
        {
          output << (file_arguments[m][0]+file_arguments[m+1][0])/2.0 << " " << log(corr_average[m][f]/corr_average[m+1][f]) << " " << eff_mass_sigma[m][f] << endl;
        }
      }
    }
    output << "&" << endl;
  }

  output << "@target G0.S" << n_functions << endl;
  output << "@type xy" << endl;

  output << plot_x_min << " " << E1 << endl;
  output << plot_x_max << " " << E1 << endl;
  output << "&" << endl;

  output.close();

  QString program = (plot_set_dialog->get_grace()).c_str();
  QStringList arguments;

  if(plot_set_dialog->get_viewplot()==1)
  {
    arguments << simple_filename.c_str();

    QProcess *grace_process = new QProcess(this);
    grace_process->setWorkingDirectory(QString(dir.c_str()));
    grace_process->start(program, arguments);

  }
  else
  {

    arguments << "-hdevice" << "SVG" << "-hardcopy" << simple_filename.c_str();

    QProcess *grace_process = new QProcess(this);
    grace_process->setWorkingDirectory(QString(dir.c_str()));
    grace_process->start(program, arguments);

    grace_process->waitForFinished();
    if(grace_process->exitCode()!=0)
    {
      QApplication::restoreOverrideCursor();
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Grace Error"));
      return;
    }
    PlotView* pv=new PlotView(QString(svg_filename.c_str()));
    pv->show();

  }

  QApplication::restoreOverrideCursor();

}
