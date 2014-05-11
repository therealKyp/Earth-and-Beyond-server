using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class PivotsChunk : Chunk
    {
        private object[][] pivot_data;

        public PivotsChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_PIVOTS);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_PIVOTS;
            HeaderName = ChunkHeader.W3D_CHUNK_PIVOTS.ToString();

            int NumPivots = chunkLength / 60;
            pivot_data = new object[NumPivots][];

            for (int i = 0; i < NumPivots ; i++)
            {
                byte[] pivotNameArray = br.ReadBytes(16);
                String PivotName = System.Text.ASCIIEncoding.ASCII.GetString(pivotNameArray);
                int ParentID = br.ReadInt32();

                Vector3 translation = new Vector3();
                translation.X = br.ReadSingle();
                translation.Y = br.ReadSingle();
                translation.Z = br.ReadSingle();

                Vector3 eularAngle = new Vector3();
                eularAngle.X = br.ReadSingle();
                eularAngle.Y = br.ReadSingle();
                eularAngle.Z = br.ReadSingle();

                float[] rotation = new float[4];
                rotation[0] = br.ReadSingle();
                rotation[1] = br.ReadSingle();
                rotation[2] = br.ReadSingle();
                rotation[3] = br.ReadSingle();

                Object[] pivotValues = new Object[5];
                pivotValues[0] = PivotName;
                pivotValues[1] = ParentID;
                pivotValues[2] = translation;
                pivotValues[3] = eularAngle;
                pivotValues[4] = rotation;

                pivot_data[i] = pivotValues;

                if (dbg)
                {
                    Console.Out.WriteLine("\t Pivot #" + i + " Name: " + PivotName);
                    Console.Out.WriteLine("\t Parent ID: " + ParentID);
                    Console.Out.WriteLine("\t Translation: (" + translation.X + ", "
                        + translation.Y + ", " + translation.Z + ")");
                    Console.Out.WriteLine("\t Eular Angles: (" + eularAngle.X + ", "
                            + eularAngle.Y + ", " + eularAngle.Z + ")");
                    Console.Out.WriteLine("\t Rotation: (" + rotation[0] + ", "
                            + rotation[1] + ", " + rotation[2] + ", " + rotation[3]
                            + ")");
                } 
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array  of Pivot Name, Parent ID, translation, eular angles and rotation per pivot ID")]
        public object[][] PivotData
        {
            get { return pivot_data; }
            set { pivot_data = value; }
        }
    }
}
