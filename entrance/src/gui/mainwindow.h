#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../../../apartment/src/client/ApartmentClient.hpp"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:
  explicit MainWindow(ApartmentClient& app_client, QWidget *parent = 0);
  ~MainWindow();

 private:
  ApartmentClient& app_client;

  private slots:

    void on_pushButton_pressed();

    void on_pushButton_released();

 private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
