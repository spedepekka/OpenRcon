/*
 * Copyright (C) 2011 The OpenRcon Project.
 *
 * This file is part of OpenRcon.
 *
 * OpenRcon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenRcon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with OpenRcon.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "OpenRcon.h"

OpenRcon *OpenRcon::m_Instance = 0;

OpenRcon::OpenRcon(QWidget *parent) : QMainWindow(parent), ui(new Ui::OpenRcon)
{
    ui->setupUi(this);

    dir = new Directory(this);
    gameManager = new GameManager(this);
    serverManager = new ServerManager(this);

    settingsDialog = new SettingsDialog(this);
    aboutDialog = new About(this);

    // Sets application title and icon
    setWindowTitle(QString("%1 %2").arg(APP_NAME).arg(APP_VERSION));
    setWindowIcon(QIcon(APP_ICON));

    // Create and read settings
    settings = new QSettings(APP_NAME, APP_NAME, this);
    readSettings();

    // Actions
    ui->actionAbout->setText(QString(tr("About &%1")).arg(APP_NAME));
    ui->actionAbout->setIcon(QIcon(APP_ICON));
    ui->actionAbout_Qt->setIcon(QIcon(":/qt-project.org/qmessagebox/images/qtlogo-64.png"));

    // ServerManager
    comboBox_sm_server = new QComboBox(this);

    foreach (ServerEntry server, serverManager->getServers()) {
        GameEntry game = gameManager->getGame(server.game);

        comboBox_sm_server->addItem(game.icon, server.name);
    }

    pushButton_sm_connect = new QPushButton(tr("Connect"), this);

    ui->toolBar_sm->addWidget(comboBox_sm_server);
    ui->toolBar_sm->addWidget(pushButton_sm_connect);

    connect(pushButton_sm_connect, SIGNAL(clicked()), this, SLOT(pushButton_sm_connect_clicked()));

    // Tabwidget
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    // Actions
    connect(ui->actionServermanager, SIGNAL(triggered()), this, SLOT(actionServermanager_triggered()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->actionConnection, SIGNAL(triggered()), this, SLOT(actionConnection_triggered()));

    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(actionSettings_triggered()));

    connect(ui->actionDocumentation, SIGNAL(triggered()), this, SLOT(actionDocumentation_triggered()));
    connect(ui->actionReport_bug, SIGNAL(triggered()), this, SLOT(actionReportBug_triggered()));

    connect(ui->actionAbout_Qt, SIGNAL(triggered()), this, SLOT(aboutQt()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
}

OpenRcon::~OpenRcon()
{
    delete ui;

    delete dir;
    delete settings;

    delete gameManager;
    delete serverManager;

    delete settingsDialog;
    delete aboutDialog;
}

void OpenRcon::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);

    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void OpenRcon::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);

    writeSettings();
}

void OpenRcon::readSettings()
{
    settings->beginGroup(SETTINGS_OPENRCON);
        resize(settings->value("Size", size()).toSize());
        move(settings->value("Position", pos()).toPoint());

        if (settings->value("IsMaximized", true).toBool()) {
            showMaximized();
        }

        if (settings->value("actionConnection", true).toBool()) {
            ui->toolBar_sm->show();
            ui->actionConnection->setChecked(true);
        } else {
            ui->toolBar_sm->hide();
            ui->actionConnection->setChecked(false);
        }
    settings->endGroup();
}

void OpenRcon::writeSettings()
{
    settings->beginGroup(SETTINGS_OPENRCON);
        settings->setValue("IsMaximized", isMaximized());
        settings->setValue("Size", size());
        settings->setValue("Position", pos());
    settings->endGroup();
}

void OpenRcon::newTab(const int &game, const QString &name, const QString &host, const int port, const QString &password)
{
    GameEntry entry = gameManager->getGame(game);
    Game *gameObject;

    switch (game) {
        case 0:
            gameObject = new BFBC2Widget(host, port, password);
            break;

        case 1:
            gameObject = new BF4Widget(host, port, password);
            break;

        case 2:
            gameObject = new MinecraftWidget(host, port, password);
            break;
    }

    ui->tabWidget->setCurrentIndex(ui->tabWidget->addTab(gameObject, entry.icon, QString("%1 [%2:%3]").arg(name).arg(host).arg(port)));
}

GameManager* OpenRcon::getGameManager()
{
    return gameManager;
}

void OpenRcon::closeTab(int index)
{
    QWidget *widget = ui->tabWidget->widget(index);
    ConnectionTabWidget *ctw = dynamic_cast<ConnectionTabWidget *>(widget);
    ctw->getConnection()->hostDisconnect();

    ui->tabWidget->removeTab(index);
}

// Application menu
void OpenRcon::actionServermanager_triggered()
{
    ServerListDialog *dlg = new ServerListDialog(this);
    dlg->exec();

    delete dlg;
}

// View menu
void OpenRcon::actionConnection_triggered()
{
    settings->beginGroup(SETTINGS_OPENRCON);
        if (ui->actionConnection->isChecked()) {
            ui->toolBar_sm->show();
            settings->setValue("actionConnection", true);
            ui->actionConnection->setChecked(true);
        } else {
            ui->toolBar_sm->hide();
            settings->setValue("actionConnection", false);
            ui->actionConnection->setChecked(false);
        }
    settings->endGroup();
}

// Tools menu
void OpenRcon::actionSettings_triggered()
{
    settingsDialog->exec();
}

// Help menu
void OpenRcon::actionReport_bug_triggered()
{
    QDesktopServices::openUrl(QUrl(APP_BUG));
}

void OpenRcon::actionDocumentation_triggered()
{
    QDesktopServices::openUrl(QUrl(APP_DOC));
}

void OpenRcon::actionAbout_triggered()
{
    aboutDialog->exec();
}

void OpenRcon::actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

// ServerManager
void OpenRcon::pushButton_sm_connect_clicked()
{
    int index = comboBox_sm_server->currentIndex();
    ServerEntry server = serverManager->getServer(index);

    newTab(server.game, server.name, server.host, server.port, server.password);
}
