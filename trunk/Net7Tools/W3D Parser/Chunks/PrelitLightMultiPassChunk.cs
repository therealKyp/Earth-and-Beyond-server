using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class PrelitLightMultiPassChunk : Chunk
    {
        public PrelitLightMultiPassChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int endPosition)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_PRELIT_LIGHTMAP_MULTI_PASS);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_PRELIT_LIGHTMAP_MULTI_PASS;
            HeaderName = ChunkHeader.W3D_CHUNK_PRELIT_LIGHTMAP_MULTI_PASS.ToString();
        }
    }
}
