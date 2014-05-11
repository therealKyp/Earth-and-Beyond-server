using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class VertexMaterialChunk : Chunk
    {
        public VertexMaterialChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int endPosition)
        {
            if (dbg) Console.Out.WriteLine("|---| " + ChunkHeader.W3D_CHUNK_VERTEX_MATERIAL);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_VERTEX_MATERIAL;
            HeaderName = ChunkHeader.W3D_CHUNK_VERTEX_MATERIAL.ToString();
        }
    }
}
