#pragma once

// based on the libdms OpenGL Toolkit Vector3 class

class V3
{
public:
	V3(void);

    V3(float x, float y, float z);
    V3(const float *v);
    V3(const V3 &v);
    V3& operator= (const V3& v);
    void set(float x, float y, float z);

	operator FLOAT* () { return vec; }
	operator CONST FLOAT* () const { return vec; }

    float& operator[](int i);
    float operator[](int i) const;

    V3& operator+=(const V3& v);
    V3& operator-=(const V3& v);
    V3& operator*=(float s);
    V3& operator/=(float s);
    V3 operator+(const V3& v) const;
    V3 operator-(const V3& v) const;
    V3 operator*(float s) const;
    V3 operator/(float s) const;
    V3 operator-(void) const;
    bool operator==(const V3& v) const;
    bool operator!=(const V3& v) const;
    
    float length(void) const;
    float lengthSquared(void) const;
    void normalize(void);
    float dot(const V3& v) const;
    V3 cross(const V3& v) const;
    float distance(const V3& v) const;
    float distanceSquared(const V3& v) const;
        
    static const V3 Zero;
    static const V3 X;
    static const V3 Y;
    static const V3 Z;

	union{
		float vec[3];
		struct {
			float x,y,z;
		};
		struct {
			float r,g,b;
		};
	};
};

inline V3 operator* (float s, const V3& v) { return v*s; }

