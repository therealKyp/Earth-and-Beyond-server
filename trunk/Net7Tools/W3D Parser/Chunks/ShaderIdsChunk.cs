using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class ShaderIdsChunk : Chunk
    {
        private float[] shaderIds;

        public ShaderIdsChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|---| " + ChunkHeader.W3D_CHUNK_SHADER_IDS);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_SHADER_IDS;
            HeaderName = ChunkHeader.W3D_CHUNK_SHADER_IDS.ToString();

            int NumShaderIds = chunkLength / 4;
            shaderIds = new float[NumShaderIds];

            for (int i = 0; i < NumShaderIds; i++)
            {
                shaderIds[i] = br.ReadInt32();

                if (dbg)
                {
                    Console.Out.WriteLine("\t   Face{" + i + "} Shader Index: " + shaderIds[i]);
                }
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("An array of Shader Indices per face.")]
        public float[] ShaderIds
        {
            get { return shaderIds; }
            set { shaderIds = value; }
        }
    }
}
