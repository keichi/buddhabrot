//
//  cl_util.hpp
//  buddhabrot
//
//  Created by Keichi Takahashi on 12/03/24.
//  Copyright (c) 2012年 Osaka University. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "cl.hpp"

void print_platform_info(const cl::Platform& platform) {
    std::string name, profile, vendor, version;
    
    platform.getInfo(CL_PLATFORM_NAME, &name);
    platform.getInfo(CL_PLATFORM_PROFILE, &profile);
    platform.getInfo(CL_PLATFORM_VENDOR, &vendor);
    platform.getInfo(CL_PLATFORM_VERSION, &version);
    
    std::cout << "Name: " << name << std::endl;
    std::cout << "Profile: " << profile << std::endl;
    std::cout << "Vendor: " << vendor << std::endl;
    std::cout << "Version: " << version << std::endl;
}

void print_device_info(const cl::Device& device) {
    std::string name, profile, vendor, version;
    
    device.getInfo(CL_DEVICE_NAME, &name);
    device.getInfo(CL_DEVICE_PROFILE, &profile);
    device.getInfo(CL_DEVICE_VENDOR, &vendor);
    device.getInfo(CL_DEVICE_VERSION, &version);
    
    std::cout << "Name: " << name << std::endl;
    std::cout << "Profile: " << profile << std::endl;
    std::cout << "Vendor: " << vendor << std::endl;
    std::cout << "Version: " << version << std::endl;
}

cl::Program load_cl_program(const char* filename, const cl::Context& context) {
    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
        
    std::ifstream ifs(filename);
    std::istreambuf_iterator<char> begin(ifs);
    std::istreambuf_iterator<char> end;
    std::string contents(begin, end);
    cl::Program::Sources sources(1, std::make_pair(contents.c_str(), contents.size()));

    cl::Program::Program program(context, sources);
    program.build(devices);
    
    ifs.close();
    
    return program;
}