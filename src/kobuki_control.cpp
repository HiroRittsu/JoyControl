#include "ros/ros.h"
#include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Float64MultiArray.h>


using namespace std;

ros::Publisher key_velocity;

bool flag = false;

double send_data[4];

void publish_msgs(const ros::Publisher& pub, double x, double az) {

	geometry_msgs::Twist twist;

	twist.linear.x = x;
	twist.angular.z = az;

	pub.publish(twist);

}

void PS4info(const sensor_msgs::Joy::ConstPtr& msg) {

	send_data[0] = msg->buttons[3] * 0.5 + msg->buttons[1] * -0.5;

	//send_data[0] = send_data[0] == 0 ? msg->buttons[1] * -0.3 : 0;

	send_data[2] = msg->buttons[0] * 2 + msg->buttons[2] * -2;

	//send_data[2] = send_data[2] == 0 ? msg->buttons[2] * -1 : 0;

	send_data[1] = 0.03;
	send_data[3] = 0.3;

	//cout << msg->buttons[0] << "\n";

	/*if (msg->buttons[3]) {
		printf("前進\n");
		publish_msgs(key_velocity, 0.2, 0);
	}

	if (msg->buttons[1]) {
		printf("後進\n");
		publish_msgs(key_velocity, -0.2, 0);
	}

	if (msg->buttons[2]) {
		printf("左回転\n");
		publish_msgs(key_velocity, 0.0, -1);
	}

	if (msg->buttons[0]) {
		printf("右回転\n");
		publish_msgs(key_velocity, 0.0, 1);
	}*/



}

void publish_velocity(const ros::Publisher& pub, double data[]) {

	std_msgs::Float64MultiArray info;

	info.data.clear();

	info.data.push_back(data[0]);
	info.data.push_back(data[1]);
	info.data.push_back(data[2]);
	info.data.push_back(data[3]);

	pub.publish(info);

}

int main(int argc, char **argv) {

	ros::init(argc, argv, "reciever");

	ros::NodeHandle n;

	ros::Subscriber sub = n.subscribe("joy", 1000, PS4info);

	//key_info = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
	//key_info = n.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity", 1000);
	key_velocity = n.advertise<std_msgs::Float64MultiArray>("/move/velocity", 1000);

	while (ros::ok()) {

		//for (int i = 0; i < 4; ++i) send_data[i] = 0;

		ros::spinOnce();

		publish_velocity(key_velocity, send_data);

	}

	return 0;

}