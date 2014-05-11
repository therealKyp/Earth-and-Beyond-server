using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Collections;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class StageTexCoordsChunk : Chunk
    {
        private object[][] tex_coord_data;

        public StageTexCoordsChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|----| " + ChunkHeader.W3D_CHUNK_STAGE_TEXCOORDS);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_STAGE_TEXCOORDS;
            HeaderName = ChunkHeader.W3D_CHUNK_STAGE_TEXCOORDS.ToString();

            int NumTexCoords = chunkLength / 8;
            object[] uv = new object[2];
            tex_coord_data = new object[NumTexCoords][];

            for (int i = 0; i < NumTexCoords; i++)
            {
                uv[0] = br.ReadSingle();
                uv[1] = br.ReadSingle();

                tex_coord_data[i] = uv;

                if (dbg)
                {
                    Console.Out.WriteLine("\t Tex Coord #" + i);
                    Console.Out.WriteLine("\t U: " + uv[0] + " V: " + uv[1]);
                }
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of UV Texcoords.")]
        public object[][] TextureCoordinates
        {
            get { return tex_coord_data; }
            set { tex_coord_data = value; }
        }
    }
}
