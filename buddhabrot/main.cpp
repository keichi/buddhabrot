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
#include <fstream>

#include "cl.hpp"
#include "cl_util.hpp"

#define IMAGE_WIDTH     (1200)
#define IMAGE_HEIGHT    (800)

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
        cl::Kernel get_mandelbrot_image(program, "get_mandelbrot_image");
        
        unsigned char img_buf[IMAGE_WIDTH * IMAGE_WIDTH];
        cl::Buffer img(context, CL_MEM_WRITE_ONLY, IMAGE_WIDTH * IMAGE_WIDTH);
        get_mandelbrot_image.setArg(0, img);
        
        queue.enqueueNDRangeKernel(get_mandelbrot_image, cl::NullRange, cl::NDRange(IMAGE_WIDTH, IMAGE_HEIGHT), cl::NullRange);
        
        queue.enqueueReadBuffer(img, CL_TRUE, 0, IMAGE_WIDTH * IMAGE_WIDTH, img_buf);
        
        std::ofstream ofs("/Users/Keichi/Desktop/mandelbrot.ppm");
        ofs << "P3" << std::endl;
        ofs << "#Generated by mandelbrot renderer" << std::endl;
        ofs << IMAGE_WIDTH << " " << IMAGE_HEIGHT<< std::endl;
        ofs << "255" << std::endl;

        for (int i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT; i++) {
            int val = (unsigned int)img_buf[i];
            
            ofs << val << " " << val << " " << val << std::endl;
        }
        ofs.close();
    }
    
    return 0;
}

