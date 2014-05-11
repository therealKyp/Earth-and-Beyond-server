using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class TrianglesChunk : Chunk
    {
        private object[][] triangles;

        public TrianglesChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_TRIANGLES);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_TRIANGLES;
            HeaderName = ChunkHeader.W3D_CHUNK_TRIANGLES.ToString();

            int NumTriangles = chunkLength / 32;
            object[] triangle_data = new object[4];
            triangles = new object[NumTriangles][];

            for (int i = 0; i < NumTriangles; i++)
            {
                Vector3 vertIndex = new Vector3(br.ReadInt32(), br.ReadInt32(), br.ReadInt32());

                int attribute = br.ReadInt32();

                Vector3 normal = new Vector3();
                normal.X = br.ReadSingle();
                normal.Y = br.ReadSingle();
                normal.Z = br.ReadSingle();

                float distance = br.ReadSingle();

                triangle_data[0] = vertIndex;
                triangle_data[1] = attribute;
                triangle_data[2] = normal;
                triangle_data[3] = distance;

                triangles[i] = triangle_data;

                if (dbg)
                {
                    Console.Out.WriteLine("\t Triangle #" + i);
                    Console.Out.WriteLine("\t Vertex Index: " + vertIndex[0] + ", " + vertIndex[1] + "," + vertIndex[2]);
                    Console.Out.WriteLine("\t Attribute: " + attribute);
                    Console.Out.WriteLine("\t Vertex Normal: " + normal);
                    Console.Out.WriteLine("\t Distance: " + distance);
                }
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of Triangle Data (Vertex Indices, attributes, vertex normals, distance)")]
        public object[][] TriangleData
        {
            get { return triangles; }
            set { triangles = value; }
        }
    }
}
