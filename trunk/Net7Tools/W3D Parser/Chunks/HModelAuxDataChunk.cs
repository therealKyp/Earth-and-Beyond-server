using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class HModelAuxDataChunk : Chunk
    {

        private int attributes;
        private int mesh_count;
        private int collision_count;
        private int skin_count;
        private int[] future_counts;
        private float lod_min;
        private float lod_max;
        private int[] future_use;

        public HModelAuxDataChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_HMODEL_AUX_DATA);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_HMODEL_AUX_DATA;
            HeaderName = ChunkHeader.W3D_CHUNK_HMODEL_AUX_DATA.ToString();

            attributes = br.ReadInt32();
            mesh_count = br.ReadInt32();
            collision_count = br.ReadInt32();
            skin_count = br.ReadInt32();
            future_counts = new int[8];

            for (int i = 0; i < 8; i++)
            {
                future_counts[i] = br.ReadInt32(); 
            }

            lod_min = br.ReadSingle();
            lod_max = br.ReadSingle();

            future_use = new int[32];

            for (int i = 0; i < 32; i++)
            {
                future_use[i] = br.ReadInt32(); 
            }

            if (dbg)
            {
                Console.Out.WriteLine("\t Attributes: " + attributes);
                Console.Out.WriteLine("\t Mesh Count: " + mesh_count);
                Console.Out.WriteLine("\t Collision Count: " + collision_count);
                Console.Out.WriteLine("\t SkinCount: " + skin_count);
                Console.Out.WriteLine("\t Future Counts: " + future_counts);
                Console.Out.WriteLine("\t LOD Min: " + lod_min);
                Console.Out.WriteLine("\t LOD Max: " + lod_max);
                Console.Out.WriteLine("\t Future Use: " + future_use);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Any attributes / flags for this chunk.")]
        public int Attributes
        {
            get { return attributes; }
            set { attributes = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of meshes in this model.")]
        public int MeshCount
        {
            get { return mesh_count; }
            set { mesh_count = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of collisions in this model.")]
        public int CollisionCount
        {
            get { return collision_count; }
            set { collision_count = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of skins in this model.")]
        public int SkinCount
        {
            get { return skin_count; }
            set { skin_count = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Placeholder for future counts.")]
        public int[] FutureCounts
        {
            get { return future_counts; }
            set { future_counts = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("the minimum distance for a LOD.")]
        public float LodMin
        {
            get { return lod_min; }
            set { lod_min = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The maximum distance for a LOD")]
        public float LodMax
        {
            get { return lod_max; }
            set { lod_max = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Placeholder for future use.")]
        public int[] FutureUse
        {
            get { return future_use; }
            set { future_use = value; }
        }
    }
}
