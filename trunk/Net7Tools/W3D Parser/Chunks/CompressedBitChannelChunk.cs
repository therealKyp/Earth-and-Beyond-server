using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class CompressedBitChannelChunk : Chunk
    {

        private int default_val;
        private int first_frame;
        private int last_frame;
        private int flags;
        private int pivot;

        public CompressedBitChannelChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_COMPRESSED_BIT_CHANNEL);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_COMPRESSED_BIT_CHANNEL;
            HeaderName = ChunkHeader.W3D_CHUNK_COMPRESSED_BIT_CHANNEL.ToString();

            long beginPos = br.BaseStream.Position;
            default_val = 0;
            last_frame = br.ReadInt16();
            first_frame = br.ReadInt16();
            pivot = br.ReadInt16();
            flags = br.ReadInt16();
            default_val = br.ReadByte();

            if (dbg)
            {
                Console.Out.WriteLine("\t First Frame: " + first_frame);
                Console.Out.WriteLine("\t Last Frame: " + last_frame);
                Console.Out.WriteLine("\t Flags: " + flags);
                Console.Out.WriteLine("\t Pivot: " + pivot);
                Console.Out.WriteLine("\t Default Value: " + default_val);
            }

            //Skip over unkown structure.
            br.BaseStream.Position = beginPos + chunkLength;
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The first frame of this animation bit channel.")]
        public int FirstFrame
        {
            get { return first_frame; }
            set { first_frame = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The last frame of this animation bit channel.")]
        public int LastFrame
        {
            get { return last_frame; }
            set { last_frame = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Any attributes for this bit channel data.")]
        public int Flags
        {
            get { return flags; }
            set { flags = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The pivot in which this bit channel affects.")]
        public int Pivot
        {
            get { return pivot; }
            set { pivot = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The Defualt Bit channel value.")]
        public int DefaultValue
        {
            get { return default_val; }
            set { default_val = value; }
        }
    }
}
