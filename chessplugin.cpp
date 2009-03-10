/*
Chess plugin for qutim

Copyright (c) 2008 by Kostin Dmitrij [DsTr] <kostindima@gmail.com>

***************************************************************************
* *
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation; either version 2 of the License, or *
* (at your option) any later version. *
* *
***************************************************************************

Thanks to Psi developers! Site: psi-im.org
*/

#include <qutim/plugininterface.h>
#include "chessplugin.h"
#include "QMessageBox"

extern QColor	cw, cb;

bool chessPlugin::init ( PluginSystemInterface *plugin_system )
{
    qRegisterMetaType<TreeModelItem> ( "TreeModelItem" );

    PluginInterface::init ( plugin_system );
    m_plugin_icon = new QIcon ( ":/icons/plugin.png" );
    m_plugin_system = plugin_system;
    TreeModelItem contact;

    m_event_receive = m_plugin_system->registerEventHandler("Core/ChatWindow/ReceiveLevel2", this);

    QAction* menuaction = new QAction(QIcon(":/icons/chessplugin.png"), tr("Play chess"), this);
    connect(menuaction, SIGNAL(triggered()), this,  SLOT(actionStart()));
    m_item_menu = plugin_system->registerEventHandler("Core/ContactList/ContactContext", this);
    m_plugin_system->registerContactMenuAction(menuaction, this);

    game_=0;

    return true;
}

void chessPlugin::processEvent ( Event &event )
{
    if ( event.id == m_item_menu ) {
	eventitem = event.at<TreeModelItem>(0);
    }

    if ( event.id == m_event_receive) {
	QString &message = event.at<QString>(1);
	if (!message.startsWith("/chess "))
		    return;

	TreeModelItem from = event.at<TreeModelItem>(0);
	QString fromJid = from.m_item_name;
	bool &b = event.at<bool>(2);
	b = true;

	if (game_ && fromJid != playingWith_)
	{
		m_plugin_system->sendCustomMessage(from, QString("/chess cancel Already playing chess, sorry"), true);
		return;
	}

	QString command = QString(message);
	command.remove(0,7);

	if (command == QString("start"))
	{
		if (game_) return;
		int ret = QMessageBox::question(0, tr("QutIM chess plugin"),
				fromJid + tr(" invites you to play chess.\n"
				   "Accept?"),
				QMessageBox::Yes | QMessageBox::No);
		if ( ret == QMessageBox::Yes )
		{
		    m_plugin_system->sendCustomMessage(from, "/chess accept", true);
		    startGame(fromJid, false, from);
		} else {
		    m_plugin_system->sendCustomMessage(from, "/chess cancel Manualy cancel", true);
		}
	}
	else if (command == QString("accept"))
	{
		if (game_) return;
		startGame(fromJid, true, from);
	}
	else if (command.startsWith("cancel"))
	{
	    QString reason;
	    reason = QString(command);
	    reason.remove(0, 7);
	    if (!reason.isEmpty())
		reason = tr(",\nwith reason: \"") + reason + "\"";
	    QMessageBox::information(0, tr("QutIM chess plugin"),
				fromJid + " " + tr("don't accept your invite to play chess") + reason,
				QMessageBox::Ok);
	}
	else if (!game_)
	{
		return;
	}
	else if (command.startsWith("command"))
	{
		command.remove(0,8);

		qDebug() << (qPrintable(QString("chess command %1").arg(command)));
		receiveData(command);
	}
    }
}

void chessPlugin::actionStart()
{
     m_plugin_system->sendCustomMessage(eventitem, "/chess start", true);
}

void chessPlugin::startGame(const QString& jid, bool meFirst, const TreeModelItem item)
{
	playingWith_=jid;
	m_with = item;
	cw = QColor(0x8F, 0xDF, 0xF0);
	cb = QColor(0x70, 0x9F, 0xDF);
	if (meFirst)
	{
		GameBoard::GameType type=GameBoard::WHITE;
		game_ = new GameBoard(type, jid, NULL);
	} else {
		game_ = new GameBoard(jid, NULL);
	}

	connect(game_, SIGNAL(sendData(const QString&)), SLOT(sendData(const QString &)));
	connect(game_, SIGNAL(destroyed()), SLOT(stopGame()));
	game_->show();
 }

void chessPlugin::stopGame()
{
	//delete game_;
	game_=0;
}

void chessPlugin::sendData(const QString& data)
{
	qDebug() << (qPrintable(QString("sendingData turn: %1").arg(data)));
	if (!game_)
		return;
	QString reply;
	reply=QString("/chess command %1").arg(data);
	qDebug() << (qPrintable(QString("sendingData: %1").arg(reply)));
	m_plugin_system->sendCustomMessage(m_with, reply, true);
}

void chessPlugin::receiveData(const QString& data)
{
	qDebug() << (qPrintable(QString("received data: %1").arg(data)));
	if (!game_)
		return;
	//game->theirMove(space);
	game_->receiveData(data);
}

Q_EXPORT_PLUGIN2 ( chessplugin,chessPlugin );
