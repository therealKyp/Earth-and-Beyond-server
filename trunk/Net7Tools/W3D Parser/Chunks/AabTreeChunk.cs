using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace WestWood3D.Chunks
{
    public class AabTreeChunk : Chunk
    {
        ///<summary>
        ///<para>Axis-Aligned Box Tree for hierarchical polygon culling:</para>
        ///<para>Each mesh can have an associated Axis-Aligned-Bounding-Box tree
        ///which is used for collision detection and certain rendering algorithms 
        ///(like texture projection).</para>
        ///</summary>
        public AabTreeChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_AABTREE);

            HeaderID = (int) ChunkHeader.W3D_CHUNK_AABTREE;
            HeaderName = ChunkHeader.W3D_CHUNK_AABTREE.ToString();
        }
    }
}
