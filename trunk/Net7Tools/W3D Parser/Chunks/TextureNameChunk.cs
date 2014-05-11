using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class TextureNameChunk : Chunk
    {
        private String texture_name;

        public TextureNameChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|----| " + ChunkHeader.W3D_CHUNK_TEXTURE_NAME);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_TEXTURE_NAME;
            HeaderName = ChunkHeader.W3D_CHUNK_TEXTURE_NAME.ToString();

            byte[] textureNameArray = br.ReadBytes(chunkLength);
            texture_name = System.Text.ASCIIEncoding.ASCII.GetString(textureNameArray);

            if (dbg)
            {
                Console.Out.WriteLine("\t\t Texture Name: " + texture_name);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The name of the texture file.")]
        public String TextureName
        {
            get { return texture_name; }
            set { texture_name = value; }
        }
    }
}
