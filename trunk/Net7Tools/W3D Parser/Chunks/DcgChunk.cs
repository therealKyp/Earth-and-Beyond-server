using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Drawing;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class DcgChunk : Chunk
    {
        private Color[] dcg_colors;

        public DcgChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|---| " + ChunkHeader.W3D_CHUNK_DCG);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_DCG;
            HeaderName = ChunkHeader.W3D_CHUNK_DCG.ToString();

            int NumColors = chunkLength / 4;
            dcg_colors = new Color[NumColors];

            for (int i = 0; i < NumColors; i++)
            {
                int R = br.ReadByte();
                int G = br.ReadByte();
                int B = br.ReadByte();
                int A = br.ReadByte();

                Color dcgColor = Color.FromArgb(A, R, G, B);
                dcg_colors[i] = dcgColor;

                if (dbg)
                {
                    Console.Out.WriteLine("\t\t dcg[" + i + "]: " + dcgColor);
                } 
            }

            
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of per-vertex diffuse color values (ARGB)")]
        public Color[] DcgColors
        {
            get { return dcg_colors; }
            set { dcg_colors = value; }
        }
    }
}
