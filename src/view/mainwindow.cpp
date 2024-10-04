#include "mainwindow.h"

#include "ui_mainwindow.h"

extern "C" {
#include "gif.h"
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  m_data = new data_t;
  loadSettings();
  setupConnections();
}

MainWindow::~MainWindow() {
  saveSettings();
  delete ui;
  freeData(m_data);
  delete m_data;
}

void MainWindow::setupConnections() {}

void MainWindow::infoUpdate() {
  ui->vertexCountNumLabel->setText(QString::number(m_data->vertexesCount));
  ui->polygonCountNumLabel->setText(QString::number(m_data->polygonsCount));
}

void MainWindow::createGif(QString filename) {
  QDir tempDir;
  tempDir.setPath(QCoreApplication::applicationDirPath() + "/temp_gif");
  tempDir.mkpath(".");
  QTime timer;

  int gifWidth = 600;
  int gifHeight = 480;

  int fps = 10;
  int durationInSeconds = 5;
  int frameCount = fps * durationInSeconds;
  int delay = 1000 / fps;

  GifWriter gif;
  GifBegin(&gif, filename.toUtf8().constData(), gifWidth, gifHeight, fps);
  for (int frame = 1; frame <= frameCount; ++frame) {
    QString framePath = tempDir.path() + "/" + QString::number(frame) + ".png";
    timer = QTime::currentTime().addMSecs(delay);
    while (QTime::currentTime() < timer) {
      QApplication::processEvents();
    }
    QPixmap pixmap = ui->openGLWidget->grab();

    pixmap = pixmap.scaled(gifWidth, gifHeight, Qt::KeepAspectRatio,
                           Qt::SmoothTransformation);
    QImage image(pixmap.toImage());

    image.save(framePath);
    GifWriteFrame(&gif, image.bits(), gifWidth, gifHeight, fps);
  }

  GifEnd(&gif);

  QDir().rmdir("temp_gif");
}

void MainWindow::on_openFileButton_clicked() {
  ui->openGLWidget->setHidden(true);
  QFileDialog dialog(this, tr("Открыть файл OBJ"), "../../../../objects/",
                     tr("Файлы (*.obj)"));
  dialog.setWindowModality(Qt::NonModal);
  QString filename = dialog.getOpenFileName();

  if (!filename.isEmpty()) {
    filename = QDir::toNativeSeparators(filename);
    currentFilename = filename;

    freeData(m_data);

    char* cStr = qstrdup(filename.toUtf8().constData());
    readObjFile(cStr, m_data);
    delete[] cStr;

    ui->filenameLabel->setText("Модель: " + QFileInfo(filename).fileName());
    infoUpdate();
    resetConf();
    ui->openGLWidget->setHidden(false);
    ui->openGLWidget->setData(m_data);
  }
  ui->openGLWidget->setHidden(false);
}

void MainWindow::resetConf() {
  xpos = 0.0;
  ypos = 0.0;
  zpos = 0.0;

  xangle = 0.0;
  yangle = 0.0;
  zangle = 0.0;

  m_scale = 1.0;

  ui->moveXDoubleSpinBox->setValue(xpos);
  ui->moveYDoubleSpinBox->setValue(ypos);
  ui->moveZDoubleSpinBox->setValue(zpos);

  ui->rotateXSlider->setValue(xangle);
  ui->rotateYSlider->setValue(yangle);
  ui->rotateZSlider->setValue(zangle);

  ui->scaleDoubleSpinBox->setValue(m_scale);
}

void MainWindow::on_moveXDoubleSpinBox_valueChanged(double a) {
  moveX(m_data, a - xpos);
  xpos = a;
}

void MainWindow::on_moveYDoubleSpinBox_valueChanged(double a) {
  moveY(m_data, a - ypos);
  ypos = a;
}

void MainWindow::on_moveZDoubleSpinBox_valueChanged(double a) {
  moveZ(m_data, a - zpos);
  zpos = a;
}

void MainWindow::on_rotateXSlider_valueChanged(int value) {
  rotateOX(m_data, (double)(value - xangle));
  xangle = (double)value;
}

void MainWindow::on_rotateYSlider_valueChanged(int value) {
  rotateOY(m_data, (double)(value - yangle));
  yangle = (double)value;
}

void MainWindow::on_rotateZSlider_valueChanged(int value) {
  rotateOZ(m_data, (double)(value - zangle));
  zangle = (double)value;
}

void MainWindow::on_scaleDoubleSpinBox_valueChanged(double a) {
  double scaleCoef = m_scale / a;
  scale(m_data, 1.0 / scaleCoef);
  m_scale = a;
}

void MainWindow::on_zoneXDoubleSpinBox_valueChanged(double a) {
  ui->openGLWidget->setZone(a, 0);
}

void MainWindow::on_zoneYDoubleSpinBox_valueChanged(double a) {
  ui->openGLWidget->setZone(a, 1);
}

void MainWindow::on_zoneZDoubleSpinBox_valueChanged(double a) {
  ui->openGLWidget->setZone(a, 2);
}

void MainWindow::on_parallelTypeRadioButton_toggled(bool checked) {
  ui->openGLWidget->setType(0);
}

void MainWindow::on_centralTypeRadioButton_toggled(bool checked) {
  ui->openGLWidget->setType(1);
}

void MainWindow::on_thicknessSpinBox_valueChanged(int a) {
  ui->openGLWidget->setPolygonWidth(a);
}

void MainWindow::on_lineTypeComboBox_currentIndexChanged(int index) {
  ui->openGLWidget->setLineType(index);
}

void MainWindow::on_lineColorRSlider_valueChanged(int value) {
  ui->openGLWidget->setLineColor(value / 100.0, 0);
}

void MainWindow::on_lineColorGSlider_valueChanged(int value) {
  ui->openGLWidget->setLineColor(value / 100.0, 1);
}

void MainWindow::on_lineColorBSlider_valueChanged(int value) {
  ui->openGLWidget->setLineColor(value / 100.0, 2);
}

void MainWindow::on_vertexTypeComboBox_currentIndexChanged(int index) {
  ui->openGLWidget->setVertexEnable(index);
}

void MainWindow::on_vertexColorRSlider_valueChanged(int value) {
  ui->openGLWidget->setVertexColor(value / 100.0, 0);
}

void MainWindow::on_vertexColorGSlider_valueChanged(int value) {
  ui->openGLWidget->setVertexColor(value / 100.0, 1);
}

void MainWindow::on_vertexColorBSlider_valueChanged(int value) {
  ui->openGLWidget->setVertexColor(value / 100.0, 2);
}

void MainWindow::on_vertexWidthSpinBox_valueChanged(int a) {
  ui->openGLWidget->setVertexWidth(a);
}

void MainWindow::on_backgroundColorRSlider_valueChanged(int value) {
  ui->openGLWidget->setBackgroundColor(value / 100.0, 0);
}

void MainWindow::on_backgroundColorGSlider_valueChanged(int value) {
  ui->openGLWidget->setBackgroundColor(value / 100.0, 1);
}

void MainWindow::on_backgroundColorBSlider_valueChanged(int value) {
  ui->openGLWidget->setBackgroundColor(value / 100.0, 2);
}

void MainWindow::saveSettings() {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  settings.setValue("filename", currentFilename);
  settings.setValue("xpos", xpos);
  settings.setValue("ypos", ypos);
  settings.setValue("zpos", zpos);

  settings.setValue("xangle", xangle);
  settings.setValue("yangle", yangle);
  settings.setValue("zangle", zangle);

  settings.setValue("m_scale", m_scale);

  settings.setValue("zonex", ui->zoneXDoubleSpinBox->value());
  settings.setValue("zoney", ui->zoneYDoubleSpinBox->value());
  settings.setValue("zonez", ui->zoneZDoubleSpinBox->value());

  settings.setValue("projectionType", ui->centralTypeRadioButton->isChecked());

  settings.setValue("lineColorR", ui->lineColorRSlider->value());
  settings.setValue("lineColorG", ui->lineColorGSlider->value());
  settings.setValue("lineColorB", ui->lineColorBSlider->value());
  settings.setValue("linethickness", ui->thicknessSpinBox->value());
  settings.setValue("lineType", ui->lineTypeComboBox->currentIndex());

  settings.setValue("vertexType", ui->vertexTypeComboBox->currentIndex());
  settings.setValue("vertexColorR", ui->vertexColorRSlider->value());
  settings.setValue("vertexColorG", ui->vertexColorGSlider->value());
  settings.setValue("vertexColorB", ui->vertexColorBSlider->value());
  settings.setValue("vertexWidth", ui->vertexWidthSpinBox->value());

  settings.setValue("backgroundColorR", ui->backgroundColorRSlider->value());
  settings.setValue("backgroundColorG", ui->backgroundColorGSlider->value());
  settings.setValue("backgroundColorB", ui->backgroundColorBSlider->value());
}

void MainWindow::loadSettings() {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  if (settings.contains("filename")) {
    currentFilename = settings.value("filename").toString();
    char* cStr = qstrdup(currentFilename.toUtf8().constData());
    readObjFile(cStr, m_data);
    delete[] cStr;

    ui->filenameLabel->setText("Модель: " +
                               QFileInfo(currentFilename).fileName());
    ui->openGLWidget->setHidden(false);
    ui->openGLWidget->setData(m_data);
  }
  if (settings.contains("xpos")) {
    xpos = settings.value("xpos").toDouble();
  }
  if (settings.contains("ypos")) {
    ypos = settings.value("ypos").toDouble();
  }
  if (settings.contains("zpos")) {
    zpos = settings.value("zpos").toDouble();
  }
  if (settings.contains("xangle")) {
    xangle = settings.value("xangle").toDouble();
  }
  if (settings.contains("yangle")) {
    yangle = settings.value("yangle").toDouble();
  }
  if (settings.contains("zangle")) {
    zangle = settings.value("zangle").toDouble();
  }
  if (settings.contains("m_scale")) {
    m_scale = settings.value("m_scale").toDouble();
  }
  if (settings.contains("zonex")) {
    ui->zoneXDoubleSpinBox->setValue(settings.value("zonex").toDouble());
  }
  if (settings.contains("zoney")) {
    ui->zoneYDoubleSpinBox->setValue(settings.value("zoney").toDouble());
  }
  if (settings.contains("zonez")) {
    ui->zoneZDoubleSpinBox->setValue(settings.value("zonez").toDouble());
  }
  if (settings.contains("projectionType")) {
    if (settings.value("projectionType").toBool()) {
      ui->centralTypeRadioButton->setChecked(true);
    } else {
      ui->parallelTypeRadioButton->setChecked(true);
    }
  }
  if (settings.contains("lineColorR")) {
    ui->lineColorRSlider->setValue(settings.value("lineColorR").toInt());
  }
  if (settings.contains("lineColorG")) {
    ui->lineColorGSlider->setValue(settings.value("lineColorG").toInt());
  }
  if (settings.contains("lineColorB")) {
    ui->lineColorBSlider->setValue(settings.value("lineColorB").toInt());
  }
  if (settings.contains("linethickness")) {
    ui->thicknessSpinBox->setValue(settings.value("linethickness").toInt());
  }
  if (settings.contains("lineType")) {
    ui->lineTypeComboBox->setCurrentIndex((settings.value("lineType").toInt()));
  }
  if (settings.contains("vertexType")) {
    ui->vertexTypeComboBox->setCurrentIndex(
        settings.value("vertexType").toInt());
  }
  if (settings.contains("vertexColorR")) {
    ui->vertexColorRSlider->setValue(settings.value("vertexColorR").toInt());
  }
  if (settings.contains("vertexColorG")) {
    ui->vertexColorGSlider->setValue(settings.value("vertexColorG").toInt());
  }
  if (settings.contains("vertexColorB")) {
    ui->vertexColorBSlider->setValue(settings.value("vertexColorB").toInt());
  }
  if (settings.contains("vertexWidth")) {
    ui->vertexWidthSpinBox->setValue(settings.value("vertexWidth").toInt());
  }
  if (settings.contains("backgroundColorR")) {
    ui->backgroundColorRSlider->setValue(
        settings.value("backgroundColorR").toInt());
  }
  if (settings.contains("backgroundColorG")) {
    ui->backgroundColorGSlider->setValue(
        settings.value("backgroundColorG").toInt());
  }
  if (settings.contains("backgroundColorB")) {
    ui->backgroundColorBSlider->setValue(
        settings.value("backgroundColorB").toInt());
  }
}

void MainWindow::on_saveImageButton_clicked() {
  QPixmap screenshot = ui->openGLWidget->grab();
  QString filePath = QFileDialog::getSaveFileName(this, "Save Screenshot", "",
                                                  "Images (*.png *.jpg)");
  if (!filePath.isEmpty()) {
    screenshot.save(filePath);
  } else {
    QMessageBox::warning(this, "", "Не удалось сохранить файл");
  }
}

void MainWindow::on_saveGifButton_clicked() {
  QString filename =
      QFileDialog::getSaveFileName(this, tr("Save Gif"), "", tr("Gif (*.gif)"));

  if (!filename.isEmpty()) {
    createGif(filename);
  } else {
    QMessageBox::warning(this, "", "Не удалось сохранить gif");
  }
}
