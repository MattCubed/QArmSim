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
    void move_arm();

    void on_lower_arm_angle_valueChanged(double arg1);

    void on_upper_arm_angle_valueChanged(double arg1);

    void on_arm_x_valueChanged(double arg1);

    void on_arm_y_valueChanged(double arg1);

    void on_cone_3_clicked();

    void on_grab_clicked();

    void on_hold_clicked();

    void on_place_cone_3_clicked();

    void on_pre_place_clicked();

private:
    Ui::MainWindow *ui;
};

class runSim : public QObject {
    Q_OBJECT

public slots:
    void runThread();
signals:
    void move_arm();
    void command_arm();

};

#endif // MAINWINDOW_H
