using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class AggregateInfoChunk : Chunk
    {
        private String base_model_name;
        private int sub_object_count;
        private object[][] sub_object_info;

        public AggregateInfoChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_AGGREGATE_INFO);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_AGGREGATE_INFO;
            HeaderName = ChunkHeader.W3D_CHUNK_AGGREGATE_INFO.ToString();

            long CorrectEndPos = br.BaseStream.Position + chunkLength;
            byte[] nameArray = br.ReadBytes(32);
            base_model_name = System.Text.ASCIIEncoding.ASCII.GetString(nameArray);
            sub_object_count = br.ReadInt32();

            if (dbg)
            {
                Console.Out.WriteLine("\t Base Model Name: " + base_model_name);
                Console.Out.WriteLine("\t Sub Object Count: " + sub_object_count);
            }

            sub_object_info = new object[sub_object_count][];

            for (int i = 0; i < sub_object_count; i++)
            {
                object[] agarray = new object[2];
                byte[] BoneNameArray = br.ReadBytes(32);
                String BoneName = System.Text.ASCIIEncoding.ASCII.GetString(BoneNameArray);
                byte[] SubObjectNameArray = br.ReadBytes(32);
                String SubObjectName = System.Text.ASCIIEncoding.ASCII.GetString(SubObjectNameArray);

                agarray[0] = BoneName;
                agarray[1] = SubObjectName;

                sub_object_info[i] = agarray;

                if (dbg)
                {
                    Console.Out.WriteLine("\t Sub Object #" + i);
                    Console.Out.WriteLine("\t Bone Name: " + BoneName);
                    Console.Out.WriteLine("\t Sub Object Name: " + SubObjectName);
                }
            }

            //Parsing of some files gets screwy so compensate.
            br.BaseStream.Position = CorrectEndPos;
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The Base Model Name of this aggregate node.")]
        public String BaseModelName
        {
            get { return base_model_name; }
            set { base_model_name = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The number of sub objects(w3d references) contained in this aggregate.")]
        public int SubObjectCount
        {
            get { return sub_object_count; }
            set { sub_object_count = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Array of Sub Object Information (Bone Name, Sub Object Name).")]
        public object[][] SubObjectInfo
        {
            get { return sub_object_info; }
            set { sub_object_info = value; }
        }
    }
}
