using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class HLodAggregateArrayChunk : Chunk
    {
        public HLodAggregateArrayChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|---| " + ChunkHeader.W3D_CHUNK_HLOD_AGGREGATE_ARRAY);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_HLOD_AGGREGATE_ARRAY;
            HeaderName = ChunkHeader.W3D_CHUNK_HLOD_AGGREGATE_ARRAY.ToString();
        }
    }
}
