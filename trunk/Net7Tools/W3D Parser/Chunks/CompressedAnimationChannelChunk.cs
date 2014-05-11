using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Collections;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class CompressedAnimationChannelChunk : Chunk
    {
        private int first_frame;
        private int last_frame;
        private int pivot;
        private int channel_type;
        private int vector_length;

        public CompressedAnimationChannelChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_COMPRESSED_ANIMATION_CHANNEL);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_COMPRESSED_ANIMATION_CHANNEL;
            HeaderName = ChunkHeader.W3D_CHUNK_COMPRESSED_ANIMATION_CHANNEL.ToString();

            long beginPos = br.BaseStream.Position;
            last_frame = br.ReadInt16();
            first_frame = br.ReadInt16();
            pivot = br.ReadInt16();
            channel_type = br.ReadInt16();
            vector_length = br.ReadInt32();

            if (dbg)
            {
                Console.Out.WriteLine("\t First Frame: " + first_frame);
                Console.Out.WriteLine("\t Last Frame: " + last_frame);
                Console.Out.WriteLine("\t Channel Type: " + channel_type);
                Console.Out.WriteLine("\t Pivot ID: " + pivot);
                Console.Out.WriteLine("\t Vector Length: " + vector_length);
            }

            //Skip over unkown structure.
            br.BaseStream.Position = beginPos + chunkLength;
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("First Frame of the Animation Data.")]
        public int FirstFrame
        {
            get { return first_frame; }
            set { first_frame = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Last Frame of the Animation Data.")]
        public int LastFrame
        {
            get { return last_frame; }
            set { last_frame = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The length of the animation vector data either: (int, vector, quaternion) lengths")]
        public int VectorLength
        {
            get { return vector_length; }
            set { vector_length = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("A flag attribute that determines the type of animation vector")]
        public int ChannelType
        {
            get { return channel_type; }
            set { channel_type = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The pivot in which this animation data affects.")]
        public int Pivot
        {
            get { return pivot; }
            set { pivot = value; }
        }
    }
}
