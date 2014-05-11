using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class HlodSubObjectArrayHeaderChunk : Chunk
    {
        private int model_count;
        private float max_screen_size;

        public HlodSubObjectArrayHeaderChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int endPosition)
        {
            if (dbg) Console.Out.WriteLine("|---| " + ChunkHeader.W3D_CHUNK_HLOD_SUB_OBJECT_ARRAY_HEADER);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_HLOD_SUB_OBJECT_ARRAY_HEADER;
            HeaderName = ChunkHeader.W3D_CHUNK_HLOD_SUB_OBJECT_ARRAY_HEADER.ToString();

            model_count = br.ReadInt32();
            max_screen_size = br.ReadSingle();

            if (dbg)
            {
                Console.Out.WriteLine("\t\t Model Count: " + model_count);
                Console.Out.WriteLine("\t\t Max Screen Size: " + max_screen_size);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of models in this hlod subobject.")]
        public int ModelCount
        {
            get { return model_count; }
            set { model_count = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The maximum screensize of this subobject.")]
        public float MaxScreenSize
        {
            get { return max_screen_size; }
            set { max_screen_size = value; }
        }
    }
}
