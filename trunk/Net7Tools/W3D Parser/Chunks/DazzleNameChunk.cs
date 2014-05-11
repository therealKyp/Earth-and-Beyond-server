using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class DazzleNameChunk : Chunk
    {
        private String name;

        public DazzleNameChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_DAZZLE_NAME);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_DAZZLE_NAME;
            HeaderName = ChunkHeader.W3D_CHUNK_DAZZLE_NAME.ToString();

            byte[] nameArray = br.ReadBytes(chunkLength);
            name = System.Text.ASCIIEncoding.ASCII.GetString(nameArray);

            if (dbg)
            {
                Console.Out.WriteLine("\t Dazzle Name: " + Name.Trim());
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The name of the dazzle object.")]
        public String Name
        {
            get { return name; }
            set { name = value; }
        }
    }
}
