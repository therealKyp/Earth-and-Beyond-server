using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class EmitterFrameKeyFramesChunk : Chunk
    {
        private int key_frame_count;
        private float random;
        private float[] time;
        private float[] frame_index;

        public EmitterFrameKeyFramesChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_EMITTER_FRAME_KEYFRAMES);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_EMITTER_FRAME_KEYFRAMES;
            HeaderName = ChunkHeader.W3D_CHUNK_EMITTER_FRAME_KEYFRAMES.ToString();

            key_frame_count = br.ReadInt32();
            random = br.ReadSingle();
        	
    	    int[] reserved = new int[2];
		    for (int i = 0; i < 2; i++) {
                reserved[i] = br.ReadInt32();
		    }    
    		
		    if(dbg){
                Console.Out.WriteLine("\t  Key Frame Count: " + key_frame_count);
			    Console.Out.WriteLine("\t  Random: "+random);
		    }

            if (key_frame_count == 0)
            {

                time = new float[1];
                frame_index = new float[1];

                frame_index[0] = br.ReadSingle();
                time[0] = br.ReadSingle();
    			
			    if(dbg){
	        	    Console.Out.WriteLine("\t  ********** Frame KeyFrame 0 **********");
	        	    Console.Out.WriteLine("\t  Time: "+time[0]);
                    Console.Out.WriteLine("\t  Frame (sub-texture Index): " + frame_index[0]);
			    }
        		
    	    }else{
                int Count = (chunkLength - 16) / 8;
    		    time = new float[Count];
                frame_index = new float[Count];
            	
        	    for (int i = 0; i < Count; i++) {
                    frame_index[i] = br.ReadSingle();
                    time[i] = br.ReadSingle();
        			
    			    if(dbg){
	    			    Console.Out.WriteLine("\t  ********** Frame KeyFrame "+i+"**********");
	            	    Console.Out.WriteLine("\t  Time: "+time[i]);
                        Console.Out.WriteLine("\t  Frame (sub-texture Index): " + frame_index[i]);
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

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of frame indices per keyframe.")]
        public float[] FrameIndex
        {
            get { return frame_index; }
            set { frame_index = value; }
        }
    }
}
