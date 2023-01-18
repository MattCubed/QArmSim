#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QGraphicsScene>
#include <QtMath>
#include <QDebug>


//Low pivot to ground 10.75
//Low pivot is 8 inches off center
//Low pivot is 7 inches from frame


QGraphicsScene *sim;
QPen line_pen(Qt::blue);

float l_arm = 36;
float u_arm = 28.84;

float robot_len = 24;

float pivot_to_frame = 7;
float pivot_to_center = 8;

float l_min = -75;
float l_max = 75;

float sim_w, sim_h = 0;
float l_arm_angle, u_arm_angle = 0;
float arm_x, arm_y = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sim = new QGraphicsScene(this);
    ui->main_sim->setScene(sim);

    MainWindow::setFixedSize(1200, 800);

    line_pen.setWidth(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int in_to_pix(int input)
{
    return input * 5;
}

float sinDEG(float inputAngle)
{
    return sin(inputAngle*(M_PI/180));
}

float cosDEG(float inputAngle)
{
    return cos(inputAngle*(M_PI/180));
}

float atanfDEG(float inputX, float inputY)
{
    return atanf(inputX / inputY) * (180/M_PI);
}

void polar_TO_cartesian(float inputDistance, float inputAngle, float *outputX, float *outputY)
{
    *outputX = sinDEG(inputAngle) * inputDistance;
    *outputY = cosDEG(inputAngle) * inputDistance;
}

float wrap360(float input)
{
    input = fmod(input, 360.0);
//    if(input > 180) input -= 180;
//    else if(input < -180) input += 180;

    return input;
}

void MainWindow::draw_ray(float x1, float y1, float x2, float y2)
{
    sim->addLine(
        in_to_pix(x1),
        (sim_h / 2) - in_to_pix(y1 + 20),
        in_to_pix(x2),
        (sim_h / 2) - in_to_pix(y2 + 20),
        line_pen
    );
}

void MainWindow::draw_scene()
{
    sim->clear();

    sim_w = ui->main_sim->size().width() - 4;
    sim_h = ui->main_sim->size().height() - 4;

    sim->addRect(-sim_w / 2, -sim_h / 2, sim_w, sim_h, line_pen);

    //Attachment
    draw_ray(0, 0, robot_len / 2 - pivot_to_center, -pivot_to_frame);
    draw_ray(0, 0, -robot_len / 2 - pivot_to_center, -pivot_to_frame);

    //Charging station
    draw_ray(-38, -20, -24, -11);
    draw_ray(38, -20, 24, -11);
    draw_ray(24, -11, -24, -11);

    //Frame
    draw_ray(-robot_len / 2 - pivot_to_center, -pivot_to_frame, robot_len / 2  - pivot_to_center, -pivot_to_frame);
    draw_ray(-robot_len / 2  - pivot_to_center, -pivot_to_frame - 2, robot_len / 2 - pivot_to_center, -pivot_to_frame - 2);
    draw_ray(-robot_len / 2  - pivot_to_center, -pivot_to_frame, -robot_len / 2  - pivot_to_center, -pivot_to_frame - 2);
    draw_ray(robot_len / 2 - pivot_to_center, -pivot_to_frame, robot_len / 2 - pivot_to_center, -pivot_to_frame - 2);

    //Center
    draw_ray(-1, 0, 1, 0);
    draw_ray(0, -1, 0, 1);

    //Target
    float target_x = ui->arm_x->value();
    float target_y = ui->arm_y->value();
    draw_ray(target_x - 1, target_y, target_x + 1, target_y);
    draw_ray(target_x, target_y - 1, target_x, target_y + 1);

    sim->addEllipse(-in_to_pix(l_arm + u_arm), -in_to_pix(l_arm + u_arm) / 2, in_to_pix(l_arm + u_arm) * 2, in_to_pix(l_arm + u_arm) * 2, line_pen);
}

void MainWindow::draw_arm()
{
    float l_arm_x, l_arm_y, u_arm_x, u_arm_y;

    polar_TO_cartesian(l_arm, l_arm_angle, &l_arm_x, &l_arm_y);
    polar_TO_cartesian(u_arm, u_arm_angle, &u_arm_x, &u_arm_y);

    draw_ray(0, 0, l_arm_x, l_arm_y);
    draw_ray(l_arm_x, l_arm_y, u_arm_x + l_arm_x, u_arm_y + l_arm_y);
}

void MainWindow::on_lower_arm_angle_valueChanged(double arg1)
{
    //Setting actual position of lower arm
    arg1 = wrap360(arg1);

    if(arg1 > l_max) arg1 = l_max;
    else if(arg1 < l_min) arg1 = l_min;

    l_arm_angle = arg1;
    draw_scene();
    draw_arm();
}

void MainWindow::on_upper_arm_angle_valueChanged(double arg1)
{
    //Setting actual position of upper arm
    u_arm_angle = arg1;
    draw_scene();
    draw_arm();
}

float sign(float input)
{
    if(input > 0) return 1;
    else return -1;
}

void MainWindow::calc_angle()
{
    float len_z = qSqrt(arm_x * arm_x + arm_y * arm_y);

    float ang_y = sign(arm_y) * (qAcos((arm_x * arm_x + len_z * len_z - arm_y * arm_y) / (2 * len_z * arm_x)));

    float ang_a = sign(arm_x) * (qAcos((l_arm * l_arm + len_z * len_z - u_arm * u_arm) / (2 * len_z * l_arm)));

    ang_y *= (180 / M_PI);
    ang_a *= (180 / M_PI);

    float l_arm_angle = 90 - (ang_a + ang_y);

    float ang_b = sign(arm_x) * (qAcos((u_arm * u_arm + l_arm * l_arm - len_z * len_z) / (2 * l_arm * u_arm)));

    ang_b *= (180 / M_PI);

    float u_arm_angle = l_arm_angle + (180 - ang_b);
    u_arm_angle = wrap360(u_arm_angle);

    if(arm_x < 0 && arm_y < 0)
    {
        l_arm_angle -= 360;
    }

    ui->lower_arm_angle->setValue(l_arm_angle);
    ui->upper_arm_angle->setValue(u_arm_angle);
}

void MainWindow::on_arm_x_valueChanged(double arg1)
{
    if(arg1 == 0) arg1 = 0.01;

    arm_x = arg1;
    calc_angle();
}

void MainWindow::on_arm_y_valueChanged(double arg1)
{
    if(arg1 == 0) arg1 = 0.01;

    arm_y = arg1;
    calc_angle();
}
