using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    public class HModelChunk : Chunk
    {
        /// <summary>
        /// <para>Hiearchical Model</para>
        /// <para>A Hierarchy Model is a set of render objects which should be attached to 
        /// bones in a hierarchy tree. There can be multiple objects per node
        /// in the tree. Or there may be no objects attached to a particular bone.</para>
        /// </summary>
        public HModelChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|-| " + ChunkHeader.W3D_CHUNK_HMODEL);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_HMODEL;
            HeaderName = ChunkHeader.W3D_CHUNK_HMODEL.ToString();
        }
    }
}
