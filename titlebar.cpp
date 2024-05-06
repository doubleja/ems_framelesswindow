#include "stdafx.h"
#include "titlebar.h"
#include "windows.h"

#include <QDebug>

TitleBar::TitleBar(QWidget* parent)
	: QWidget(parent)
{
	//resize(1360, 38);
	setFixedHeight(TITLE_HEIGHT);
    setAttribute(Qt::WA_StyledBackground,true);

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	QLabel* logoLabel = new QLabel(this);
	logoLabel->setObjectName("logoLabel");
    logoLabel->setText("Taihuelectic");
    // 创建字体对象并设置字体属性
    QFont font;
    font.setFamily("Arial"); // 设置字体族
    font.setPointSize(12);   // 设置字体大小
    font.setBold(true);      // 设置粗体
    font.setItalic(false);   // 设置斜体

    // 将字体应用到 QLabel
    logoLabel->setFont(font);
	layout->addWidget(logoLabel);
	layout->addStretch(1);

	QToolButton* minToolButton = new QToolButton(this);
	minToolButton->setObjectName("minToolButton");
	connect(minToolButton, &QToolButton::clicked, this->window(), &QWidget::showMinimized);
	layout->addWidget(minToolButton);

	QToolButton* maxToolButton = new QToolButton(this);
	maxToolButton->setObjectName("maxToolButton");
	connect(maxToolButton, &QToolButton::clicked, this, &TitleBar::toggleMaxState);
	maxToolButton->setProperty("MaxState", false);
	layout->addWidget(maxToolButton);

	QToolButton* closeToolButton = new QToolButton(this);
	closeToolButton->setObjectName("closeToolButton");
	connect(closeToolButton, &QToolButton::clicked, this->window(), &QWidget::close);
	layout->addWidget(closeToolButton);
}

TitleBar::~TitleBar()
{
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
	toggleMaxState();
}

void TitleBar::mousePressEvent(QMouseEvent* event)
{
	ReleaseCapture();
	SendMessage((HWND)window()->winId(), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
	event->ignore();
}

void TitleBar::toggleMaxState()
{
	if (window()->isMaximized())
	{
		window()->showNormal();
	}
	else
	{
		window()->showMaximized();
	}
}
