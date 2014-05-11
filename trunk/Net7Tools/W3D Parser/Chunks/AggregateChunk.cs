using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    public class AggregateChunk : Chunk
    {
        /// <summary>
        /// An 'aggregate' is simply a 'shell' that contains references to a hierarchy model and subobjects to attach to its bones.
        /// </summary>
        public AggregateChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int endPosition)
        {
            if (dbg) Console.Out.WriteLine("|-| " + ChunkHeader.W3D_CHUNK_AGGREGATE);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_AGGREGATE;
            HeaderName = ChunkHeader.W3D_CHUNK_AGGREGATE.ToString();
        }
    }
}
