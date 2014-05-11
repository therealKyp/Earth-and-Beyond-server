using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class HModelHeaderChunk : Chunk
    {
        private int version;
        private String name;
        private String hierarchy_name;
        private int num_connections;

        public HModelHeaderChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_HMODEL_HEADER);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_HMODEL_HEADER;
            HeaderName = ChunkHeader.W3D_CHUNK_HMODEL_HEADER.ToString();

            version = br.ReadInt32();
            byte[] nameArray = br.ReadBytes(16);
            name = System.Text.ASCIIEncoding.ASCII.GetString(nameArray);
            byte[] hierarchyNameArray = br.ReadBytes(16);
            hierarchy_name = System.Text.ASCIIEncoding.ASCII.GetString(hierarchyNameArray);
            num_connections = br.ReadInt16();

            //Not sure what the rest of the structure is, so iterate over the remaining bytes.
            br.ReadBytes(chunkLength - 38);

            if (dbg)
            {
                Console.Out.WriteLine("\t Version: " + version);
                Console.Out.WriteLine("\t Name: " + name);
                Console.Out.WriteLine("\t Hierarchy Name: " + hierarchy_name);
                Console.Out.WriteLine("\t Number Of Connections: " + num_connections);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The version of this hmodel.")]
        public int Version
        {
            get { return version; }
            set { version = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The name of this hmodel.")]
        public String Name
        {
            get { return name; }
            set { name = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The hierarchy name of this model")]
        public String HierarchyName
        {
            get { return hierarchy_name; }
            set { hierarchy_name = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of connections in this hmodel.")]
        public int ConnectionCount
        {
            get { return num_connections; }
            set { num_connections = value; }
        }
    }
}
