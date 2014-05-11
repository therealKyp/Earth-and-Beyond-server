using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class EmitterBlurTimeKeyFramesChunk : Chunk
    {
        private int key_frame_count;
        private float random;
        private int reserved;
        private float[] time;
        private float[] blur_time;

        public EmitterBlurTimeKeyFramesChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_EMITTER_BLUR_TIME_KEYFRAMES);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_EMITTER_BLUR_TIME_KEYFRAMES;
            HeaderName = ChunkHeader.W3D_CHUNK_EMITTER_BLUR_TIME_KEYFRAMES.ToString();

            key_frame_count = br.ReadInt32();
    	    random = br.ReadSingle();
    	    reserved = br.ReadInt32();
        	
    	    if(dbg){
                Console.Out.WriteLine("\t  Key Frame Count: " + key_frame_count);
			    Console.Out.WriteLine("\t  Random: "+random);
    	    }

            if (key_frame_count == 0)
            {

                time = new float[1];
                blur_time = new float[1];

        	    time[0] = br.ReadSingle();
			    blur_time[0] = br.ReadSingle();
    			
			    if(dbg){
	        	    Console.Out.WriteLine("\t  ********** Blur Time KeyFrame 0 **********");
	        	    Console.Out.WriteLine("\t  Time: "+time);
	    		    Console.Out.WriteLine("\t  Blur Time: "+blur_time);
			    }
        		
    	    }else{
                time = new float[key_frame_count];
                blur_time = new float[key_frame_count];

                for (int i = 0; i < key_frame_count; i++)
                {
    			    time[i] = br.ReadSingle();
    			    blur_time[i] = br.ReadSingle();
        			
    			    if(dbg){
	    			    Console.Out.WriteLine("\t  ********** Blur Time KeyFrame "+i+"**********");
	            	    Console.Out.WriteLine("\t  Time: "+time);
	        		    Console.Out.WriteLine("\t  Blur Time: "+blur_time);
    			    }
    		    }    
    	    }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of keyframes affected by the blur time.")]
        public int KeyFrameCount
        {
            get { return key_frame_count; }
            set { key_frame_count = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("A random value (function currently unkown).")]
        public float Random
        {
            get { return random; }
            set { random = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of times per keyframe.")]
        public float[] Time
        {
            get { return time; }
            set { time = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of blur-times per keyframe.")]
        public float[] BlurTime
        {
            get { return blur_time; }
            set { blur_time = value; }
        }
    }
}
