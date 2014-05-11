using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class MaterialInfoChunk : Chunk
    {
        private int pass_count;
        private int vertex_material_count;
        private int shader_count;
        private int texture_count;

        public MaterialInfoChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int endPosition)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_MATERIAL_INFO);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_MATERIAL_INFO;
            HeaderName = ChunkHeader.W3D_CHUNK_MATERIAL_INFO.ToString();

            pass_count = br.ReadInt32();
            vertex_material_count = br.ReadInt32();
            shader_count = br.ReadInt32();
            texture_count = br.ReadInt32();

            if (dbg)
            {
                Console.Out.WriteLine("\t Pass Count: " + pass_count);
                Console.Out.WriteLine("\t Vertex Material Count: " + vertex_material_count);
                Console.Out.WriteLine("\t Shader Count: " + shader_count);
                Console.Out.WriteLine("\t Texture Count: " + texture_count);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of material passes.")]
        public int PassCount
        {
            get { return pass_count; }
            set { pass_count = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of vertex materials to be applied.")]
        public int VertexMaterialCount
        {
            get { return vertex_material_count; }
            set { vertex_material_count = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of shaders to be applied.")]
        public int ShaderCount
        {
            get { return shader_count; }
            set { shader_count = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of textures to be applied.")]
        public int TextureCount
        {
            get { return texture_count; }
            set { texture_count = value; }
        }
    }
}
