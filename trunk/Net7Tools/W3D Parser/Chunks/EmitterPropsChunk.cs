using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Drawing;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class EmitterPropsChunk : Chunk
    {
        private int color_key_frames;
        private int opacity_key_frames;
        private int size_key_frames;
        private Color color_random;
        private float opacity_random;
        private float size_random;
        private object[][] color_keyframe_data;
        private object[][] opacity_keyframe_data;
        private object[][] size_keyframe_data;

        public EmitterPropsChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_EMITTER_PROPS);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_EMITTER_PROPS;
            HeaderName = ChunkHeader.W3D_CHUNK_EMITTER_PROPS.ToString();

            color_key_frames = br.ReadInt32();
    	    opacity_key_frames = br.ReadInt32();
    	    size_key_frames = br.ReadInt32();

            int R = br.ReadByte();
            int G = br.ReadByte();
            int B = br.ReadByte();
            int A = br.ReadByte();
        	
    	    color_random = Color.FromArgb(A, R, G, B);
    	    opacity_random = br.ReadSingle();
    	    size_random = br.ReadSingle();
        	
    	    int[] reserved = new int[4];
		    for (int i = 0; i < 4; i++) {
			    reserved[i] = br.ReadInt32();
		    }    	
    		
		    if(dbg){
			    Console.Out.WriteLine("\t Color Key Frames: "+color_key_frames);
			    Console.Out.WriteLine("\t Opacity Key Frames: "+opacity_key_frames);
			    Console.Out.WriteLine("\t Size Key Frames: "+size_key_frames);
			    Console.Out.WriteLine("\t Color Random: "+color_random);
			    Console.Out.WriteLine("\t Opacity Random: "+opacity_random);
			    Console.Out.WriteLine("\t Size Random: "+size_random);
		    }
    		
		    if(color_key_frames == 0){
			    color_key_frames = 1;
		    }
		    if(opacity_key_frames == 0){
			    opacity_key_frames = 1;
		    }
		    if(size_key_frames == 0){
			    size_key_frames = 1;
		    }

            color_keyframe_data = new object[color_key_frames][];
            opacity_keyframe_data = new object[opacity_key_frames][];
            size_keyframe_data = new object[size_key_frames][];
    		
		    for (int i = 0; i < color_key_frames; i++) {
                float colorTime = br.ReadSingle();

                int cR = br.ReadByte();
                int cG = br.ReadByte();
                int cB = br.ReadByte();
                int cA = br.ReadByte();

                Color color = Color.FromArgb(cA, cR, cG, cB);

                object[] tmp = new object[2];
                tmp[0] = colorTime;
                tmp[1] = color;

                color_keyframe_data[i] = tmp;

			    if(dbg){
				    Console.Out.WriteLine("\t  ********** Color KeyFrame "+i+"**********");
	        	    Console.Out.WriteLine("\t  Color Time: "+colorTime);
	    		    Console.Out.WriteLine("\t  Color: "+color);
			    }
		    }		
    		
		    for (int i = 0; i < opacity_key_frames; i++) {
                float opacityTime = br.ReadSingle();
                float opacity = br.ReadSingle();

                object[] tmp = new object[2];
                tmp[0] = opacityTime;
                tmp[1] = opacity;

                opacity_keyframe_data[i] = tmp;
    			
			    if(dbg){
				    Console.Out.WriteLine("\t  ********** Opacity KeyFrame "+i+"**********");
	        	    Console.Out.WriteLine("\t  Opacity Time: "+opacityTime);
	    		    Console.Out.WriteLine("\t  Opacity: "+opacity);
			    }
		    }		
    		
		    for (int i = 0; i < size_key_frames; i++) {
                float sizeTime = br.ReadSingle();
                float size = br.ReadSingle();

                object[] tmp = new object[2];
                tmp[0] = sizeTime;
                tmp[1] = size;

                size_keyframe_data[i] = tmp;

			    if(dbg){
				    Console.Out.WriteLine("\t  ********** Size KeyFrame "+i+"**********");
	        	    Console.Out.WriteLine("\t  Size Time: "+sizeTime);
	    		    Console.Out.WriteLine("\t  Size: "+size);
			    }
		    }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The Count of color keyframes.")]
        public int ColorKeyFrames
        {
            get { return color_key_frames; }
            set { color_key_frames = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The count of opacity keyframes.")]
        public int OpacityKeyFrames
        {
            get { return opacity_key_frames; }
            set { opacity_key_frames = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The count of size keyframes.")]
        public int SizeKeyFrames
        {
            get { return size_key_frames; }
            set { size_key_frames = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Random color for the emitter.")]
        public Color ColorRandom
        {
            get { return color_random; }
            set { color_random = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Random opacity value.")]
        public float OpacityRandom
        {
            get { return opacity_random; }
            set { opacity_random = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Random size value.")]
        public float SizeRandom
        {
            get { return size_random; }
            set { size_random = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Array of ColorTime and Color values per keyframe.")]
        public Object[][] ColorKeyFrameData
        {
            get { return color_keyframe_data; }
            set { color_keyframe_data = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Array of OpacityTime and Opacity values per keyframe.")]
        public Object[][] OpacityKeyFrameData
        {
            get { return opacity_keyframe_data; }
            set { opacity_keyframe_data = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Array of SizeTimes and Size values per keyframe.")]
        public Object[][] SizeKeyFrameData
        {
            get { return size_keyframe_data; }
            set { size_keyframe_data = value; }
        }
    }
}
