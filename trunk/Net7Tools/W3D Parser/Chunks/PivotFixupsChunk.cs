using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class PivotFixupsChunk : Chunk
    {
        private object[][] pivot_max_matrix;

        public PivotFixupsChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_PIVOT_FIXUPS);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_PIVOT_FIXUPS;
            HeaderName = ChunkHeader.W3D_CHUNK_PIVOT_FIXUPS.ToString();

            int NumPivots = chunkLength / 48;
            pivot_max_matrix = new object[NumPivots][];

            for (int i = 0; i < NumPivots; i++)
            {
                Object[] maxMatrix = new Object[4];

                for (int i2 = 0; i2 < 4; i2++)
                {
                    Vector3 values = new Vector3();
                    values.X = br.ReadSingle();
                    values.Y = br.ReadSingle();
                    values.Z = br.ReadSingle();

                    maxMatrix[i2] = values;
                }

                if (dbg)
                {
                    Console.Out.WriteLine("\t Pivot #" + i);

                    for (int i3 = 0; i3 < 4; i3++)
			        {
                        object[] vOutput =  (object[]) maxMatrix[i3];
                        Console.Out.WriteLine("\t\t " + vOutput[0] + ", " + vOutput[1] + ", " + vOutput[2]); 
			        }
                }

                pivot_max_matrix[i] = maxMatrix;
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of Pivot fixups. Max 4x3 matrix.")]
        public object[][] PivotFixupMatrices
        {
            get { return pivot_max_matrix; }
            set { pivot_max_matrix = value; }
        }
    }
}
