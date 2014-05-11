using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class MeshUserTextChunk : Chunk
    {
        private String user_text;

        public MeshUserTextChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_MESH_USER_TEXT);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_MESH_USER_TEXT;
            HeaderName = ChunkHeader.W3D_CHUNK_MESH_USER_TEXT.ToString();

            byte[] nameArray = br.ReadBytes(chunkLength);
            user_text = System.Text.ASCIIEncoding.ASCII.GetString(nameArray);

            if (dbg)
            {
                Console.Out.WriteLine("\t\t Mesh User Text: " + UserText.Trim());
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Any user text exported from the max comment field.")]
        public String UserText
        {
            get { return user_text; }
            set { user_text = value; }
        }
    }
}
