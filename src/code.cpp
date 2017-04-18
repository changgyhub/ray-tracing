#include "stdafx.h"
#include "code.h"

int winWidth = 640;		// window width
int winHeight = 480;	// window height

V3 ViewPoint;		// view point
V3 ImageLL;		// coordinates at lower left corner of image
V3 ImageLR;		// coordinates at lower right corner of image
V3 ImageUL;		// coordinates at upper left corner of image
V3 ImageUR;		// coordinates at upper right corner of image

int MaxTraceDepth = 5;			// depth of recursive ray-tracing

// scene objects
vector<CLightSource *> vLightSource;		// array of light sources
vector<CPrimitive *> vObjects;				// array of objects


void RayTracing(V3 * colorMap)
{	
	for (int i = 0; i < winWidth - 1; ++i){
		for (int j = 0; j < winHeight - 1; ++j){         // visit each pixel on the screen
			V3 p(ImageLL + i*(ImageLR - ImageLL) / (winWidth - 1) + j*(ImageUL - ImageLL) / (winHeight - 1));  // world coordinates of p
			V3 rayStart(ViewPoint);
			V3 rayDir(p - rayStart);                     // fire a ray from the viewpoint through p
			V3 color;
			Trace(rayStart, rayDir, 0, color);           // trace the ray and get the color of p
			colorMap[j*winWidth + i] = color;            // Save color in colorMap
		}
	}
}

void Trace(V3& rayStart, V3& rayDir, int depth, V3& color)
{
	V3 intersection, normal;
	CPrimitive *objHit;
	rayDir.normalize();
	if (Intersect(rayStart, rayDir, objHit, intersection, normal)){
		Shade(objHit, rayStart, rayDir, intersection, normal, depth, color);
	}
	else {
		color.set(.0, .0, .0);
	}
}

// compute color at a given point
void Shade(CPrimitive *obj, V3& rayStart, V3& rayDir, V3& intersection, V3& normal, int depth, V3& color)
{
	V3 ambient_color, diffuse_color, specular_color;
	obj->GetAmbient(intersection, ambient_color);
	color = ambient_color;              // ambient term

	for (vector<CLightSource *>::iterator iter = vLightSource.begin(); iter != vLightSource.end(); iter++){ // diffuse and specular terms due to each light source
		V3 sRay((*iter)->position - intersection); sRay.normalize(); float cos_theta = normal.dot(sRay);
		if (cos_theta >= 0){     // see whether sRay is blocked
			V3 dummy_intersection, dummy_normal;
			if (!Intersect(intersection, sRay, obj, dummy_intersection, dummy_normal)){  // check if sRay is blocked
				obj->GetDiffuse(intersection, diffuse_color);
				obj->GetSpecular(intersection, specular_color);
				V3 R(2 * cos_theta * normal - sRay), L(rayStart - intersection); L.normalize(); float cos_phi = R.dot(L); float specular = pow(cos_phi, obj->m_Shininess);
				color.x += (*iter)->color.x * (obj->m_Opacity * diffuse_color.x * cos_theta + obj->m_Reflectance * specular_color.x * specular);
				color.y += (*iter)->color.y * (obj->m_Opacity * diffuse_color.y * cos_theta + obj->m_Reflectance * specular_color.y * specular);
				color.z += (*iter)->color.z * (obj->m_Opacity * diffuse_color.z * cos_theta + obj->m_Reflectance * specular_color.z * specular);
			}
		}
	}

	if (depth < MaxTraceDepth){  // reflected ray contribution
		if (obj->m_Reflectance > 0){
			V3 rRay(rayDir + 2 * normal.dot(-rayDir) * normal);     // ray in reflection direction from intersection
			V3 rColor;
			Trace(intersection, rRay, depth + 1, rColor);
			color += rColor * obj->m_Reflectance;                   // scale rColor by reflectance and add to color
		}
	}

	if (color.x > 1.0) color.x = 1.0;
	if (color.y > 1.0) color.y = 1.0;
	if (color.z > 1.0) color.z = 1.0;    // clamp color to 1.0
}

bool IntersectQuadratic(V3 rayStart, V3 rayDir, float * coeffMatrix, float& t, V3& intersection)
{		
	float S[] = { rayStart[0], rayStart[1], rayStart[2], 1 },
		  D[] = { rayDir[0], rayDir[1], rayDir[2], 0 };    // R(t) = S + Dt;
	float a, b, c;                                         // at^2 + bt + c = 0
	float DtA[4], StA[4];
	VectorMultMatrix(D, coeffMatrix, DtA);
	VectorMultMatrix(S, coeffMatrix, StA);
	a = VectorMultVector(DtA, D);                          // a = DtAD
	b = 2 * VectorMultVector(StA, D);                      // b = StAD
	c = VectorMultVector(StA, S);                          // c = StAS
	float delta = b*b - 4*a*c;                             // calculate and determine closest t
	if (delta > 0){
		float t0 = (-b + sqrt(delta)) / (2 * a), t1 = (-b - sqrt(delta)) / (2 * a);
		t = t0 > t1 ? t1 : t0;
	}
	else if (delta == 0){
		t = -b / (2 * a);
	}
	else {
		return false;
	}
	intersection = rayStart + t * rayDir;
	return true;
}
bool IntersectTriangle(V3 rayStart, V3 rayDir, V3 v0, V3 v1, V3 v2, float& t, V3& intersection)
{	
	V3 N ((v1 - v0).cross(v2 - v0));
	float d = N.dot(v0);                                        // plane is NX = d;
	if (N.dot(rayDir)){
		float _t = (d - N.dot(rayStart)) / (N.dot(rayDir));     // N(rayStart + t * rayDir) = d
		V3 _intersection(rayStart + _t * rayDir);
		V3 to0(v0 - _intersection), to1(v1 - _intersection), to2(v2 - _intersection);
		V3 cross1(to0.cross(to1)), cross2(to1.cross(to2)), cross3(to2.cross(to0));
		if (cross1.x * cross2.x > 0 && cross2.x * cross3.x > 0){              // check whether the intersection point is inside the triangle
			t = _t;
			intersection = _intersection;
			return true;
		}
	}
	return false;
}

void MatrixMultVector(float *m,float *v,float *rv)//rv=m*v
{
	rv[0]=m[0]*v[0]+m[4]*v[1]+m[8]*v[2]+m[12]*v[3];
	rv[1]=m[1]*v[0]+m[5]*v[1]+m[9]*v[2]+m[13]*v[3];
	rv[2]=m[2]*v[0]+m[6]*v[1]+m[10]*v[2]+m[14]*v[3];
	rv[3]=m[3]*v[0]+m[7]*v[1]+m[11]*v[2]+m[15]*v[3];
}
void VectorMultMatrix(float *v,float *m,float *lv)//lv=v^Tm
{
	lv[0]=m[0]*v[0]+m[1]*v[1]+m[2]*v[2]+m[3]*v[3];
	lv[1]=m[4]*v[0]+m[5]*v[1]+m[6]*v[2]+m[7]*v[3];
	lv[2]=m[8]*v[0]+m[9]*v[1]+m[10]*v[2]+m[11]*v[3];
	lv[3]=m[12]*v[0]+m[13]*v[1]+m[14]*v[2]+m[15]*v[3];
}
float VectorMultVector(float *v1,float *v2)//v3=v1^Tv2
{
	return v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2]+v1[3]*v2[3];
}



