using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class HierarchyHeaderChunk : Chunk
    {
        private int version;
        private String name;
        private int numPivots;
        private Vector3 center;

        public HierarchyHeaderChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int endPosition)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_HIERARCHY_HEADER);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_HIERARCHY_HEADER;
            HeaderName = ChunkHeader.W3D_CHUNK_HIERARCHY_HEADER.ToString();

            version = br.ReadInt32();
            byte[] nameArray = br.ReadBytes(16);
            name = System.Text.ASCIIEncoding.ASCII.GetString(nameArray);
            numPivots = br.ReadInt32();
            float x = br.ReadUInt32();
            float y = br.ReadUInt32();
            float z = br.ReadUInt32();

            center = new Vector3(x, y, z);

            if (dbg)
            {
                Console.Out.WriteLine("\t Version: " + version);
                Console.Out.WriteLine("\t Name: " + name);
                Console.Out.WriteLine("\t NumPivots: " + numPivots);
                Console.Out.WriteLine("\t Center: (" + center.X + ", " + center.Y + ", " + center.Z + ")");
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The version of this hierarchy.")]
        public int Version
        {
            get { return version; }
            set { version = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The name of the hierarchy")]
        public String Name
        {
            get { return name; }
            set { name = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of pivots for this hierarchy object.")]
        public int NumPivots
        {
            get { return numPivots; }
            set { numPivots = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The center position of this hierarchy.")]
        public Vector3 Center
        {
            get { return center; }
            set { center = value; }
        }
    }
}
