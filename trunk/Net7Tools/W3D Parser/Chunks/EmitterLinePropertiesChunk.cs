using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class EmitterLinePropertiesChunk : Chunk
    {
        private int flags;
        private int sub_division_level;
        private float noise_amplitude;
        private float merge_abort_factor;
        private float texture_tile_factor;
        private float u_per_sec;
        private float v_per_sec;

        public EmitterLinePropertiesChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int endPosition)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_EMITTER_LINE_PROPERTIES);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_EMITTER_LINE_PROPERTIES;
            HeaderName = ChunkHeader.W3D_CHUNK_EMITTER_LINE_PROPERTIES.ToString();

            flags = br.ReadInt32();
            sub_division_level = br.ReadInt32();
            noise_amplitude = br.ReadSingle();
            merge_abort_factor = br.ReadSingle();
            texture_tile_factor = br.ReadSingle();
            u_per_sec = br.ReadSingle();
            v_per_sec = br.ReadSingle();
        	
    	    int[] reserved = new int[9];
		    for (int i = 0; i < 9; i++) {
			    reserved[i] = br.ReadInt32();
		    }    	
    		
		    if(dbg){
			    Console.Out.WriteLine("\t  Flags: "+flags);
			    Console.Out.WriteLine("\t  Sub Division Level: "+sub_division_level);
			    Console.Out.WriteLine("\t  Noise Amplitude: "+noise_amplitude);
			    Console.Out.WriteLine("\t  Merge Abort Factor: "+merge_abort_factor);
			    Console.Out.WriteLine("\t  Texture Tile Factor: "+texture_tile_factor);
			    Console.Out.WriteLine("\t  U Per Second: "+u_per_sec);
			    Console.Out.WriteLine("\t  V Per Second: "+v_per_sec);
		    }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Any attributes this chunk may have.")]
        public int Flags
        {
            get { return flags; }
            set { flags = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("How many times is the emitter sub divided.")]
        public int SubDivisionLevel
        {
            get { return sub_division_level; }
            set { sub_division_level = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The noise within the particles.")]
        public float NoiseAmplitude
        {
            get { return noise_amplitude; }
            set { noise_amplitude = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The factor in which the merging of the emitter will abort.")]
        public float MergeAbortFactor
        {
            get { return merge_abort_factor; }
            set { merge_abort_factor = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The tiling factor of the texture for the particles.")]
        public float TextureTileFactor
        {
            get { return texture_tile_factor; }
            set { texture_tile_factor = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The u tex coord change per sec.")]
        public float UPerSec
        {
            get { return u_per_sec; }
            set { u_per_sec = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The v tex coord change per sec.")]
        public float VPerSec
        {
            get { return v_per_sec; }
            set { v_per_sec = value; }
        }
    }
}
