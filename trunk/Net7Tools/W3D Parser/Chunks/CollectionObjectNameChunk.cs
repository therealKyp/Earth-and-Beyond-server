using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class CollectionObjectNameChunk : Chunk
    {
        private String[] names;

        public CollectionObjectNameChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_COLLECTION_OBJ_NAME);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_COLLECTION_OBJ_NAME;
            HeaderName = ChunkHeader.W3D_CHUNK_COLLECTION_OBJ_NAME.ToString();

            for (int i = 0; i < chunkLength / 16; i++)
            {
                byte[] nameArray = br.ReadBytes(16);
                names[i] = System.Text.ASCIIEncoding.ASCII.GetString(nameArray); 
            }

            if (dbg)
            {
                for (int i = 0; i < names.Length; i++)
                {
                    Console.Out.WriteLine("\t\t RenderObjectName: " + names[i].Trim()); 
                }
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of the name of the render object.")]
        public String[] Name
        {
            get { return names; }
            set { names = value; }
        }
    }
}
