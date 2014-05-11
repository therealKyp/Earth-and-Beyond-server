using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class EmitterRotationKeyFramesChunk : Chunk
    {
        private int key_frame_count;
        private float random;
        private float orientation_random;
        private object[][] rotational_velocity_keyframe_data;

        public EmitterRotationKeyFramesChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_EMITTER_ROTATION_KEYFRAMES);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_EMITTER_ROTATION_KEYFRAMES;
            HeaderName = ChunkHeader.W3D_CHUNK_EMITTER_ROTATION_KEYFRAMES.ToString();

            key_frame_count = br.ReadInt32();
            random = br.ReadSingle();
            orientation_random = br.ReadSingle();
            int reserved = br.ReadInt32();
            rotational_velocity_keyframe_data = new object[key_frame_count][];
        	
    	    if(dbg){
			    Console.Out.WriteLine("\t  Key Frame Count: "+key_frame_count);
			    Console.Out.WriteLine("\t  Random: "+random);
			    Console.Out.WriteLine("\t  Orientation Random: "+orientation_random);
    	    }

            float[] rotation = new float[key_frame_count];
            float[] time = new float[key_frame_count];
            object[] tmp = new object[2];
        	
		    if (key_frame_count == 0){

                time[0] = br.ReadSingle();
                rotation[0] = br.ReadSingle();

                tmp[0] = time;
                tmp[1] = rotation;
                rotational_velocity_keyframe_data[0] = tmp;
    			
			    if(dbg){
	        	    Console.Out.WriteLine("\t  ********** Rotational Velocity KeyFrame 0 **********");
	        	    Console.Out.WriteLine("\t  Time: "+time);
	    		    Console.Out.WriteLine("\t  Rotation Velocity: "+rotation);
			    }
        		
    	    }else{
        	    for (int i = 0; i < key_frame_count; i++) {
                    time[i] = br.ReadSingle();
                    rotation[i] = br.ReadSingle();

                    tmp[0] = time;
                    tmp[1] = rotation;
                    rotational_velocity_keyframe_data[i] = tmp;
        			
    			    if(dbg){
	    			    Console.Out.WriteLine("\t  ********** Rotational Velocity KeyFrame "+i+"**********");
	            	    Console.Out.WriteLine("\t  Time: "+time);
	        		    Console.Out.WriteLine("\t  Rotation Velocity: "+rotation);
    			    }
    		    }    
    	    }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of keyframes the emitter has.")]
        public int KeyFrameCount
        {
            get { return key_frame_count; }
            set { key_frame_count = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("A random value (unknown)")]
        public float Random
        {
            get { return random; }
            set { random = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("A random orientation value")]
        public float OrientationRandom
        {
            get { return orientation_random; }
            set { orientation_random = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of time values and a rotational velocities per keyframe.")]
        public object[][] RotationalVelocityKeyframeData
        {
            get { return rotational_velocity_keyframe_data; }
            set { rotational_velocity_keyframe_data = value; }
        }
    }
}
