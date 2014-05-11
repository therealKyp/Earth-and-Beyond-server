using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class AabTreePolyIndicesChunk : Chunk
    {
        private int[] poly_index;

        /// <summary>
        /// An array of polygon indices. 
        /// </summary>
        public AabTreePolyIndicesChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|---| " + ChunkHeader.W3D_CHUNK_AABTREE_POLYINDICES);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_AABTREE_POLYINDICES;
            HeaderName = ChunkHeader.W3D_CHUNK_AABTREE_POLYINDICES.ToString();

            int NumPolygonIndices = chunkLength / 4;
            poly_index = new int[NumPolygonIndices];

            for (int i = 0; i < NumPolygonIndices; i++)
            {
                poly_index[i] = br.ReadInt32();

                if (dbg)
                {
                    Console.Out.WriteLine("\t\t Polygon Index #" + i + " Index: " + poly_index);
                }
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Array of Polygon Indices.")]
        public int[] PolygonIncides
        {
            get { return poly_index; }
            set { poly_index = value; }
        }
    }
}
