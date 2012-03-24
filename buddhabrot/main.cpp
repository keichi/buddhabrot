//
//  main.cpp
//  buddhabrot
//
//  Created by Keichi Takahashi on 12/03/24.
//  Copyright (c) 2012年 Osaka University. All rights reserved.
//

#define __CL_ENABLE_EXCEPTIONS
#include <iostream>
#include <string>
#include <vector>

#include "cl.hpp"
#include "cl_util.hpp"

int main(int argc, const char * argv[])
{
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    
    std::cout << platforms.size() << " platforms are found." << std::endl;
    for (int i = 0; i < platforms.size(); ++i) {

        std::string separator(80, '=');
        std::cout << separator << std::endl;
        std::cout << "Platform " << i << ":" << std::endl;
        print_platform_info(platforms[i]);
        
        std::vector<cl::Device> devices;
        platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &devices);
        std::cout << devices.size() << " devices are found." << std::endl;
        for (int j = 0; j < devices.size(); ++j) {
            std::string separator(80, '-');
            std::cout << separator << std::endl;
            std::cout << "Device " << i << ":" << std::endl;
            print_device_info(devices[j]);
        }
        
        cl::Context::Context context(devices);
        cl::Program program = load_cl_program("kernel.cl", context);
        cl::Kernel kernel(program, "hello");
        
        cl::CommandQueue queue(context, devices[0]);
        
        cl::Event ev;
        queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(4, 4), cl::NullRange, NULL, &ev);
        ev.wait();
    }
    
    return 0;
}

