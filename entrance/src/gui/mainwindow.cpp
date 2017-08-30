#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(ApartmentClient& app_client, QWidget *parent) :
  app_client(app_client),
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pushButton_pressed()
{
  ui->pushButton->setText("DRIIIIIN");
  app_client.ringBell();
}

void MainWindow::on_pushButton_released()
{
  ui->pushButton->setText("");
}
