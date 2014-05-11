using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class AabTreeNodesChunk : Chunk
    {
        private int num_nodes;
        private object[][] node_info;

        /// <summary>
        /// <para>AABTree Node. This is a node in the AABTree.</para>
        /// <remarks>If the MSB of FrontOrPoly0 is 1, then the node is a leaf and contains Poly0 and PolyCount
        /// else, the node is not a leaf and contains indices to its front and back children. 
        /// <para>This matches the format used by AABTreeClass in WW3D.</para></remarks>
        /// </summary>
        public AabTreeNodesChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|---| " + ChunkHeader.W3D_CHUNK_AABTREE_NODES);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_AABTREE_NODES;
            HeaderName = ChunkHeader.W3D_CHUNK_AABTREE_NODES.ToString();

            num_nodes = chunkLength / 32;
            node_info = new object[NumNodes][];

            for (int i = 0; i < NumNodes; i++)
            {
                object[] info = new object[4];

                Vector3 Min = new Vector3(br.ReadSingle(), br.ReadSingle(), br.ReadSingle());
                Vector3 Max = new Vector3(br.ReadSingle(), br.ReadSingle(), br.ReadSingle());

                int Poly0 = br.ReadInt32();
                int PolyCount = br.ReadInt32();

                if (dbg)
                {
                    Console.Out.WriteLine("\t\t Node " + i + " Min: " + Min);
                    Console.Out.WriteLine("\t\t Node " + i + " Max: " + Min);
                    Console.Out.WriteLine("\t\t Node " + i + " Poly0: " + Poly0);
                    Console.Out.WriteLine("\t\t Node " + i + " PolyCount: " + PolyCount);
                }

                info[0] = Min;
                info[1] = Max;
                info[2] = Poly0;
                info[3] = PolyCount;
                node_info[i] = info;
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The Number of Nodes.")]
        public int NumNodes
        {
            get { return num_nodes; }
            set { num_nodes = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Array of Min, Max, Poly0 and PolyCount")]
        public object[][] NodeInfo
        {
            get { return node_info; }
            set { node_info = value; }
        }
    }
}
