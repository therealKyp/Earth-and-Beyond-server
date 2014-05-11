using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class AggregateHeaderChunk : Chunk
    {
        private int version;
        private String name;

        public AggregateHeaderChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int endPosition)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_AGGREGATE_HEADER);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_AGGREGATE_HEADER;
            HeaderName = ChunkHeader.W3D_CHUNK_AGGREGATE_HEADER.ToString();

            version = br.ReadInt32();
            byte[] nameArray = br.ReadBytes(16);
            name = System.Text.ASCIIEncoding.ASCII.GetString(nameArray);

            if (dbg)
            {
                Console.Out.WriteLine("\t Version: " + version);
                Console.Out.WriteLine("\t Name: " + name);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The Version of the aggregate.")]
        public int Version
        {
            get { return version; }
            set { version = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The Name of the aggregate.")]
        public String Name
        {
            get { return name; }
            set { name = value; }
        }
    }
}
