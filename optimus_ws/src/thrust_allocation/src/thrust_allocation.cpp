#include <algorithm>
#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include <std_msgs/msg/header.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <sensor_msgs/msg/joy_feedback.hpp>

using namespace std::placeholders;

class ThrustAllocationNode : public rclcpp::Node 
{
public:
// Recieve joy
    ThrustAllocationNode() : Node("thrust_allocation_node") 
    {
    input_sub_ = create_subscription<sensor_msgs::msg::Joy> (
        "sensor_msgs/msg/Joy", 100, std::bind(&ThrustAllocationNode::commands, this, _1));
    }

// Interpret sticks

// Math
    void commands(const sensor_msgs::msg::Joy::ConstSharedPtr msg){
        
    }
// Publish


private:
    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscriber_;
    float32 input_[];
    int32 output_[];
    int8 m2;
    int8 m3;
    int8 m1;
};

int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);
    auto node = std::make_unique<ThrustAllocationNode>();    
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}