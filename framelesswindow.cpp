#include "stdafx.h"
#include "framelesswindow.h"
#include "titlebar.h"
#include "windoweffect.h"
#include "bms.h"
#include "pcs.h"
#include "QNetworkAccessManager"
#include "QNetworkReply"
FramelessWindow::FramelessWindow(QWidget* parent)
	: QWidget(parent)
{
	memset(&monitorInfo, 0, sizeof(MONITORINFO));
	monitorInfo.cbSize = sizeof(MONITORINFO);


    QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	titleBar = new TitleBar(this);
	titleBar->setObjectName("titleBar");
    // titleBar->setStyleSheet("background: rgb(47,70,146);");
	layout->addWidget(titleBar);



	QLabel* testLabel = new QLabel(this);
	testLabel->setObjectName("testLabel");
    testLabel->setText("testLabel");
	testLabel->setStyleSheet("background: rgb(240, 240, 240);");
	layout->addWidget(testLabel);

    QPushButton* button1 = new QPushButton(this);
    button1->setObjectName("button1");
    button1->setText("button1");
    layout->addWidget(button1);


    PCS* pcs = new PCS(this);
    pcs->setObjectName("pcs");
    layout->addWidget(pcs);

    // layout->addWidget(bms);

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

	WindowEffect::addWindowAnimation((HWND)winId());
    // WindowEffect::addShadowEffect((HWND)winId());

	connect(windowHandle(), &QWindow::screenChanged, this, [=] {
		auto hWnd = reinterpret_cast<HWND>(windowHandle()->winId());
		SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
		});


}


QString FramelessWindow::onenetRetData(const QString &id, const QString &sendTimeStr) {
    // 暂停2秒
    QThread::sleep(2);

    QString apikey = "R218Q5jpynJPvh8ow9b8LBXFNr0=";
    QNetworkRequest request;
    request.setUrl(QUrl("http://api.heclouds.com/devices/" + id + "/datapoints?datastream_id=BIN_DATA&sort=DESC&limit=1"));

    // 将发送时间字符串转换为 QDateTime 对象
    QDateTime sendTime = QDateTime::fromString(sendTimeStr, "yyyy-MM-ddTHH:mm:ss");

    // 确保查询的开始时间晚于发送时间的 10 秒前
    QDateTime startTime = sendTime.addSecs(-10);

    request.setRawHeader("api-key", apikey.toUtf8());
    request.setRawHeader("start", startTime.toString("yyyy-MM-ddTHH:mm:ss").toUtf8());

    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(request);
    while (!reply->isFinished()) {
        qApp->processEvents();
    }

    QByteArray responseData = reply->readAll();
    QJsonObject data = QJsonDocument::fromJson(responseData).object();
    delete reply;

    // if (data["data"]["datastreams"].toArray().isEmpty()) {
    //     qDebug() << "NO value";
    //     return "NO value";
    // }

    // QString bindata = data["data"]["datastreams"].toArray()[0].toObject()["datapoints"].toArray()[0].toObject()["value"].toString();
    qDebug() << "获取onenet数据流最新数据:" << data;
    return "bindata";
    // return bindata;
}

void FramelessWindow::cmdSend(const QString &cmd) {
    QString id = "1097949264";
    // this->node_accept->insertPlainText(QDateTime::currentDateTime().toString("\nyyyy-MM-dd HH:mm:ss"));
    // this->node_accept->append(zui);
    try {
        QString apikey = "R218Q5jpynJPvh8ow9b8LBXFNr0=";
        QNetworkRequest request;
        request.setUrl(QUrl("http://api.heclouds.com/cmds?device_id=" + id));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader("api-key", apikey.toUtf8());

        QNetworkAccessManager manager;
        QNetworkReply *reply = manager.post(request, cmd.toUtf8());
        while (!reply->isFinished()) {
            qApp->processEvents();
        }

        QByteArray responseData = reply->readAll();
        QJsonObject data = QJsonDocument::fromJson(responseData).object();
        qDebug() << "\n" << cmd;
        qDebug() << "data" << data << "\n";

        delete reply;
    } catch (...) {
        // Handle exception
    }
}



bool FramelessWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
	MSG* msg = reinterpret_cast<MSG*>(message);
	if (msg->message == WM_NCHITTEST)
	{
		int xPos = (LOWORD(msg->lParam) - frameGeometry().x()) % 65536;
		int yPos = HIWORD(msg->lParam) - frameGeometry().y();
		int w = width();
		int h = height();
		bool lx = xPos < BORDER_WIDTH;
		bool rx = xPos > w - BORDER_WIDTH;
		bool ty = yPos < BORDER_WIDTH;
		bool by = yPos > h - BORDER_WIDTH;

		if (lx && ty)
		{
			*result = HTTOPLEFT;
			return true;
		}
		else if (rx && by)
		{
			*result = HTBOTTOMRIGHT;
			return true;
		}
		else if (rx && ty) {
			*result = HTTOPRIGHT;
			return true;
		}
		else if (lx && by) {
			*result = HTBOTTOMLEFT;
			return true;
		}
		else if (ty) {
			*result = HTTOP;
			return true;
		}
		else if (by) {
			*result = HTBOTTOM;
			return true;
		}
		else if (lx) {
			*result = HTLEFT;
			return true;
		}
		else if (rx) {
			*result = HTRIGHT;
			return true;
		}
	}
	else if (msg->message == WM_NCCALCSIZE)
	{
		if (isWindowMaximized(msg->hwnd))
		{
			monitorNCCALCSIZE(msg);
		}

		*result = HTNOWHERE;
		return true;
	}
	else if (msg->message == WM_GETMINMAXINFO)
	{
		if (isWindowMaximized(msg->hwnd))
		{
			RECT window_rect;
			if (!GetWindowRect(msg->hwnd, &window_rect))
			{
				*result = HTNOWHERE;
				return false;
			}

			HMONITOR hMonitor = MonitorFromRect(&window_rect, MONITOR_DEFAULTTONULL);
			if (!hMonitor)
			{
				*result = HTNOWHERE;
				return false;
			}

			GetMonitorInfoW(hMonitor, &monitorInfo);
			RECT monitor_rect = monitorInfo.rcMonitor;
			RECT work_area = monitorInfo.rcWork;

			LPMINMAXINFO info = reinterpret_cast<LPMINMAXINFO>(msg->lParam);

			info->ptMaxSize.x = work_area.right - work_area.left;
			info->ptMaxSize.y = work_area.bottom - work_area.top;
			info->ptMaxTrackSize.x = info->ptMaxSize.x;
			info->ptMaxTrackSize.y = info->ptMaxSize.y;

			info->ptMaxPosition.x = abs(window_rect.left - monitor_rect.left);
			info->ptMaxPosition.y = abs(window_rect.top - monitor_rect.top);
			*result = HTCLIENT;
			return true;
		}
	}

	return QWidget::nativeEvent(eventType, message, result);
}

void FramelessWindow::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::WindowStateChange)
	{
		RefreshStyle(titleBar->findChild<QToolButton*>("maxToolButton"), "MaxState", isMaximized());
	}

	return QWidget::changeEvent(event);
}

bool FramelessWindow::isWindowMaximized(HWND hWnd)
{
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	if (!GetWindowPlacement(hWnd, &wp))
	{

	}
	return wp.showCmd == SW_MAXIMIZE;
}

void FramelessWindow::monitorNCCALCSIZE(MSG* msg)
{
	HMONITOR hMonitor = MonitorFromWindow(msg->hwnd, MONITOR_DEFAULTTONULL);
	if (hMonitor != NULL)
	{
		GetMonitorInfoW(hMonitor, &monitorInfo);
	}
	
	NCCALCSIZE_PARAMS& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
	params.rgrc[0].left = monitorInfo.rcWork.left;
	params.rgrc[0].top = monitorInfo.rcWork.top;
	params.rgrc[0].right = monitorInfo.rcWork.right;
	params.rgrc[0].bottom = monitorInfo.rcWork.bottom;

	//qDebug() << params.rgrc[0].left << params.rgrc[0].top << params.rgrc[0].right << params.rgrc[0].bottom;
}
