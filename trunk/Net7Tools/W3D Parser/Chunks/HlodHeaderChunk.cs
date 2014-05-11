using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class HlodHeaderChunk : Chunk
    {
        private int version;
        private int lodCount;
        private String w3dName;
        private String hierarchyName;

        public HlodHeaderChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int endPosition)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_HLOD_HEADER);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_HLOD_HEADER;
            HeaderName = ChunkHeader.W3D_CHUNK_HLOD_HEADER.ToString();

            version = br.ReadInt32();
            lodCount = br.ReadInt32();
            byte[] w3dNameArray = br.ReadBytes(16);
            w3dName = System.Text.ASCIIEncoding.ASCII.GetString(w3dNameArray);
            byte[] hierarchyNameArray = br.ReadBytes(16);
            hierarchyName = System.Text.ASCIIEncoding.ASCII.GetString(w3dNameArray);
		
		    if(dbg){
			    Console.Out.WriteLine("\t Version: "+version);
			    Console.Out.WriteLine("\t LOD Count: "+lodCount);
			    Console.Out.WriteLine("\t Name: "+w3dName);
                Console.Out.WriteLine("\t Hierachy Name: " + hierarchyName);
		    }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The version of this HLOD.")]
        public int Version
        {
            get { return version; }
            set { version = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of LOD's in this hierarchy lod object.")]
        public int LodCount
        {
            get { return lodCount; }
            set { lodCount = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The name of the w3d file.")]
        public String W3DName
        {
            get { return w3dName; }
            set { w3dName = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The hierarchy name.")]
        public String HierarchyName
        {
            get { return hierarchyName; }
            set { hierarchyName = value; }
        }
    }
}
