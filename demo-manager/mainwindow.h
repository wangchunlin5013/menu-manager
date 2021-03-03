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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>

class MainWindowPrivate;

class MainWindow : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MainWindow)
public:
    explicit MainWindow(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~MainWindow();

    void init();

signals:

public slots:

protected:
    MainWindow(MainWindowPrivate &dd, QWidget* parent, Qt::WindowFlags f);

};

#endif // MAINWINDOW_H
