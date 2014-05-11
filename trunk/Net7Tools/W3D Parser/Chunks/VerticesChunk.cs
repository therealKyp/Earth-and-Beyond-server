using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class VerticesChunk : Chunk
    {
        private Vector3[] vertices_data;

        public VerticesChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_VERTICES);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_VERTICES;
            HeaderName = ChunkHeader.W3D_CHUNK_VERTICES.ToString();

            int NumVertices = chunkLength / 12;
            vertices_data = new Vector3[NumVertices];

            for (int i = 0; i < NumVertices; i++)
            {
                Vector3 vertice = new Vector3(br.ReadSingle(), br.ReadSingle(), br.ReadSingle());
                vertices_data[i] = vertice;

                if (dbg)
                {
                    Console.Out.WriteLine("\t Vertices #" + i);
                    Console.Out.WriteLine("\t Vector: " + vertice);
                }
            }

            
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of Vertices.")]
        public Vector3[] Vertices
        {
            get { return vertices_data; }
            set { vertices_data = value; }
        }
    }
}
