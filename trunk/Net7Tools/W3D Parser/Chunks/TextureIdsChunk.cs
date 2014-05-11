using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class TextureIdsChunk : Chunk
    {
        private float[] textureIds;

        public TextureIdsChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|----| " + ChunkHeader.W3D_CHUNK_TEXTURE_IDS);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_TEXTURE_IDS;
            HeaderName = ChunkHeader.W3D_CHUNK_TEXTURE_IDS.ToString();

            int NumTextureIds = chunkLength / 4;
            textureIds = new float[NumTextureIds];

            for (int i = 0; i < NumTextureIds; i++)
            {
                textureIds[i] = br.ReadInt32();

                if (dbg)
                {
                    Console.Out.WriteLine("\t\t Face{" + i + "} Texture Index: " + textureIds[i]);
                }
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of Texture indices per face.")]
        public float[] TextureIDs
        {
            get { return textureIds; }
            set { textureIds = value; }
        }
    }
}
