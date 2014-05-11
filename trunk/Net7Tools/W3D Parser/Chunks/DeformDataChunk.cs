using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;
using System.Drawing;

namespace WestWood3D.Chunks
{
    class DeformDataChunk : Chunk
    {
        private int vertex_index;
        private Vector3 position;
        private Color color;
        private int[] reserved = new int[2];

        public DeformDataChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|-----|" + ChunkHeader.W3D_CHUNK_DEFORM_DATA);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_DEFORM_DATA;
            HeaderName = ChunkHeader.W3D_CHUNK_DEFORM_DATA.ToString();

            for (int i = 0; i < chunkLength / 28; i++)
            {
                vertex_index = br.ReadInt32();
                position = new Vector3(br.ReadSingle(), br.ReadSingle(), br.ReadSingle());

                int R = br.ReadByte();
                int G = br.ReadByte();
                int B = br.ReadByte();
                int A = br.ReadByte();

                color = Color.FromArgb(A, R, G, B);

                //reserved
                for (int i2 = 0; i2 < 2; i2++)
                {
                    reserved[i2] = br.ReadInt32();
                }  
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The vertex index for this data to be appliced too.")]
        public int VertexIndex
        {
            get { return vertex_index; }
            set { vertex_index = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The vector position of this data.")]
        public Vector3 Position
        {
            get { return position; }
            set { position = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The color assosicated with the deform data.")]
        public Color DeformColor
        {
            get { return color; }
            set { color = value; }
        }
    }
}
