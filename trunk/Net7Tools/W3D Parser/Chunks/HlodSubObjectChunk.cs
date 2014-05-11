using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class HlodSubObjectChunk : Chunk
    {
        private int boneIndex;
        private String name;

        public HlodSubObjectChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|---| " + ChunkHeader.W3D_CHUNK_HLOD_SUB_OBJECT);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_HLOD_SUB_OBJECT;
            HeaderName = ChunkHeader.W3D_CHUNK_HLOD_SUB_OBJECT.ToString();

            int NumChars = chunkLength - 4;
            long correctEndPosition = br.BaseStream.Position + chunkLength;
            boneIndex = br.ReadInt32();
            byte[] nameArray = br.ReadBytes(NumChars);
            name = System.Text.ASCIIEncoding.ASCII.GetString(nameArray);

            if (br.BaseStream.Position > correctEndPosition)
            {
                br.BaseStream.Position = correctEndPosition;
            }

            if (dbg)
            {
                Console.Out.WriteLine("\t\t Name: " + Name.Trim());
                Console.Out.WriteLine("\t\t Bone Index: " + boneIndex);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The name of the mesh to attach to a bone.")]
        public String Name
        {
            get { return name; }
            set { name = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The bone id to attach too.")]
        public int BoneIndex
        {
            get { return boneIndex; }
            set { boneIndex = value; }
        }
    }
}
