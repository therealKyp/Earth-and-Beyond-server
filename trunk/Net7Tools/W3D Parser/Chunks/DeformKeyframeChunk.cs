using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class DeformKeyframeChunk : Chunk
    {
        private float deform_percent;
        private int data_count;
        private int[] reserved = new int[2];

        public DeformKeyframeChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|---|" + ChunkHeader.W3D_CHUNK_DEFORM_KEYFRAME);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_DEFORM_KEYFRAME;
            HeaderName = ChunkHeader.W3D_CHUNK_DEFORM_KEYFRAME.ToString();

            for (int i = 0; i < chunkLength / 16; i++)
            {
                deform_percent = br.ReadSingle();
                data_count = br.ReadInt32();

                //reserved
                for (int i2 = 0; i2 < 2; i2++)
                {
                    reserved[i2] = br.ReadInt32();
                } 
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The percentage of deformation for this keyframe.")]
        public float DeformPercent
        {
            get { return deform_percent; }
            set { deform_percent = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of data for this keyframe.")]
        public int DataCount
        {
            get { return data_count; }
            set { data_count = value; }
        }
    }
}
