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


#ifndef chessPLUGIN_H
#define chessPLUGIN_H

#include "qutim/plugininterface.h"
#include "gameboard.h"
#include <QAction>

using namespace qutim_sdk_0_2;

class chessPlugin : public QObject, SimplePluginInterface, EventHandler
{
    Q_OBJECT
    Q_INTERFACES ( qutim_sdk_0_2::PluginInterface )
public:
    virtual bool init ( PluginSystemInterface *plugin_system );
    virtual void release(){ }

    virtual void processEvent ( PluginEvent &event ) {}
    virtual void processEvent ( Event &event );
    virtual QWidget *settingsWidget(){return new QWidget();}
    virtual void setProfileName ( const QString &profile_name ) { m_profile_name=profile_name; }
    virtual QString name(){ return "Chess plugin"; }
    virtual QString description(){ return "Plugin to play chess over different protocols"; }
    virtual QString type(){ return "game"; }
    virtual QIcon *icon(){ return m_plugin_icon; }
    virtual void removeSettingsWidget(){}
    virtual void saveSettings(){}
private:
    QIcon *m_plugin_icon;
    QString m_profile_name;
    QString m_account_name;
    quint16 m_event_receive;
    quint16 m_item_menu;

    GameBoard* game_;
    QString playingWith_;

    void startGame(const QString& jid, bool meFirst, const TreeModelItem item);

    TreeModelItem m_with;
    TreeModelItem eventitem;

private slots:
    void sendData(const QString& data);
    void receiveData(const QString& data);
    void actionStart();
    void stopGame();
};

#endif

