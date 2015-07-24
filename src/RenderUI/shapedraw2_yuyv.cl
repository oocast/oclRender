inline int ContainCSG(float xt1, float xt2, float yt, __local int2 * struc, __local float4 * paras)
{
    int2 level0, level1, level2;
    int offsetStruc=0;
    int offsetParas=0;
    bool res01, res11, res21;
    bool res02, res12, res22;
    res01=res02=0;
    level0=struc[offsetStruc++];
    for (int ic=0; ic<level0.y; ic++)
    {
        level1=struc[offsetStruc++];
        res11=res12=1;
        for (int jc=0; jc<level1.y; jc++)
        {
            level2=struc[offsetStruc++];
            if (level2.y==0)
            {
                float4 a=paras[offsetParas++];
                float4 b=paras[offsetParas++];
                res21=xt1*xt1*a.x+yt*yt*a.y+xt1*yt*a.z+xt1*b.x+yt*b.y+b.z<0;
                res22=xt2*xt2*a.x+yt*yt*a.y+xt2*yt*a.z+xt2*b.x+yt*b.y+b.z<0;
            } 
            else
            {
                res21=res22=1;
                for (int ip=0; ip<level2.y; ip++)
                {
                    float4 a=paras[offsetParas++];
                    res21*=((a.x*xt1+a.y*yt+a.z))>0;
                    res22*=((a.x*xt2+a.y*yt+a.z))>0;
                }
            }
            res11*=(level2.x==res21);
            res12*=(level2.x==res22);
        }
        res01+=(level1.x==res11);
        res02+=(level1.x==res12);
    }
    return ((level0.x==res01)*2+(level0.x==res02));
}

__kernel void ShapeDraw(__global uchar4 * image,
                        __global int2 * struc,
                        __global float4 * paras,
                        __global float2 * jitPos,
                        int w,
                        int h,
                        uchar Y,
                        uchar U,
                        uchar V,
                        float a,
                        int ib,
                        int jb,
                        __local int2 * strucLocal,
                        __local float4 * parasLocal,
                        int strucSize,
                        int paraSize)
{
    //__local int2 strucLocal[50];
    //__local float4 parasLocal[4];
    __local float2 jitPosLocal[36];
    int k=get_local_id(1)*get_local_size(1)+get_local_id(0);
    if (k<strucSize) strucLocal[k]=struc[k];
    if (k<paraSize) parasLocal[k]=paras[k];
    if (k<36) jitPosLocal[k]=jitPos[k];
    int i=get_global_id(0)+ib;
    //if (i>=h) return;
    int j=get_global_id(1)+jb;
    //if (j>=w/2) return;
    barrier(CLK_LOCAL_MEM_FENCE);

    float y=i*1.0f/w;
    float x2=(j*2+1)*1.0f/w;
    float x1=j*2*1.0f/w;
    float xt1, xt2, yt;
    float count1=0.0f;
    float count2=0.0f;
    #pragma unroll
    for (int k=0; k<36; k++)
    {
        float2 jit=jitPosLocal[k];
        xt1=x1+jit.x;
        xt2=x2+jit.x;
        yt=y+jit.y;
        int result=ContainCSG(xt1, xt2, yt, strucLocal, parasLocal);
        count1+=result/2;
        count2+=result%2;
    }
    count1=count1*a/36.0f;
    count2=count2*a/36.0f;

    int it=(i*w/2+j);
    uchar4 pix=image[it];
    float ft1=1.0f-count1;
    float ft2=1.0f-count2;
    pix.x=pix.x*ft1+Y*count1;
    pix.y=(pix.y*(ft1+ft2)+U*(count1+count2))/2;
    pix.z=pix.z*ft2+Y*count2;
    pix.w=(pix.w*(ft1+ft2)+V*(count1+count2))/2;
    image[it]=pix;
}
            
