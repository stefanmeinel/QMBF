#ifndef PLOTVIEW_H
#define PLOTVIEW_H

#include <QtWidgets>
#include <QtSvg>

class PlotView: public QWidget
{
  Q_OBJECT

  public:
    PlotView(const QString& file, QWidget* parent=0);

  private:

    QVBoxLayout* vlayout;
    QSvgWidget* svg_image;

};

#endif
