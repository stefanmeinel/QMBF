#include "plotview.h"

PlotView::PlotView(const QString& file, QWidget* parent)
{
  setWindowTitle(file);

  vlayout=new QVBoxLayout();

  svg_image=new QSvgWidget(file);

  vlayout->addWidget(svg_image);

  setLayout(vlayout);
}
