using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class TextureInfoChunk : Chunk
    {
        private int attributes;
        private int frame_count;
        private float frame_rate;

        public TextureInfoChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|----| " + ChunkHeader.W3D_CHUNK_TEXTURE_INFO);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_TEXTURE_INFO;
            HeaderName = ChunkHeader.W3D_CHUNK_TEXTURE_INFO.ToString();

            attributes = br.ReadInt32();
            frame_count = br.ReadInt32();
            frame_rate = br.ReadSingle();

            if (dbg)
            {
                Console.Out.WriteLine("\t\t   Attributes: " + attributes);
                Console.Out.WriteLine("\t\t   Frame Count: " + frame_count);
                Console.Out.WriteLine("\t\t   Frame Rate: " + frame_rate);
            } 
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Any attributes / flags in this chunk.")]
        public int Attributes
        {
            get { return attributes; }
            set { attributes = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The frame count of the texture animation.")]
        public int FrameCount
        {
            get { return frame_count; }
            set { frame_count = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The frame rate of the texture animation.")]
        public float FrameRate
        {
            get { return frame_rate; }
            set { frame_rate = value; }
        }
    }
}
