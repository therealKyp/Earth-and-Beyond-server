using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class DeformSetChunk : Chunk
    {
        private int keyframe_count;
        private int flags;

        public DeformSetChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--|" + ChunkHeader.W3D_CHUNK_DEFORM_SET);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_DEFORM_SET;
            HeaderName = ChunkHeader.W3D_CHUNK_DEFORM_SET.ToString();

            for (int i = 0; i < chunkLength / 12; i++)
            {
                keyframe_count = br.ReadInt32();
                flags = br.ReadInt32();

                //reserved
                br.ReadInt32();
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of keyframes in this deformation set.")]
        public int KeyFrameCount
        {
            get { return keyframe_count; }
            set { keyframe_count = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Any flags / attributes associated with this set.")]
        public int Flags
        {
            get { return flags; }
            set { flags = value; }
        }
    }
}
