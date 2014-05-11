using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class AnimationHeaderChunk : Chunk
    {
        private int version;
        private String name;
        private String hierarchy_name;
        private int num_frames;
        private int frame_rate;

        public AnimationHeaderChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_ANIMATION_HEADER);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_ANIMATION_HEADER;
            HeaderName = ChunkHeader.W3D_CHUNK_ANIMATION_HEADER.ToString();

            version = br.ReadInt32();
            byte[] nameArray = br.ReadBytes(16);
            name = System.Text.ASCIIEncoding.ASCII.GetString(nameArray);
            byte[] hierarchyNameArray = br.ReadBytes(16);
            hierarchy_name = System.Text.ASCIIEncoding.ASCII.GetString(hierarchyNameArray);
            num_frames = br.ReadInt32();
            frame_rate = br.ReadInt32();

            if (dbg)
            {
                Console.Out.WriteLine("\t Version: " + version);
                Console.Out.WriteLine("\t Name: " + name);
                Console.Out.WriteLine("\t Hierarchy Name: " + hierarchy_name);
                Console.Out.WriteLine("\t Number Of Frames: " + num_frames);
                Console.Out.WriteLine("\t Frame Rate: " + frame_rate);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The version of this animation.")]
        public int Version
        {
            get { return version; }
            set { version = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The name of this animation.")]
        public String Name
        {
            get { return name; }
            set { name = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The name of the model hierarchy to apply this animation too.")]
        public String HierarchyName
        {
            get { return hierarchy_name; }
            set { hierarchy_name = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The Number of animation keyframes.")]
        public int NumFrames
        {
            get { return num_frames; }
            set { num_frames = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The frame rate of this animation.")]
        public int FrameRate
        {
            get { return frame_rate; }
            set { frame_rate = value; }
        }
    }
}
