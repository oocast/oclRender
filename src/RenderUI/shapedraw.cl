inline bool ContainEll(float xt, float yt, __global float * paras)
{
    float3 a=vload3(0, paras);
    float3 b=vload3(1, paras);
    float temp=xt*xt*a.x+yt*yt*a.y+xt*yt*a.z+xt*b.x+yt*b.y+b.z;
    return temp<0;
}

inline bool ContainPol(float xt, float yt, __global float * paras, int id)
{
    int temp=1;
    for (int it=0; it<id; it++)
    {
        float3 a=vload3(it, paras);
        temp*=((a.x*xt+a.y*yt+a.z))>0;
    }
    return temp;
}

__kernel void ShapeDraw(__global float * image,
                         __global float * paras,
                         __global float * jitPos,
                         int s,
                         int w,
                         int h,
//                         float4 color;
                         float r,
                         float g,
                         float b,
                         float a,
                         int id,
                         int ib,
                         int jb)
{
    int i=get_global_id(0)+ib;
    if (i>=h) return;
    int j=get_global_id(1)+jb;
    if (j>=w) return;

    float y=i*1.0f/w;
    float x=j*1.0f/w;
    float xt, yt;
    float count=0.0f;
    for (int k=0; k<s; k++)
    {
        float2 jit=vload2(k, jitPos);
        xt=x+jit.x;
        yt=y+jit.y;
        if (id==0) count+=ContainEll(xt, yt, paras);
        else count+=ContainPol(xt, yt, paras, id);
    }
    count=count*a/s;

    float4 pix=vload4((i*w+j), image);
    float ft=1.0f-count;
    pix.x=pix.x*ft+r*count;
    pix.y=pix.y*ft+g*count;
    pix.z=pix.z*ft+b*count;
    pix.w=1.0f-ft*(1-pix.w);
    vstore4(pix, ((i*w)+j), image);
}

