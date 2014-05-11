using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class TextureReplacerChunk : Chunk
    {
        private int replaced_textures_count;

        public TextureReplacerChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_TEXTURE_REPLACER_INFO);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_TEXTURE_REPLACER_INFO;
            HeaderName = ChunkHeader.W3D_CHUNK_TEXTURE_REPLACER_INFO.ToString();

            replaced_textures_count = br.ReadInt32();

            if (dbg)
            {
                Console.Out.WriteLine("\t Replaced Textures Count: " + replaced_textures_count);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of textures to be replaced.")]
        public int ReplacedTexturesCount
        {
            get { return replaced_textures_count; }
            set { replaced_textures_count = value; }
        }
    }
}
