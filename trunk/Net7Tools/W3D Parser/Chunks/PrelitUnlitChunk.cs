using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class PrelitUnlitChunk : Chunk
    {
        public PrelitUnlitChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int endPosition)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_PRELIT_UNLIT);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_PRELIT_UNLIT;
            HeaderName = ChunkHeader.W3D_CHUNK_PRELIT_UNLIT.ToString();
        }
    }
}
