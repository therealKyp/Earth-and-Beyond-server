// ChunkTypes.cpp : Defines the entry point for the console application.
//

#include"stdafx.h"
#include "..\INC\w3d_file.h"
#include <windows.h>

FILE *outf;
char *filename;
int indent;
int count;


#define ASSERT(exp)                          \
{                                            \
    if ( !(exp) )                            \
    {                                        \
        printf("%s, %d\n", #exp, __LINE__);  \
        getchar();                           \
        exit(1);                             \
    }                                        \
}                                            \

// forward references
void ProcessFile(char *filename);
DWORD ReadHeader(FILE *f, int depth, DWORD max_size);
bool IsNested(DWORD type);
void ProcessChunk(DWORD type, BYTE *data, DWORD size);
void ChunkMesh(BYTE *data, DWORD size);
void ChunkAggregateHeader(BYTE *data, DWORD size);
void ChunkAggregateInfo(BYTE *data, DWORD size);
void ChunkAggregateClassInfo(BYTE *data, DWORD size);
void ChunkMeshUserText(BYTE *data, DWORD size);
void ChunkMeshHeader(BYTE *data, DWORD size);
void ChunkMeshHeader3(BYTE *data, DWORD size);
void ChunkHierarchyHeader(BYTE *data, DWORD size);
void ChunkAnimationHeader(BYTE *data, DWORD size);
void ChunkCompressedAnimHeader(BYTE *data, DWORD size);
void ChunkPivot(BYTE *data, DWORD size);
void ChunkMorphAnimHeader(BYTE *data, DWORD size);
void ChunkHModelHeader(BYTE *data, DWORD size);
void ChunkHModelNode(BYTE *data, DWORD size);
void ChunkEmitterHeader(BYTE *data, DWORD size);
void ChunkHLodHeader(BYTE *data, DWORD size);
void ChunkHLodArrayHeader(BYTE *data, DWORD size);
void ChunkHLodSubObject(BYTE *data, DWORD size);
void ChunkBox(BYTE *data, DWORD size);
void ChunkTextureName(BYTE *data, DWORD size);


struct chunk_types_S
{
    DWORD type;
    char *text;
    DWORD min_size;
    DWORD max_size;
    DWORD count;
};

struct chunk_types_S chunk_table[] =
{
    { 0x00000000, "W3D_CHUNK_MESH", 0xFFFFFFFF, 0, 0 },
    { 0x00000001, "W3D_CHUNK_MESH_HEADER", 0xFFFFFFFF, 0, 0 },
    { 0x00000002, "W3D_CHUNK_VERTICES", 0xFFFFFFFF, 0, 0 },
    { 0x00000003, "W3D_CHUNK_VERTEX_NORMALS", 0xFFFFFFFF, 0, 0 },
    { 0x00000004, "W3D_CHUNK_SURRENDER_NORMALS", 0xFFFFFFFF, 0, 0 },
    { 0x00000005, "W3D_CHUNK_TEXCOORDS", 0xFFFFFFFF, 0, 0 },
    { 0x00000006, "O_W3D_CHUNK_MATERIALS", 0xFFFFFFFF, 0, 0 },
    { 0x00000007, "O_W3D_CHUNK_TRIANGLES", 0xFFFFFFFF, 0, 0 },
    { 0x00000008, "O_W3D_CHUNK_QUADRANGLES", 0xFFFFFFFF, 0, 0 },
    { 0x00000009, "O_W3D_CHUNK_SURRENDER_TRIANGLES", 0xFFFFFFFF, 0, 0 },
    { 0x0000000A, "O_W3D_CHUNK_POV_TRIANGLES", 0xFFFFFFFF, 0, 0 },
    { 0x0000000B, "O_W3D_CHUNK_POV_QUADRANGLES", 0xFFFFFFFF, 0, 0 },
    { 0x0000000C, "W3D_CHUNK_MESH_USER_TEXT", 0xFFFFFFFF, 0, 0 },
    { 0x0000000D, "W3D_CHUNK_VERTEX_COLORS", 0xFFFFFFFF, 0, 0 },
    { 0x0000000E, "W3D_CHUNK_VERTEX_INFLUENCES", 0xFFFFFFFF, 0, 0 },
    { 0x0000000F, "O_W3D_CHUNK_DAMAGE", 0xFFFFFFFF, 0, 0 },
    { 0x00000010, "O_W3D_CHUNK_DAMAGE_HEADER", 0xFFFFFFFF, 0, 0 },
    { 0x00000011, "O_W3D_CHUNK_DAMAGE_MATERIALS", 0xFFFFFFFF, 0, 0 },
    { 0x00000012, "O_W3D_CHUNK_DAMAGE_VERTICES", 0xFFFFFFFF, 0, 0 },
    { 0x00000013, "O_W3D_CHUNK_DAMAGE_COLORS", 0xFFFFFFFF, 0, 0 },
    { 0x00000014, "O_W3D_CHUNK_MATERIALS2", 0xFFFFFFFF, 0, 0 },
    { 0x00000015, "W3D_CHUNK_VERTEX_MATERIALS3", 0xFFFFFFFF, 0, 0 },
    { 0x00000016, "W3D_CHUNK_VERTEX_MATERIAL3", 0xFFFFFFFF, 0, 0 },
    { 0x00000017, "W3D_CHUNK_VERTEX_MATERIAL3_NAME", 0xFFFFFFFF, 0, 0 },
    { 0x00000018, "W3D_CHUNK_VERTEX_MATERIAL3_INFO", 0xFFFFFFFF, 0, 0 },
    { 0x00000019, "W3D_CHUNK_VERTEX_MATERIAL3_DC_MAP", 0xFFFFFFFF, 0, 0 },
    //  W3D_CHUNK_VERTEX_MATERIAL3_DI_MAP ???
    { 0x0000001A, "W3D_CHUNK_MAP3_FILENAME", 0xFFFFFFFF, 0, 0 },
    { 0x0000001B, "W3D_CHUNK_MAP3_INFO", 0xFFFFFFFF, 0, 0 },
    { 0x0000001D, "W3D_CHUNK_VERTEX_MATERIAL3_SC_MAP", 0xFFFFFFFF, 0, 0 },
    { 0x0000001E, "W3D_CHUNK_VERTEX_MATERIAL3_SI_MAP", 0xFFFFFFFF, 0, 0 },
    { 0x0000001F, "W3D_CHUNK_MESH_HEADER3", 0xFFFFFFFF, 0, 0 },
    { 0x00000020, "W3D_CHUNK_TRIANGLES", 0xFFFFFFFF, 0, 0 },
    { 0x00000021, "W3D_CHUNK_PER_TRI_MATERIALS", 0xFFFFFFFF, 0, 0 },
    { 0x00000022, "W3D_CHUNK_VERTEX_SHADE_INDICES", 0xFFFFFFFF, 0, 0 },
    { 0x00000023, "W3D_CHUNK_PRELIT_UNLIT", 0xFFFFFFFF, 0, 0 },
    { 0x00000024, "W3D_CHUNK_PRELIT_VERTEX", 0xFFFFFFFF, 0, 0 },
    { 0x00000025, "W3D_CHUNK_PRELIT_LIGHTMAP_MULTI_PASS", 0xFFFFFFFF, 0, 0 },
    { 0x00000026, "W3D_CHUNK_PRELIT_LIGHTMAP_MULTI_TEXTURE", 0xFFFFFFFF, 0, 0 },
    { 0x00000028, "W3D_CHUNK_MATERIAL_INFO", 0xFFFFFFFF, 0, 0 },
    { 0x00000029, "W3D_CHUNK_SHADERS", 0xFFFFFFFF, 0, 0 },
    { 0x0000002A, "W3D_CHUNK_VERTEX_MATERIALS", 0xFFFFFFFF, 0, 0 },
    { 0x0000002B, "W3D_CHUNK_VERTEX_MATERIAL", 0xFFFFFFFF, 0, 0 },
    { 0x0000002C, "W3D_CHUNK_VERTEX_MATERIAL_NAME", 0xFFFFFFFF, 0, 0 },
    { 0x0000002D, "W3D_CHUNK_VERTEX_MATERIAL_INFO", 0xFFFFFFFF, 0, 0 },
    { 0x0000002E, "W3D_CHUNK_VERTEX_MAPPER_ARGS0", 0xFFFFFFFF, 0, 0 },
    { 0x0000002F, "W3D_CHUNK_VERTEX_MAPPER_ARGS1", 0xFFFFFFFF, 0, 0 },
    { 0x00000030, "W3D_CHUNK_TEXTURES", 0xFFFFFFFF, 0, 0 },
    { 0x00000031, "W3D_CHUNK_TEXTURE", 0xFFFFFFFF, 0, 0 },
    { 0x00000032, "W3D_CHUNK_TEXTURE_NAME", 0xFFFFFFFF, 0, 0 },
    { 0x00000033, "W3D_CHUNK_TEXTURE_INFO", 0xFFFFFFFF, 0, 0 },
    { 0x00000038, "W3D_CHUNK_MATERIAL_PASS", 0xFFFFFFFF, 0, 0 },
    { 0x00000039, "W3D_CHUNK_VERTEX_MATERIAL_IDS", 0xFFFFFFFF, 0, 0 },
    { 0x0000003A, "W3D_CHUNK_SHADER_IDS", 0xFFFFFFFF, 0, 0 },
    { 0x0000003B, "W3D_CHUNK_DCG", 0xFFFFFFFF, 0, 0 },
    { 0x0000003C, "W3D_CHUNK_DIG", 0xFFFFFFFF, 0, 0 },
    { 0x0000003E, "W3D_CHUNK_SCG", 0xFFFFFFFF, 0, 0 },
    { 0x00000048, "W3D_CHUNK_TEXTURE_STAGE", 0xFFFFFFFF, 0, 0 },
    { 0x00000049, "W3D_CHUNK_TEXTURE_IDS", 0xFFFFFFFF, 0, 0 },
    { 0x0000004A, "W3D_CHUNK_STAGE_TEXCOORDS", 0xFFFFFFFF, 0, 0 },
    { 0x0000004B, "W3D_CHUNK_PER_FACE_TEXCOORD_IDS", 0xFFFFFFFF, 0, 0 },
    { 0x00000058, "W3D_CHUNK_DEFORM", 0xFFFFFFFF, 0, 0 },
    { 0x00000059, "W3D_CHUNK_DEFORM_SET", 0xFFFFFFFF, 0, 0 },
    { 0x0000005A, "W3D_CHUNK_DEFORM_KEYFRAME", 0xFFFFFFFF, 0, 0 },
    { 0x0000005B, "W3D_CHUNK_DEFORM_DATA", 0xFFFFFFFF, 0, 0 },
    { 0x00000080, "W3D_CHUNK_PS2_SHADERS", 0xFFFFFFFF, 0, 0 },
    { 0x00000090, "W3D_CHUNK_AABTREE", 0xFFFFFFFF, 0, 0 },
    { 0x00000091, "W3D_CHUNK_AABTREE_HEADER", 0xFFFFFFFF, 0, 0 },
    { 0x00000092, "W3D_CHUNK_AABTREE_POLYINDICES", 0xFFFFFFFF, 0, 0 },
    { 0x00000093, "W3D_CHUNK_AABTREE_NODES", 0xFFFFFFFF, 0, 0 },
    { 0x00000100, "W3D_CHUNK_HIERARCHY", 0xFFFFFFFF, 0, 0 },
    { 0x00000101, "W3D_CHUNK_HIERARCHY_HEADER", 0xFFFFFFFF, 0, 0 },
    { 0x00000102, "W3D_CHUNK_PIVOTS", 0xFFFFFFFF, 0, 0 },
    { 0x00000103, "W3D_CHUNK_PIVOT_FIXUPS", 0xFFFFFFFF, 0, 0 },
    { 0x00000200, "W3D_CHUNK_ANIMATION", 0xFFFFFFFF, 0, 0 },
    { 0x00000201, "W3D_CHUNK_ANIMATION_HEADER", 0xFFFFFFFF, 0, 0 },
    { 0x00000202, "W3D_CHUNK_ANIMATION_CHANNEL", 0xFFFFFFFF, 0, 0 },
    { 0x00000203, "W3D_CHUNK_BIT_CHANNEL", 0xFFFFFFFF, 0, 0 },
    { 0x00000280, "W3D_CHUNK_COMPRESSED_ANIMATION", 0xFFFFFFFF, 0, 0 },
    { 0x00000281, "W3D_CHUNK_COMPRESSED_ANIMATION_HEADER", 0xFFFFFFFF, 0, 0 },
    { 0x00000282, "W3D_CHUNK_COMPRESSED_ANIMATION_CHANNEL", 0xFFFFFFFF, 0, 0 },
    { 0x00000283, "W3D_CHUNK_COMPRESSED_BIT_CHANNEL", 0xFFFFFFFF, 0, 0 },
    { 0x000002C0, "W3D_CHUNK_MORPH_ANIMATION", 0xFFFFFFFF, 0, 0 },
    { 0x000002C1, "W3D_CHUNK_MORPHANIM_HEADER", 0xFFFFFFFF, 0, 0 },
    { 0x000002C2, "W3D_CHUNK_MORPHANIM_CHANNEL", 0xFFFFFFFF, 0, 0 },
    { 0x000002C3, "W3D_CHUNK_MORPHANIM_POSENAME", 0xFFFFFFFF, 0, 0 },
    { 0x000002C4, "W3D_CHUNK_MORPHANIM_KEYDATA", 0xFFFFFFFF, 0, 0 },
    { 0x000002C5, "W3D_CHUNK_MORPHANIM_PIVOTCHANNELDATA", 0xFFFFFFFF, 0, 0 },
    { 0x00000300, "W3D_CHUNK_HMODEL", 0xFFFFFFFF, 0, 0 },
    { 0x00000301, "W3D_CHUNK_HMODEL_HEADER", 0xFFFFFFFF, 0, 0 },
    { 0x00000302, "W3D_CHUNK_NODE", 0xFFFFFFFF, 0, 0 },
    { 0x00000303, "W3D_CHUNK_COLLISION_NODE", 0xFFFFFFFF, 0, 0 },
    { 0x00000304, "W3D_CHUNK_SKIN_NODE", 0xFFFFFFFF, 0, 0 },
    { 0x00000305, "O_W3D_CHUNK_HMODEL_AUX_DATA", 0xFFFFFFFF, 0, 0 },
    { 0x00000306, "O_W3D_CHUNK_SHADOW_NODE", 0xFFFFFFFF, 0, 0 },
    { 0x00000400, "W3D_CHUNK_LODMODEL", 0xFFFFFFFF, 0, 0 },
    { 0x00000401, "W3D_CHUNK_LODMODEL_HEADER", 0xFFFFFFFF, 0, 0 },
    { 0x00000402, "W3D_CHUNK_LOD", 0xFFFFFFFF, 0, 0 },
    { 0x00000420, "W3D_CHUNK_COLLECTION", 0xFFFFFFFF, 0, 0 },
    { 0x00000421, "W3D_CHUNK_COLLECTION_HEADER", 0xFFFFFFFF, 0, 0 },
    { 0x00000422, "W3D_CHUNK_COLLECTION_OBJ_NAME", 0xFFFFFFFF, 0, 0 },
    { 0x00000423, "W3D_CHUNK_PLACEHOLDER", 0xFFFFFFFF, 0, 0 },
    { 0x00000424, "W3D_CHUNK_TRANSFORM_NODE", 0xFFFFFFFF, 0, 0 },
    { 0x00000440, "W3D_CHUNK_POINTS", 0xFFFFFFFF, 0, 0 },
    { 0x00000460, "W3D_CHUNK_LIGHT", 0xFFFFFFFF, 0, 0 },
    { 0x00000461, "W3D_CHUNK_LIGHT_INFO", 0xFFFFFFFF, 0, 0 },
    { 0x00000462, "W3D_CHUNK_SPOT_LIGHT_INFO", 0xFFFFFFFF, 0, 0 },
    { 0x00000463, "W3D_CHUNK_NEAR_ATTENUATION", 0xFFFFFFFF, 0, 0 },
    { 0x00000464, "W3D_CHUNK_FAR_ATTENUATION", 0xFFFFFFFF, 0, 0 },
    { 0x00000500, "W3D_CHUNK_EMITTER", 0xFFFFFFFF, 0, 0 },
    { 0x00000501, "W3D_CHUNK_EMITTER_HEADER", 0xFFFFFFFF, 0, 0 },
    { 0x00000502, "W3D_CHUNK_EMITTER_USER_DATA", 0xFFFFFFFF, 0, 0 },
    { 0x00000503, "W3D_CHUNK_EMITTER_INFO", 0xFFFFFFFF, 0, 0 },
    { 0x00000504, "W3D_CHUNK_EMITTER_INFOV2", 0xFFFFFFFF, 0, 0 },
    { 0x00000505, "W3D_CHUNK_EMITTER_PROPS", 0xFFFFFFFF, 0, 0 },
    { 0x00000506, "O_W3D_CHUNK_EMITTER_COLOR_KEYFRAME", 0xFFFFFFFF, 0, 0 },
    { 0x00000507, "O_W3D_CHUNK_EMITTER_OPACITY_KEYFRAME", 0xFFFFFFFF, 0, 0 },
    { 0x00000508, "O_W3D_CHUNK_EMITTER_SIZE_KEYFRAME", 0xFFFFFFFF, 0, 0 },
    { 0x00000509, "W3D_CHUNK_EMITTER_LINE_PROPERTIES", 0xFFFFFFFF, 0, 0 },
    { 0x0000050A, "W3D_CHUNK_EMITTER_ROTATION_KEYFRAMES", 0xFFFFFFFF, 0, 0 },
    { 0x0000050B, "W3D_CHUNK_EMITTER_FRAME_KEYFRAMES", 0xFFFFFFFF, 0, 0 },
    { 0x0000050C, "W3D_CHUNK_EMITTER_BLUR_TIME_KEYFRAMES", 0xFFFFFFFF, 0, 0 },
    { 0x0000050D, "W3D_CHUNK_EMITTER_UNKNOWN_CHUNK_TYPE", 0xFFFFFFFF, 0, 0 },
    { 0x00000600, "W3D_CHUNK_AGGREGATE", 0xFFFFFFFF, 0, 0 },
    { 0x00000601, "W3D_CHUNK_AGGREGATE_HEADER", 0xFFFFFFFF, 0, 0 },
    { 0x00000602, "W3D_CHUNK_AGGREGATE_INFO", 0xFFFFFFFF, 0, 0 },
    { 0x00000603, "W3D_CHUNK_TEXTURE_REPLACER_INFO", 0xFFFFFFFF, 0, 0 },
    { 0x00000604, "W3D_CHUNK_AGGREGATE_CLASS_INFO", 0xFFFFFFFF, 0, 0 },
    { 0x00000700, "W3D_CHUNK_HLOD", 0xFFFFFFFF, 0, 0 },
    { 0x00000701, "W3D_CHUNK_HLOD_HEADER", 0xFFFFFFFF, 0, 0 },
    { 0x00000702, "W3D_CHUNK_HLOD_LOD_ARRAY", 0xFFFFFFFF, 0, 0 },
    { 0x00000703, "W3D_CHUNK_HLOD_SUB_OBJECT_ARRAY_HEADER", 0xFFFFFFFF, 0, 0 },
    { 0x00000704, "W3D_CHUNK_HLOD_SUB_OBJECT", 0xFFFFFFFF, 0, 0 },
    { 0x00000705, "W3D_CHUNK_HLOD_AGGREGATE_ARRAY", 0xFFFFFFFF, 0, 0 },
    { 0x00000706, "W3D_CHUNK_HLOD_PROXY_ARRAY", 0xFFFFFFFF, 0, 0 },
    { 0x00000740, "W3D_CHUNK_BOX", 0xFFFFFFFF, 0, 0 },
    { 0x00000741, "W3D_CHUNK_SPHERE", 0xFFFFFFFF, 0, 0 },
    { 0x00000742, "W3D_CHUNK_RING", 0xFFFFFFFF, 0, 0 },
    { 0x00000750, "W3D_CHUNK_NULL_OBJECT", 0xFFFFFFFF, 0, 0 },
    { 0x00000800, "W3D_CHUNK_LIGHTSCAPE", 0xFFFFFFFF, 0, 0 },
    { 0x00000801, "W3D_CHUNK_LIGHTSCAPE_LIGHT", 0xFFFFFFFF, 0, 0 },
    { 0x00000802, "W3D_CHUNK_LIGHT_TRANSFORM", 0xFFFFFFFF, 0, 0 },
    { 0x00000900, "W3D_CHUNK_DAZZLE", 0xFFFFFFFF, 0, 0 },
    { 0x00000901, "W3D_CHUNK_DAZZLE_NAME", 0xFFFFFFFF, 0, 0 },
    { 0x00000902, "W3D_CHUNK_DAZZLE_TYPENAME", 0xFFFFFFFF, 0, 0 },
    { 0x00000A00, "W3D_CHUNK_SOUNDROBJ", 0xFFFFFFFF, 0, 0 },
    { 0x00000A01, "W3D_CHUNK_SOUNDROBJ_HEADER", 0xFFFFFFFF, 0, 0 },
    { 0x00000A02, "W3D_CHUNK_SOUNDROBJ_DEFINITION", 0xFFFFFFFF, 0, 0 },
    { 0xFFFFFFFF, "", 0, 0 }
};

int main(int argc, char* argv[])
{
    WIN32_FIND_DATA fdata = {NULL};
    filename = fdata.cFileName;
    count = 0;

    outf = fopen("C:\\MSI\\JOBS\\BETC\\ChunkTypes\\output.txt", "w");
    // outf = fopen("K:\\Perzonal\\fun\\FL\\ChunkTypes\\output.txt", "w");
    if (!outf)
    {
        printf("*ERROR* Unable to open output.txt\n");
        return 1;
    }

    SetCurrentDirectory("K:\\Perzonal\\fun\\enb\\EB1\\Data\\client\\mixfiles\\Art\\");
//    SetCurrentDirectory("K:\\Perzonal\\fun\\enb\\mixfiles\\Art\\");
//    SetCurrentDirectory("K:\\Perzonal\\fun\\enb\\EB1\\EBCONFIG\\");

    HANDLE h = FindFirstFile("*.w3d", &fdata);

    /*
    do
    {
        ProcessFile(filename);
    } while (FindNextFile(h, &fdata));
    */

    ProcessFile("defender.w3d");
    ProcessFile("enforcer.w3d");
    ProcessFile("explorer.w3d");
    ProcessFile("privateer.w3d");
    ProcessFile("scout.w3d");
    ProcessFile("seeker.w3d");
    ProcessFile("sentinel.w3d");
    ProcessFile("tradesman.w3d");
    ProcessFile("warrior.w3d");

    printf("total count=%d\n", count);

    fprintf(outf, "total count=%d\n", count);

    fprintf(outf, "fixed size:\n");

    struct chunk_types_S *p = chunk_table;

    while (p->type != 0xFFFFFFFF)
    {
        if ((p->count > 0) &&
            (!IsNested(p->type)) &&
            (p->min_size == p->max_size))
        {
            fprintf(outf, "%08X, %s, %d, %d, %d\n",
                p->type, p->text, p->count, p->min_size, p->max_size);
        }
        p++;
    }

    fprintf(outf, "\nvariable size:\n");

    p = chunk_table;

    while (p->type != 0xFFFFFFFF)
    {
        if ((p->count > 0) &&
            (!IsNested(p->type)) &&
            (p->min_size != p->max_size))
        {
            fprintf(outf, "%08X, %s, %d, %d, %d\n",
                p->type, p->text, p->count, p->min_size, p->max_size);
        }
        p++;
    }

    fprintf(outf, "\nnested\n");

    p = chunk_table;

    while (p->type != 0xFFFFFFFF)
    {
        if ((p->count > 0) &&
            (IsNested(p->type)))
        {
            fprintf(outf, "%08X, %s, %d, %d, %d\n",
                p->type, p->text, p->count, p->min_size, p->max_size);
        }
        p++;
    }

	fclose(outf);

    return 0;
}

void ProcessFile(char *filename)
{
    FILE *f;
    count++;
    fprintf(outf, "%d:%s\n", count, filename);
    printf("%d:%s\n", count, filename);
    f = fopen(filename, "rb");
    if (f)
    {
	    fseek(f, 0, SEEK_END);
	    DWORD file_size = ftell(f);
	    fseek(f, 0, SEEK_SET);
        while (!feof(f))
        {
            ReadHeader(f, 0, file_size);
        }
        fclose(f);
    }
    printf("----------\n");
    fprintf(outf,"----------\n");
}

bool IsNested(DWORD type)
{
    static DWORD nested[] =
    {
        W3D_CHUNK_MESH,
        W3D_CHUNK_VERTEX_MATERIALS3,
        W3D_CHUNK_VERTEX_MATERIAL3,
        W3D_CHUNK_VERTEX_MATERIAL3_DC_MAP,
        W3D_CHUNK_VERTEX_MATERIAL3_SC_MAP,
        W3D_CHUNK_VERTEX_MATERIAL3_SI_MAP,
        W3D_CHUNK_VERTEX_MATERIALS,
        W3D_CHUNK_VERTEX_MATERIAL,
        W3D_CHUNK_TEXTURES,
        W3D_CHUNK_TEXTURE,
        W3D_CHUNK_MATERIAL_PASS,
        W3D_CHUNK_TEXTURE_STAGE,
        W3D_CHUNK_AABTREE,
        W3D_CHUNK_HIERARCHY,
        W3D_CHUNK_ANIMATION,
        W3D_CHUNK_COMPRESSED_ANIMATION,
        W3D_CHUNK_MORPH_ANIMATION,
        W3D_CHUNK_HMODEL,
        W3D_CHUNK_LODMODEL,
        W3D_CHUNK_EMITTER,
        W3D_CHUNK_AGGREGATE,
        W3D_CHUNK_HLOD,
        W3D_CHUNK_HLOD_LOD_ARRAY,
        W3D_CHUNK_DAZZLE,
        W3D_CHUNK_COLLECTION,
        W3D_CHUNK_LIGHT,
        W3D_CHUNK_LIGHTSCAPE,
        W3D_CHUNK_SOUNDROBJ,
        0xFFFFFFFF
    };

    DWORD *p = nested;
    while (*p != 0xFFFFFFFF)
    {
        if (type == *p)
        {
            return (true);
        }
        p++;
    }

    return (false);
}

char *lookup(DWORD type, DWORD size)
{
    struct chunk_types_S *p = chunk_table;

    while (p->type != 0xFFFFFFFF)
    {
        if (type == p->type)
        {
            if (p->max_size < size)
            {
                p->max_size = size;
            }
            if (p->min_size > size)
            {
                p->min_size = size;
            }
            p->count++;
            return (p->text);
        }
        p++;
    }

    return ("*ERROR*");
}

char *Indent()
{
    static char text[80];
    sprintf(text, "%*.*s", indent, indent, "|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ");
    return (text);
}

DWORD ReadHeader(FILE *f, int depth, DWORD max_size)
{
    struct
    {
        DWORD type;
        DWORD size;
    } header;

    DWORD size = 0;

    if (fread(&header, 1, sizeof(header), f) == sizeof(header))
    {
        size = header.size & 0x7FFFFFFF;

        char *chunk_type = lookup(header.type, size);
        indent = depth * 3;
        fprintf(outf, "%s%08X %s [%d]\n", Indent(), header.type, chunk_type, size);
        indent = depth * 3 + 9;

        if (size > 4*1024*1024)
        {
            printf("*ERROR* header is exceedingly big!\n");
        }
        else if (depth > 10)
        {
            printf("*ERROR* nested too deep!\n");
        }
        else if (size + sizeof(header) > max_size)
        {
            printf("*ERROR* structure size exceeds container!\n");
        }
        else
        {
            if (IsNested(header.type))
            {
                long remaining = size;
                while (remaining > 0)
                {
                    DWORD bytes_read = ReadHeader(f, depth + 1, remaining) + sizeof(header);
                    remaining -= bytes_read;
                }
            }
            else
            {
                BYTE *data = new BYTE[size];
                fread(data, size, 1, f);
                ProcessChunk(header.type, data, size);
                delete data;
            }
        }
    }
    else
    {
        if (depth > 0)
        {
            printf("*ERROR* fread failed\n");
        }
    }

    return (size);
}

void ProcessChunk(DWORD type, BYTE *data, DWORD size)
{
    struct chunk_types_S *p = chunk_table;

    while (p->type != 0xFFFFFFFF)
    {
        if (type == p->type)
        {
            if (p->max_size < size)
            {
                p->max_size = size;
            }
            if (p->min_size > size)
            {
                p->min_size = size;
            }
            p->count++;
            switch (type)
            {
            case W3D_CHUNK_MESH :
                ChunkMesh(data, size);
                break;
            case W3D_CHUNK_AGGREGATE_HEADER :
                ChunkAggregateHeader(data, size);
                break;
            case W3D_CHUNK_AGGREGATE_INFO :
                ChunkAggregateInfo(data, size);
                break;
            case W3D_CHUNK_AGGREGATE_CLASS_INFO :
                ChunkAggregateClassInfo(data, size);
                break;
            case W3D_CHUNK_MESH_USER_TEXT :
                ChunkMeshUserText(data, size);
                break;
            case O_W3D_CHUNK_MESH_HEADER :
                ChunkMeshHeader(data, size);
                break;
            case W3D_CHUNK_MESH_HEADER3 :
                ChunkMeshHeader3(data, size);
                break;
            case W3D_CHUNK_HIERARCHY_HEADER :
                ChunkHierarchyHeader(data, size);
                break;
            case W3D_CHUNK_ANIMATION_HEADER :
                ChunkAnimationHeader(data, size);
                break;
            case W3D_CHUNK_COMPRESSED_ANIMATION_HEADER :
                ChunkCompressedAnimHeader(data, size);
                break;
            case W3D_CHUNK_PIVOTS :
                ChunkPivot(data, size);
                break;
            case W3D_CHUNK_MORPHANIM_HEADER :
                ChunkMorphAnimHeader(data, size);
                break;
            case W3D_CHUNK_HMODEL_HEADER :
                ChunkHModelHeader(data, size);
                break;
            case W3D_CHUNK_NODE :
                ChunkHModelNode(data, size);
                break;
            case W3D_CHUNK_EMITTER_HEADER :
                ChunkEmitterHeader(data, size);
                break;
            case W3D_CHUNK_HLOD_HEADER :
                ChunkHLodHeader(data, size);
                break;
            case W3D_CHUNK_HLOD_SUB_OBJECT_ARRAY_HEADER :
                ChunkHLodArrayHeader(data, size);
                break;
            case W3D_CHUNK_HLOD_SUB_OBJECT :
                ChunkHLodSubObject(data, size);
                break;
            case W3D_CHUNK_BOX :
                ChunkBox(data, size);
                break;
            case W3D_CHUNK_TEXTURE_NAME :
                ChunkTextureName(data, size);
                break;

            case W3D_CHUNK_MATERIAL_INFO :
                break;

            // TODO:
            case W3D_CHUNK_AABTREE :
            case W3D_CHUNK_AABTREE_HEADER :
            case W3D_CHUNK_AABTREE_POLYINDICES :
            case W3D_CHUNK_AABTREE_NODES :
            case W3D_CHUNK_PIVOT_FIXUPS :
            case W3D_CHUNK_VERTICES :
            case W3D_CHUNK_VERTEX_NORMALS :
            case W3D_CHUNK_TRIANGLES :
            case W3D_CHUNK_VERTEX_SHADE_INDICES :
            case W3D_CHUNK_VERTEX_MATERIAL_NAME :
            case W3D_CHUNK_VERTEX_MATERIAL_INFO :
            case W3D_CHUNK_VERTEX_MAPPER_ARGS0 :
            case W3D_CHUNK_VERTEX_MAPPER_ARGS1 :
            case W3D_CHUNK_VERTEX_MATERIAL_IDS :
            case W3D_CHUNK_SHADER_IDS :
            case W3D_CHUNK_TEXTURE_STAGE :
            case W3D_CHUNK_TEXTURE_IDS :
            case W3D_CHUNK_TEXTURE_INFO :
            case W3D_CHUNK_STAGE_TEXCOORDS :
            case W3D_CHUNK_SHADERS :
            case W3D_CHUNK_DCG :
            case W3D_CHUNK_HLOD_PROXY_ARRAY :
                break;

            default :
                fprintf(outf, "%sWarning: WW3D Chunk Type 0x%08X skipped\n", Indent(), type);
                break;
            }
            return;
        }
        p++;
    }

    printf("*ERROR* Unrecognized Chunk Type %08X\n", type);
}

void ChunkMesh(BYTE *data, DWORD size)
{
    // defender.w3d     1
    // enforcer.w3d     2
    // explorer.w3d     3
    // privateer.w3d    4
    // scout.w3d        5
    // seeker.w3d       6
    // sentinel.w3d     7
    // tradesman.w3d    8
    // warrior.w3d      9

    // warrior02.w3d

    // defender03.w3d   1
    // enforcer03.w3d   2
    // explorer03.w3d   3
    // privateer03.w3d  4
    // scout03.w3d      5
    // seeker03.w3d     6
    // sentinel03.w3d   7
    // tradesman03.w3d  8
    // warrior03.w3d    9

    // defender_tail.w3d
    // enforcer_wing.w3d
    // explorer_tail.w3d
    // privateer_wing.w3d
    // scout_wing.w3d
    // seeker_tail.w3d
    // sentinel_wing.w3d
    // tradesman_wing.w3d
    // warrior_prof.w3d
    // warrior_wing.w3d
}

void ChunkAggregateHeader(BYTE *data, DWORD size)
{
    ASSERT(size == sizeof(W3dAggregateHeaderStruct));

    W3dAggregateHeaderStruct *header = (W3dAggregateHeaderStruct *) data;

    // The aggregate name is always the same as the filename
    printf("Aggregate: \"%s\"\n", header->Name);
    fprintf(outf, "%sAggregate: \"%s\"\n", Indent(), header->Name);
}

void ChunkAggregateInfo(BYTE *data, DWORD size)
{
    W3dAggregateInfoStruct *info = (W3dAggregateInfoStruct *) data;

    printf("Aggregate BaseModelName: \"%s\"\n", info->BaseModelName);
    printf("Aggregate SubobjectCount: %d\n", info->SubobjectCount);
    fprintf(outf, "%sAggregate BaseModelName: \"%s\"\n", Indent(), info->BaseModelName);
    fprintf(outf, "%sAggregate SubobjectCount: %d\n", Indent(), info->SubobjectCount);

    ASSERT(size == sizeof(W3dAggregateInfoStruct) + info->SubobjectCount * sizeof(W3dAggregateSubobjectStruct));

    char BaseModel[MAX_PATH];
    sprintf(BaseModel, "%s.w3d", info->BaseModelName);
    ProcessFile(BaseModel);

    W3dAggregateSubobjectStruct *subobject = (W3dAggregateSubobjectStruct *) (data + sizeof(W3dAggregateInfoStruct));
    for (DWORD i=0; i<info->SubobjectCount; i++)
    {
        printf("SubObject[%d].SubobjectName = \"%s\"\n", i, subobject[i].SubobjectName);
        printf("SubObject[%d].BoneName = \"%s\"\n", i, subobject[i].BoneName);
        fprintf(outf, "%sSubObject[%d].SubobjectName = \"%s\"\n", Indent(), i, subobject[i].SubobjectName);
        fprintf(outf, "%sSubObject[%d].BoneName = \"%s\"\n", Indent(), i, subobject[i].BoneName);

        char SubObject[MAX_PATH];
        sprintf(SubObject, "%s.w3d", subobject[i].SubobjectName);
        ProcessFile(SubObject);
    }
}

void ChunkAggregateClassInfo(BYTE *data, DWORD size)
{
    ASSERT(size == sizeof(W3dAggregateMiscInfo));

    W3dAggregateMiscInfo *info = (W3dAggregateMiscInfo *) data;

    printf("Aggregate ClassInfo.OriginalClassID: %d\n", info->OriginalClassID);
    printf("Aggregate ClassInfo.Flags: %08x\n", info->Flags);
}

void ChunkMeshUserText(BYTE *data, DWORD size)
{
    printf("MeshUserText:\n<<<\n%s>>>\n", data);
}

void ChunkMeshHeader(BYTE *data, DWORD size)
{
    ASSERT(size == sizeof(W3dMeshHeaderStruct));

    W3dMeshHeaderStruct *header = (W3dMeshHeaderStruct *) data;

    printf("W3dMeshHeaderStruct.MeshName: \"%s\"\n", header->MeshName);
    printf("MeshHeader.NumTris: %d\n", header->NumTris);
    printf("MeshHeader.NumVertices: %d\n", header->NumVertices);
    printf("MeshHeader.NumMaterials: %d\n", header->NumMaterials);
    printf("W3dMeshHeaderStruct.HierarchyTreeName: \"%s\"\n", header->HierarchyTreeName);
    printf("W3dMeshHeaderStruct.HierarchyModelName: \"%s\"\n", header->HierarchyModelName);

    fprintf(outf, "%sW3dMeshHeaderStruct.MeshName: \"%s\"\n", Indent(), header->MeshName);
    fprintf(outf, "%sW3dMeshHeaderStruct.HierarchyTreeName: \"%s\"\n", Indent(), header->HierarchyTreeName);
    fprintf(outf, "%sW3dMeshHeaderStruct.HierarchyModelName: \"%s\"\n", Indent(), header->HierarchyModelName);
}

void ChunkMeshHeader3(BYTE *data, DWORD size)
{
    ASSERT(size == sizeof(W3dMeshHeader3Struct));

    W3dMeshHeader3Struct *header = (W3dMeshHeader3Struct *) data;
    printf("MeshHeader3.MeshName: \"%s\"\n", header->MeshName);
    fprintf(outf, "%sMeshHeader3.MeshName: \"%s\"\n", Indent(), header->MeshName);
}

void ChunkHierarchyHeader(BYTE *data, DWORD size)
{
    ASSERT(size == sizeof(W3dHierarchyStruct));

    W3dHierarchyStruct *header = (W3dHierarchyStruct *) data;

    printf("Hierarchy.Name: \"%s\"\n", header->Name);
    fprintf(outf,"%sHierarchy.Name: \"%s\"\n", Indent(), header->Name);
}

void ChunkAnimationHeader(BYTE *data, DWORD size)
{
    ASSERT(size == sizeof(W3dAnimHeaderStruct));

    W3dAnimHeaderStruct *header = (W3dAnimHeaderStruct *) data;

    printf("Animation.Name: \"%s\"\n", header->Name);
    fprintf(outf,"%sAnimation.Name: \"%s\"\n", Indent(), header->Name);
}

void ChunkCompressedAnimHeader(BYTE *data, DWORD size)
{
    ASSERT(size == sizeof(W3dCompressedAnimHeaderStruct));

    W3dCompressedAnimHeaderStruct *header = (W3dCompressedAnimHeaderStruct *) data;

    printf("CompressedAnim.Name: \"%s\"\n", header->Name);
    fprintf(outf,"%sCompressedAnim.Name: \"%s\"\n", Indent(), header->Name);
}

void ChunkPivot(BYTE *data, DWORD size)
{
    W3dPivotStruct *pivot = (W3dPivotStruct *) data;

    int num_pivots = size / sizeof(W3dPivotStruct);

    for (int i=0; i<num_pivots; i++)
    {
        printf("Pivot.Name: \"%s\"\n", pivot[i].Name);
        fprintf(outf,"%sPivot.Name: \"%s\"\n", Indent(), pivot[i].Name);
    }
}

void ChunkMorphAnimHeader(BYTE *data, DWORD size)
{
    ASSERT(size == sizeof(W3dMorphAnimHeaderStruct));

    W3dMorphAnimHeaderStruct *header = (W3dMorphAnimHeaderStruct *) data;

    printf("MorphAnim.Name: \"%s\"\n", header->Name);
    fprintf(outf,"%sMorphAnim.Name: \"%s\"\n", Indent(), header->Name);
}

void ChunkHModelHeader(BYTE *data, DWORD size)
{
    ASSERT(size == sizeof(W3dHModelHeaderStruct));

    W3dHModelHeaderStruct *header = (W3dHModelHeaderStruct *) data;

    printf("HModel.Name: \"%s\"\n", header->Name);
    fprintf(outf,"%sHModel.Name: \"%s\"\n", Indent(), header->Name);
}

void ChunkHModelNode(BYTE *data, DWORD size)
{
    W3dHModelNodeStruct *node = (W3dHModelNodeStruct *) data;

    int num_nodes = size / sizeof(W3dHModelNodeStruct);

    for (int i=0; i<num_nodes; i++)
    {
        printf("HModelNode.Name: \"%s\"\n", node->RenderObjName);
        fprintf(outf,"%sHModelNode.Name: \"%s\"\n", Indent(), node->RenderObjName);
    }
}

void ChunkEmitterHeader(BYTE *data, DWORD size)
{
    ASSERT(size == sizeof(W3dEmitterHeaderStruct));

    W3dEmitterHeaderStruct *header = (W3dEmitterHeaderStruct *) data;

    printf("Emitter.Name: \"%s\"\n", header->Name);
    fprintf(outf,"%sEmitter.Name: \"%s\"\n", Indent(), header->Name);
}

void ChunkHLodHeader(BYTE *data, DWORD size)
{
    ASSERT(size == sizeof(W3dHLodHeaderStruct));

    W3dHLodHeaderStruct *header = (W3dHLodHeaderStruct *) data;

    printf("HLod.Name: \"%s\"\n", header->Name);
    fprintf(outf,"%sHLod.Name: \"%s\"\n", Indent(), header->Name);
}

void ChunkHLodArrayHeader(BYTE *data, DWORD size)
{
    ASSERT(size == sizeof(W3dHLodArrayHeaderStruct));

    W3dHLodArrayHeaderStruct *info = (W3dHLodArrayHeaderStruct *) data;

//    printf("HLOD ModelCount: %d\n", info->ModelCount);
//    fprintf(outf,"HLOD ModelCount: %d\n", info->ModelCount);
}

void ChunkHLodSubObject(BYTE *data, DWORD size)
{
    ASSERT(size == sizeof(W3dHLodSubObjectStruct));

    W3dHLodSubObjectStruct *subobject = (W3dHLodSubObjectStruct *) (data + sizeof(W3dHLodArrayHeaderStruct));

    if ((isalpha(subobject->Name[0])) &&
        (isalpha(subobject->Name[1])) &&
        (isalpha(subobject->Name[2])) &&
        (isalpha(subobject->Name[3])))
    {
        printf("HLOD SubObject.Name = \"%s\"\n", subobject->Name);
        fprintf(outf, "%sHLOD SubObject.Name = \"%s\"\n", Indent(), subobject->Name);
    }
}

void ChunkBox(BYTE *data, DWORD size)
{
    ASSERT(size == sizeof(W3dBoxStruct));

    W3dBoxStruct *header = (W3dBoxStruct *) data;

    printf("Box.Name: \"%s\"\n", header->Name);
    fprintf(outf,"%sBox.Name: \"%s\"\n", Indent(), header->Name);
}

void ChunkTextureName(BYTE *data, DWORD size)
{
    printf("TextureName: \"%s\"\n", data);
    fprintf(outf,"%sTextureName: \"%s\"\n", Indent(), data);
}

void ChunkMaterialInfo(BYTE *data, DWORD size)
{
    ASSERT(size == sizeof(W3dMaterialInfoStruct));

    W3dMaterialInfoStruct *material_info = (W3dMaterialInfoStruct *) data;
    if (material_info->ShaderCount > 2)
    {
        printf("ShaderCount=%d\n", material_info->ShaderCount);
        fprintf(outf,"ShaderCount=%d\n", material_info->ShaderCount);
    }
}

/*

  EnB W3D mesh layout

  Pivot names for Hull:
    ~01             (profession/tail)
    ~02             (wing)
    ~FL             (front left)
    ~FR             (front right)
    ~WEAP_05        (weapon #5)
    ~WEAP_06        (weapon #6)
    COCKPIT
    FLARE1
    FLARE2
    MESH01
    MESH02
    PART0Q0
    SCNDARY
    TRACT_IN
    TRACTOR

  Pivot names for Wing:
    ~03_01          (engine mount)
    ~03_02          (engine mount)
    ~BL             (back left)
    ~BR             (back right)
    ~FL             (front left)
    ~FR             (front right)
    ~WEAP_01        (weapon #1)
    ~WEAP_02        (weapon #2)
    ~WEAP_03        (weapon #3)
    ~WEAP_04        (weapon #4)
    ~WEAP_05        (weapon #5)
    ~WEAP_06        (weapon #6)
    BOX01
    DECAL01
    DECAL02
    LFGREEN01       (green docking light)
    LFRED01         (red docking light)
    PART0Q1
    PART0Q3
    R_FINBOT
    R_FINBOT01
    R_FINTOP
    R_FINTOP01
    R_ROTATE01
    R_ROTATE02

  Pivot names for Profession (Tail):
    ~03_01          (engine mount)
    ~03_02          (engine mount)
    ~BL             (back left)
    ~BR             (back right)
    ~WEAP_01        (weapon #1)
    ~WEAP_02        (weapon #2)
    ~WEAP_06        (weapon #6)
    BACK
    BL00            (bottom left)
    BL01
    BR00            (bottom right)
    BR01
    CIRCLE
    CTAILARMSHAPE2
    CTAILARMSHAPE4
    CYLINDER01
    CYLINDER03
    ENG BRACE 04    (engine brace)
    ENG BRACE 05
    FINEROTA05
    FINEROTA06
    FINEROTA07
    FINEROTA08
    FINEROTA09
    FINFX05
    FINFX06
    FINFX07
    FINFX08
    FINFX09
    FLARE10
    FLARE11
    FLARE13
    FLARE14
    FLARE15
    FLARE17
    FLARE18
    FLARE3
    FLARE5
    FLARE6
    FLARE7
    FLARE8
    MESH01
    MESH02
    OBJECT01
    OBJECT03
    OBJECT04
    OBJECT05
    OBJECT06
    OBJECT07
    OBJECT08
    OBJECT09
    PART0Q2         (?)
    TL00            (top left)
    TL01
    TR00            (top right)
    TR01
    TR02
    TURRENT THING
    TURRET ARM
    WING ROT R
    WING ROT R01
    WING_L
    WING_L_1
    WING_L_2
    WING_R
    WING_R_1
    WING_R_2

  Need to figure out the following Freelancer HardPoints:

    Fixed:
        Hp|Fixed|HpEngine01         ~03_01
        Hp|Fixed|HpEngine02         ~03_02
        Hp|Fixed|HpEngine03         /
        Hp|Fixed|HpEngine04         /
        Hp|Fixed|HpThruster01       /
        Hp|Fixed|HpTractor_Source   TRACTOR
        Hp|Fixed|HpShield01         /
        Hp|Fixed|HpMount            /
        Hp|Fixed|HpConTrail01       /
        Hp|Fixed|HpConTrail02       /
        Hp|Fixed|HpConTrail03       /
        Hp|Fixed|HpCM01             /
        Hp|Fixed|HpMine01           /
        Hp|Fixed|HpRunningLight01   LFGREEN01
        Hp|Fixed|HpRunningLight02   LFRED01
        Hp|Fixed|HpPilot            /
        Hp|Fixed|HpScanner01        TRACT_IN
        Hp|Fixed|HpHeadLight        /
        Hp|Fixed|HpDockLight01      /
        Hp|Fixed|HpDockLight02      /

    Revolute:
        Hp|Revolute|HpTorpedo01     /
        Hp|Revolute|HpTurret01      TURRET ARM
        Hp|Revolute|HpWeapon01      ~WEAP_01
        Hp|Revolute|HpWeapon02      ~WEAP_02
        Hp|Revolute|HpWeapon03      ~WEAP_03
        Hp|Revolute|HpWeapon04      ~WEAP_04
        Hp|Revolute|HpWeapon05      ~WEAP_05
        Hp|Revolute|HpWeapon06      ~WEAP_06

  EnB uses material name ~s1 for metal and ~s2 for shiny metal

*/
