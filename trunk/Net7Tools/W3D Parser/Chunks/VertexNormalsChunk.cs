using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class VertexNormalsChunk : Chunk
    {
        private Vector3[] vertex_normals_data;

        public VertexNormalsChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_VERTEX_NORMALS);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_VERTEX_NORMALS;
            HeaderName = ChunkHeader.W3D_CHUNK_VERTEX_NORMALS.ToString();

            int NumVertices = chunkLength / 12;
            vertex_normals_data = new Vector3[NumVertices];

            for (int i = 0; i < NumVertices; i++)
            {
                Vector3 vertex_normal = new Vector3(br.ReadSingle(), br.ReadSingle(), br.ReadSingle());
                vertex_normals_data[i] = vertex_normal;

                if (dbg)
                {
                    Console.Out.WriteLine("\t Vertex Normal #" + i);
                    Console.Out.WriteLine("\t Vector: " + vertex_normal);
                }
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of Vertex Normal values.")]
        public Vector3[] VertexNormals
        {
            get { return vertex_normals_data; }
            set { vertex_normals_data = value; }
        }
    }
}
