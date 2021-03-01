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

#include <QStandardPaths>
#include <QDebug>
#include <QFileInfo>
#include <QSettings>

MenuManagerPrivate::MenuManagerPrivate()
{

}

MenuManagerPrivate::~MenuManagerPrivate()
{

}

void MenuManagerPrivate::parseFile()
{
    sysMenus.clear();

    QSettings setting(configPath, QSettings::NativeFormat);
    QStringList groups = setting.childGroups();
    if (groups.isEmpty()) {
        hasCustom = false;
        return;
    }
    for (auto group : groups) {
        ActionData data;
        setting.beginGroup(group);
        QString type = setting.value(QString("Type")).toString();
        if (type == QString("Action")) {
            data.type = ActionType::Action;
            data.name = setting.value(QString("Name")).toString();
            data.icon = setting.value(QString("Icon")).toString();
            data.tips = setting.value(QString("Tips")).toString();
            data.commd = setting.value(QString("Commd")).toString();
        } else if (type == QString("Separator")) {
            data.type = ActionType::Separator;
        } else {
            data.type = ActionType::Unknow;
        }
        setting.endGroup();
        if (data.type != ActionType::Unknow) {
            userMenus.append(data);
        }
    }
    qDebug()<<"=====actions count:"<<userMenus.count();
}

MenuManager::MenuManager(QObject *parent)
    : QObject(*new MenuManagerPrivate, parent)
{
    init();
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
    for(int i=0; i<20; i++) {
        ActionData data;
        data.type = ActionType::Action;
        data.name = QString("action%1").arg(i);
        data.tips = QString("this is test action %1").arg(i);

        d->sysMenus.append(data);
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
