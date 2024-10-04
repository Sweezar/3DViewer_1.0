#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QSettings>
#include <QSlider>
#include <QString>
#include <QTextEdit>
#include <QVBoxLayout>

#include "mainscene.h"

extern "C" {
#include "../model/read_obj.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_openFileButton_clicked();

  void on_moveXDoubleSpinBox_valueChanged(double arg1);
  void on_moveYDoubleSpinBox_valueChanged(double arg1);
  void on_moveZDoubleSpinBox_valueChanged(double arg1);

  void on_rotateXSlider_valueChanged(int value);
  void on_rotateYSlider_valueChanged(int value);
  void on_rotateZSlider_valueChanged(int value);

  void on_scaleDoubleSpinBox_valueChanged(double arg1);

  void on_zoneXDoubleSpinBox_valueChanged(double arg1);
  void on_zoneYDoubleSpinBox_valueChanged(double arg1);
  void on_zoneZDoubleSpinBox_valueChanged(double arg1);

  void on_parallelTypeRadioButton_toggled(bool checked);
  void on_centralTypeRadioButton_toggled(bool checked);

  void on_thicknessSpinBox_valueChanged(int arg1);
  void on_lineColorRSlider_valueChanged(int value);
  void on_lineColorGSlider_valueChanged(int value);
  void on_lineColorBSlider_valueChanged(int value);

  void on_vertexTypeComboBox_currentIndexChanged(int index);
  void on_vertexColorRSlider_valueChanged(int value);
  void on_vertexColorGSlider_valueChanged(int value);
  void on_vertexColorBSlider_valueChanged(int value);
  void on_vertexWidthSpinBox_valueChanged(int arg1);

  void on_backgroundColorRSlider_valueChanged(int value);
  void on_backgroundColorGSlider_valueChanged(int value);
  void on_backgroundColorBSlider_valueChanged(int value);

  void saveSettings();
  void loadSettings();

  void on_lineTypeComboBox_currentIndexChanged(int index);

  void on_saveImageButton_clicked();

  void on_saveGifButton_clicked();

 private:
  Ui::MainWindow *ui;
  data_t *m_data;
  double xpos;
  double ypos;
  double zpos;

  double xangle;
  double yangle;
  double zangle;

  double m_scale;

  QString currentFilename;

  void resetConf();
  void setupConnections();
  void infoUpdate();
  void createGif(QString filename);
};
#endif  // MAINWINDOW_H
