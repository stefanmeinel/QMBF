#ifndef REPORT_VIEW_H
#define REPORT_VIEW_H

#include <QtWidgets>

class ReportView: public QDialog
{
  Q_OBJECT

  public:
    ReportView(QString dir, QWidget* parent=0);

    QTextEdit* terminal;

  private slots:

    void save();

  private:

    QPushButton* savebutton;
    QPushButton* closebutton;

    QVBoxLayout* vlayout;
    QHBoxLayout* hlayout;

    QString current_dir;

};

#endif
