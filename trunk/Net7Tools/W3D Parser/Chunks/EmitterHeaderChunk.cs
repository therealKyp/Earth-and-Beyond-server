using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class EmitterHeaderChunk : Chunk
    {
        private int version;
        private String name;

        public EmitterHeaderChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_EMITTER_HEADER);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_EMITTER_HEADER;
            HeaderName = ChunkHeader.W3D_CHUNK_EMITTER_HEADER.ToString();

            version = br.ReadInt32();
            byte[] nameArray = br.ReadBytes(16);
            name = System.Text.ASCIIEncoding.ASCII.GetString(nameArray);

            if (dbg)
            {
                Console.Out.WriteLine("\t Version: " + version);
                Console.Out.WriteLine("\t Name: " + name);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("the version of this emitter")]
        public int Version
        {
            get { return version; }
            set { version = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The name of the emitter")]
        public String Name
        {
            get { return name; }
            set { name = value; }
        }
    }
}
