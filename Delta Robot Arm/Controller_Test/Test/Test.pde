import processing.serial.*;
import net.java.games.input.*;
import org.gamecontrolplus.*;
import org.gamecontrolplus.gui.*;
import cc.arduino.*;
import org.firmata.*;

ControlDevice cont; //
ControlIO control;
Arduino arduino;
float x_axis;
float y_axis;
float z_axis;

// robot geometry
 // (look at pics above for explanation)
float e = 30;     // end effector
float f = 65;     // base
float re = 135;
float rf = 100;
 //Trig stuff
float sqrt3 = sqrt(3.0);
float pi = 3.141592653;    // PI
float sin120 = sqrt(3)/2.0;   
float cos120 = -0.5;        
float tan60 = sqrt(3);
float sin30 = 0.5;
float tan30 = 1/sqrt(3);

void setup() {
  size(360, 200);
  control = ControlIO.getInstance(this);
  cont = control.getMatchedDevice("Delta");

  if (cont == null) {
    println("not today chump"); // write better exit statements than me
    System.exit(-1);
  }
  // println(Arduino.list());
  arduino = new Arduino(this, Arduino.list()[1], 57600);
  arduino.pinMode(2, Arduino.SERVO);
  arduino.pinMode(3, Arduino.SERVO);
  arduino.pinMode(4, Arduino.SERVO);
}

public void getUserInput() {
  x_axis = map(cont.getSlider("X_axis").getValue() , -1, 1, -64.664, 64.664);
  y_axis = map(cont.getSlider("Y_axis").getValue() , -1, 1, -64.664, 64.664);
  z_axis = map(cont.getSlider("Y_rot").getValue(), -1, 1, -198.809, -69.481);
}

void draw() {
  getUserInput();
  float[] serv_array = delta_calcInverse(x_axis, y_axis, z_axis);
  float theta1 = map(serv_array[0], -25.85, 95.81, 0, 180);
  float theta2 = map(serv_array[1], -39.36, 95.85, 0, 180);
  float theta3 = map(serv_array[2], -39.36, 95.85, 0, 180);
  background(170,100,255);
  arduino.servoWrite(2, int(theta2) + 50);
  arduino.servoWrite(3, int(theta1) + 50);
  arduino.servoWrite(4, int(theta3) + 50);
  print(theta1 + "" + theta2 + "" + theta3);
  println();
}


// forward kinematics: (theta1, theta2, theta3) -> (x0, y0, z0)
 // returned status: 0=OK, -1=non-existing position
float[] delta_calcForward(float theta1, float theta2, float theta3) {
  
     float x0 = -1.0;
     float y0 = -1.0; 
     float z0 = -1.0;
     
     float t = (f-e)*tan30/2;
     float dtr = pi/(float)180.0;
 
     theta1 *= dtr;
     theta2 *= dtr;
     theta3 *= dtr;
 
     float y1 = -(t + rf*cos(theta1));
     float z1 = -rf*sin(theta1);
 
     float y2 = (t + rf*cos(theta2))*sin30;
     float x2 = y2*tan60;
     float z2 = -rf*sin(theta2);
 
     float y3 = (t + rf*cos(theta3))*sin30;
     float x3 = -y3*tan60;
     float z3 = -rf*sin(theta3);
 
     float dnm = (y2-y1)*x3-(y3-y1)*x2;
 
     float w1 = y1*y1 + z1*z1;
     float w2 = x2*x2 + y2*y2 + z2*z2;
     float w3 = x3*x3 + y3*y3 + z3*z3;
     
     // x = (a1*z + b1)/dnm
     float a1 = (z2-z1)*(y3-y1)-(z3-z1)*(y2-y1);
     float b1 = -((w2-w1)*(y3-y1)-(w3-w1)*(y2-y1))/2.0;
 
     // y = (a2*z + b2)/dnm;
     float a2 = -(z2-z1)*x3+(z3-z1)*x2;
     float b2 = ((w2-w1)*x3 - (w3-w1)*x2)/2.0;
 
     // a*z^2 + b*z + c = 0
     float a = a1*a1 + a2*a2 + dnm*dnm;
     float b = 2*(a1*b1 + a2*(b2-y1*dnm) - z1*dnm*dnm);
     float c = (b2-y1*dnm)*(b2-y1*dnm) + b1*b1 + dnm*dnm*(z1*z1 - re*re);
  
     // discriminant
     float d = b*b - (float)4.0*a*c;
     if (d < 0) return new float[]{x0,y0,z0}; // non-existing point
 
     z0 = -(float)0.5*(b+sqrt(d))/a;
     x0 = (a1*z0 + b1)/dnm;
     y0 = (a2*z0 + b2)/dnm;
     float[] axis_arr = new float[]{ x0,y0,z0 };
     return axis_arr;
 }
 
 // inverse kinematics
 // helper functions, calculates angle theta1 (for YZ-pane)
 float delta_calcAngleYZ(float x0, float y0, float z0) {
     float theta;
     float y1 = -0.5 * 0.57735 * f; // f/2 * tg 30
     y0 -= 0.5 * 0.57735    * e;    // shift center to edge
     // z = a + b*y
     float a = (x0*x0 + y0*y0 + z0*z0 +rf*rf - re*re - y1*y1)/(2*z0);
     float b = (y1-y0)/z0;
     // discriminant
     float d = -(a+b*y1)*(a+b*y1)+rf*(b*b*rf+rf); 
     if (d < 0) return -1.0; // non-existing point
     float yj = (y1 - a*b - sqrt(d))/(b*b + 1); // choosing outer point
     float zj = a + b*yj;
     theta = 180.0*atan(-zj/(y1 - yj))/pi + ((yj>y1)?180.0:0.0);
     return theta;
 }
 
 // inverse kinematics: (x0, y0, z0) -> (theta1, theta2, theta3)
 // returned status: 0=OK, -1=non-existing position
 float[] delta_calcInverse(float x0, float y0, float z0) {
     float theta1; 
     float theta2; 
     float theta3;
     theta1 = theta2 = theta3 = -1.0;
     theta1 = delta_calcAngleYZ(x0, y0, z0);
     if (theta1 != -1.0) theta2 = delta_calcAngleYZ(x0*cos120 + y0*sin120, y0*cos120-x0*sin120, z0);  // rotate coords to +120 deg
     if (theta2 != -1.0) theta3 = delta_calcAngleYZ(x0*cos120 - y0*sin120, y0*cos120+x0*sin120, z0);  // rotate coords to -120 deg
     float[] theta_arr = new float[] { theta1, theta2, theta3 };
     return theta_arr;
 }
