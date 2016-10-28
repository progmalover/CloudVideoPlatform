#include "cmd_stream_start_handler.h"
#include "../mediaserverinfo.h"
#include <QDateTime>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QByteArray>
#include <QHttpMultiPart>
#include <QEventLoop>
#include <QProcess>

QString CmdStreamStartHandler::cmd(){
	return "stream_start";
}

void CmdStreamStartHandler::handle(QJsonObject obj, IWebSocketClient *wsc){

	// ; rtmp://54.173.34.172:1935/live/u1976m5453c5424_primary?sid=oCJ4s05MhUbG
	QString stream_url = "rtmp://" + wsc->settings()->servercm_media_server() + "/" + wsc->settings()->camera_media_url() + "Main";
	stream_url += "?sid=" + wsc->settings()->servercm_sid();
	
	QString video_stream_command = wsc->settings()->streams_video_stream_command();
	video_stream_command.replace(QString("%RTMPURL%"), stream_url);
	qDebug() << "[WS] video_stream_command: " << video_stream_command;
	
	if(wsc->process()->isStarted()){
		wsc->process()->stop();
	}
	wsc->process()->start(video_stream_command);

	wsc->sendMessage(wsc->makeCommandDone(cmd(), obj["msgid"].toInt(), "OK"));
}