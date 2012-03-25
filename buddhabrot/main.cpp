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
        cl::CommandQueue queue(context, devices[0]);
        
        cl::Program program = load_cl_program("OpenCL/kernel.cl.x86_64.bc", context);
        cl::Kernel kernel(program, "hello");
        cl::Buffer buffer(context, CL_MEM_READ_WRITE, 6);
        kernel.setArg(0, buffer);
        
        queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(1), cl::NullRange, NULL);
        
        char mem[6];
        queue.enqueueReadBuffer(buffer, CL_TRUE, 0, 6, mem);
        printf("%s\n", mem);
    }
    
    return 0;
}

