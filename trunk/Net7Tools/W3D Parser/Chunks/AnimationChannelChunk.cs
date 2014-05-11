using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Collections;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class AnimationChannelChunk : Chunk
    {
        private Hashtable key_frame_data = new Hashtable();
        private int first_frame;
        private int last_frame;
        private int vector_length;
        private int channel_type;
        private int pivot;
        private int padding;

        public AnimationChannelChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_ANIMATION_CHANNEL);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_ANIMATION_CHANNEL;
            HeaderName = ChunkHeader.W3D_CHUNK_ANIMATION_CHANNEL.ToString();

            first_frame = br.ReadInt16();
            last_frame = br.ReadInt16();
            vector_length = br.ReadInt16();
            channel_type = br.ReadInt16();
            pivot = br.ReadInt16();
            padding = br.ReadInt16();
            
            if (dbg)
            {
                Console.Out.WriteLine("\t First Frame: " + first_frame);
                Console.Out.WriteLine("\t Last Frame: " + last_frame);
                Console.Out.WriteLine("\t Channel Type: " + channel_type);
                Console.Out.WriteLine("\t Pivot ID: " + pivot);
                Console.Out.WriteLine("\t Vector length: " + vector_length);
            }

            long RemainingChunkLength = chunkLength - 12;
            long NumData = RemainingChunkLength / (VectorLength * 4);
            
            for (int i = 0; i < NumData; i++)
            {
                float[] veclen = new float[VectorLength];
                for (int i2 = 0; i2 < VectorLength; i2++)
                {
                    veclen[i2] = br.ReadSingle();
                }

                key_frame_data.Add(i, veclen);
            }

            if (dbg)
            {
                IDictionaryEnumerator en = key_frame_data.GetEnumerator();
                while (en.MoveNext())
                {
                    float[] tmp = (float[]) en.Value;
                    for (int i3 = 0; i3 < tmp.Length; i3++)
	                {
	                    Console.Out.WriteLine("\t Frame["+ en.Key + "] Vector[" + i3 + "]:" + tmp[i3]);  
	                }
                }
             }
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

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of the animation data per keyframe.")]
        public Hashtable KeyFrameData
        {
            get { return key_frame_data; }
            set { key_frame_data = value; }
        }
    }
}
