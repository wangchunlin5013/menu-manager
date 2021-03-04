/*

 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.

 *

 * Author:     wangcl <wangchunlin@uniontech.com>

 *

 * Maintainer: wangcl <wangchunlin@uniontech.com>

 *

 * This program is free software: you can redistribute it and/or modify

 * it under the terms of the GNU General Public License as published by

 * the Free Software Foundation, either version 3 of the License, or

 * any later version.

 *

 * This program is distributed in the hope that it will be useful,

 * but WITHOUT ANY WARRANTY; without even the implied warranty of

 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the

 * GNU General Public License for more details.

 *

 * You should have received a copy of the GNU General Public License

 * along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */
#include "menumanager.h"
#include "menumanager_p.h"
#include "../common/dataDefine.h"

#include <QStandardPaths>
#include <QDebug>
#include <QFileInfo>
#include <QSettings>
#include <QString>

MenuManagerPrivate::MenuManagerPrivate()
{

}

MenuManagerPrivate::~MenuManagerPrivate()
{

}

void MenuManagerPrivate::parseFile()
{
    userMenus.clear();
    userMenuTxts.clear();

    QSettings setting(configPath, QSettings::NativeFormat);
    QStringList groups = setting.childGroups();
    if (groups.isEmpty()) {
        hasCustom = false;
        return;
    }
    for (auto group : groups) {
        ActionData data;
        setting.beginGroup(group);
        QString type = setting.value(MENUTYPE).toString();
        if (MENUTYPEACTION == type) {
            data.type = ActionType::Action;
            data.name = setting.value(MENUNAME).toString();
            data.icon = setting.value(MENUICON).toString();
            data.tips = setting.value(MENUTIPS).toString();
            data.commd = setting.value(MENUCOMMD).toString();
            userMenus.append(data);
            userMenuTxts.append(data.name);
        } else if (MENUTYPESEPARATOR == type) {
            data.type = ActionType::Separator;
            userMenus.append(data);
        } else {
            data.type = ActionType::Unknow;
        }
        setting.endGroup();
    }
}

void MenuManagerPrivate::saveFile(const QList<ActionData> &menus)
{
    QSettings setting(configPath, QSettings::NativeFormat);
    setting.clear();

    userMenus = menus;
    if (userMenus.isEmpty())
        return;

    for (int i=0; i<userMenus.count(); i++) {
        QString group = MENUGROUP + QString("%1").arg(i);
        QString type = userMenus.at(i).type == ActionType::Action ? MENUTYPEACTION : MENUTYPESEPARATOR;
        setting.beginGroup(group);
        setting.setValue(MENUTYPE, type);
        setting.setValue(MENUNAME, userMenus.at(i).name);
        setting.setValue(MENUICON, userMenus.at(i).icon);
        setting.setValue(MENUTIPS, userMenus.at(i).tips);
        setting.setValue(MENUCOMMD, userMenus.at(i).commd);
        setting.endGroup();
    }

}

MenuManager::MenuManager(QObject *parent)
    : QObject(*new MenuManagerPrivate, parent)
{
    init();
}

MenuManager *MenuManager::instance()
{
    static MenuManager menuManager;
    return &menuManager;
}

const QList<ActionData> MenuManager::getAllMenus() const
{
    const Q_D(MenuManager);
    return d->sysMenus;
}

const QStringList MenuManager::getAllMenuTxts() const
{
    const Q_D(MenuManager);
    return d->sysMenuTxts;
}

const QList<ActionData> MenuManager::getUserMenus() const
{
    const Q_D(MenuManager);
    return d->userMenus;
}

const QStringList MenuManager::getUserMenuTxts() const
{
    const Q_D(MenuManager);
    return d->userMenuTxts;
}

void MenuManager::saveUserMenus(const QList<ActionData> &userMenus)
{
    Q_D(MenuManager);
    d->saveFile(userMenus);
}

void MenuManager::init()
{
    loadSysMenu();
    loadUserMenu();
}

bool MenuManager::loadSysMenu()
{
    Q_D(MenuManager);

    // TODO:根据规范或系统默认实现方式，加载所有菜单
    d->sysMenus.clear();
    d->sysMenuTxts.clear();
    for(int i=0; i<20; i++) {
        ActionData data;
        data.type = ActionType::Action;
        data.name = QString("action%1").arg(i);
        data.tips = QString("this is test action %1").arg(i);

        d->sysMenus.append(data);
        d->sysMenuTxts.append(data.name);
    }
    return true;
}

bool MenuManager::loadUserMenu()
{
    Q_D(MenuManager);

    // 加载用户自定义菜单的配置文件
    d->userMenus.clear();

    d->configPath = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first();
    d->configPath += "/custommenu.conf";
    qDebug()<<"======configPath:"<<d->configPath;

    QFileInfo fileInfo(d->configPath);
    if (fileInfo.exists() && fileInfo.isReadable()) {
        d->hasCustom = true;
        d->parseFile();
    } else {
        d->hasCustom = false;
        d->userMenus.clear();
    }
    qDebug()<<"======hasCustom:"<<d->hasCustom;

    return true;
}

MenuManager::MenuManager(MenuManagerPrivate &dd, QObject *parent)
    : QObject (dd, parent)
{
    init();
}
