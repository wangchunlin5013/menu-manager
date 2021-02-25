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

MainWindowPrivate::MainWindowPrivate()
{
    configPath = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first();
}

MainWindowPrivate::~MainWindowPrivate()
{

}

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags f)
    : QWidget(*new MainWindowPrivate, parent, f)
{

}

MainWindow::~MainWindow()
{

}

MainWindow::MainWindow(MainWindowPrivate &dd, QWidget *parent, Qt::WindowFlags f)
    : QWidget (dd, parent, f)
{

}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint pos = event->pos();

    QMenu menu;
    menu.addAction("action1");
    menu.exec(pos);
}
