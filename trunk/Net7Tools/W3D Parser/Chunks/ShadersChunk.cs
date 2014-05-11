using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class ShadersChunk : Chunk
    {
        private object[][] shader_data;
        private int depth_compare;
        private int depth_mask;
        private int color_mask;
        private int dest_blend;
        private int fog_func;
        private int pri_gradient;
        private int sec_gradient;
        private int src_blend;
        private int texturing;
        private int detail_color_func;
        private int detail_alpha_func;
        private int shader_preset;
        private int alpha_test;
        private int post_detail_color_func;
        private int post_detail_alpha_func;


        public ShadersChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_SHADERS);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_SHADERS;
            HeaderName = ChunkHeader.W3D_CHUNK_SHADERS.ToString();

            int NumShaders = chunkLength / 16;
            object[] shaders = new object[15];
            shader_data = new object[NumShaders][];

            for (int i = 0; i < NumShaders; i++)
            {
                depth_compare = br.ReadByte();
                depth_mask = br.ReadByte();
                color_mask = br.ReadByte();
                dest_blend = br.ReadByte();
                fog_func = br.ReadByte();
                pri_gradient = br.ReadByte();
                sec_gradient = br.ReadByte();
                src_blend = br.ReadByte();
                texturing = br.ReadByte();
                detail_color_func = br.ReadByte();
                detail_alpha_func = br.ReadByte();
                shader_preset = br.ReadByte();
                alpha_test = br.ReadByte();
                post_detail_color_func = br.ReadByte();
                post_detail_alpha_func = br.ReadByte();
                int pad = br.ReadByte();

                shaders[0] = depth_compare;
                shaders[1] = depth_mask;
                shaders[2] = color_mask;
                shaders[3] = dest_blend;
                shaders[4] = fog_func;
                shaders[5] = pri_gradient;
                shaders[6] = sec_gradient;
                shaders[7] = src_blend;
                shaders[8] = texturing;
                shaders[9] = detail_color_func;
                shaders[10] = detail_alpha_func;
                shaders[11] = shader_preset;
                shaders[12] = alpha_test;
                shaders[13] = post_detail_color_func;
                shaders[14] = post_detail_alpha_func;

                shader_data[i] = shaders;

                if (dbg)
                {
                    Console.Out.WriteLine("\t Shader " + i + " DepthCompare: " + depth_compare);
                    Console.Out.WriteLine("\t Shader " + i + " DepthMask: " + depth_mask);
                    Console.Out.WriteLine("\t Shader " + i + " ColorMask: " + color_mask);
                    Console.Out.WriteLine("\t Shader " + i + " DestBlend: " + dest_blend);
                    Console.Out.WriteLine("\t Shader " + i + " FogFunc: " + fog_func);
                    Console.Out.WriteLine("\t Shader " + i + " PriGradient: " + pri_gradient);
                    Console.Out.WriteLine("\t Shader " + i + " SecGradient: " + sec_gradient);
                    Console.Out.WriteLine("\t Shader " + i + " SrcBlend: " + src_blend);
                    Console.Out.WriteLine("\t Shader " + i + " Texturing: " + texturing);
                    Console.Out.WriteLine("\t Shader " + i + " DetailColorFunc: " + detail_color_func);
                    Console.Out.WriteLine("\t Shader " + i + " DetailAlphaFunc: " + detail_alpha_func);
                    Console.Out.WriteLine("\t Shader " + i + " ShaderPreset: " + shader_preset);
                    Console.Out.WriteLine("\t Shader " + i + " AlphaTest: " + alpha_test);
                    Console.Out.WriteLine("\t Shader " + i + " PostDetailColorFunc: " + post_detail_color_func);
                    Console.Out.WriteLine("\t Shader " + i + " PostDetailAlphaFunc: " + post_detail_alpha_func);
                } 
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of Shader data per number of shaders. (see chunk class format for data description.)")]
        public object[][] ShaderData
        {
            get { return shader_data; }
            set { shader_data = value; }
        }
    }
}
