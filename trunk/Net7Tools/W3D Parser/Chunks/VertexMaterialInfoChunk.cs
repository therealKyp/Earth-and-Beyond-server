using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Drawing;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class VertexMaterialInfoChunk : Chunk
    {
        private int attributes;
        private Color ambiant;
        private Color diffuse;
        private Color specular;
        private Color emmissive;
        private float shinniness;
        private float opacity;
        private float translucency;

        public VertexMaterialInfoChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int endPosition)
        {
            if (dbg) Console.Out.WriteLine("|----| " + ChunkHeader.W3D_CHUNK_VERTEX_MATERIAL_INFO);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_VERTEX_MATERIAL_INFO;
            HeaderName = ChunkHeader.W3D_CHUNK_VERTEX_MATERIAL_INFO.ToString();

            int Attributes = br.ReadInt32();

            ambiant = Color.FromArgb(br.ReadByte(), br.ReadByte(), br.ReadByte());
            Byte pad1 = br.ReadByte();
            diffuse = Color.FromArgb(br.ReadByte(), br.ReadByte(), br.ReadByte());
            Byte pad2 = br.ReadByte();
            specular = Color.FromArgb(br.ReadByte(), br.ReadByte(), br.ReadByte());
            Byte pad3 = br.ReadByte();
            emmissive = Color.FromArgb(br.ReadByte(), br.ReadByte(), br.ReadByte());
            Byte pad4 = br.ReadByte();

            shinniness = br.ReadSingle();
            opacity = br.ReadSingle();
            translucency = br.ReadSingle();

            if (dbg)
            {
                Console.Out.WriteLine("\t\t Attributes: " + attributes);
                Console.Out.WriteLine("\t\t Ambiant: " + ambiant);
                Console.Out.WriteLine("\t\t Diffuse: " + diffuse);
                Console.Out.WriteLine("\t\t Specular: " + specular);
                Console.Out.WriteLine("\t\t Emmissive: " + emmissive);
                Console.Out.WriteLine("\t\t Shinniness: " + shinniness);
                Console.Out.WriteLine("\t\t Opacity: " + opacity);
                Console.Out.WriteLine("\t\t Translucency: " + translucency);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Any attributes / flags this chunk may have.")]
        public int Attributes
        {
            get { return attributes; }
            set { attributes = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("A ambiant color value for this material.")]
        public Color Ambiant
        {
            get { return ambiant; }
            set { ambiant = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("A diffuse color value for this material.")]
        public Color Diffuse
        {
            get { return diffuse; }
            set { diffuse = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("A specular color value for this material.")]
        public Color Specular
        {
            get { return specular; }
            set { specular = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("A emissive color value for this material.")]
        public Color Emmissive
        {
            get { return emmissive; }
            set { ambiant = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The shinniness value of this material.")]
        public float Shinniness
        {
            get { return shinniness; }
            set { shinniness = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The opacity of this material.")]
        public float Opacity
        {
            get { return opacity; }
            set { opacity = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The translucency of this material.")]
        public float Translucency
        {
            get { return translucency; }
            set { translucency = value; }
        }
    }
}
