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
#include "mainwindow.h"
#include "mainwindow_p.h"

#include <QMenu>
#include <QStandardPaths>
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QSettings>

MainWindowPrivate::MainWindowPrivate()
{

}

MainWindowPrivate::~MainWindowPrivate()
{

}

void MainWindowPrivate::init()
{
    configPath = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first();
    configPath += "/custommenu.conf";
    qDebug()<<"======configPath:"<<configPath;

    QFileInfo fileInfo(configPath);
    if (fileInfo.exists() && fileInfo.isReadable()) {
        hasCustom = true;
        parseFile();
    } else {
        hasCustom = false;
        actiondatas.clear();
    }
    qDebug()<<"======hasCustom:"<<hasCustom;
}

void MainWindowPrivate::parseFile()
{
    actiondatas.clear();

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
            actiondatas.append(data);
        }
    }
    qDebug()<<"=====actions count:"<<actiondatas.count();
}

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags f)
    : QWidget(*new MainWindowPrivate, parent, f)
{
    Q_D(MainWindow);
    d->init();
}

MainWindow::~MainWindow()
{

}

MainWindow::MainWindow(MainWindowPrivate &dd, QWidget *parent, Qt::WindowFlags f)
    : QWidget (dd, parent, f)
{
    Q_D(MainWindow);
    d->init();
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    Q_D(MainWindow);

    QPoint pos = event->pos();
    QMenu menu;

    if (d->hasCustom) {
        for (auto action : d->actiondatas) {
            if (action.type == ActionType::Action) {
                QAction *a = new QAction;
                a->setText(action.name);
                a->setIcon(QIcon(action.icon));
                menu.addAction(a);
            } else if (action.type == ActionType::Separator) {
                menu.addSeparator();
            }
        }
    } else {
        // TODO:加载所有菜单
        menu.addAction(QString("testAction1"));
        menu.addAction(QString("testAction2"));
        menu.addSeparator();
        menu.addAction(QString("testAction3"));
        menu.addAction(QString("testAction4"));
    }

    connect(&menu, &QMenu::triggered, this, [=](QAction *action) {
        qDebug()<<"trigger action:"<<action->data();
    });

    menu.exec(pos);
}
