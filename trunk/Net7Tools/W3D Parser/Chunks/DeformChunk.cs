using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class DeformChunk : Chunk
    {
        private int set_count;
        private int alpha_passes;

        public DeformChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|-|" + ChunkHeader.W3D_CHUNK_DEFORM);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_DEFORM;
            HeaderName = ChunkHeader.W3D_CHUNK_DEFORM.ToString();

            set_count = br.ReadInt32();
            alpha_passes = br.ReadInt32();

            //reserved
            br.ReadBytes(chunkLength - 8);
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of sets in this deform object.")]
        public int SetCount
        {
            get { return set_count; }
            set { set_count = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The total number of alpha passes in the deform set.")]
        public int AlphaPasses
        {
            get { return alpha_passes; }
            set { alpha_passes = value; }
        }
    }
}
