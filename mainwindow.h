#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void draw_scene();
    void draw_arm();
    void draw_ray(float x1, float y1, float x2, float y2);
    void calc_angle();


private slots:
    void on_lower_arm_angle_valueChanged(double arg1);

    void on_upper_arm_angle_valueChanged(double arg1);

    void on_arm_x_valueChanged(double arg1);

    void on_arm_y_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
