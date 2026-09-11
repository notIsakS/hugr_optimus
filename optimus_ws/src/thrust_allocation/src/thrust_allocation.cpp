#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joy.h"
#include "joy/joy.hpp"
#include "joy/game_controller.hpp"

class ThrustAllocationNode : public rclcpp::Node 
{
public:
// Recieve joy
    ThrustAllocationNode() : Node("thrust_allocation_node") 
    {
    input_sub_ = create_subscription<sensor_msgs/msg/Joy> (
        "sensor_msgs/msg/Joy", 100, std::bind(&ThrustAllocationNode::funksjon, this, _1));
    }

    
// Interpret sticks

// Math

// Publish
private:
    rclcpp::Subscription<sensor_msgs/msg/Joy>::SharedPtr subscriber_;
    float32 axes_;
    int32 buttons_;
};

int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);
    auto node = std::make_unique<ThrustAllocationNode>();    
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}