__kernel void vector_add(__global float *a, __global float *b, __global float *c) {
    int i = get_global_id(0);
    
    c[i] = a[i] + b[i];
}

__kernel void get_mandelbrot_image(__global uchar* image) {
    float x = (float)get_global_id(0) / get_global_size(0) * 3 - 2;
    float y = (float)get_global_id(1) / get_global_size(1) * 2 - 1;
    int width = get_global_size(0);
    
    float2 z = (float2)(0, 0);
    int count = -1;
    
    for (int i = 0; i < 100; i++) {
        float2 nz = (float2)(0, 0);
        nz.x = z.x * z.x  - z.y * z.y + x;
        nz.y = 2.0 * z.x * z.y + y;
        if (length(nz) > 2) {
            count = i;
            break;
        }
        z = nz;
    }
    
    if (count == -1) {
        image[get_global_id(0) + get_global_id(1) * width] = 0xff;
    } else {
        image[get_global_id(0) + get_global_id(1) * width] = 0x0;
    }
}