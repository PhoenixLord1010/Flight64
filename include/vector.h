#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef struct
{
    float x,y;
}Vec2D;

typedef struct
{
    float x,y,z;
}Vec3D;

typedef struct
{
    float x,y,z,w;
}Vec4D;

Vec2D vec2d(float x,float y);
Vec3D vec3d(float x,float y, float z);
Vec4D vec4d(float x,float y, float z, float w);

/**
 * @brief normalizes the vector passed.  does nothing for a zero length vector.
 *
 * @param v pointer to the vector to be normalized.
 */
float vec3d_magnitude(Vec3D V);
void vec3d_normalize(Vec3D *V);
int vec3d_and(Vec3D V, Vec3D W);

/**
 * basic operations
 */
#ifndef MIN
#define MIN(a,b)          (a<=b?a:b)
#endif

#ifndef MAX
#define MAX(a,b)          (a>=b?a:b)
#endif

#define vec2d_set(v,a,b)		(v.x=(a), v.y=(b))
#define vec3d_set(v,a,b,c)		(v.x=(a), v.y=(b), v.z=(c))
#define vec4d_set(v,a,b,c,d)	(v.x=(a), v.y=(b), v.z=(c), v.w=(d))

#define vec2d_negate(dst,src)      (dst.x = -src.x,dst.y = -src.y)
#define vec3d_negate(dst,src)      (dst.x = -src.x,dst.y = -src.y,dst.z = -src.z)
#define vec4d_negate(dst,src)      (dst.x = -src.x,dst.y = -src.y,dst.z = -src.z,dst.w = -src.w)

#define vec2d_cpy(dst,src)   (dst.x = src.x,dst.y = src.y)
#define vec3d_cpy(dst,src)   (dst.x = src.x,dst.y = src.y,dst.z = src.z)
#define vec4d_cpy(dst,src)   (dst.x = src.x,dst.y = src.y,dst.z = src.z,dst.w = src.w)

#define vec2d_add(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y)
#define vec3d_add(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y,dst.z = a.z+b.z)
#define vec4d_add(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y,dst.z = a.z+b.z,dst.w = a.w+b.w)

#define vec3d_scale(dst,src,scale) (dst.x = src.x *scale,dst.y = src.y *scale,dst.z = src.z *scale)

#endif
