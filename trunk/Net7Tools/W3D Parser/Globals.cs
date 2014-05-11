using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WestWood3D
{
    [Flags]
    public enum ChunkHeader
    {
	    W3D_CHUNK_MESH							    = 0x00000000,	// Mesh definition 
	    W3D_CHUNK_VERTICES						    = 0x00000002,	// array of vertices (array of W3dVectorStruct's)
	    W3D_CHUNK_VERTEX_NORMALS				    = 0x00000003,	// array of normals (array of W3dVectorStruct's)
	    W3D_CHUNK_MESH_USER_TEXT				    = 0x0000000C,	// Text from the MAX comment field (Null terminated string)
	    W3D_CHUNK_VERTEX_INFLUENCES			        = 0x0000000E,	// Mesh Deformation vertex connections (array of W3dVertInfStruct's)
	    W3D_CHUNK_MESH_HEADER3					    = 0x0000001F,	//	mesh header contains general info about the mesh. (W3dMeshHeader3Struct)
	    W3D_CHUNK_TRIANGLES					        = 0x00000020,	// New improved triangles chunk (array of W3dTriangleStruct's)
	    W3D_CHUNK_VERTEX_SHADE_INDICES			    = 0x00000022,	// shade indexes for each vertex (array of uint32's)
    	
	    W3D_CHUNK_PRELIT_UNLIT					    = 0x00000023,	// optional unlit material chunk wrapper
	    W3D_CHUNK_PRELIT_VERTEX				        = 0x00000024,	// optional vertex-lit material chunk wrapper
	    W3D_CHUNK_PRELIT_LIGHTMAP_MULTI_PASS	    = 0x00000025,	// optional lightmapped multi-pass material chunk wrapper
	    W3D_CHUNK_PRELIT_LIGHTMAP_MULTI_TEXTURE     = 0x00000026,	// optional lightmapped multi-texture material chunk wrapper

	    W3D_CHUNK_MATERIAL_INFO				        = 0x00000028,	// materials information, pass count, etc (contains W3dMaterialInfoStruct)

	    W3D_CHUNK_SHADERS						    = 0x00000029,	// shaders (array of W3dShaderStruct's)
    		
	    W3D_CHUNK_VERTEX_MATERIALS				    = 0x0000002A,	// wraps the vertex materials
	    W3D_CHUNK_VERTEX_MATERIAL				    = 0x0000002B,
	    W3D_CHUNK_VERTEX_MATERIAL_NAME			    = 0x0000002C,	// vertex material name (NULL-terminated string)
	    W3D_CHUNK_VERTEX_MATERIAL_INFO			    = 0x0000002D,	// W3dVertexMaterialStruct
	    W3D_CHUNK_VERTEX_MAPPER_ARGS0			    = 0x0000002E,	// Null-terminated string
	    W3D_CHUNK_VERTEX_MAPPER_ARGS1			    = 0x0000002F,	// Null-terminated string

	    W3D_CHUNK_TEXTURES						    = 0x00000030,	// wraps all of the texture info
	    W3D_CHUNK_TEXTURE						    = 0x00000031,	// wraps a texture definition
	    W3D_CHUNK_TEXTURE_NAME					    = 0x00000032,	// texture filename (NULL-terminated string)
	    W3D_CHUNK_TEXTURE_INFO					    = 0x00000033,	// optional W3dTextureInfoStruct
    		
	    W3D_CHUNK_MATERIAL_PASS				        = 0x00000038,	// wraps the information for a single material pass
	    W3D_CHUNK_VERTEX_MATERIAL_IDS			    = 0x00000039,	// single or per-vertex array of uint32 vertex material indices (check chunk size)
	    W3D_CHUNK_SHADER_IDS					    = 0x0000003A,	// single or per-tri array of uint32 shader indices (check chunk size)
	    W3D_CHUNK_DCG							    = 0x0000003B,	// per-vertex diffuse color values (array of W3dRGBAStruct's)
	    W3D_CHUNK_DIG							    = 0x0000003C,	// per-vertex diffuse illumination values (array of W3dRGBStruct's)
	    W3D_CHUNK_SCG							    = 0x0000003E,	// per-vertex specular color values (array of W3dRGBStruct's)

	    W3D_CHUNK_TEXTURE_STAGE				        = 0x00000048,	// wrapper around a texture stage.
	    W3D_CHUNK_TEXTURE_IDS					    = 0x00000049,	// single or per-tri array of uint32 texture indices (check chunk size)
	    W3D_CHUNK_STAGE_TEXCOORDS				    = 0x0000004A,	// per-vertex texture coordinates (array of W3dTexCoordStruct's)
	    W3D_CHUNK_PER_FACE_TEXCOORD_IDS		        = 0x0000004B,	// indices to W3D_CHUNK_STAGE_TEXCOORDS, (array of Vector3i)

	    W3D_CHUNK_DEFORM						    = 0x00000058,	// mesh deform or 'damage' information.
	    W3D_CHUNK_DEFORM_SET					    = 0x00000059,	// set of deform information
	    W3D_CHUNK_DEFORM_KEYFRAME				    = 0x0000005A,	// a keyframe of deform information in the set
	    W3D_CHUNK_DEFORM_DATA					    = 0x0000005B,	// deform information about a single vertex

	    W3D_CHUNK_PS2_SHADERS					    = 0x00000080,	// Shader info specific to the Playstation 2.
    	
	    W3D_CHUNK_AABTREE						    = 0x00000090,	// Axis-Aligned Box Tree for hierarchical polygon culling
	    W3D_CHUNK_AABTREE_HEADER 				    = 0x00000091,	// catalog of the contents of the AABTree
	    W3D_CHUNK_AABTREE_POLYINDICES			    = 0x00000092,	// array of uint32 polygon indices with count=mesh.PolyCount
	    W3D_CHUNK_AABTREE_NODES				        = 0x00000093,	// array of W3dMeshAABTreeNode's with count=aabheader.NodeCount

	    W3D_CHUNK_HIERARCHY					        = 0x00000100,	// hierarchy tree definition
	    W3D_CHUNK_HIERARCHY_HEADER				    = 0x00000101,
	    W3D_CHUNK_PIVOTS						    = 0x00000102,
	    W3D_CHUNK_PIVOT_FIXUPS					    = 0x00000103,	// only needed by the exporter...

	    W3D_CHUNK_ANIMATION					        = 0x00000200,	// hierarchy animation data
	    W3D_CHUNK_ANIMATION_HEADER				    = 0x00000201,
	    W3D_CHUNK_ANIMATION_CHANNEL			        = 0x00000202,	// channel of vectors
	    W3D_CHUNK_BIT_CHANNEL					    = 0x00000203,	// channel of boolean values (e.g. visibility)

	    W3D_CHUNK_COMPRESSED_ANIMATION			    = 0x00000280,	// compressed hierarchy animation data
	    W3D_CHUNK_COMPRESSED_ANIMATION_HEADER	    = 0x00000281,	// describes playback rate, number of frames, and type of compression
	    W3D_CHUNK_COMPRESSED_ANIMATION_CHANNEL	    = 0x00000282,	// compressed channel, format dependent on type of compression
	    W3D_CHUNK_COMPRESSED_BIT_CHANNEL		    = 0x00000283,	// compressed bit stream channel, format dependent on type of compression

	    W3D_CHUNK_MORPH_ANIMATION				    = 0x000002C0,	// hierarchy morphing animation data (morphs between poses, for facial animation)
	    W3D_CHUNK_MORPHANIM_HEADER				    = 0x000002C0,	// W3dMorphAnimHeaderStruct describes playback rate, number of frames, and type of compression
	    W3D_CHUNK_MORPHANIM_CHANNEL			        = 0x000002C0,	// wrapper for a channel
	    W3D_CHUNK_MORPHANIM_POSENAME			    = 0x000002C0,	// name of the other anim which contains the poses for this morph channel
	    W3D_CHUNK_MORPHANIM_KEYDATA			        = 0x000002C0,	// morph key data for this channel
	    W3D_CHUNK_MORPHANIM_PIVOTCHANNELDATA	    = 0x000002C0,	// uin32 per pivot in the htree, indicating which channel controls the pivot

	    W3D_CHUNK_HMODEL						    = 0x00000300,	// blueprint for a hierarchy model
	    W3D_CHUNK_HMODEL_HEADER				        = 0x00000301,	// Header for the hierarchy model
	    W3D_CHUNK_NODE							    = 0x00000302,	// render objects connected to the hierarchy
	    W3D_CHUNK_COLLISION_NODE				    = 0x00000303,	// collision meshes connected to the hierarchy
	    W3D_CHUNK_SKIN_NODE					        = 0x00000304,	// skins connected to the hierarchy
	    W3D_CHUNK_HMODEL_AUX_DATA		            = 0x00000305,	// extension of the hierarchy model header
	    OBSOLETE_W3D_CHUNK_SHADOW_NODE			    = 0x00000306,	// shadow object connected to the hierarchy

	    W3D_CHUNK_LODMODEL						    = 0x00000400,	// blueprint for an LOD model. This is simply a
	    W3D_CHUNK_LODMODEL_HEADER				    = 0x00000400,	// collection of 'n' render objects, ordered in terms
	    W3D_CHUNK_LOD							    = 0x00000400,	// of their expected rendering costs. (highest is first)

	    W3D_CHUNK_COLLECTION					    = 0x00000420,	// collection of render object names
	    W3D_CHUNK_COLLECTION_HEADER			        = 0x00000421,	// general info regarding the collection
	    W3D_CHUNK_COLLECTION_OBJ_NAME			    = 0x00000422,	// contains a string which is the name of a render object
	    W3D_CHUNK_PLACEHOLDER					    = 0x00000423,	// contains information about a 'dummy' object that will be instanced later
	    W3D_CHUNK_TRANSFORM_NODE				    = 0x00000424,	// contains the filename of another w3d file that should be transformed by this node

	    W3D_CHUNK_POINTS						    = 0x00000440,	// array of W3dVectorStruct's. May appear in meshes, hmodels, lodmodels, or collections.

	    W3D_CHUNK_LIGHT						        = 0x00000460,	// description of a light
	    W3D_CHUNK_LIGHT_INFO					    = 0x00000461,	// generic light parameters
	    W3D_CHUNK_SPOT_LIGHT_INFO				    = 0x00000462,	// extra spot light parameters
	    W3D_CHUNK_NEAR_ATTENUATION				    = 0x00000463,	// optional near attenuation parameters
	    W3D_CHUNK_FAR_ATTENUATION				    = 0x00000464,	// optional far attenuation parameters

	    W3D_CHUNK_EMITTER						    = 0x00000500,	// description of a particle emitter
	    W3D_CHUNK_EMITTER_HEADER				    = 0x00000501,	// general information such as name and version
	    W3D_CHUNK_EMITTER_USER_DATA			        = 0x00000502,	// user-defined data that specific loaders can switch on
	    W3D_CHUNK_EMITTER_INFO					    = 0x00000503,	// generic particle emitter definition
	    W3D_CHUNK_EMITTER_INFOV2				    = 0x00000504,	// generic particle emitter definition (version 2.0)
	    W3D_CHUNK_EMITTER_PROPS				        = 0x00000505,	// Key-frameable properties
	    OBSOLETE_W3D_CHUNK_EMITTER_COLOR_KEYFRAME	= 0x00000506,	// structure defining a single color keyframe
	    OBSOLETE_W3D_CHUNK_EMITTER_OPACITY_KEYFRAME	= 0x00000507,	// structure defining a single opacity keyframe
	    OBSOLETE_W3D_CHUNK_EMITTER_SIZE_KEYFRAME	= 0x00000508,	// structure defining a single size keyframe
	    W3D_CHUNK_EMITTER_LINE_PROPERTIES		    = 0x00000509,	// line properties, used by line rendering mode
	    W3D_CHUNK_EMITTER_ROTATION_KEYFRAMES	    = 0x0000050a,	// rotation keys for the particles
	    W3D_CHUNK_EMITTER_FRAME_KEYFRAMES		    = 0x0000050b,	// frame keys (u-v based frame animation)
	    W3D_CHUNK_EMITTER_BLUR_TIME_KEYFRAMES	    = 0x0000050c,	// length of tail for line groups

	    W3D_CHUNK_AGGREGATE					        = 0x00000600,	// description of an aggregate object
	    W3D_CHUNK_AGGREGATE_HEADER				    = 0x00000601,	// general information such as name and version
	    W3D_CHUNK_AGGREGATE_INFO				    = 0x00000602,	// references to 'contained' models
	    W3D_CHUNK_TEXTURE_REPLACER_INFO		        = 0x00000603,	// information about which meshes need textures replaced
	    W3D_CHUNK_AGGREGATE_CLASS_INFO			    = 0x00000604,	// information about the original class that created this aggregate

	    W3D_CHUNK_HLOD							    = 0x00000700,	// description of an HLod object (see HLodClass)
	    W3D_CHUNK_HLOD_HEADER					    = 0x00000701,	// general information such as name and version
	    W3D_CHUNK_HLOD_LOD_ARRAY				    = 0x00000702,	// wrapper around the array of objects for each level of detail
	    W3D_CHUNK_HLOD_SUB_OBJECT_ARRAY_HEADER	    = 0x00000703,	// info on the objects in this level of detail array
	    W3D_CHUNK_HLOD_SUB_OBJECT				    = 0x00000704,	// an object in this level of detail array
	    W3D_CHUNK_HLOD_AGGREGATE_ARRAY			    = 0x00000705,	// array of aggregates, contains W3D_CHUNK_SUB_OBJECT_ARRAY_HEADER and W3D_CHUNK_SUB_OBJECT_ARRAY
	    W3D_CHUNK_HLOD_PROXY_ARRAY				    = 0x00000706,	// array of proxies, used for application-defined purposes, provides a name and a bone.

	    W3D_CHUNK_BOX							    = 0x00000740,	// defines an collision box render object (W3dBoxStruct)
	    W3D_CHUNK_SPHERE						    = 0x00000741,
	    W3D_CHUNK_RING							    = 0x00000742,

	    W3D_CHUNK_NULL_OBJECT					    = 0x00000750,	// defines a NULL object (W3dNullObjectStruct)

	    W3D_CHUNK_LIGHTSCAPE					    = 0x00000800,	// wrapper for lights created with Lightscape.	
	    W3D_CHUNK_LIGHTSCAPE_LIGHT				    = 0x00000801,	// definition of a light created with Lightscape.
	    W3D_CHUNK_LIGHT_TRANSFORM				    = 0x00000802,	// position and orientation (defined as right-handed 4x3 matrix transform W3dLightTransformStruct).

	    W3D_CHUNK_DAZZLE						    = 0x00000900,	// wrapper for a glare object. Creates halos and flare lines seen around a bright light source
	    W3D_CHUNK_DAZZLE_NAME					    = 0x00000901,	// null-terminated string, name of the dazzle (typical w3d object naming: "container.object")
	    W3D_CHUNK_DAZZLE_TYPENAME				    = 0x00000902,	// null-terminated string, type of dazzle (from dazzle.ini)

	    W3D_CHUNK_SOUNDROBJ					        = 0x00000A00,	// description of a sound render object
	    W3D_CHUNK_SOUNDROBJ_HEADER				    = 0x00000A00,	// general information such as name and version
	    W3D_CHUNK_SOUNDROBJ_DEFINITION			    = 0x00000A00,	// chunk containing the definition of the sound that is to play	
        }

    [Flags]
    public enum ShaderEnum
    {
        /*
	    W3DSHADER_DEPTHCOMPARE_PASS_NEVER = 0,			// pass never (i.e. always fail depth comparison test)
	    W3DSHADER_DEPTHCOMPARE_PASS_LESS,				// pass if incoming less than stored
	    W3DSHADER_DEPTHCOMPARE_PASS_EQUAL,				// pass if incoming equal to stored
	    W3DSHADER_DEPTHCOMPARE_PASS_LEQUAL,				// pass if incoming less than or equal to stored (default)
	    W3DSHADER_DEPTHCOMPARE_PASS_GREATER,			// pass if incoming greater than stored	
	    W3DSHADER_DEPTHCOMPARE_PASS_NOTEQUAL,			// pass if incoming not equal to stored
	    W3DSHADER_DEPTHCOMPARE_PASS_GEQUAL,				// pass if incoming greater than or equal to stored
	    W3DSHADER_DEPTHCOMPARE_PASS_ALWAYS,				// pass always
	    W3DSHADER_DEPTHCOMPARE_PASS_MAX,					// end of enumeration

	    W3DSHADER_DEPTHMASK_WRITE_DISABLE = 0,			// disable depth buffer writes 
	    W3DSHADER_DEPTHMASK_WRITE_ENABLE,				// enable depth buffer writes		(default)
	    W3DSHADER_DEPTHMASK_WRITE_MAX,					// end of enumeration

	    W3DSHADER_ALPHATEST_DISABLE = 0,					// disable alpha testing (default)
	    W3DSHADER_ALPHATEST_ENABLE,						// enable alpha testing
	    W3DSHADER_ALPHATEST_MAX,							// end of enumeration

 	    W3DSHADER_DESTBLENDFUNC_ZERO = 0,				// destination pixel doesn't affect blending (default)
 	    W3DSHADER_DESTBLENDFUNC_ONE,						// destination pixel added unmodified
 	    W3DSHADER_DESTBLENDFUNC_SRC_COLOR,				// destination pixel multiplied by fragment RGB components
 	    W3DSHADER_DESTBLENDFUNC_ONE_MINUS_SRC_COLOR, // destination pixel multiplied by one minus (i.e. inverse) fragment RGB components
 	    W3DSHADER_DESTBLENDFUNC_SRC_ALPHA,				// destination pixel multiplied by fragment alpha component
 	    W3DSHADER_DESTBLENDFUNC_ONE_MINUS_SRC_ALPHA,	// destination pixel multiplied by fragment inverse alpha
 	    W3DSHADER_DESTBLENDFUNC_SRC_COLOR_PREFOG,		// destination pixel multiplied by fragment RGB components prior to fogging
	    W3DSHADER_DESTBLENDFUNC_MAX,						// end of enumeration

	    W3DSHADER_PRIGRADIENT_DISABLE = 0,				// disable primary gradient (same as OpenGL 'decal' texture blend)
	    W3DSHADER_PRIGRADIENT_MODULATE,					// modulate fragment ARGB by gradient ARGB (default)
	    W3DSHADER_PRIGRADIENT_ADD,							// add gradient RGB to fragment RGB, copy gradient A to fragment A
	    W3DSHADER_PRIGRADIENT_BUMPENVMAP,				// environment-mapped bump mapping
	    W3DSHADER_PRIGRADIENT_MAX,							// end of enumeration

	    W3DSHADER_SECGRADIENT_DISABLE = 0,				// don't draw secondary gradient (default)
	    W3DSHADER_SECGRADIENT_ENABLE,						// add secondary gradient RGB to fragment RGB 
	    W3DSHADER_SECGRADIENT_MAX,							// end of enumeration

 	    W3DSHADER_SRCBLENDFUNC_ZERO = 0,					// fragment not added to color buffer
 	    W3DSHADER_SRCBLENDFUNC_ONE,						// fragment added unmodified to color buffer (default)
 	    W3DSHADER_SRCBLENDFUNC_SRC_ALPHA,				// fragment RGB components multiplied by fragment A
 	    W3DSHADER_SRCBLENDFUNC_ONE_MINUS_SRC_ALPHA,	// fragment RGB components multiplied by fragment inverse (one minus) A
	    W3DSHADER_SRCBLENDFUNC_MAX,						// end of enumeration

	    W3DSHADER_TEXTURING_DISABLE = 0,					// no texturing (treat fragment initial color as 1,1,1,1) (default)
	    W3DSHADER_TEXTURING_ENABLE,						// enable texturing
	    W3DSHADER_TEXTURING_MAX,							// end of enumeration

	    W3DSHADER_DETAILCOLORFUNC_DISABLE = 0,			// local (default)
	    W3DSHADER_DETAILCOLORFUNC_DETAIL,				// other
	    W3DSHADER_DETAILCOLORFUNC_SCALE,					// local * other
	    W3DSHADER_DETAILCOLORFUNC_INVSCALE,				// ~(~local * ~other) = local + (1-local)*other
	    W3DSHADER_DETAILCOLORFUNC_ADD,					// local + other
	    W3DSHADER_DETAILCOLORFUNC_SUB,					// local - other
	    W3DSHADER_DETAILCOLORFUNC_SUBR,					// other - local
	    W3DSHADER_DETAILCOLORFUNC_BLEND,					// (localAlpha)*local + (~localAlpha)*other
	    W3DSHADER_DETAILCOLORFUNC_DETAILBLEND,			// (otherAlpha)*local + (~otherAlpha)*other
	    W3DSHADER_DETAILCOLORFUNC_MAX,					// end of enumeration

	    W3DSHADER_DETAILALPHAFUNC_DISABLE = 0,			// local (default)
	    W3DSHADER_DETAILALPHAFUNC_DETAIL,				// other
	    W3DSHADER_DETAILALPHAFUNC_SCALE,					// local * other
	    W3DSHADER_DETAILALPHAFUNC_INVSCALE,				// ~(~local * ~other) = local + (1-local)*other
	    W3DSHADER_DETAILALPHAFUNC_MAX,					// end of enumeration

	    W3DSHADER_DEPTHCOMPARE_DEFAULT = W3DSHADER_DEPTHCOMPARE_PASS_LEQUAL,
	    W3DSHADER_DEPTHMASK_DEFAULT = W3DSHADER_DEPTHMASK_WRITE_ENABLE,
	    W3DSHADER_ALPHATEST_DEFAULT = W3DSHADER_ALPHATEST_DISABLE,
	    W3DSHADER_DESTBLENDFUNC_DEFAULT = W3DSHADER_DESTBLENDFUNC_ZERO,
	    W3DSHADER_PRIGRADIENT_DEFAULT = W3DSHADER_PRIGRADIENT_MODULATE,
	    W3DSHADER_SECGRADIENT_DEFAULT = W3DSHADER_SECGRADIENT_DISABLE,
	    W3DSHADER_SRCBLENDFUNC_DEFAULT = W3DSHADER_SRCBLENDFUNC_ONE,
	    W3DSHADER_TEXTURING_DEFAULT = W3DSHADER_TEXTURING_DISABLE,
	    W3DSHADER_DETAILCOLORFUNC_DEFAULT = W3DSHADER_DETAILCOLORFUNC_DISABLE,
	    W3DSHADER_DETAILALPHAFUNC_DEFAULT = W3DSHADER_DETAILALPHAFUNC_DISABLE
        */
    }

    [Flags]
    public enum MaterialEnum
    {
        /*
        /////////////////////////////////////////////////////////////////////////////////////////////
        //	 MATERIALS
        //
        //	 
        //	 The VertexMaterial defines parameters which control the calculation of the primary
        //	 and secondary gradients. The shader defines how those gradients are combined with
        //	 the texel and the frame buffer contents.
        //
        /////////////////////////////////////////////////////////////////////////////////////////////
    	
	    protected static final int W3DVERTMAT_USE_DEPTH_CUE	=							0x00000001;
	    protected static final int W3DVERTMAT_ARGB_EMISSIVE_ONLY =						0x00000002;
	    protected static final int W3DVERTMAT_COPY_SPECULAR_TO_DIFFUSE =				0x00000004;
	    protected static final int W3DVERTMAT_DEPTH_CUE_TO_ALPHA =						0x00000008;

	    protected static final int W3DVERTMAT_STAGE0_MAPPING_MASK =						0x00FF0000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_UV	=						0x00000000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_ENVIRONMENT =				0x00010000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_CHEAP_ENVIRONMENT =		0x00020000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_SCREEN =					0x00030000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_LINEAR_OFFSET =			0x00040000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_SILHOUETTE	=				0x00050000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_SCALE =					0x00060000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_GRID =						0x00070000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_ROTATE	=					0x00080000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_SINE_LINEAR_OFFSET	=		0x00090000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_STEP_LINEAR_OFFSET	=		0x000A0000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_ZIGZAG_LINEAR_OFFSET =		0x000B0000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_WS_CLASSIC_ENV	=			0x000C0000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_WS_ENVIRONMENT	=			0x000D0000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_GRID_CLASSIC_ENV =			0x000E0000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_GRID_ENVIRONMENT =			0x000F0000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_RANDOM	=					0x00100000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_EDGE =						0x00110000;
	    protected static final int W3DVERTMAT_STAGE0_MAPPING_BUMPENV =					0x00120000;

	    protected static final int W3DVERTMAT_STAGE1_MAPPING_MASK =						0x0000FF00;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_UV	=						0x00000000;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_ENVIRONMENT =				0x00000100;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_CHEAP_ENVIRONMENT =		0x00000200;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_SCREEN	=					0x00000300;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_LINEAR_OFFSET =			0x00000400;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_SILHOUETTE	=				0x00000500;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_SCALE =					0x00000600;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_GRID =						0x00000700;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_ROTATE	=					0x00000800;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_SINE_LINEAR_OFFSET	=		0x00000900;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_STEP_LINEAR_OFFSET	=		0x00000A00;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_ZIGZAG_LINEAR_OFFSET =		0x00000B00;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_WS_CLASSIC_ENV	=			0x00000C00;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_WS_ENVIRONMENT	=			0x00000D00;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_GRID_CLASSIC_ENV =			0x00000E00;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_GRID_ENVIRONMENT =			0x00000F00;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_RANDOM	=					0x00001000;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_EDGE =						0x00001100;
	    protected static final int W3DVERTMAT_STAGE1_MAPPING_BUMPENV =					0x00001200;

	    protected static final int W3DVERTMAT_PSX_MASK =								0xFF000000;
	    protected static final int W3DVERTMAT_PSX_TRANS_MASK = 							0x07000000;
	    protected static final int W3DVERTMAT_PSX_TRANS_NONE = 							0x00000000;
	    protected static final int W3DVERTMAT_PSX_TRANS_100	= 							0x01000000;
	    protected static final int W3DVERTMAT_PSX_TRANS_50 = 							0x02000000;
	    protected static final int W3DVERTMAT_PSX_TRANS_25 = 							0x03000000;
	    protected static final int W3DVERTMAT_PSX_TRANS_MINUS_100 = 					0x04000000;
	    protected static final int W3DVERTMAT_PSX_NO_RT_LIGHTING = 						0x08000000;
        */
    }

    [Flags]
    public enum PS2_SHADER_SETTINGS
    {
        //PSS_SRC = 0,
        //PSS_DEST,
        //PSS_ZERO,

        //PSS_SRC_ALPHA = 0,
        //PSS_DEST_ALPHA,
        //PSS_ONE,

        //// From combo box. To match the PC default gradient.
        //PSS_PRIGRADIENT_DECAL = 0,
        //PSS_PRIGRADIENT_MODULATE,
        //PSS_PRIGRADIENT_HIGHLIGHT,
        //PSS_PRIGRADIENT_HIGHLIGHT2,

        //// Actual PS2 numbers.
        //PSS_PS2_PRIGRADIENT_MODULATE = 0,
        //PSS_PS2_PRIGRADIENT_DECAL,
        //PSS_PS2_PRIGRADIENT_HIGHLIGHT,
        //PSS_PS2_PRIGRADIENT_HIGHLIGHT2,


        //PSS_DEPTHCOMPARE_PASS_NEVER = 0,
        //PSS_DEPTHCOMPARE_PASS_LESS,
        //PSS_DEPTHCOMPARE_PASS_ALWAYS,
        //PSS_DEPTHCOMPARE_PASS_LEQUAL,
    }

    [Flags]
    public enum TextureAnimationParams
    {
        /////////////////////////////////////////////////////////////////////////////////////////////
        // Texture Animation parameters
        // May occur inside a texture chunk if its needed
        /////////////////////////////////////////////////////////////////////////////////////////////

        //#define W3DTEXTURE_PUBLISH					0x0001		// this texture should be "published" (indirected so its changeable in code)
        //#define W3DTEXTURE_RESIZE_OBSOLETE		0x0002		// this texture should be resizeable (OBSOLETE!!!)
        //#define W3DTEXTURE_NO_LOD					0x0004		// this texture should not have any LOD (mipmapping or resizing)
        //#define W3DTEXTURE_CLAMP_U					0x0008		// this texture should be clamped on U
        //#define W3DTEXTURE_CLAMP_V					0x0010		// this texture should be clamped on V
        //#define W3DTEXTURE_ALPHA_BITMAP			0x0020		// this texture's alpha channel should be collapsed to one bit

        //// Specify desired no. of mip-levels to be generated.
        //#define W3DTEXTURE_MIP_LEVELS_MASK		0x00c0
        //#define W3DTEXTURE_MIP_LEVELS_ALL		0x0000		// generate all mip-levels
        //#define W3DTEXTURE_MIP_LEVELS_2			0x0040		// generate up to 2 mip-levels (NOTE: use W3DTEXTURE_NO_LOD to generate just 1 mip-level)
        //#define W3DTEXTURE_MIP_LEVELS_3			0x0080		// generate up to 3 mip-levels
        //#define W3DTEXTURE_MIP_LEVELS_4			0x00c0		// generate up to 4 mip-levels

        //// Hints to describe the intended use of the various passes / stages
        //// This will go into the high byte of Attributes.
        //#define W3DTEXTURE_HINT_SHIFT				8				// number of bits to shift up
        //#define W3DTEXTURE_HINT_MASK				0x0000ff00	// mask for shifted hint value

        //#define W3DTEXTURE_HINT_BASE				0x0000		// base texture
        //#define W3DTEXTURE_HINT_EMISSIVE			0x0100		// emissive map
        //#define W3DTEXTURE_HINT_ENVIRONMENT		0x0200		// environment/reflection map
        //#define W3DTEXTURE_HINT_SHINY_MASK		0x0300		// shinyness mask map

        //#define W3DTEXTURE_TYPE_MASK				0x1000	
        //#define W3DTEXTURE_TYPE_COLORMAP			0x0000		// Color map.
        //#define W3DTEXTURE_TYPE_BUMPMAP			0x1000		// Grayscale heightmap (to be converted to bumpmap).

        //// Animation types
        //#define W3DTEXTURE_ANIM_LOOP				0x0000
        //#define W3DTEXTURE_ANIM_PINGPONG			0x0001
        //#define W3DTEXTURE_ANIM_ONCE				0x0002
        //#define W3DTEXTURE_ANIM_MANUAL			0x0003
    }

    [Flags]
    public enum TriangleAttributes
    {
        /////////////////////////////////////////////////////////////////////////////////////////////
        // Flags for the Triangle Attributes member
        /////////////////////////////////////////////////////////////////////////////////////////////

        //SURFACE_TYPE_LIGHT_METAL = 0,
        //SURFACE_TYPE_HEAVY_METAL,
        //SURFACE_TYPE_WATER,
        //SURFACE_TYPE_SAND,
        //SURFACE_TYPE_DIRT,
        //SURFACE_TYPE_MUD,
        //SURFACE_TYPE_GRASS,
        //SURFACE_TYPE_WOOD,
        //SURFACE_TYPE_CONCRETE,
        //SURFACE_TYPE_FLESH,
        //SURFACE_TYPE_ROCK,
        //SURFACE_TYPE_SNOW,
        //SURFACE_TYPE_ICE,
        //SURFACE_TYPE_DEFAULT,
        //SURFACE_TYPE_GLASS,
        //SURFACE_TYPE_CLOTH,
        //SURFACE_TYPE_TIBERIUM_FIELD,
        //SURFACE_TYPE_FOLIAGE_PERMEABLE,
        //SURFACE_TYPE_GLASS_PERMEABLE,
        //SURFACE_TYPE_ICE_PERMEABLE,
        //SURFACE_TYPE_CLOTH_PERMEABLE,
        //SURFACE_TYPE_ELECTRICAL,
        //SURFACE_TYPE_FLAMMABLE,
        //SURFACE_TYPE_STEAM,
        //SURFACE_TYPE_ELECTRICAL_PERMEABLE,
        //SURFACE_TYPE_FLAMMABLE_PERMEABLE,
        //SURFACE_TYPE_STEAM_PERMEABLE,
        //SURFACE_TYPE_WATER_PERMEABLE,
        //SURFACE_TYPE_TIBERIUM_WATER,
        //SURFACE_TYPE_TIBERIUM_WATER_PERMEABLE,
        //SURFACE_TYPE_UNDERWATER_DIRT,
        //SURFACE_TYPE_UNDERWATER_TIBERIUM_DIRT,
        //SURFACE_TYPE_MAX			// NOTE: if you add a surface type, add it to the SurfaceEffects.INI file!
    }


    [Flags]
    public enum MeshAttributes
    {
        /////////////////////////////////////////////////////////////////////////////////////////////
        // Flags for the Mesh Attributes member
        /////////////////////////////////////////////////////////////////////////////////////////////

        //#define W3D_MESH_FLAG_NONE										0x00000000		// plain ole normal mesh
        //#define W3D_MESH_FLAG_COLLISION_BOX							0x00000001		// (obsolete as of 4.1) mesh is a collision box (should be 8 verts, should be hidden, etc)
        //#define W3D_MESH_FLAG_SKIN										0x00000002		// (obsolete as of 4.1) skin mesh 
        //#define W3D_MESH_FLAG_SHADOW									0x00000004		// (obsolete as of 4.1) intended to be projected as a shadow
        //#define W3D_MESH_FLAG_ALIGNED									0x00000008		// (obsolete as of 4.1) always aligns with camera

        //#define W3D_MESH_FLAG_COLLISION_TYPE_MASK					0x00000FF0		// mask for the collision type bits						//0000	1111	1111	0000
        //#define W3D_MESH_FLAG_COLLISION_TYPE_SHIFT							4		// shifting to get to the collision type bits					
        //#define W3D_MESH_FLAG_COLLISION_TYPE_PHYSICAL			0x00000010		// physical collisions									//0000	0000	0001	0000
        //#define W3D_MESH_FLAG_COLLISION_TYPE_PROJECTILE			0x00000020		// projectiles (rays) collide with this							//0000	0000	0010	0000
        //#define W3D_MESH_FLAG_COLLISION_TYPE_VIS					0x00000040		// vis rays collide with this mesh						//0000	0000	0100	0000
        //#define W3D_MESH_FLAG_COLLISION_TYPE_CAMERA				0x00000080		// camera rays/boxes collide with this mesh 						//0000	0000	1000	0000
        //#define W3D_MESH_FLAG_COLLISION_TYPE_VEHICLE				0x00000100		// vehicles collide with this mesh (and with physical collision meshes) 	//0000	0001	0000	0000
        //#define W3D_MESH_FLAG_COLLISION_TYPE_EXTRA1					0x00000200	// Extra 1											//0000	0010	0000	0000
        //#define W3D_MESH_FLAG_COLLISION_TYPE_EXTRA2					0x00000400	// Extra 2											//0000	0100	0000	0000
        //#define W3D_MESH_FLAG_COLLISION_TYPE_EXTRA3					0x00000800	// Extra 2											//0000	0100	0000	0000


        //#define W3D_MESH_FLAG_HIDDEN									0x00001000		// this mesh is hidden by default					//0001	0000	0000	0000
        //#define W3D_MESH_FLAG_TWO_SIDED								0x00002000		// render both sides of this mesh
        //#define OBSOLETE_W3D_MESH_FLAG_LIGHTMAPPED				0x00004000		// obsolete lightmapped mesh
        //                                                                                            // NOTE: retained for backwards compatibility - use W3D_MESH_FLAG_PRELIT_* instead.
        //#define W3D_MESH_FLAG_CAST_SHADOW							0x00008000		// this mesh casts shadows

        //#define W3D_MESH_FLAG_GEOMETRY_TYPE_MASK					0x00FF0000		// (introduced with 4.1)
        //#define W3D_MESH_FLAG_GEOMETRY_TYPE_NORMAL				0x00000000		// (4.1+) normal mesh geometry
        //#define W3D_MESH_FLAG_GEOMETRY_TYPE_CAMERA_ALIGNED		0x00010000		// (4.1+) camera aligned mesh
        //#define W3D_MESH_FLAG_GEOMETRY_TYPE_SKIN					0x00020000		// (4.1+) skin mesh
        //#define OBSOLETE_W3D_MESH_FLAG_GEOMETRY_TYPE_SHADOW	0x00030000		// (4.1+) shadow mesh OBSOLETE!
        //#define W3D_MESH_FLAG_GEOMETRY_TYPE_AABOX					0x00040000		// (4.1+) aabox OBSOLETE!
        //#define W3D_MESH_FLAG_GEOMETRY_TYPE_OBBOX					0x00050000		// (4.1+) obbox OBSOLETE!
        //#define W3D_MESH_FLAG_GEOMETRY_TYPE_CAMERA_ORIENTED	0x00060000		// (4.1+) camera oriented mesh (points _towards_ camera)

        //#define W3D_MESH_FLAG_PRELIT_MASK							0x0F000000		// (4.2+) 
        //#define W3D_MESH_FLAG_PRELIT_UNLIT							0x01000000		// mesh contains an unlit material chunk wrapper
        //#define W3D_MESH_FLAG_PRELIT_VERTEX							0x02000000		// mesh contains a precalculated vertex-lit material chunk wrapper 
        //#define W3D_MESH_FLAG_PRELIT_LIGHTMAP_MULTI_PASS		0x04000000		// mesh contains a precalculated multi-pass lightmapped material chunk wrapper
        //#define W3D_MESH_FLAG_PRELIT_LIGHTMAP_MULTI_TEXTURE	0x08000000		// mesh contains a precalculated multi-texture lightmapped material chunk wrapper

        //#define W3D_MESH_FLAG_SHATTERABLE							0x10000000		// this mesh is shatterable.
        //#define W3D_MESH_FLAG_NPATCHABLE								0x20000000		// it is ok to NPatch this mesh

        //#define W3D_CURRENT_MESH_VERSION		W3D_MAKE_VERSION(4,2)
    }

    [Flags]
    public enum VertexChannelFlags
    {
        //#define W3D_VERTEX_CHANNEL_LOCATION		0x00000001	// object-space location of the vertex
        //#define W3D_VERTEX_CHANNEL_NORMAL		0x00000002	// object-space normal for the vertex
        //#define W3D_VERTEX_CHANNEL_TEXCOORD		0x00000004	// texture coordinate
        //#define W3D_VERTEX_CHANNEL_COLOR			0x00000008	// vertex color
        //#define W3D_VERTEX_CHANNEL_BONEID		0x00000010	// per-vertex bone id for skins

        //#define W3D_FACE_CHANNEL_FACE				0x00000001	// basic face info, W3dTriStruct...
    }

    [Flags]
    public enum AnimationFlags
    {
        //ANIM_CHANNEL_X = 0,
        //ANIM_CHANNEL_Y,
        //ANIM_CHANNEL_Z,
        //ANIM_CHANNEL_XR,
        //ANIM_CHANNEL_YR,
        //ANIM_CHANNEL_ZR,
        //ANIM_CHANNEL_Q,

        //ANIM_CHANNEL_TIMECODED_X,
        //ANIM_CHANNEL_TIMECODED_Y,
        //ANIM_CHANNEL_TIMECODED_Z,
        //ANIM_CHANNEL_TIMECODED_Q,

        //ANIM_CHANNEL_ADAPTIVEDELTA_X,
        //ANIM_CHANNEL_ADAPTIVEDELTA_Y,
        //ANIM_CHANNEL_ADAPTIVEDELTA_Z,
        //ANIM_CHANNEL_ADAPTIVEDELTA_Q,
    }


    [Flags]
    public enum AnimationFlavors
    {
        //
        // Flavor Enumerations
        //
        //ANIM_FLAVOR_TIMECODED = 0,
        //ANIM_FLAVOR_ADAPTIVE_DELTA,

        //ANIM_FLAVOR_VALID
    }

    [Flags]
    public enum AnimationFrameFlags
    {
        //BIT_CHANNEL_VIS = 0,							// turn meshes on and off depending on anim frame.
        //BIT_CHANNEL_TIMECODED_VIS,
    };
}
