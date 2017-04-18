C0271 Computer Graphics
----------------------------------------------------------------------
Assignment 4 Template
Version 3.1	Nov.2006

Note!
This template is for C0271 course students only.


[Introduction]
==============
The template is to help you to quickly develop a high quality ray tracer.
Most of the interface elements such as objects modification, material,
light source and texture settings and scene preview are implemented for
you. All you have to do is to add your own ray tracing codes into the
template.


[Main Features of the template]
===============================
o Viewpoint move, turn, slide and twist, adjustable view angle etc.
o Object add, duplicate, delete, move and resize.
o Group object selection by shift click or click-and-drag
o Group update for all object attributes
o Render selected region instead of whole window saves testing time
o Zoom mode for easy debugging
o File open/save (*.ray)
o OpenGL preview with selectable alpha blending and texture mapping
  support to allow faster operation on slower machine.
o Support JPG and BMP as textures maps


[The Package]
=============
You can find the following files in the package:

	Image library
	-----------------------------------------
	bmpfile.cpp		// bmp related code
	bmpfile.h		// bmp related header
	jconfig.h		// jpg related header
	jmorecfg.h		// jpg related header
	jpegfile.cpp		// jpg related code
	jpegfile.h		// jpg related header
	jpegLib.lib		// the jpg library

	Template files
	--------------
	LightSource.cpp		// light source class	
	LightSource.h		// light source class header
	MainFrm.cpp		// main frame window code
	MainFrm.h		// main frame window header
	Material.cpp		// material dialog class
	Material.h		// material dialog class header
	PerferencesDialog.cpp	// preferences dialog class
	PerferencesDialog.h	// preferences dialog class header
	Primitive.cpp		// 3D object class
	Primitive.h		// 3D object class header
	ray.cpp			// the ray application class
	ray.h			// the ray application class header	
	ray.rc			// resource template
	rayDoc.cpp		// ray document class
	rayDoc.h		// ray document class header
	rayView.cpp		// ray view class
	rayView.h		// ray view class header
	readme.txt		// this file
	resource.h		// resource header
	StdAfx.cpp		// MFC related code
	StdAfx.h		// MFC related header
	Texture.cpp		// texture class
	Texture.h		// texture class header
	Vertex.cpp		// vertex class
	Vertex.h		// vertex class header
	res\Ray.bmp		// the prism image in about box
	res\ray.ico		// application icon
	res\ray.rc2		// resource file
	res\rayDoc.ico		// document icon, same as ray.ico
	res\Toolbar.bmp		// toolbar image
	samples\*		// sample *.ray files and related textures


[System Requirements]
=====================
o Pentium class PC or above.
o 64 MB RAM or above.
o Microsoft Windows 95/98/NT4/2000/XP.
o Microsoft Visual Studio .NET 2003/2005.
o A graphics board supports OpenGl is highly recommended.


[Installation]
==============
The template uses IJG's JPEG and BMP image reading/writing routines for
importing textures maps and exporting rendered image. The JpegLib
library must be installed properly before the template can be compiled.
Installation of the JpegLib is simple, just copy the file JpegLib.lib to
the default library path will do. Typically, that path is:
	C:\Program Files\Microsoft Visual Studio\VC98\lib
Some program resources are stored in the res folder. Make sure you
extract the files to their correct sub-folders.

Click File->Open Project/Solution in MS VS .NET to open the file ray.sln to begin


[Note]
======
By default, alpha blending is turned off. That means when you set some objects'
opacity to less than one, the scene preview will not reflect that change.
To enable alpha blending, click Edit->Preferences then check the alpha blending
box. If you want to change the default value, modify the variable assignment
statement m_bBlending=FALSE in the constructor of the class CRayView.

When you look carefully, you can see that transparent objects are not rendered
correctly in the preview. Some triangles are gone when looked from some 
angles. This problem is caused by two factors: first is the z-buffer algorithm
used by OpenGL, second is the order of rendering of transparent triangles.
This is a well known problem and there are some solutions to this but most of
them require partitioning of space and global sorting of triangles and they are
not implementated in the template.


[Template Structure]
====================
The template is designed to follow the document/view architecture of the
Microsoft Foundation Class. Under this architecture, most data manipulation
and interface functons are controlled by two main classes: the CRayDoc
and CRayView.

CRayDoc stores the information related to the scene file. This information
includes viewpoint position, object type, object coordinates, light
sources colors., etc. This class is also responsible for reading the
infomation from and to files.

CRayView, on the other hand, stores most interface related infomation
such as current mouse position, control mode and rendering settings.
Its main job is to present the information stored in CRayDoc. In our
case, to render the preview of the scene by using opengl and to display the
ray traced image.

The structure of this template is quite complicated, you
have to study the program very carefully. No special functions are
isolated for you to insert code directly. You have to add member
functions to different classes by yourselves to finish the assignment.
A good point to start is the member function Render() in class CRayView.
Like assignment 1, put the color values in the array 'm_pULMap'. This time,
it is an unsigned long array, each element represents a pixel and the
color is stored in ABGR format.

This following list show the varies classes and member variables needed by
ray tracing. There are many other interface related variables and functions
not shown. For detailed list, please directly refer to the header files.

<CRayView> class
	CRayView is the class for displaying the scene. Only one instance
	of it is created. Most of the program wide parameters are put in
	CRayView class.	

	int	m_nLeft,m_nTop,m_nWidth,m_nHeight;
	// OpenGL's drawing area's position and size, relative to the screen
	// no need to use directly in ray tracing.
	

	Some important functions are:
	
	void	Render();
	// called by OnPaint(), Render() allocates memory to m_pULMap
	// before ray tracing. start adding code in this function

	// vector, matrix operations
	void	MatrixMultVector(float*,float*,float*);	// dimension 4x4

	Other variables and functions are not directly related to ray tracing.
	Most of them are used by the interface.
	
<CRayDoc> class
	CRayDoc is the class for storing scene data. Only one instance
	of it is created. Ray tracing related variables include:

	typedef struct _CAMERAINFO{
		V3	viewpoint;			// viewpoint position
		V3	huv;				// head up vector (unit vector)
		V3	los;				// line of sight (unit vector)
		V3	rhv;				// right hand vector (unit vector)
		float	viewangle;		// view angle	
	} CAMERAINFO;

	CAMERAINFO m_camera;

	list<LPPRIMITIVE>	m_objects;  //contains CPrimitive objects
	list<LPLIGHTSOURCE> m_lightsources; //contains CLightsource objects
	list<LPTEXTURE>		m_textures; //contains CTexture objects, for texture map

	list<LPPRIMITIVE>	m_selectedobjects; //contains selected CPrimitive objects


<CPrimitive> class
	CPrimtive is the object class. Objects definition is defined in this
	class. There are a few types of objects divided into two categories:
	Quadric surfaces and Triangles formed solids. Both classes of objects
	are represented by an intrinsic-transform format. i.e. vertices or
	equation coefficients of objects are recorded in standard form instead
	of general form. A 4x4 transform matrix M is used to generalize the
	object. The advantages are: better integration with the interface,
	simpler finite quadrics intersection testing and easier texture mapping.
	The following are class variables that are useful in ray tracing:

	short	m_nType;
	// object type: OBJ_CUBE,OBJ_PYR,OBJ_ELLI,OBJ_CONE,OBJ_CYL...

	short	m_nIType;
	// object categories: IOB_TRI,IOB_QUADR

	////// quadric related parametrics

	float	m_Qa,m_Qb,...,m_Qk;
	// quadric equation ax^2+by^2+cz^2+2dxy+2eyz+2fxz+2gx+2hy+2jz+k=0

	float	m_Za,m_Zb;
	// z-range of finite quadrics in intrinic coordinate

	////// triangles related parameters

	int	m_nNumTriangle;
	// number of triangles, cube=12, pyramid=6, tetrahedron=4, octahedron=8,
	// icosahedron=20, dodecahedron=36 because there are 12 pentagons which
	// cuts into 3 triangles each.

	list<VERTEX> m_vertices;	
	// list of vertices, 3 vertices for every triangle. the same
	// vertex may repeats. it simplifies the program a bit, not a memory
	// saving approach though. For instance, one dodecahedron only have 20
	// distict vertice but in the template, 108 vertices are stored!
	// as long as the main memory is enough, it should not be a big problem.

	list<VERTEX> m_objvertices;
	// vertices in intrinsic coordinate, before transform

	float	m_Ba,m_Bb,...,m_Bk;
	// another quadric surface: bounding ellipsoid, for triangles formed solids only

	float	m_Matrix[16];
	// the transform matrix (4x4 homogeneous)

	float	m_invMatrix[16];
	// inverse of m_flMatrix, should be used in some places, such as texture mapping
	// and tracing finite size quadrics surface, in ray tracing

	// material setting
	float	m_Ambient[4];		// ambient component, amb[3] not-used
	float	m_Diffuse[4];		// diffuse component, dif[3] not-used
	float	m_Specular[4];	// specular component, dif[3] not-used
	float	m_Shininess;		// shininess range 1-256, 1-128 in OpenGL
	float	m_RefrIndex;		// refractive index > 1.0
	float	m_Reflectance;	// range 0-1
	float	m_Opacity;		// range 0-1

	void	*m_pTX;
	// color texture, any BMP or JPG file
	// points to a CTexture object


	Some useful functions are:

	// vector, matrix operations
	void	MultMatrix44(float*,float*,float*);	// 4x4 matrix multiplcation
	void	InvMatrix44(float*,float*);		// find inverse of 4x4 matrix
	void	TransMatrix44(float*,float*);		// find transpose of 4x4 matrix



<CVertex> class
	CVertex objects are basic vertices data structure. It contains data such as:

	V3      position;	// coordinate
	float	s,t;		// texture coordinate
	V3      normal;

<CLightSource> class
	CLightSource class. variables include:

	V3		position;		// light position
	V3		color;		// light color

<CTexture> class
	CTexture objects either store color texture map. Only one useful function:

    // return color3f at given texture coordinate (s,t), no maxification, no
	// minification, just find the nearest value
	void	GetColorAt(float s,float t,float *color);
	

<Global Macro/constants>
	DotProduct(x,y);	find the dot product of two 3D vectors
	Magnitude(x);		find the magnitude of one 3D vector

	M_PI			=3.1415926, for quadric surface's texture mapping
	TOLERANCE		=0.001, to elminate the artifacts caused
				by precision error of floating point operations

<Other ray tracing un-related classes>
	<CRayApp> 		Ray application class
	<CAboutDialog>		about dialog box class
	<CMaterial>		material settings dialog box class
	<CPreferencesDialog>	preferences dialog box class


[Hints]
=======
You need to add member functions to classes CRayView, CRayDoc and
CPrimitive only. You may add a loop in the given Render() function to
fire rays. It does not need to do real intersection calculation. Leave
that to functions in CPrimitive will be more efficient because most
necessary infomation is stored in CPrimitive. i.e. add Intersect() funtion
to CPrimitive to do the real job.

A Shade() function is also needed in CPrimitive to find the color. The
Shade() function will have to call Shade() again to find colors from
secondary rays.

Intersect() needs the object list in order to work, however the object
list is stored in CRayDoc. Therefore, make sure you make the m_pObjectList
as input to both Intersect() and Shade() so that it is possible to access
the object list in CPrimitive.
	
Whenever you want to access the variables such as m_pObjectList in CRayDoc
from a function in CRayView, define a pointer pDoc=(CRayDoc*)GetDocument()
and use pDoc->m_pObjectList to access them.

Proposed implementation steps:
1. Fire rays
2. Test intersections, quadrics first, then triangles -> object's silhouette
	Quadrics are easier and faster to trace. triangles involves some
	projections which depends on the angle. A bit tidious.
3. Find normals, then ambient and diffuse colors -> dull looking objects
	At this stage, it should look similar to the preview, but a bit dull.
4. Add specular component -> correctly lighted object without reflection
	At this stage, it should look a bit better than the preview.
5. Add secondary reflection -> objects with reflection
	Much better than preview now. you can see multple reflection between
	near objects. The number is related to the variable m_nMaxLevel.
	Try setting the number to a different values to see the effect.
6. Add refraction -> transparency
	You need to consider one more case that is when the ray hit the object
	surface from the inside. This step can go wrong very easily because
	it is not easy to guess how a virtual transparent object looks like.
	That is why the	executable is provided.
7. Add texture mapping -> objects with rich details.
	The hardest part is how to map the 3D coordinate to the 2D texture
	coordinate for triangles. Do not make it complicated. It is a simple
	ratio problem. For the 3 quadrics, they are similar.
8. Ray trace 3 images each no smaller than 640x480 pixels using your own ray
	tracer and hand them in.

During linking, there may be a warning #4098 appear from time to time:
"defaultlib "LIBCD" conflicts with use of other libs; use /NODEFAULTLIB:library".
It is caused by the JPG library. According to the IJG library documentation, that
is normal and will not cause any harm, just ignore it.

Please refer to the course homepage's assignment 3 FAQ for more about
implementation details.


[Interface Control]
===================
There are 3 control modes:
<Navigate Mode>
o The navigate mode allow the user to move the view point around,
o Left click and draging up or down to move foreword or backward.
o Left click and draging left or right allows the view point to turn left or
  right on a horizontal plane.
o The horizontal and vertical mouse motion can be mixed together. for instance,
  left click and drag to the upper right will result in foreward right turning
  motion.
o Right clicking/draging allows the users to elevate or look down without moving
  the view point's position.

<Select/Edit Mode>
o In select mode, user can select object or objects and apply changes. Hold down
  shift key and click on object to select multiple objects. Another way is to
  click somewhere in background and drag a windows. All objects touch the window
  will be selected.
o Each selected object will have a dark cyan bounding box. There will also be a
  big axis-aligned bounding box bounding all selected objects.
o To move object(s), click and drag on any line segment of the bounding boxes.
o To rotate object(s), click and drag on the body of selected objects. The centre
  of rotation will be the centre of the big bounding box.
o To resize the object(s), click and drag on the arrows. The use can only resize
  the object one dimension at a time.
o If some objects are selected, right click and drag rotates the view point about
  the selected object(s).
o Light sources can be select only in select mode. To move light sources,
  left click and drag any one of the three lines pass through the selected light
  source. Note that only one light source can be selected at one time.

<Zoom Mode>
o In zoom mode, a 10x10 pixel square will move following the mouse pointer. A
  magnified image of the square will follow the mouse pointer as well but
  positioned a bit further away.
o This mode is mainly for debugging. You can use it to see the effect of your
  implementation clearly.
o The zoom factor can be adjusted from 2x to 10x by set the slider in the
  preferences dialog box.
o When magnified, you might see the dithering effect when your display setting's
  color depth value is not high enough. Try avoid from using 16-bit (65536 colors).
  use at least 24 or 32 bits.


[Data Files]
============
You can save your drawing in scene files (*.ray). Please fill feel to exchange
data files with your coursemates. The *.ray files are in fact text files. You can
modify them manually using text editor.

The *.ray scene file contains all the necessary information of a scene. That
includes view point information, light sources information and object
information. Most of the information is stored in the file except for textures.
The filenames of texture images are recorded instead of the texture images
themselves. No path is recorded, therefore, the texture images related to a
scene should be placed in the same directory with the scene file.

Pretty ones will be posted on the course homepage.

Enjoy!

Course tutor
