
#include <kipr/botball.h>
#include <comp_lib.h>
#include <L_Mechanism.h>
#include <arm_hand_position.h>





//int grey;
//int analog_white = grey - 500; 
//int analog_black = grey - 500;
int white = 1;
int black = 2;
int physical = 3;
double bias = 0.0;
void move(int l_speed,int r_speed){
    mav(right_motor,r_speed);
    mav(left_motor,l_speed);
}


void square_up(int ending,int speed){
    if(speed > 0 && speed < 600){
        black_speed = 1.2*speed;
    }
    else{black_speed = 0.25*speed;}
    if(ending == 1 || ending == 2){
        while(1){
            if(analog(left_IR)<analog_white && analog(right_IR)<analog_white){
                move(speed,speed);
            }
            if(analog(right_IR)>analog_white){
                move(speed,stop);
            }
            if(analog(left_IR)>analog_white){
                move(stop,speed);
            }
            if(analog(left_IR)>analog_white && analog(right_IR)>analog_white) {
                move(stop,stop);
                break;
            }
        }
    }
    if(ending == 3){
        ao();
    }
    switch(ending){
        case 3:
            {
                while(1){
                    if(digital(digital_right)==0 && digital(digital_left)==0){
                        move(speed,speed);
                    }
                    if(digital(digital_right)==1){
                        move(speed,stop);
                    }
                    if(digital(digital_left)==1){
                        move(stop,speed);
                    }
                    if(digital(digital_right)==1 && digital(digital_left)==1){
                        move(stop,stop);
                        break;
                    }
                }
            }
        case 1:
            {
                while(1){
                    if(analog(left_IR)>analog_white && analog(right_IR)>analog_white){
                        move(black_speed,black_speed);
                    }
                    if(analog(left_IR)<analog_white){
                        move(stop,black_speed);
                    }
                    if(analog(right_IR)<analog_white){
                        move(black_speed,stop);
                    }
                    if(analog(left_IR)<analog_white && analog(right_IR)<analog_white){
                        move(stop,stop);
                        break;
                    }
                }
            }
        case 2:
            {
                while(1){
                    if(analog(left_IR)>analog_white && analog(right_IR)>analog_white){
                        move(-1*black_speed,-1*black_speed);
                    }
                    if(analog(left_IR)<analog_white){
                        move(stop,-1*black_speed);
                    }
                    if(analog(right_IR)<analog_white){
                        move(-1*black_speed,stop);
                    }
                    if(analog(left_IR)<analog_white && analog(right_IR)<analog_white){
                        move(stop,stop);
                        break;
                    }
                }
            }
    }
}

double calibrate_gyro(){
    int i = 0;
    double avg = 0;
    while( i < 50){
        avg += gyro_z();
        msleep(1);
        i++;
        printf("Gyro Z: %d\n",gyro_z());
    }
    bias = avg / 50.0;
    printf("New Bias: %f\n", bias);
    return bias;
}

void drive_with_gyro(int speed, double time){
    calibrate_gyro();
    double startTime = seconds();
    double theta = 0;
    while(seconds() - startTime < time){
        if (theta < 1000 && theta > -1000){
            mav(right_motor, speed);
            mav(left_motor, speed);
        }
        else if (theta < 1000){
            mav(right_motor, speed + 100);
            mav(left_motor, speed - 100);
        }
        else{
            mav(right_motor, speed - 100);
            mav(left_motor, speed + 100);
        }
        msleep(10);
        theta += (gyro_z() - bias) * 10;
        printf("%f",theta);
    }
    ao();
}

void PID_gyro_drive(int speed, double time){
    calibrate_gyro();
    double startTime = seconds();
    double theta = 0;
    while((seconds() - startTime) < time){
        if(speed > 0){
            mav(right_motor, (speed - (speed * (theta/100000))));            
            mav(left_motor, (speed + (speed * theta/100000)));
        }


        else{
            mav(left_motor, (speed - (speed * theta/100000)));            
            mav(right_motor, (speed + (speed * (theta/100000))));
        }
        msleep(10);
        theta += (gyro_z() - bias) * 10;
    }
    move(0,0);
}

void turn_with_gyro_create(int speed, int deg){
    double theta = 0;
    int targetTheta; 
    switch(deg){

        case 45:
            targetTheta = target_theta_45;
            create_drive_direct(speed,speed*-1);
            break;
        case 90:
            targetTheta = target_theta_90; //507500
            create_drive_direct(speed,speed*-1);
            break;
        case 180:
            targetTheta = target_theta_180;
            create_drive_direct(speed,speed*-1);
            break;
        case 360:
            targetTheta = target_theta_360;
            create_drive_direct(speed,speed*-1);
            break;
        case -45:
            targetTheta = target_theta_m45;
            create_drive_direct(speed*-1,speed);
            break;
        case -90:
            targetTheta = target_theta_m90;
            create_drive_direct(speed*-1,speed);
            break;
        case -180:
            targetTheta = target_theta_m180;
            create_drive_direct(speed*-1,speed);
            break;
        case -360:
            targetTheta = target_theta_m360;
            create_drive_direct(speed*-1,speed);
            break;
        default:
            targetTheta = 0;
            break;
    }  
    while(theta < targetTheta){
        msleep(10);
        theta += abs(gyro_z() - bias) * 10;
        printf("Turn Gyro Z: %d\n",gyro_z());
    }
    create_drive_direct(0,0);
}

void turn_with_gyro(int speed, int deg){
    calibrate_gyro();
    double theta = 0;

    int targetTheta; 
    switch(deg){
        case 80:
            targetTheta = target_theta_building_2;
            move(speed,speed*-1);
            break;
        case 45:
            targetTheta = target_theta_45;
            move(speed,speed*-1);
            break;
        case 90:
            targetTheta = target_theta_90;
            move(speed,speed*-1);
            break;
        case 180:
            targetTheta = target_theta_180;
            move(speed,speed*-1);
            break;
        case 360:
            targetTheta = target_theta_360;
            move(speed,speed*-1);
            break;
        case -45:
            targetTheta = target_theta_m45;
            move(speed*-1,speed);
            break;
        case -90:
            targetTheta = target_theta_m90;
            move(speed*-1,speed);
            break;
        case -180:
            targetTheta = target_theta_m180;
            move(speed*-1,speed);
            break;
        case -360:
            targetTheta = target_theta_m360;
            move(speed*-1,speed);
            break;
        default:
            targetTheta = 0;
            break;
    }  
    while(theta < targetTheta){
        msleep(10);
        theta += abs(gyro_z() - bias) * 10;
        printf("Turn Gyro Z: %d\n",gyro_z());
    }
    mav(right_motor, 0);
    mav(right_motor, 0);
    ao();
}

void backward_linefollow(int distance)
{
    cmpc(1);
    cmpc(2);
    while(gmpc(1)<distance)
    {
        mav(left_motor,.55*(analog(front_IR)));
        mav(right_motor,.55*(3400-analog(front_IR)));
    }
    ao();
}

void cube_verify()
{
    if(buffer(ET)>1600)
    {
        move(-400,-400);
        while(buffer(ET)>1460){}
        move(0,0);
    }
    if(buffer(ET)<1600)
    {
        move(400,400);
        while(buffer(ET)<1460){}
        move(0,0);
    }
}
void PID_gyro_drive_distance(int speed,int distance)
{


    calibrate_gyro();
    double theta = 0;
    while(buffer(ET)< (distance-30) || buffer(ET)>(distance + 30))
    {
        if(buffer(ET) < distance)
        {
            mav(right_motor, (speed - (speed * (theta/100000))));            
            mav(left_motor, (speed + (speed * theta/100000)));
        }
        else
        {

            mav(right_motor, -200);            
            mav(left_motor,-200);
        }
        msleep(10);
        printf("%d/n",buffer(ET));
        theta += (gyro_z() - bias) * 10;
    }


    move(0,0);
}

void pre_test(){

    square_up(black,600*multiplier);
    printf("did it square up??? If so move to line now");
    while(left_button()!=1){}
    backward_linefollow(2000);
    ao();
    printf("did it linefollow??? If so move to ET test");
    while(left_button()!=1){}
    slow_arm(arm_min,fast);
    PID_gyro_drive_distance(600*multiplier,2400);
    printf("If evertything works setup in box and begin");
    while(left_button()!=1){}
}
/*void calibration_square(){

     analog_white = (analog(0)+analog(1))/2;
    while((analog(0)-white)1100||(analog(1)-white)<1100){

        move(600,600);
        move(600,600);
        printf("%d\n",analog(1));
    }
    ao();
    analog_black = (analog(0)+analog(1))/2;
    grey = (analog_white + analog_black)/2;
}*/

void valve_verify()
{   
    int n=0;
    while(n < 10)
    {
        mav(left_motor,.33*(analog(front_IR)));
        mav(right_motor,.33*(3400-analog(front_IR)));
        if(analog(ET)>1530)
        {
            n = n + 1;
        }
        else
        {
            n = 0;
        }
    }
    ao();
    n = 0;
    while(n < 50)
    {
        motor(left_motor,.33 *(1530 - analog(front_IR)));
        motor(right_motor,.33*(1530 - analog(front_IR)));
        msleep(5);
        if(analog(ET)<1530){
            
           n = n + 1;
        }
        else
        {
            n = 0;
        }
    }
    move(0,0);
    ao();
    n = 0;
}
/*void PID_get_cube()
{
    while(ET > 1550)
    {
        if(analog(3) > 1615)   
        {
            move(1300,900);
            msleep(3);
        }
        else if (analog(3)<1585)
        {
            move(900,1300);  
            msleep(3);
        }
        else
        {
            move(1200,1200);
            msleep(3);
        }
    }
    while(n<50){
        motor(left_motor,.4*(1440-analog(ET)));
        motor(right_motor,.4*(1440-analog(ET)));
        msleep(5);
    }
    if(analog(ET)>1440)
    {
        n = n+1;
    }
    else{
        n=0;
    }
        
}*/