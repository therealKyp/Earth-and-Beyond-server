using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class DazzleTypeNameChunk : Chunk
    {
        private String type_name;
        public DazzleTypeNameChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|-" + ChunkHeader.W3D_CHUNK_DAZZLE_TYPENAME);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_DAZZLE_TYPENAME;
            HeaderName = ChunkHeader.W3D_CHUNK_DAZZLE_TYPENAME.ToString();

            byte[] nameArray = br.ReadBytes(chunkLength);
            type_name = System.Text.ASCIIEncoding.ASCII.GetString(nameArray);

            if (dbg)
            {
                Console.Out.WriteLine("\t Dazzle Type Name: " + type_name.Trim());
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The type name of the dazzle object.")]
        public String TypeName
        {
            get { return type_name; }
            set { type_name = value; }
        }
    }
}
