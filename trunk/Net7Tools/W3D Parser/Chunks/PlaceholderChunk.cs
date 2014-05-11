using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class PlaceholderChunk : Chunk
    {
        private int version;
        private float[] transform_a;
        private float[] transform_b;
        private float[] transform_c;
        private float[] transform_d;
        private String name;

        public PlaceholderChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_PLACEHOLDER);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_PLACEHOLDER;
            HeaderName = ChunkHeader.W3D_CHUNK_PLACEHOLDER.ToString();

            version = br.ReadInt32();

            transform_a = new float[3];
            transform_a[0] = br.ReadSingle();
            transform_a[1] = br.ReadSingle();
            transform_a[2] = br.ReadSingle();

            transform_b = new float[3];
            transform_b[0] = br.ReadSingle();
            transform_b[1] = br.ReadSingle();
            transform_b[2] = br.ReadSingle();

            transform_c = new float[3];
            transform_c[0] = br.ReadSingle();
            transform_c[1] = br.ReadSingle();
            transform_c[2] = br.ReadSingle();

            transform_d = new float[3];
            transform_d[0] = br.ReadSingle();
            transform_d[1] = br.ReadSingle();
            transform_d[2] = br.ReadSingle();

            byte[] nameArray = br.ReadBytes(chunkLength - 52);
            name = System.Text.ASCIIEncoding.ASCII.GetString(nameArray);

            if (dbg)
            {
                Console.Out.WriteLine("\t Version: " + version);
                Console.Out.WriteLine("\t Transform A: " + transform_a);
                Console.Out.WriteLine("\t Transform B: " + transform_b);
                Console.Out.WriteLine("\t Transform C: " + transform_c);
                Console.Out.WriteLine("\t Transform D: " + transform_d);
                Console.Out.WriteLine("\t Name: " + name);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The version of this placeholder.")]
        public int Version
        {
            get { return version; }
            set { version = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("A Vector tranform.")]
        public float[] TransformA
        {
            get { return transform_a; }
            set { transform_a = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("A Vector tranform.")]
        public float[] TransformB
        {
            get { return transform_b; }
            set { transform_b = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("A Vector tranform.")]
        public float[] TransformC
        {
            get { return transform_c; }
            set { transform_c = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("A Vector tranform.")]
        public float[] TransformD
        {
            get { return transform_d; }
            set { transform_d = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The name of this placeholder.")]
        public String Name
        {
            get { return name; }
            set { name = value; }
        }
    }
}
