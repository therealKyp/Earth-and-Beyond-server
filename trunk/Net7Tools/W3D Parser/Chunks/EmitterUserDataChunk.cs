using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class EmitterUserDataChunk : Chunk
    {
        private int type;
        private int string_param_size;
        private String string_param;

        public EmitterUserDataChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_EMITTER_USER_DATA);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_EMITTER_USER_DATA;
            HeaderName = ChunkHeader.W3D_CHUNK_EMITTER_USER_DATA.ToString();

            type = br.ReadInt32();
            string_param_size = br.ReadInt32();
            byte[] stringParamArray = br.ReadBytes(chunkLength - 8);
            string_param = System.Text.ASCIIEncoding.ASCII.GetString(stringParamArray);

            if (dbg)
            {
                Console.Out.WriteLine("\t Type: " + type);
                Console.Out.WriteLine("\t Size Of String Param: " + string_param_size);
                Console.Out.WriteLine("\t String Param: " + string_param);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The type of user data.")]
        public int Type
        {
            get { return type; }
            set { type = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The string parameter size.")]
        public int StringParamSize
        {
            get { return string_param_size; }
            set { string_param_size = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The string parameter.")]
        public String StringParameter
        {
            get { return string_param; }
            set { string_param = value; }
        }
    }
}
