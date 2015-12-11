#ifndef __TYPES_H__
#define __TYPES_H__


#define EPSILON   1e-6f
#define TWOPI 6.283185308

/* conversion factor for converting from radians to degrees*/
#define RADTODEG  57.295779513082

/* conversion factor for converting from degrees to radians*/
#define DEGTORAD  0.017453292519943295769236907684886


/**
 * @brief 16 character buffer used for short tags
 */
typedef char Word[16];
#define WORDLEN   16
#define word_cmp(a,b) (strncmp(a,b,WORDLEN))
#define word_cpy(dst,src) (strncpy(dst,src,WORDLEN))
#define word_clear(a)  (memset(a,0,sizeof(char)*WORDLEN))
/**
 * @brief 128 character buffer used for statements
 */
typedef char Line[128];
#define LINELEN   128
#define line_cmp(a,b) (strncmp(a,b,LINELEN))
#define line_cpy(dst,src) (strncpy(dst,src,LINELEN))
#define line_clear(a)  (memset(a,0,sizeof(char)*LINELEN))
/**
 * @brief 512 character buffer used for text blocks
 */
typedef char Text[512];
#define TEXTLEN   512
#define text_cmp(a,b) (strncmp(a,b,TEXTLEN))
#define text_cpy(dst,src) (strncpy(dst,src,TEXTLEN))
#define text_clear(a)  (memset(a,0,sizeof(char)*TEXTLEN))


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

#define vec_in_rect(v, r)  ((v.x >= r.x)&&(v.x < (r.x + r.w))&&(v.y >= r.y)&&(v.y < (r.y + r.h)))

Vec2D vec2d(float x,float y);
Vec3D vec3d(float x,float y, float z);
Vec4D vec4d(float x,float y, float z, float w);

/**
 * @brief sets the outvector to a unit vector pointing at the angle specified
 * @param out output.  Vector will be set to a unit vector rotated by radians
 * @param radians specify the angle of the vector to be set.
 */
void vec3d_set_angle_by_radians(Vec3D *out,float radians);
void vec4d_set_angle_by_radians(Vec4D *out,float radians);

/**
 * @brief returns the angle of a vector in 2D space.
 * @param x the x component of the vector
 * @param y the y component of the vector
 * @return the angle of the vector in radians.
 */
float vector_angle(float x,float y);

/**
 * @brief returns the angle of a vector in 2D space.
 * @param v the vector
 * @return the angle of the vector in radians.
 */
float vec2d_angle(Vec2D v);

/**
 * @brief returns the angle between the two angles.
 * Takes into account wraping around 0 and 360
 * units are degrees
 * @Param a one of the angles in question
 * @Param b one of the angles in question
 * @return the angle, in degrees, between the two angles
 */
float angle_between_degrees(float a, float b);

void angle_clamp_degrees(float *a);


typedef struct
{
	float x,y,w,h;
}RectFloat;

/**
 * @brief constructs and returns an RectFloat
 * @param x component of the rect
 * @param y component of the rect
 * @param w component of the rect
 * @param h component of the rect
 */
RectFloat rect(
  float  x,
  float  y,
  float  w,
  float  h
);

void rect_set(
    RectFloat * d,
    float       x,
    float       y,
    float       w,
    float       h
);

void rect_copy(
    RectFloat * d,
    RectFloat   s
);

/**
 * @purpose A bounding box for 3D
 */
typedef struct
{
  float x,y,z;  /**<upper left front corner*/
  float w,h,d;  /**<width, height and depth*/
}Rect3D;

/**
 * @brief zeros the rect
 * @param r a rect to zero
 */
void rect3d_clear(Rect3D *r);

/**
 * @brief constructs a rect3D from two vector types
 * @param corner the upper left front corner of the rect
 * @param size the width, heigh and depth of the rect
 * @return a rect sized to the specified dimensions
 */
Rect3D rect3d_from_vec3d(Vec3D corner,Vec3D size);

/**
 * @brief constructs a rect3D from component dimensions
 * @param x the upper position of the rect
 * @param y the left position of the rect
 * @param z the front position of the rect
 * @param w the width
 * @param h the height
 * @param d the depth
 * @return a rect sized to the specified dimensions
 */
Rect3D rect3d(float x,float y,float z,float w,float h,float d);

/**
 * @brief checks if the two rects overlap
 * @param a one rect to check
 * @param b another rect to check
 * @return 1 if the overlap at all, 0 otherwise
 */
int rect3d_overlap(Rect3D a, Rect3D b);


typedef struct
{
	Vec3D position;
	Vec3D scale;
	Vec3D rotation;
	Vec3D color;
	float alpha;
}Orientation;

/**
 * @brief Adds the two input orientations together.
 * useful for applying vector to position
 * @param out the output orientation.  If NULL, nothing happens.
 * @param in1 one component
 * @param in2 the other component
 */
void orientation_add(Orientation * out,
                         Orientation   in1,
                         Orientation   in2);

/**
 * @brief Merges the two.  Add position and rotation, but multiplies scale, color and alpha.
 * useful for applying layered  transformations.
 * @param out the output orientation.  If NULL, nothing happens.
 * @param in1 one component
 * @param in2 the other component
 */
void orientation_mix(Orientation * out,
                         Orientation   in1,
                         Orientation   in2);

/**
 * @brief sets the passed orientation to a default,sane set up.
 * sets alpha and scale to 1, all else to zero
 * @param out the orientation to reset
 */
void orientation_clear(Orientation *out);

/**
 * @brief sets the passed orientation to all zero
 * @param out the orientation to zero
 */
void orientation_zero(Orientation *out);

/**
 * @brief copies the data from one orientation to another
 * @param dst the destination orientation, if null this is a no-op
 * @param src the source orientation
 */
void orientation_copy(Orientation *dst,Orientation src);


/**
 * basic operations
 */
#ifndef MIN
#define MIN(a,b)          (a<=b?a:b)
#endif

#ifndef MAX
#define MAX(a,b)          (a>=b?a:b)
#endif

/**
 * 3D Vector Math
 * These macros handle most of the common operations for vector math.
 */

#define vec4d_into_array(ar,vec)   (ar[0] = vec.x,ar[1] = vec.y,ar[2] = vec.z,ar[3] = vec.w)
#define vec3d_into_array(ar,vec)   (ar[0] = vec.x,ar[1] = vec.y,ar[2] = vec.z)
#define vec2d_into_array(ar,vec)   (ar[0] = vec.x,ar[1] = vec.y)

/**
 * @brief calculates the cross product of the two input vectors
 * @param out the resultant cross product
 * @param v1 one of the components
 * @param v2 one of the components
 */
void vec3d_cross_product(Vec3D *out, Vec3D v1, Vec3D v2);

/**
 * @brief Macro to get the dot product from a vector.
 *
 * @param a Vect3D component of the dot product
 * @param b Vect3D component of the dot product
 *
 * @return the calculated dot product
 */
#define vec4d_dot_product(a,b)      (a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w)
#define vec3d_dot_product(a,b)      (a.x*b.x+a.y*b.y+a.z*b.z)
#define vec2d_dot_product(a,b)      (a.x*b.x+a.y*b.y)

/**
 * @brief copies the data from one vector into another
 *
 * @param dst the destination vector
 * @param src the source vector
 */
#define vec2d_cpy(dst,src)   (dst.x = src.x,dst.y = src.y)
#define vec3d_cpy(dst,src)   (dst.x = src.x,dst.y = src.y,dst.z = src.z)
#define vec4d_cpy(dst,src)   (dst.x = src.x,dst.y = src.y,dst.z = src.z,dst.w = src.w)

/**
 * @brief checks if vectors are exactly matches of each other
 *
 * @param a one vector to check
 * @param b other vector to check
 */
#define vec2d_equal(a,b) ((a.x == b.x)&&(a.y == b.y))
#define vec3d_equal(a,b) ((a.x == b.x)&&(a.y == b.y)&&(a.z == b.z))
#define vec4d_equal(a,b) ((a.x == b.x)&&(a.y == b.y)&&(a.z == b.z)&&(a.w == b.w))

/**
 * @brief checks if vectors are exactly matches of each other
 *
 * @param a one vector to check
 * @param b other vector to check
 */
#define vec2d_equal(a,b) ((a.x == b.x)&&(a.y == b.y))
#define vec3d_equal(a,b) ((a.x == b.x)&&(a.y == b.y)&&(a.z == b.z))
#define vec4d_equal(a,b) ((a.x == b.x)&&(a.y == b.y)&&(a.z == b.z)&&(a.w == b.w))

/**
 * @brief Macro to subtract two vectors
 * varient ending in p takes a pointer to Vect3D instead.
 * Varients ending with 2D only operate on the x an y components of vectors
 *
 * @param dst result Vect3D output
 * @param a Vect3D input
 * @param b Vect3D input
 */
#define vec2d_sub(dst,a,b)     (dst.x=a.x-b.x,dst.y=a.y-b.y)
#define vec3d_sub(dst,a,b)     (dst.x=a.x-b.x,dst.y=a.y-b.y,dst.z=a.z-b.z)
#define vec4d_sub(dst,a,b)     (dst.x=a.x-b.x,dst.y=a.y-b.y,dst.z=a.z-b.z,dst.w=a.w-b.w)

/**
 * @brief Macro to add two vectors
 * varient ending in p takes a pointer to Vect3D instead.
 * Varients ending with 2D only operate on the x an y components of vectors
 *
 * @param dst result Vect3D output
 * @param a Vect3D input
 * @param b Vect3D input
 */
#define vec2d_add(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y)
#define vec3d_add(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y,dst.z = a.z+b.z)
#define vec4d_add(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y,dst.z = a.z+b.z,dst.w = a.w+b.w)

/**
 * @brief Macro to scale a vector by a scalar value
 * varient ending in p takes a pointer to Vect3D instead.
 * Varients ending with 2D only operate on the x an y components of vectors
 *
 * @param dst result Vect3D output
 * @param src Vect3D input
 * @Param factpr the scalar value to scale the vector by.
 */
#define vec2d_scale(dst,src,factor) (dst.x = src.x *factor,\
                                     dst.y = src.y *factor)
#define vec3d_scale(dst,src,factor) (dst.x = src.x *factor,\
                                     dst.y = src.y *factor,\
                                     dst.z = src.z *factor)
#define vec4d_scale(dst,src,factor) (dst.x = src.x *factor,\
                                     dst.y = src.y *factor,\
                                     dst.z = src.z *factor,\
                                     dst.w = src.w *factor)

/**
 * @brief Macro that sets vector to zero.
 * @param a Vect[2D|3D|4D] input
 */
#define vec2d_clear(a)       (a.x=a.y=0)
#define vec3d_clear(a)       (a.x=a.y=a.z=0)
#define vec4d_clear(a)       (a.x=a.y=a.z=a.w=0)

/**
 * @brief Macro to get the negative of a vector
 *
 * @param src Vect[2D|3D|4D] input
 * @param dst Vect[2D|3D|4D] negated output
 */
#define vec2d_negate(dst,src)      (dst.x = -src.x,dst.y = -src.y)
#define vec3d_negate(dst,src)      (dst.x = -src.x,dst.y = -src.y,dst.z = -src.z)
#define vec4d_negate(dst,src)      (dst.x = -src.x,dst.y = -src.y,dst.z = -src.z,dst.w = -src.w)

/**
 * @brief Macro to set the components of the vector
 *
 * @param v Vect3D output
 * @param a float x component
 * @param b float y component
 * @param c float z component (only in 3D & 4D version)
 * @param d float w component (only in 4D version)
 */
#define vec2d_set(v,a,b)		(v.x=(a), v.y=(b))
#define vec3d_set(v,a,b,c)		(v.x=(a), v.y=(b), v.z=(c))
#define vec4d_set(v,a,b,c,d)	(v.x=(a), v.y=(b), v.z=(c), v.w=(d))

void vec3d_print(Text out, Vec3D v);

Vec2D *vec2d_new();
Vec3D *vec3d_new();
Vec4D *vec4d_new();

Vec2D *vec2d_dup(Vec2D old);
Vec3D *vec3d_dup(Vec3D old);
Vec4D *vec4d_dup(Vec4D old);


/**
 * @brief normalizes the vector passed.  does nothing for a zero length vector.
 *
 * @param v pointer to the vector to be normalized.
 */
void vec2d_normalize (Vec2D *V);
void vec3d_normalize (Vec3D *V);
void vec4d_normalize (Vec4D *V);

float vec2d_magnitude(Vec2D V);
float vec3d_magnitude(Vec3D V);
float vec4d_magnitude(Vec4D V);

/**
 * @brief returns the magnitude squared, which is faster than getting the magnitude
 * which would involve taking the square root of a floating point number.
 * @param V the vector to get the magnitude for
 * @return the square of the magnitude of V
 */
float vec2d_magnitude_squared(Vec2D V);
float vec3d_magnitude_squared(Vec3D V);
float vec4d_magnitude_squared(Vec4D V);

/**
 * @brief checks if the magnitude of V is less than size.  It does this without
 * doing square roots, which are costly.  It will still do floating point multiplication
 * @param V the vector to check
 * @param size the magnitude to check against
 * @return 1 if the magnitude of V is less than size or 0 otherwise
 */
int  vec2d_magnitude_less_than(Vec2D V,float size);
int  vec3d_magnitude_less_than(Vec3D V,float size);
int  vec4d_magnitude_less_than(Vec4D V,float size);

/**
 * @brief checks if the distance between the two points provided is less than size.
 * @param p1 one point for the distance check
 * @param p2 another point for the distance check
 * @param size the value to check against
 * @return 1 if the distance between P1 and P2 is less than size, 0 otherwise
 */
int distance_between_less_than2d(Vec3D p1,Vec3D p2,float size);
int distance_between_less_than3d(Vec3D p1,Vec3D p2,float size);
int distance_between_less_than4d(Vec3D p1,Vec3D p2,float size);

/**
 * @brief given a rotation, get the component vectors
 * @param angles the input rotation
 * @param forward output optional calculated forward vector
 * @param right output optional calculated right vector
 * @param up output optional calculated up vector
 */
void vec3d_angle_vectors(Vec3D angles, Vec3D *forward, Vec3D *right, Vec3D *up);

/**
 * @brief rotate an Vec3D about another vector
 * @param dst output result
 * @param dir the vector to rotate about
 * @param point the point to rotate
 * @param degrees how far to rotate
 */
void vec3d_rotate_about_vector(Vec3D *dst, Vec3D dir, Vec3D point, float degrees);

void vec3d_rotate_about_x(Vec3D *vect, float angle);
void vec3d_rotate_about_y(Vec3D *vect, float angle);
void vec3d_rotate_about_z(Vec3D *vect, float angle);

int vec3d_and(Vec3D V, Vec3D W);


#endif