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

#include <QMessageBox>
#include <QApplication>

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
    const QStringList userMenuTxts = MenuManager::instance()->getUserMenuTxts();
    const QList<ActionData> allMenus = MenuManager::instance()->getAllMenus();
    for(auto menu : allMenus) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(menu.name);
        allWidget->addItem(item);

        Qt::ItemFlags flags = item->flags() & (~Qt::ItemIsSelectable);
        item->setFlags(flags);
        item->setTextColor(Qt::darkGray);
        item->setBackgroundColor(Qt::lightGray);

        if (userMenuTxts.contains(menu.name)) {
            setItemState(item, false);
        } else {
            setItemState(item, true);
        }
    }
}

MainWindowPrivate::~MainWindowPrivate()
{
    userView->deleteLater();
    userModel->deleteLater();
    userDelegate->deleteLater();
    allWidget->deleteLater();
}

void MainWindowPrivate::setItemState(QListWidgetItem *item, bool isEnable)
{
    if (!item)
        return;
    if (isEnable) {
        Qt::ItemFlags flags = item->flags() | Qt::ItemIsSelectable;
        item->setFlags(flags);
        item->setTextColor(txtColor);
        item->setBackgroundColor(backgroundColor);
    } else {
        Qt::ItemFlags flags = item->flags() & (~Qt::ItemIsSelectable);
        item->setFlags(flags);
        item->setTextColor(Qt::darkGray);
        item->setBackgroundColor(Qt::lightGray);
    }
}

void MainWindowPrivate::setBtnsEnable(bool enable)
{
    moveUpBtn->setEnabled(enable);
    moveDownBtn->setEnabled(enable);
    delFromUserBtn->setEnabled(enable);
    insertSeparatorBtn->setEnabled(enable);
    delSeparatorBtn->setEnabled(enable);
}

void MainWindowPrivate::moveUserMenusToAll()
{
    QModelIndexList indexList = userView->selectionModel()->selectedIndexes();
    QStringList removedMenus;
    for (auto index : indexList) {
        if (!index.isValid())
            continue;
        QString menuTxt = index.data(Qt::DisplayRole).toString();
        userModel->delMenu(menuTxt);

        removedMenus.append(menuTxt);
    }

    // 从所有菜单中恢复该项为可选
    for (auto txt : removedMenus) {
        auto list = allWidget->findItems(txt, Qt::MatchExactly);
        for (auto menu : list) {
            setItemState(menu, true);
        }
    }
}

void MainWindowPrivate::saveUserMenusToFile()
{
    QList<ActionData> userMenus;
    QList<UserListModel::ModelData> userDatas = userModel->getAllDatas();
    for (auto data : userDatas) {
        ActionData menu;
        menu.type = ActionType::Action;
        menu.name = data.text;
        userMenus.append(menu);

        if (data.hasSeparator) {
            ActionData separator;
            separator.type = ActionType::Separator;
            userMenus.append(separator);
        }
    }
    MenuManager::instance()->saveUserMenus(userMenus);
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
    d->setBtnsEnable(false);
    d->saveAndQuit          = new QPushButton("保存");
    d->cancelAndQuit        = new QPushButton("退出");
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

    //======================================================//
    // 选择项可添加到自定义中
    connect(d->allWidget, &QListWidget::itemSelectionChanged, this, [this]() {
        Q_D(MainWindow);
        if (d->allWidget->selectedItems().isEmpty()) {
            d->addToUser->setEnabled(false);
        } else {
            d->addToUser->setEnabled(true);
        }
    });
    // 选择项可从自定义中删除、改变位置、添加分割线等
    connect(d->userView->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this](const QItemSelection &selected, const QItemSelection &deselected) {
        Q_UNUSED(selected)
        Q_UNUSED(deselected)
        Q_D(MainWindow);

        QModelIndexList selectedIndexs = d->userView->selectionModel()->selectedIndexes();
        if (selectedIndexs.isEmpty()) {
            d->delFromUser->setEnabled(false);
            d->setBtnsEnable(false);
        } else {
            d->delFromUser->setEnabled(true);
            if (selectedIndexs.count() == 1) {
                // 上移、下移、分割线操作只支持对一个菜单项
                d->setBtnsEnable(true);

                QModelIndex index = selectedIndexs.first();
                if (index.row() == 0) {
                    // 第一个不允许上移操作
                    d->moveUpBtn->setEnabled(false);
                } else if (index.row() == (MenuManager::instance()->getUserMenuTxts().count() -1)) {
                    //  最后一个不允许下移和插入分隔符操作
                    d->moveDownBtn->setEnabled(false);
                    d->insertSeparatorBtn->setEnabled(false);
                }

                // 根据选择的菜单是否有分隔符决定删除分隔符按钮是否启用
                bool hasSeparator = d->userModel->data(index, UserListModel::Roles::hasSeparator).toBool();
                d->delSeparatorBtn->setEnabled(hasSeparator);
            } else {
                d->setBtnsEnable(false);
                d->delFromUserBtn->setEnabled(true);
            }
        }
    });

    //======================================================//
    connect(d->addToUser, &QPushButton::clicked, this, [this](){
        Q_D(MainWindow);
        QModelIndexList indexList = d->allWidget->selectionModel()->selectedIndexes();
        for (auto index : indexList) {
            if (!index.isValid())
                continue;
            int row = index.row();
            QListWidgetItem *item = d->allWidget->item(row);
            if (item) {
                // 已添加的项，禁止重复选择
                d->setItemState(item, false);

                // 添加到自定义中
                d->userModel->addMenu(item->text());
            }
        }
    });

    connect(d->delFromUser, &QPushButton::clicked, this, [this](){
        Q_D(MainWindow);
        d->moveUserMenusToAll();
    });

    //======================================================//
    connect(d->moveUpBtn, &QPushButton::clicked, this, [this](){
        Q_D(MainWindow);

        d->setBtnsEnable(false);
        QModelIndexList indexList = d->userView->selectionModel()->selectedIndexes();
        if (indexList.count() != 1 || indexList.first().row() == 0)
            return ;
        d->userModel->moveUpMenu(indexList.first().data(Qt::DisplayRole).toString());
    });

    connect(d->moveDownBtn, &QPushButton::clicked, this, [this](){
        Q_D(MainWindow);

        d->setBtnsEnable(false);
        QModelIndexList indexList = d->userView->selectionModel()->selectedIndexes();
        if (indexList.count() != 1 || indexList.first().row() == MenuManager::instance()->getUserMenuTxts().count()-1)
            return ;
        d->userModel->moveDownMenu(indexList.first().data(Qt::DisplayRole).toString());
    });

    connect(d->insertSeparatorBtn, &QPushButton::clicked, this, [this](){
        Q_D(MainWindow);

        d->setBtnsEnable(false);
        QModelIndexList indexList = d->userView->selectionModel()->selectedIndexes();
        if (indexList.count() != 1 || indexList.first().row() == MenuManager::instance()->getUserMenuTxts().count()-1)
            return ;
        d->userModel->insertSeparator(indexList.first().data(Qt::DisplayRole).toString());
    });

    connect(d->delSeparatorBtn, &QPushButton::clicked, this, [this](){
        Q_D(MainWindow);

        d->setBtnsEnable(false);
        QModelIndexList indexList = d->userView->selectionModel()->selectedIndexes();
        if (indexList.count() != 1)
            return ;
        d->userModel->delSeparator(indexList.first().data(Qt::DisplayRole).toString());
    });

    connect(d->delFromUserBtn, &QPushButton::clicked, this, [this](){
        Q_D(MainWindow);
        d->moveUserMenusToAll();
    });

    //======================================================//
    connect(d->saveAndQuit, &QPushButton::clicked, this, [this](){
        Q_D(MainWindow);
        d->saveUserMenusToFile();
    });

    connect(d->cancelAndQuit, &QPushButton::clicked, this, [this](){
        Q_D(MainWindow);
        QMessageBox::StandardButton btn = QMessageBox::information(this, QString("提示"), QString("退出前请确保已经保存！"), QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Close);
        if (QMessageBox::Save == btn) {
            d->saveUserMenusToFile();
            qApp->quit();
        } else if (QMessageBox::Close == btn) {
            qApp->quit();
        } else {
            return ;
        }
    });
}
