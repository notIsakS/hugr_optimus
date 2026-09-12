#include <algorithm>
#include <chrono>
#include <memory>
#include <string>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include <std_msgs/msg/header.hpp>
#include <geometry_msgs/msg/vector3.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <sensor_msgs/msg/joy_feedback.hpp>

using namespace std::placeholders;
using namespace std::chrono_literals;

class ThrustAllocationNode : public rclcpp::Node
{
private:
// sub'n'pub
    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscriber_;
    rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

// Hull constants
    double mass_;
    double d1x_;
    double d1y_;
    double d2x_;
    double d2y_;
    double d3x_;
    double d3y_;
// Joystick commands
    double Fx_;
    double Fy_;
    double tau_;
    double deadzone_;
// Thruster output
    double T1_;
    double T2_;
    double T3_;

public:
    ThrustAllocationNode() : Node("thrust_allocation_node")
    {
// Parametres
        // Hull
        this->declare_parameter("mass", rclcpp::PARAMETER_DOUBLE);
        this->declare_parameter("d1x", 0.35);
        this->declare_parameter("d1y", 0.35);
        this->declare_parameter("d2x", 0.35);
        this->declare_parameter("d2y", 0.35);
        this->declare_parameter("d3x", 0.35);
        this->declare_parameter("d3y", 0.35);
        this->declare_parameter("deadzone", 0.05);

        mass_ = this->get_parameter("mass").as_double();
        d1x_  = this->get_parameter("d1x").as_double();
        d1y_  = this->get_parameter("d1y").as_double();
        d2x_  = this->get_parameter("d2x").as_double();
        d2y_  = this->get_parameter("d2y").as_double();
        d3x_  = this->get_parameter("d3x").as_double();
        d3y_  = this->get_parameter("d3y").as_double();
        
        // Controller
        deadzone_ = this->get_parameter("deadzone").as_double();

// Recieve joy
        subscriber_ = create_subscription<sensor_msgs::msg::Joy> (
            "/joy", 10,
            std::bind(&ThrustAllocationNode::joystickCommands, this, _1));

// Publish
        publisher_ = create_publisher<geometry_msgs::msg::Vector3>(
            "thrust_allocation", 10);
        
        auto timer_callback =
        [this]() -> void {
            auto message = geometry_msgs::msg::Vector3();
            message.x = Fx_;
            message.y = Fy_;
            message.z = tau_;
            RCLCPP_INFO(this->get_logger(),
            "Publishing: '%.2lf', '%.2lf', '%.2lf'",
            message.x, message.y, message.z);
            this->publisher_->publish(message);
        };
        timer_ = this->create_wall_timer(20ms, timer_callback);
    }

// Interpret sticks
    void joystickCommands(const sensor_msgs::msg::Joy::ConstSharedPtr msg){
        // Initializing declarations
        Fx_  = msg->axes.at(1);
        Fy_  = -msg->axes.at(0);
        tau_ = -msg->axes.at(3);
        deadzone_ = 0.05;
        // Simple deadzone logic
        if(abs(Fx_)  <= deadzone_)
            Fx_ = 0;
        if(abs(Fy_)  <= deadzone_)
            Fy_ = 0;
        if(abs(tau_) <= deadzone_)
            tau_ = 0;
    }

// Math
/*
    void thrusterForce(const geometry_msgs::msg::Twist::ConstSharedPtr msg){
    // to be added, further inspection needed //
        T1_ = 0;
        T2_ = 0;
        T3_ = 0;
    }
*/
};

int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ThrustAllocationNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}