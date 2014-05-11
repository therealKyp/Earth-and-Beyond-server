using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

using WestWood3D;

namespace WestWood3D.Chunks
{
    ///<summary>
    ///This Class Is a Container Chunk. It has no direct information, only sub-chunks
    ///</summary>
    public class MeshChunk : Chunk
    {
        public MeshChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int endPosition)
        {
            if (dbg) Console.Out.WriteLine("|-| " + ChunkHeader.W3D_CHUNK_MESH);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_MESH;
            HeaderName = ChunkHeader.W3D_CHUNK_MESH.ToString();
        }
    }
}
