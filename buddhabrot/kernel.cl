__kernel void get_buddhabrot_image(
                                   __global unsigned int *image,
                                   __global float  *cx,
                                   __global float *cy,
                                   int width,
                                   int height
                                   ) {
    int count = -1;
    float2 z = (float2)(0, 0);
    float2 nz = (float2)(0, 0);
    float2 c = (float2)(cx[get_global_id(0)], cy[get_global_id(0)]);
    __local float2 z_history[1024];
    
    for (int i = 0; i < 1024; i++) {
        nz.x = z.x * z.x  - z.y * z.y + c.x;
        nz.y = 2.0 * z.x * z.y + c.y;
        if (length(nz) > 2) {
            count = i;
            break;
        }
        z_history[i] = nz;
        z = nz;
    }
    
    for (int i = 0; i < count; i++) {
        int ix = (int)(z_history[i].x * width / 4 + width / 2);
        int iy = (int)(z_history[i].y * height / 4 + height / 2);
        
        atomic_inc(&image[ix + iy * width]);
    }
}