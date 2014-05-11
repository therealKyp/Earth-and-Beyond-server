using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class BitChannelChunk : Chunk
    {
        private int default_val;
        private int first_frame;
        private int last_frame;
        private int flags;
        private int pivot;
        private Boolean[] result_vectors;

        public BitChannelChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunklength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_BIT_CHANNEL);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_BIT_CHANNEL;
            HeaderName = ChunkHeader.W3D_CHUNK_BIT_CHANNEL.ToString();

            default_val = 0;
            last_frame = br.ReadInt16();
            first_frame = br.ReadInt16();
            flags = br.ReadInt16();
            pivot = br.ReadInt16();
            default_val = br.ReadByte();
        	
            if(dbg){
	            Console.Out.WriteLine("\t First Frame: "+first_frame);
	            Console.Out.WriteLine("\t Last Frame: "+last_frame);
	            Console.Out.WriteLine("\t Flags: "+flags);
	            Console.Out.WriteLine("\t Pivot: "+pivot);
                Console.Out.WriteLine("\t Default Value: " + default_val);
            }

            int numBits = last_frame - first_frame + 1;
            int numBytes = (int)(numBits / 8);
            if((numBits % 8) != 0)
	            numBytes++;
    		
            result_vectors = new Boolean[numBits];
    		
            int k = 0;
            for(int i = 0; i < numBytes; i++)
            {
	            int temp = br.ReadByte();
	            for(int j = 0; j < 8 && k < numBits; j++)
	            {
                    result_vectors[k] = ((temp >> j) & 0x01) == 1;
		            if(dbg)
                        Console.Out.WriteLine("\t BitChannelChunk result vector: " + result_vectors[k]);
		            k++;
	            }
            }
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

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of the bit channel result vector per keyframe.")]
        public Boolean[] BitChannelResultVector
        {
            get { return result_vectors; }
            set { result_vectors = value; }
        }
    }
}
