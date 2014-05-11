using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class AggregateClassInfoChunk : Chunk
    {
        private int original_class_id;
        private int flags;

        public AggregateClassInfoChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_AGGREGATE_CLASS_INFO);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_AGGREGATE_CLASS_INFO;
            HeaderName = ChunkHeader.W3D_CHUNK_AGGREGATE_CLASS_INFO.ToString();

            original_class_id = br.ReadInt32();
            flags = br.ReadInt32();
            
            //Unknown Bytes
            br.ReadBytes(chunkLength - 8); 

            if (dbg)
            {
                Console.Out.WriteLine("\t Original Class Id: " + original_class_id);
                Console.Out.WriteLine("\t Flags: " + flags);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The Orignal Class Id that created this chunk.")]
        public int OriginalClassID
        {
            get { return original_class_id; }
            set { original_class_id = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The Orignal Class Id that created this chunk.")]
        public int Flags
        {
            get { return flags; }
            set { flags = value; }
        }
    }
}
