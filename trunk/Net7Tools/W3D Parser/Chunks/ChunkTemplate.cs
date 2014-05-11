using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class ChunkTemplate : Chunk
    {
        public ChunkTemplate()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|-|" + ChunkHeader.W3D_CHUNK_HIERARCHY);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_BOX;
            HeaderName = ChunkHeader.W3D_CHUNK_BOX.ToString();
        }

        //[CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("")]
        //public int Version
        //{
        //    get { return version; }
        //    set { version = value; }
        //}
    }
}
