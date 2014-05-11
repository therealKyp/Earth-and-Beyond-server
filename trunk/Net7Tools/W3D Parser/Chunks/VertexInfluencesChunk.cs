using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class VertexInfluencesChunk : Chunk
    {
        private float[] BoneIds;

        public VertexInfluencesChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_VERTEX_INFLUENCES);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_VERTEX_INFLUENCES;
            HeaderName = ChunkHeader.W3D_CHUNK_VERTEX_INFLUENCES.ToString();

            int NumVertexInfluences = chunkLength / 8;
            BoneIds = new float[NumVertexInfluences];

            for (int i = 0; i < NumVertexInfluences; i++)
            {
                BoneIds[i] = br.ReadInt16();
                for (int i2 = 0; i2 < 6; i2++)
                {
                    br.ReadByte(); 
                }

                if (dbg)
                {
                    Console.Out.WriteLine("\t   Vertex Influence{" + i + "} Bone Id: " + BoneIds[i]);
                }
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of bone id's that contain vertex influences.")]
        public float[] VertexBoneIds
        {
            get { return BoneIds; }
            set { BoneIds = value; }
        }
    }
}
