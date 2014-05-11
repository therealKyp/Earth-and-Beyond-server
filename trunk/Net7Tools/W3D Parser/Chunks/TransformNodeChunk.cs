using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class TransformNodeChunk : Chunk
    {
        private String name;
        public TransformNodeChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_TRANSFORM_NODE);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_TRANSFORM_NODE;
            HeaderName = ChunkHeader.W3D_CHUNK_TRANSFORM_NODE.ToString();

            byte[] nameArray = br.ReadBytes(chunkLength);
            name = System.Text.ASCIIEncoding.ASCII.GetString(nameArray);

            if (dbg)
            {
                Console.Out.WriteLine("\t\t Transform Node W3D FileName: " + name.Trim());
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The name of node to be transformed.")]
        public String Name
        {
            get { return name; }
            set { name = value; }
        }
    }
}
