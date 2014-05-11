using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class VertexShadeIndicesChunk : Chunk
    {
        private int[] vertexShadeIndices;

        public VertexShadeIndicesChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_VERTEX_SHADE_INDICES);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_VERTEX_SHADE_INDICES;
            HeaderName = ChunkHeader.W3D_CHUNK_VERTEX_SHADE_INDICES.ToString();

            int NumVertexShades = chunkLength / 4;
            vertexShadeIndices = new int[NumVertexShades + 2];
            
            for (int i = 0; i < NumVertexShades; i++)
            {
                vertexShadeIndices[i] = br.ReadInt32();

                if (dbg)
                {
                    Console.Out.WriteLine("\t Vertex Iternation #" + i);
                    Console.Out.WriteLine("\t Index Value: " + vertexShadeIndices[i]);
                }
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of vertex shade indices.")]
        public int[] VertexShadeIndices
        {
            get { return vertexShadeIndices; }
            set { vertexShadeIndices = value; }
        }
    }
}
