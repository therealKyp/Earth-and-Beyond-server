using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class AabTreeHeaderChunk : Chunk
    {
        private int node_count;
        private int poly_count;

        /// <summary>
        /// Catalog of the contents of the AABTree.
        /// </summary>
        public AabTreeHeaderChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|---| " + ChunkHeader.W3D_CHUNK_AABTREE_HEADER);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_AABTREE_HEADER;
            HeaderName = ChunkHeader.W3D_CHUNK_AABTREE_HEADER.ToString();

            node_count = br.ReadInt32();
            poly_count = br.ReadInt32();

            //Unkown bytes or padding
            br.ReadBytes(chunkLength - 8);

            if (dbg)
            {
                Console.Out.WriteLine("\t\t Node Count: " + NodeCount);
                Console.Out.WriteLine("\t\t Poly Count: " + PolyCount);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The Number of Nodes in the AAB Tree.")]
        public int NodeCount
        {
            get { return node_count; }
            set { node_count = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The Number of Polygons in the AAB Tree.")]
        public int PolyCount
        {
            get { return poly_count; }
            set { poly_count = value; }
        }
    }
}
