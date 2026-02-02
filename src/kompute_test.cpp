#include <rclcpp/rclcpp.hpp>
#include <kompute/Kompute.hpp>
#include <iostream>

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("kompute_verfication_node");

    RCLCPP_INFO(node->get_logger(), "Starting Kompute Verification...");

    try {
        kp::Manager mgr; 

        const auto& props = mgr.getDeviceProperties();
        std::string gpu_name(props.deviceName.data());

        auto deviceTypeToStr = [](vk::PhysicalDeviceType t) {
            switch (t) {
            case vk::PhysicalDeviceType::eDiscreteGpu:
                return "DISCRETE";
            case vk::PhysicalDeviceType::eIntegratedGpu:
                return "INTEGRATED";
            case vk::PhysicalDeviceType::eVirtualGpu:
                return "VIRTUAL";
            case vk::PhysicalDeviceType::eCpu:
                return "CPU";
            default:
                return "OTHER";
            }
        };
        RCLCPP_INFO_STREAM(node->get_logger(), "[UVDARDetectorFastGpu]: Using GPU: " << gpu_name << " Type: " << deviceTypeToStr(props.deviceType));
    } catch (const std::exception& e) {
        RCLCPP_FATAL(node->get_logger(), "Exception during Kompute init: %s", e.what());
        return 1;
    }

    RCLCPP_INFO(node->get_logger(), "Include and Link check passed. Shutting down.");
    
    rclcpp::shutdown();
    return 0;
}