/*
inline bool ContainEll(float xt, float yt, __global float * paras, int & offset)
{
    float3 a=vload3(offset++, paras);
    float3 b=vload3(offset++, paras);
    float temp=xt*xt*a.x+yt*yt*a.y+xt*yt*a.z+xt*b.x+yt*b.y+b.z;
    return temp<0;
} 

inline bool ContainPol(float xt, float yt, __global float * paras, int & offset, int numPoly)
{
    int temp=1;
    for (int ip=0; ip<numPoly; ip++)
    {
        float3 a=vload3(offset++, paras);
        temp*=((a.x*xt+a.y*yt+a.z))>0;
    }
    return temp;
}
*/
inline bool ContainCSG(float xt, float yt, __global float * paras, __global int * struc)
{
    int2 level0, level1, level2;
    int offsetStruc=0;
    int offsetParas=0;
    bool res0, res1, res2;
    res0=0;
    level0=vload2(offsetStruc++, struc);
    for (int ic=0; ic<level0.y; ic++)
    {
        level1=vload2(offsetStruc++, struc);
        res1=1;
        for (int jc=0; jc<level1.y; jc++)
        {
            level2=vload2(offsetStruc++, struc);
            if (level2.y==0) //res2=ContainEll(xt, yt, paras, offsetParas);
            {
                float3 a=vload3(offsetParas++, paras);
                float3 b=vload3(offsetParas++, paras);
                res2=xt*xt*a.x+yt*yt*a.y+xt*yt*a.z+xt*b.x+yt*b.y+b.z<0;
            } 
            else //res2=ContainPol(xt, yt, paras, offsetParas, level2.y);
            {
                res2=1;
                for (int ip=0; ip<level2.y; ip++)
                {
                    float3 a=vload3(offsetParas++, paras);
                    res2*=((a.x*xt+a.y*yt+a.z))>0;
                }
            }
            res1*=(level2.x==res2);
        }
        res0+=(level1.x==res1);
    }
    return (level0.x==res0);
}

__kernel void ShapeDraw(__global float * image,
                        __global float * paras,
                        __global float * jitPos,
                        __global int * struc,
                        int s,
                        int w,
                        int h,
//                        float4 color;
                        float r,
                        float g,
                        float b,
                        float a,
//                        int id,
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
        count+=ContainCSG(xt, yt, paras, struc);
        //if (id==0) count+=ContainEll(xt, yt, paras);
        //else count+=ContainPol(xt, yt, paras, id);
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
            
