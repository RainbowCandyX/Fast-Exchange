#include "exchangewidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

#if defined(Q_OS_WIN)
	app.setFont(QFont("Microsoft YaHei UI", 10));
#elif defined(Q_OS_MACOS)
	app.setFont(QFont("PingFang SC", 13));
#else
	app.setFont(QFont("Noto Sans CJK SC", 10));
#endif

	ExchangeWidget w;
	w.show();

	return app.exec();
}
