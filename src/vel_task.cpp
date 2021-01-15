#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
float t_last=0;
float delta_t;
float x_last;
float y_last;
float z_last;
float roll_last;
float yaw_last;
float pitch_last;
geometry_msgs::Twist transf_;

void get_omni_pose(const geometry_msgs::PoseStamped& pose_stamped_){
    t_last =  ros::Time::now().toSec();
    delta_t = ros::Time::now().toSec()-t_last;
    x_last = pose_stamped_.pose.position.x;
    y_last = pose_stamped_.pose.position.y;
    z_last = pose_stamped_.pose.position.z; 
    roll_last = pose_stamped_.pose.orientation.x;
    yaw_last = pose_stamped_.pose.orientation.y;
    pitch_last = pose_stamped_.pose.orientation.z;
    transf_.linear.x = (pose_stamped_.pose.position.x-x_last)/delta_t;  
    transf_.linear.y = (pose_stamped_.pose.position.y-y_last)/delta_t;
    transf_.linear.z = (pose_stamped_.pose.position.z-z_last)/delta_t;
    transf_.angular.x = (pose_stamped_.pose.orientation.x-roll_last)/delta_t;
    transf_.angular.y = (pose_stamped_.pose.orientation.y-yaw_last)/delta_t;
    transf_.angular.z = (pose_stamped_.pose.orientation.z-pitch_last)/delta_t;   
    
//std::cout<< x_last<< std::endl;
}
int main(int argc, char** argv)
{
  ros::init(argc, argv, "pkg_task_space_node");
  ros::NodeHandle nh_;
  ros::Publisher pub_ = nh_.advertise<geometry_msgs::Twist>("ur5/cartesian_velocity_controller_sim/command_cart_vel",1000);
  ros::Subscriber sub_ = nh_.subscribe("omni1_pose", 1000, get_omni_pose);
  ros::Rate loop_rate(10);
  

  while(ros::ok()){
      
geometry_msgs::Twist cmd_vel_;
 
  cmd_vel_.linear.x = transf_.linear.x;
  cmd_vel_.linear.y = transf_.linear.y;
  cmd_vel_.linear.z  =transf_.linear.z;
  cmd_vel_.angular.x =transf_.angular.x;
  cmd_vel_.angular.y =transf_.angular.y;
  cmd_vel_.angular.z =transf_.angular.z;
  std::cout<< cmd_vel_<< std::endl;
  pub_.publish(cmd_vel_);
      ros::spinOnce();
      loop_rate.sleep();
  }
  ros::spin();
  
  return 0;
}
  
