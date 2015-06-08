inline bool contain(float xt, float yt, __global float * paras)
{
    float3 a=vload3(0, paras);
    float3 b=vload3(1, paras);
    float temp=xt*xt*a.x+yt*yt*a.y+xt*yt*a.z+xt*b.x+yt*b.y+b.z;
    return temp<0;
}

inline bool containPOL(float xt, float yt, __global float * paras, int id)
{
    int temp=1;
    for (int ii=0; ii<id; ii++)
    {
        float3 a=vload3(ii, paras);
        temp*=((a.x*xt+a.y*yt+a.z))>0;
    }
    return temp;
}

__kernel void drawKernel(__global float * image,
                         __global float * paras,
                         __global float * jitPos,
                         int s,
                         int w,
                         int h,
                         float r,
                         float g,
                         float b,
                         int id)
//                         __global float * out)
{
    int i=get_global_id(0);

    for(int j=0; j<w; j++)
    {
        
        float y=i*1.0f/w;
        float x=j*1.0f/w;
        float xt, yt;
        float count=0.0f;
        for (int k=0; k<s; k++)
        {
            float2 jit=vload2(k, jitPos);
            xt=x+jit.x;
            yt=y+jit.y;
            //count+=(id==0)*contain(xt, yt, paras);
            //count+=(id!=0)*containPOL(xt, yt, paras, id);
            if (id==0) count+=contain(xt, yt, paras);
            else count+=containPOL(xt, yt, paras, id);
        }
        count=count/s;

        float4 pix=vload4((i*w+j), image);
        float ft=1.0f-count;
        pix.x=pix.x*ft+r*count;
        pix.y=pix.y*ft+g*count;
        pix.z=pix.z*ft+b*count;
        pix.w=1;
        vstore4(pix, ((i*w)+j), image);
    }
}

