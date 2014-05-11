using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class VertexMaterialNameChunk : Chunk
    {
        private String material_name;

        public VertexMaterialNameChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|----| " + ChunkHeader.W3D_CHUNK_VERTEX_MATERIAL_NAME);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_VERTEX_MATERIAL_NAME;
            HeaderName = ChunkHeader.W3D_CHUNK_VERTEX_MATERIAL_NAME.ToString();

            byte[] MaterialNameArray = br.ReadBytes(chunkLength);
            material_name = System.Text.ASCIIEncoding.ASCII.GetString(MaterialNameArray);

            if (dbg)
            {
                Console.Out.WriteLine("\t\t Material Name: " + MaterialName);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The name of this material.")]
        public String MaterialName
        {
            get { return material_name; }
            set { material_name = value; }
        }
    }
}
