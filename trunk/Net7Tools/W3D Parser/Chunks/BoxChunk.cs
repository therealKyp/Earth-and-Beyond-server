using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Drawing;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    /********************************************************************************

	    Collision Boxes

	    Collision boxes are meant to be used for, you guessed it, collision detection.
	    For this reason, they only contain a minimal amount of rendering information
	    (a color). 

	    Axis Aligned - This is a bounding box which is *always* aligned with the world 
	    coordinate system. So, the center point is to be transformed by whatever
	    transformation matrix is being used but the extents always point down the
	    world space x,y, and z axes. (in effect, you are translating the center).

	    Oriented - This is an oriented 3D box. It is aligned with the coordinate system
	    it is in. So its extents always point along the local coordinate system axes.

    ********************************************************************************/

    public class BoxChunk : Chunk
    {
        private int version;
        private int attributes;
        private String name;
        private Color rgb;
        private Vector3 center;
        private Vector3 extents;

        public BoxChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|-| " + ChunkHeader.W3D_CHUNK_BOX);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_BOX;
            HeaderName = ChunkHeader.W3D_CHUNK_BOX.ToString();

            version = br.ReadInt32();
            attributes = br.ReadInt32();
            byte[] nameArray = br.ReadBytes(chunkLength - 35);
            name = System.Text.ASCIIEncoding.ASCII.GetString(nameArray);
            rgb = Color.FromArgb(br.ReadByte(), br.ReadByte(), br.ReadByte());
            center = new Vector3((double) br.ReadSingle(), (double) br.ReadSingle(), (double) br.ReadSingle());
            extents = new Vector3((double)br.ReadSingle(), (double) br.ReadSingle(), (double)br.ReadSingle());

            if (dbg)
            {
                Console.Out.WriteLine("\t Version: " + version);
                Console.Out.WriteLine("\t Attributes: " + attributes);
                Console.Out.WriteLine("\t Name: " + name);
                Console.Out.WriteLine("\t RGB: " + rgb);
                Console.Out.WriteLine("\t center: " + center);
                Console.Out.WriteLine("\t extents: " + extents);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The version of this box.")]
        public int Version
        {
            get { return version; }
            set { version = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The flagged attributes of this box.")]
        public int Attributes
        {
            get { return attributes; }
            set { attributes = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The name of the box model.")]
        public String Name
        {
            get { return name; }
            set { name = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The rgb color of the box.")]
        public Color RGB
        {
            get { return rgb; }
            set { rgb = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The center point of the box.")]
        public Vector3 CenterBox
        {
            get { return center; }
            set { center = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The extents of the box.")]
        public Vector3 BoxExtents
        {
            get { return extents; }
            set { extents = value; }
        }
    }
}
