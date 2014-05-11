using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class EmitterInfo2Chunk : Chunk
    {
        private int burst_size;
        private int creation_volume_classID;
        private float creation_volume_1;
        private float creation_volume_2;
        private float creation_volume_3;
        private int vel_random_volume_classID;
        private float vel_random_volume_1;
        private float vel_random_volume_2;
        private float vel_random_volume_3;
        private float outward_velocity;
        private float velocity_inherit;

        public EmitterInfo2Chunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_EMITTER_INFOV2);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_EMITTER_INFOV2;
            HeaderName = ChunkHeader.W3D_CHUNK_EMITTER_INFOV2.ToString();

            burst_size = br.ReadInt32();
            creation_volume_classID = br.ReadInt32();
            creation_volume_1 = br.ReadSingle();
            creation_volume_2 = br.ReadSingle();
            creation_volume_3 = br.ReadSingle();

		    for (int i = 0; i < 4; i++) {
			    int padding = br.ReadInt32();
		    }

            vel_random_volume_classID = br.ReadInt32();
            vel_random_volume_1 = br.ReadSingle();
            vel_random_volume_2 = br.ReadSingle();
            vel_random_volume_3 = br.ReadSingle();

            for (int i = 0; i < 4; i++) {
			    int padding = br.ReadInt32();
		    }

            outward_velocity = br.ReadSingle();
            velocity_inherit = br.ReadSingle();

            //TODO: Need To figure out the parsing below.
            br.ReadBytes(chunkLength - 76);

            //int ShaderDepthCompare = br.ReadByte();
            //int ShdaerDepthMask = br.ReadByte();
            //int ShaderDestBlend = br.ReadByte();
            //int ShaderPrimaryGradient = br.ReadByte();
            //int ShaderSecondaryGradient = br.ReadByte();
            //int ShaderSrcBlend = br.ReadByte();
            //int ShaderTexturing = br.ReadByte();
            //int ShaderDetailColor = br.ReadByte();
            //int ShaderDetailAlpha = br.ReadByte();
            //int ShaderAlphaTest = br.ReadByte();

            //int rendorMode = br.ReadInt32();
            //int frameMode = br.ReadInt32();

            //for (int i = 0; i < 6; i++)
            //{
            //    int padding = br.ReadInt32();
            //}  

		    if(dbg){
                Console.Out.WriteLine("\t Burst Size: " + burst_size);
                Console.Out.WriteLine("\t Creation Volume Class ID: " + creation_volume_classID);
                Console.Out.WriteLine("\t Creation Volume Value 1: " + creation_volume_1);
                Console.Out.WriteLine("\t Creation Volume Value 2: " + creation_volume_2);
                Console.Out.WriteLine("\t Creation Volume Value 3: " + creation_volume_3);
                Console.Out.WriteLine("\t Vel Random Volume Class ID: " + vel_random_volume_classID);
                Console.Out.WriteLine("\t Vel Random Volume Value 1: " + vel_random_volume_1);
                Console.Out.WriteLine("\t Vel Random Volume Value 2: " + vel_random_volume_2);
                Console.Out.WriteLine("\t Vel Random Volume Value 3: " + vel_random_volume_3);
                Console.Out.WriteLine("\t Outward Velocity: " + outward_velocity);
                Console.Out.WriteLine("\t Velocity Inherit: " + velocity_inherit);
                //Console.Out.WriteLine("\t Shader Depth Compare: " + ShaderDepthCompare);
                //Console.Out.WriteLine("\t Shader Depth Mask: " + ShaderDepthCompare);
                //Console.Out.WriteLine("\t Shader Dest Blend: " + ShaderDepthCompare);
                //Console.Out.WriteLine("\t Shader Primary Gradient: " + ShaderDepthCompare);
                //Console.Out.WriteLine("\t Shader Secondary Gradient: " + ShaderDepthCompare);
                //Console.Out.WriteLine("\t Shader Source Blend: " + ShaderDepthCompare);
                //Console.Out.WriteLine("\t Shader Texturing: " + ShaderDepthCompare);
                //Console.Out.WriteLine("\t Shader Detail Color: " + ShaderDepthCompare);
                //Console.Out.WriteLine("\t Shader Detail Alpha: " + ShaderDepthCompare);
                //Console.Out.WriteLine("\t Shader Alpha Test: " + ShaderDepthCompare);
                //Console.Out.WriteLine("\t Rendor Mode: " + rendorMode);
                //Console.Out.WriteLine("\t Frame Mode: " + frameMode);
		    }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The size of the emitter's burst.")]
        public int BurstSize
        {
            get { return burst_size; }
            set { burst_size = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The original class id of this creation volume.")]
        public int CreationVolumeClassID
        {
            get { return creation_volume_classID; }
            set { creation_volume_classID = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Creation Volume value 1 (unkown meaning).")]
        public float CreationVolumeValue1
        {
            get { return creation_volume_1; }
            set { creation_volume_1 = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Creation Volume value 2 (unkown meaning).")]
        public float CreationVolumeValue2
        {
            get { return creation_volume_2; }
            set { creation_volume_2 = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Creation Volume value 3 (unkown meaning).")]
        public float CreationVolumeValue3
        {
            get { return creation_volume_3; }
            set { creation_volume_3 = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The original class id of the this velocity random volume.")]
        public int VelocityRandomVolumeClassID
        {
            get { return vel_random_volume_classID; }
            set { vel_random_volume_classID = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Velocity Random Volume value 1 (unkown meaning).")]
        public float VelocityRandomVolume1
        {
            get { return vel_random_volume_1; }
            set { vel_random_volume_1 = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Velocity Random Volume value 2 (unkown meaning).")]
        public float VelocityRandomVolume2
        {
            get { return vel_random_volume_2; }
            set { vel_random_volume_2 = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Velocity Random Volume value 2 (unkown meaning).")]
        public float VelocityRandomVolume3
        {
            get { return vel_random_volume_3; }
            set { vel_random_volume_3 = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The outward velocity of this particle.")]
        public float OutwardVelocity
        {
            get { return outward_velocity; }
            set { outward_velocity = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The inherit velocity of this particle.")]
        public float VelocityInherit
        {
            get { return velocity_inherit; }
            set { velocity_inherit = value; }
        }
    }
}
