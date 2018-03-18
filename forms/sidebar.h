#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>

namespace Ui {
	class Sidebar;
}

class Sidebar : public QWidget {
  Q_OBJECT

  public:
	explicit Sidebar(QWidget* parent = 0);
	~Sidebar() override;

  private:
	Ui::Sidebar* ui;
};

#endif // SIDEBAR_H
