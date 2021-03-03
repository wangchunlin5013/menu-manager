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
#include "menumanager.h"

MainWindowPrivate::MainWindowPrivate()
{
    userView = new QListView;
    userModel = new UserListModel;
    userDelegate = new UserListDelegate;

    userView->setModel(userModel);
    userView->setItemDelegate(userDelegate);
    userView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    allWidget = new QListWidget;
    allWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    const QList<ActionData> allMenus = MenuManager::instance()->getAllMenus();
    for(auto menu : allMenus) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(menu.name);
        allWidget->addItem(item);
    }


}

MainWindowPrivate::~MainWindowPrivate()
{
    userView->deleteLater();
    userModel->deleteLater();
    userDelegate->deleteLater();
    allWidget->deleteLater();
}

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags f)
  : QWidget(*new MainWindowPrivate, parent, f)
{
    init();
}

MainWindow::MainWindow(MainWindowPrivate &dd, QWidget *parent, Qt::WindowFlags f)
    :QWidget(dd, parent, f)
{
    init();
}

MainWindow::~MainWindow()
{

}

void MainWindow::init()
{
    Q_D(MainWindow);

    d->allActionsTitle = new QLabel("所有菜单");
    QVBoxLayout *allActionsLayout = new QVBoxLayout;
    allActionsLayout->addWidget(d->allActionsTitle);
    allActionsLayout->addWidget(d->allWidget);

    d->userActionsTitle = new QLabel("自定义菜单");
    QVBoxLayout *userActionsLayout = new QVBoxLayout;
    userActionsLayout->addWidget(d->userActionsTitle);
    userActionsLayout->addWidget(d->userView);

    d->addToUser = new QPushButton("-->");
    d->delFromUser = new QPushButton("<--");
    QVBoxLayout *operatorLayout = new QVBoxLayout;
    operatorLayout->addStretch();
    operatorLayout->addWidget(d->addToUser);
    operatorLayout->addWidget(d->delFromUser);
    operatorLayout->addStretch();

    d->moveUpBtn            = new QPushButton("上移一项");
    d->moveDownBtn          = new QPushButton("下移一项");
    d->delFromUserBtn       = new QPushButton("移除自定义");
    d->insertSeparatorBtn   = new QPushButton("插入分隔符");
    d->delSeparatorBtn      = new QPushButton("删除分隔符");
    d->saveAndQuit          = new QPushButton("保存退出");
    d->cancelAndQuit        = new QPushButton("取消退出");
    QVBoxLayout *btnLayout  = new QVBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(d->moveUpBtn);
    btnLayout->addWidget(d->moveDownBtn);
    btnLayout->addWidget(d->delFromUserBtn);
    btnLayout->addWidget(d->insertSeparatorBtn);
    btnLayout->addWidget(d->delSeparatorBtn);
    btnLayout->addWidget(d->saveAndQuit);
    btnLayout->addWidget(d->cancelAndQuit);
    btnLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(allActionsLayout);
    mainLayout->addLayout(operatorLayout);
    mainLayout->addLayout(userActionsLayout);
    mainLayout->addLayout(btnLayout);

    connect(d->allWidget, &QListWidget::itemSelectionChanged, this, [this]() {
        Q_D(MainWindow);
        if (d->allWidget->selectedItems().isEmpty()) {
            d->addToUser->setEnabled(false);
        } else {
            d->addToUser->setEnabled(true);
        }
    });
    connect(d->userView->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this](const QItemSelection &selected, const QItemSelection &deselected) {
        Q_UNUSED(selected)
        Q_UNUSED(deselected)
        Q_D(MainWindow);
        if (d->userView->selectionModel()->selectedIndexes().isEmpty()) {
            d->delFromUser->setEnabled(false);
        } else {
            d->delFromUser->setEnabled(true);
        }
    });

    connect(d->addToUser, &QPushButton::click, this, [this](){
        Q_D(MainWindow);
        QModelIndexList indexList = d->allWidget->selectionModel()->selectedIndexes();
        for (auto index : indexList) {
            if (!index.isValid())
                continue;
            int row = index.row();
            QListWidgetItem *item = d->allWidget->item(row);
            if (item) {
                // 已添加的项，禁止重复选择
                Qt::ItemFlags flags = item->flags() & (~Qt::ItemIsSelectable);
                item->setFlags(flags);

                // 添加到自定义中
//                d->userModel.
            }
        }
    });
}
