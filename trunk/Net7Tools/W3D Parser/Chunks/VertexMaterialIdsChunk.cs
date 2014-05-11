using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class VertexMaterialIdsChunk : Chunk
    {
        private float[] vertMaterialIds;

        public VertexMaterialIdsChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|---| " + ChunkHeader.W3D_CHUNK_VERTEX_MATERIAL_IDS);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_VERTEX_MATERIAL_IDS;
            HeaderName = ChunkHeader.W3D_CHUNK_VERTEX_MATERIAL_IDS.ToString();

            int NumVertexMaterialIds = chunkLength / 4;
            vertMaterialIds = new float[NumVertexMaterialIds];

            for (int i = 0; i < NumVertexMaterialIds; i++)
            {
                vertMaterialIds[i] = br.ReadInt32();

                if (dbg)
                {
                    Console.Out.WriteLine("\t   Vertex{" + i + "} Vertex Material Id: "+ vertMaterialIds[i]);
                }
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of material id's per vertex.")]
        public float[] VertexMaterialIDs
        {
            get { return vertMaterialIds; }
            set { vertMaterialIds = value; }
        }
    }
}
