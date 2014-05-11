using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class CollectionHeaderChunk : Chunk
    {
        private int version;
        private String name;
        private int render_object_count;

        public CollectionHeaderChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_COLLECTION_HEADER);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_COLLECTION_HEADER;
            HeaderName = ChunkHeader.W3D_CHUNK_COLLECTION_HEADER.ToString();

            version = br.ReadInt32();
            byte[] nameArray = br.ReadBytes(chunkLength - 8);
            name = System.Text.ASCIIEncoding.ASCII.GetString(nameArray);
            render_object_count = br.ReadInt32();

            if (dbg)
            {
                Console.Out.WriteLine("\t\t Version: " + version);
                Console.Out.WriteLine("\t\t Name: " + name.Trim());
                Console.Out.WriteLine("\t\t Render Object Count: " + render_object_count);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The version of this Collection.")]
        public int Version
        {
            get { return version; }
            set { version = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The name of this Collection.")]
        public String Name
        {
            get { return name; }
            set { name = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of render objects that this collection contains.")]
        public int RenderObjectCount
        {
            get { return render_object_count; }
            set { render_object_count = value; }
        }
    }
}
